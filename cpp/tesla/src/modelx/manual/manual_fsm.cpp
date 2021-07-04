/*
 * Title      :   manual_fsm.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <functional>
#include <iostream>
#include <cmath>
#include <arch_config.h>
#include <xlog/xlog.h>
#include "manual/manual_fsm.h"
#include "modelx_feature.h"

manual_fsm_t::manual_fsm_t(xfsm::sender_t s, 
                             std::shared_ptr<io_t> i,
                             std::shared_ptr<motor_t> m,
                             std::shared_ptr<manual_data_t> d)
    : core_fsm_(s),
      io_(i),
      mt_(m),
      manual_data_(d),
      xfsm_set_init_state(manual_fsm_t, idle) {
  xfsm_enable_state(manual_fsm_t, idle);
  xfsm_enable_state(manual_fsm_t, creating);
  xfsm_enable_state(manual_fsm_t, initializing);
  xfsm_enable_state(manual_fsm_t, pending);
  xfsm_enable_state(manual_fsm_t, active);
  xfsm_enable_state(manual_fsm_t, emergency);
  xfsm_enable_state(manual_fsm_t, exception);
  xfsm_enable_state(manual_fsm_t, shutdown);
  xfsm_enable_state(manual_fsm_t, terminating);

  last_error_.set_error(exception_data_t::no_exception);
  period_us_ = 5000;  // 5ms
  tick_count_ = 0;

  xlogv << "constructed";
}

manual_fsm_t::~manual_fsm_t() { xlogv << "destructed"; }

/*
 * states of fsm
 */
xfsm_state_of(manual_fsm_t, idle) {
  store_current_state(prev_state_);
  store_current_state(current_state_);

  xlogd << "entered idle state";

  if (!this->do_idle()) {
    oneself().send(mnmsg::throw_exception());
  }

  mesg()
      .wait()
      .handle(dispatch, mnmsg::start)
      .handle(dispatch, mnmsg::throw_exception)
      .handle(dispatch, cnmsg::shutdown_subsystem);
}

xfsm_state_of(manual_fsm_t, creating) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered creating state";
  }

  if (!this->create_device()) {
#if 1
    last_error_.set_error(exception_data_t::manual_creation_fault);
    oneself().send(mnmsg::throw_exception(last_error_.get_error()));
#else
    oneself().send(mnmsg::throw_exception());
#endif
  }

  mesg()
      //.wait()
      .wait(std::chrono::milliseconds(100))
      .handle(dispatch, mnmsg::created)
      .handle(dispatch, mnmsg::throw_exception)
      .handle(dispatch, cnmsg::shutdown_subsystem);
}

xfsm_state_of(manual_fsm_t, initializing) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered initializing state";
  }

  if (!this->init_device()) {
#if 1
    last_error_.set_error(exception_data_t::manual_initialization_fault);
    oneself().send(mnmsg::throw_exception(last_error_.get_error()));
#else
    oneself().send(mnmsg::throw_exception());
#endif
  }

  mesg()
      .wait(std::chrono::milliseconds(100))
      .handle(dispatch, mnmsg::initialized)
      .handle(dispatch, mnmsg::throw_exception)
      .handle(dispatch, cnmsg::shutdown_subsystem);
}

xfsm_state_of(manual_fsm_t, pending) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered pending state";
  }

  this->do_nothing();

  mesg()
      .wait(std::chrono::milliseconds(100))
      .handle(dispatch, cnmsg::start_manual_driving)
      .handle(dispatch, cnmsg::start_emergency)
      .handle(dispatch, cnmsg::shutdown_subsystem)
      .handle(dispatch, mnmsg::throw_exception);
}

xfsm_state_of(manual_fsm_t, active) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered active state";
  }

  tick_count_++;
  this->get_steering_data();

  mesg()
      .wait(std::chrono::microseconds(period_us_))
      .handle(dispatch, cnmsg::stop_manual_driving)
      .handle(dispatch, cnmsg::start_emergency)
      .handle(dispatch, cnmsg::shutdown_subsystem)
      .handle(dispatch, mnmsg::throw_exception);
}

xfsm_state_of(manual_fsm_t, emergency) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered emergency state";
  }

  mesg()
      .wait(std::chrono::milliseconds(100))
      .handle(dispatch, cnmsg::stop_manual_driving)
      .handle(dispatch, cnmsg::stop_emergency)
      .handle(dispatch, cnmsg::shutdown_subsystem)
      .handle(dispatch, mnmsg::throw_exception);
}

xfsm_state_of(manual_fsm_t, exception) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered exception state";
  }

  // in do_exception, send mnmsg::exception_raised to manual_fsm_t
  if (!this->do_exception()) {
    // enforce terminate vehicle
  }

  mesg().wait().handle(dispatch, cnmsg::shutdown_subsystem);
}

xfsm_state_of(manual_fsm_t, shutdown) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered shutdown state";
  }

  if (!this->do_shutdown()) {
    // enforce terminate vehicle
  }

  mesg().wait().handle(dispatch, mnmsg::terminate);
}

xfsm_state_of(manual_fsm_t, terminating) {
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
 * dispatch function from manual_fsm_t
 */
void manual_fsm_t::dispatch(mnmsg::start const &msg) {
  xlogv << "got mnmsg::start";
  update_state(xfsm_state_ref(manual_fsm_t, creating), msg);
}

void manual_fsm_t::dispatch(mnmsg::created const &msg) {
  xlogv << "got mnmsg::created";
  update_state(xfsm_state_ref(manual_fsm_t, initializing), msg);
}

void manual_fsm_t::dispatch(mnmsg::initialized const &msg) {
  xlogv << "got mnmsg::initialized";
  update_state(xfsm_state_ref(manual_fsm_t, pending), msg);
}

void manual_fsm_t::dispatch(cnmsg::start_manual_driving const &msg) {
  xlogv << "got cnmsg::start_manual_driving";
  update_state(xfsm_state_ref(manual_fsm_t, active), msg);
}

void manual_fsm_t::dispatch(cnmsg::stop_manual_driving const &msg) {
  xlogv << "got cnmsg::stop_manual_driving";
  update_state(xfsm_state_ref(manual_fsm_t, pending), msg);
}

void manual_fsm_t::dispatch(cnmsg::start_emergency const &msg) {
  xlogv << "got cnmsg::start_emergency";
  update_state(xfsm_state_ref(manual_fsm_t, emergency), msg);
}

void manual_fsm_t::dispatch(cnmsg::stop_emergency const &msg) {
  xlogv << "got cnmsg::stop_emergency";
  update_state(xfsm_state_ref(manual_fsm_t, pending), msg);
}

void manual_fsm_t::dispatch(mnmsg::throw_exception const &msg) {
  xlogv << "got mnmsg::throw_exception";
  update_state(xfsm_state_ref(manual_fsm_t, exception), msg);
}

void manual_fsm_t::dispatch(cnmsg::shutdown_subsystem const &msg) {
  xlogv << "got cnmsg::shutdown_subsystem";
  update_state(xfsm_state_ref(manual_fsm_t, shutdown), msg);
}

void manual_fsm_t::dispatch(mnmsg::terminate const &msg) {
  xlogv << "got mnmsg::terminate";
  update_state(xfsm_state_ref(manual_fsm_t, terminating), msg);
}

/*
 * do action
 */
bool manual_fsm_t::do_idle() {
  xlogv << "send mnmsg::start";
  oneself().send(mnmsg::start());

  return true;
}

bool manual_fsm_t::create_device() {
  xlogv << "send mnmsg::created";
  oneself().send(mnmsg::created());
  return true;
}

bool manual_fsm_t::init_device() {
  xlogv << "send mnmsg::initiallized";
  oneself().send(mnmsg::initialized());
  return true;
}

void manual_fsm_t::deinit_device() {
  xlogv << "done";
}

void manual_fsm_t::do_nothing() {
  xlogv << "done";
}

bool manual_fsm_t::get_steering_data() {
  bool ret = true;

#if ENABLE_INPUT_EMUL
  steering_data_.px = 0.01f * tick_count_;
  steering_data_.py = 0.02f * tick_count_;
  steering_data_.pz = 0.03f * tick_count_;
  steering_data_.oa = 0.04f * tick_count_;
  steering_data_.ob = 0.05f * tick_count_;
  steering_data_.og = 0.06f * tick_count_;
  std::lock_guard<std::mutex> const lk(mutex_);
  manual_data_->set_steering_data(steering_data_);
#endif
  return ret;
}

bool manual_fsm_t::do_exception() {
  core_fsm_.send(mnmsg::exception_raised(last_error_.get_error()));
  return true;
}

bool manual_fsm_t::do_shutdown() {
  this->deinit_device();
  xlogv << "send mnmsg::terminate";
  oneself().send(mnmsg::terminate());
  return true;
}

bool manual_fsm_t::do_terminating() {
  xlogv << "state machine soon destroy...";
  return true;
}

/*
 * local helper
 */
bool manual_fsm_t::check_current_state() {
  if (prev_state_ == current_state_) {
    return true;
  } else {
    prev_state_ = current_state_;
    return false;
  }
}
