/*
 * Title      :   autopilot_fsm.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <functional>
#include <iostream>
#include <xlog/xlog.h>
#include "autopilot/autopilot_fsm.h"

autopilot_fsm_t::autopilot_fsm_t(xfsm::sender_t s, 
                               std::shared_ptr<io_t> i,
                               std::shared_ptr<motor_t> m,
                               std::shared_ptr<autopilot_data_t> d)
    : core_fsm_(s),
      io_(i),
      mt_(m),
      data_(d),
      xfsm_set_init_state(autopilot_fsm_t, idle) {
  xfsm_entry_action(autopilot_fsm_t, pending,
                    std::bind(&autopilot_fsm_t::entry_pending, this));
  xfsm_entry_action(autopilot_fsm_t, active,
                    std::bind(&autopilot_fsm_t::entry_active, this));
  xfsm_entry_action(autopilot_fsm_t, emergency,
                    std::bind(&autopilot_fsm_t::entry_emergency, this));
  xfsm_exit_action(autopilot_fsm_t, active,
                    std::bind(&autopilot_fsm_t::exit_active, this));
  xfsm_exit_action(autopilot_fsm_t, emergency,
                    std::bind(&autopilot_fsm_t::exit_emergency, this));

  xfsm_enable_state(autopilot_fsm_t, idle);
  xfsm_enable_state(autopilot_fsm_t, creating);
  xfsm_enable_state(autopilot_fsm_t, initializing);
  xfsm_enable_state(autopilot_fsm_t, pending);
  xfsm_enable_state(autopilot_fsm_t, active);
  xfsm_enable_state(autopilot_fsm_t, emergency);
  xfsm_enable_state(autopilot_fsm_t, exception);
  xfsm_enable_state(autopilot_fsm_t, shutdown);
  xfsm_enable_state(autopilot_fsm_t, terminating);

  last_error_.set_error(exception_data_t::no_exception);
  period_us_ = 5000;  // 5ms
  tick_count_ = 0;
  driving_fl_ = false;
  motor_fatal_fl_ = false;

  xlogv << "constructed";
}

autopilot_fsm_t::~autopilot_fsm_t() { xlogv << "destructed"; }

/*
 * states of fsm
 */
xfsm_state_of(autopilot_fsm_t, idle) {
  store_current_state(prev_state_);
  store_current_state(current_state_);

  xlogd << "entered idle state";

  if (!this->do_idle()) {
    last_error_.set_error(exception_data_t::autopilot_fsm_fault);
    oneself().send(anmsg::throw_exception(last_error_.get_error()));
  }

  mesg()
      .wait()
      .handle(dispatch, anmsg::start)
      .handle(dispatch, anmsg::throw_exception)
      .handle(dispatch, cnmsg::shutdown_subsystem);
}

xfsm_state_of(autopilot_fsm_t, creating) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered creating state";
  }

  if (!this->create_control_instance()) {
    last_error_.set_error(exception_data_t::autopilot_fsm_fault);
    oneself().send(anmsg::throw_exception(last_error_.get_error()));
  }

  mesg()
      .wait()
      .handle(dispatch, anmsg::created)
      .handle(dispatch, anmsg::throw_exception)
      .handle(dispatch, cnmsg::shutdown_subsystem);
}

xfsm_state_of(autopilot_fsm_t, initializing) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered initializing state";
  }

  if (!this->motor_init()) {
    xloge << "insrument servo init failure";
    last_error_.set_error(exception_data_t::autopilot_motor_init_fault);
    oneself().send(anmsg::throw_exception(last_error_.get_error()));
  } else {
    if (!this->servo_on()) {
      xloge << "insrument servo on failure";
      last_error_.set_error(exception_data_t::autopilot_servo_on_fault);
      oneself().send(anmsg::throw_exception(last_error_.get_error()));
    }
  }

  xlogv << "send anmsg::initiallized";
  oneself().send(anmsg::initialized());

  mesg()
      .wait()
      //.wait(std::chrono::milliseconds(100))
      .handle(dispatch, anmsg::initialized)
      .handle(dispatch, anmsg::throw_exception)
      .handle(dispatch, cnmsg::shutdown_subsystem);
}

xfsm_state_of(autopilot_fsm_t, pending) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered pending state";
  }

  mesg()
      //.wait()
      .wait(std::chrono::milliseconds(100))
      .handle(dispatch, cnmsg::start_emergency)
      .handle(dispatch, cnmsg::start_autopilot_driving)
      .handle(dispatch, cnmsg::shutdown_subsystem)
      .handle(dispatch, anmsg::throw_exception);
}

xfsm_state_of(autopilot_fsm_t, active) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered active state";
  }

  if (!motor_fatal_fl_) {
    if (ctl_->do_autopilot_driving() != 0) {
      last_error_.set_error(exception_data_t::autopilot_driving_fault);
      oneself().send(anmsg::throw_exception(last_error_.get_error()));
    }
  }

  mesg()
      //.wait()
      .wait(std::chrono::microseconds(period_us_))
      .handle(dispatch, cnmsg::start_emergency)
      .handle(dispatch, cnmsg::stop_autopilot_driving)
      .handle(dispatch, cnmsg::forced_break_subsystem)
      .handle(dispatch, cnmsg::shutdown_subsystem)
      .handle(dispatch, anmsg::throw_exception);
}

xfsm_state_of(autopilot_fsm_t, emergency) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered emergency state";
  }

  // to do here

  mesg()
      //.wait()
      .wait(std::chrono::milliseconds(100))
      .handle(dispatch, cnmsg::stop_emergency)
      .handle(dispatch, anmsg::throw_exception);
}

xfsm_state_of(autopilot_fsm_t, exception) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered exception state";
  }

  // in do_exception, send anmsg::exception_raised to autopilot_fsm_t
  if (!this->do_exception()) {
    // enforce terminate vehicle
  }

  mesg().wait().handle(dispatch, cnmsg::shutdown_subsystem);
}

xfsm_state_of(autopilot_fsm_t, shutdown) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered shutdown state";
  }

  if (!this->servo_off()) {
    xloge << "servo_off failure";
  }

  if (!this->motor_deinit()) {
    xloge << "motor_deinit failure";
  }

  if (!this->do_shutdown()) {
    xloge << "do_shutdown failure";
  }

  mesg().wait().handle(dispatch, anmsg::terminate);
}

xfsm_state_of(autopilot_fsm_t, terminating) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered terminating state";
  }

  this->do_terminating();

  xlogv << "spin forever...";
  mesg()
      //.wait();
      .wait(std::chrono::milliseconds(500));
}

/*
 * dispatch function from autopilot_fsm_t
 */
void autopilot_fsm_t::dispatch(anmsg::start const &msg) {
  xlogv << "got anmsg::start";
  update_state(xfsm_state_ref(autopilot_fsm_t, creating), msg);
}

void autopilot_fsm_t::dispatch(anmsg::created const &msg) {
  xlogv << "got anmsg::created";
  update_state(xfsm_state_ref(autopilot_fsm_t, initializing), msg);
}

void autopilot_fsm_t::dispatch(anmsg::initialized const &msg) {
  xlogv << "got anmsg::initialized";
  update_state(xfsm_state_ref(autopilot_fsm_t, pending), msg);
}

void autopilot_fsm_t::dispatch(cnmsg::start_autopilot_driving const &msg) {
  xlogv << "got cnmsg::start_autopilot_driving";
  driving_fl_ = true;
  update_state(xfsm_state_ref(autopilot_fsm_t, active), msg);
}

void autopilot_fsm_t::dispatch(cnmsg::stop_autopilot_driving const &msg) {
  xlogv << "got cnmsg::stop_autopilot_driving";
  driving_fl_ = false;
  update_state(xfsm_state_ref(autopilot_fsm_t, pending), msg);
}

void autopilot_fsm_t::dispatch(cnmsg::start_emergency const &msg) {
  xlogv << "got cnmsg::start_emergency";
  update_state(xfsm_state_ref(autopilot_fsm_t, emergency), msg);
}

void autopilot_fsm_t::dispatch(cnmsg::stop_emergency const &msg) {
  xlogv << "got cnmsg::stop_emergency";
  update_state(xfsm_state_ref(autopilot_fsm_t, pending), msg);
}

void autopilot_fsm_t::dispatch(cnmsg::forced_break_subsystem const &msg) {
  xlogv << "got cnmsg::forced_break_subsystem";
  update_state(xfsm_state_ref(autopilot_fsm_t, pending), msg);
}

void autopilot_fsm_t::dispatch(anmsg::throw_exception const &msg) {
  xlogv << "got anmsg::throw_exception";
  update_state(xfsm_state_ref(autopilot_fsm_t, exception), msg);
}

void autopilot_fsm_t::dispatch(cnmsg::shutdown_subsystem const &msg) {
  xlogv << "got cnmsg::shutdown_subsystem";
  update_state(xfsm_state_ref(autopilot_fsm_t, shutdown), msg);
}

void autopilot_fsm_t::dispatch(anmsg::terminate const &msg) {
  xlogv << "got anmsg::terminate";
  update_state(xfsm_state_ref(autopilot_fsm_t, terminating), msg);
}

/*
 * entry action
 */
void autopilot_fsm_t::entry_pending() {

  if (!this->servo_on()) {
    last_error_.set_error(exception_data_t::autopilot_servo_on_fault);
    oneself().send(anmsg::throw_exception(last_error_.get_error()));
  } else {
    xlogv << "servo on";
  }

  xlogv << "done";
}

void autopilot_fsm_t::entry_active() {
  if (!this->servo_on()) {
    last_error_.set_error(exception_data_t::autopilot_servo_on_fault);
    oneself().send(anmsg::throw_exception(last_error_.get_error()));
  } else {
    xlogv << "servo on";
  }
}

void autopilot_fsm_t::entry_emergency() {
  xlogv << "done";
}

/*
 * do action
 */
bool autopilot_fsm_t::do_idle() {
  xlogv << "send anmsg::start";
  oneself().send(anmsg::start());

  return true;
}

bool autopilot_fsm_t::create_control_instance() {
  bool ret = false;
  try {
    autopilot_ctl_t *const imem =
        static_cast<autopilot_ctl_t *>(operator new(sizeof(autopilot_ctl_t)));
    ctl_.reset(new (imem)
                   autopilot_ctl_t(autopilot::apx, io_, mt_, data_, period_us_));
  } catch (std::exception const &e) {
    xloge << e.what() << "\n";
  }
  if (ctl_ != nullptr) {
    ret = true;
    xlogv << "send anmsg::created";
    oneself().send(anmsg::created());
  }

  return ret;
}

bool autopilot_fsm_t::do_exception() {
  core_fsm_.send(anmsg::exception_raised(last_error_.get_error()));
  return true;
}

bool autopilot_fsm_t::do_shutdown() {
  xlogv << "send anmsg::terminate";
  oneself().send(anmsg::terminate());
  return true;
}

bool autopilot_fsm_t::do_terminating() {
  xlogv << "state machine soon destroy...";
  return true;
}

/*
 * exit action
 */
void autopilot_fsm_t::exit_active() {
  ctl_->do_stop_autopilot();
  xlogv << "done";
}

void autopilot_fsm_t::exit_emergency() { xlogv << "done"; }

/*
 * local helper
 */
bool autopilot_fsm_t::motor_init() {
  if (!mt_->motor_init(motor_t::autopilot)) {
    xloge << "autopilot motors init failure";
    motor_fatal_fl_ = true;
    return false;
  } else {
    return true;
  }
}

bool autopilot_fsm_t::motor_deinit() {
  if (!mt_->motor_deinit(motor_t::autopilot)) {
    xloge << "autopilot motors deinit failure";
    return false;
  } else {
    return true;
  }
}

bool autopilot_fsm_t::servo_on() {
  xlogv << "done";
  return true;
}

bool autopilot_fsm_t::servo_off() {
  xlogv << "done";
  return true;
}

bool autopilot_fsm_t::check_current_state() {
  if (prev_state_ == current_state_) {
    return true;
  } else {
    prev_state_ = current_state_;
    return false;
  }
}
