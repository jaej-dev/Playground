/*
 * Title      :   diag_fsm.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <functional>
#include <iostream>
#include <xlog/xlog.h>
#include "diag/diag_fsm.h"

diag_fsm_t::diag_fsm_t(xfsm::sender_t s,
                                     std::shared_ptr<diag_data_t> d)
    : core_fsm_(s),
      diag_data_(d),
      xfsm_set_init_state(diag_fsm_t, idle) {
  xfsm_enable_state(diag_fsm_t, idle);
  xfsm_enable_state(diag_fsm_t, creating);
  xfsm_enable_state(diag_fsm_t, initializing);
  xfsm_enable_state(diag_fsm_t, pending);
  xfsm_enable_state(diag_fsm_t, waiting);
  xfsm_enable_state(diag_fsm_t, active);
  xfsm_enable_state(diag_fsm_t, exception);
  xfsm_enable_state(diag_fsm_t, shutdown);
  xfsm_enable_state(diag_fsm_t, terminating);

  last_error_.set_error(exception_data_t::no_exception);
  period_us_ = 100000;  // 100ms
  tick_count_ = 0;
  xlogv << "constructed";
}

diag_fsm_t::~diag_fsm_t() { xlogv << "destructed"; }

/*
 * states of fsm
 */
xfsm_state_of(diag_fsm_t, idle) {
  store_current_state(prev_state_);
  store_current_state(current_state_);

  xlogd << "entered idle state";

  if (this->do_idle() == false) {
    last_error_.set_error(exception_data_t::diag_fsm_fault);
    oneself().send(dnmsg::throw_exception(last_error_.get_error()));
  }

  mesg()
      .wait()
      .handle(dispatch, dnmsg::start)
      .handle(dispatch, cnmsg::shutdown_subsystem)
      .handle(dispatch, dnmsg::throw_exception);
}

xfsm_state_of(diag_fsm_t, creating) {
  store_current_state(current_state_);
  if (check_current_state() == false) {
    xlogd << "entered creating state";
  }

  if (this->do_creating() == false) {
    last_error_.set_error(exception_data_t::diag_fsm_fault);
    oneself().send(dnmsg::throw_exception(last_error_.get_error()));
  }

  mesg()
      .wait()
      .handle(dispatch, dnmsg::created)
      .handle(dispatch, cnmsg::shutdown_subsystem)
      .handle(dispatch, dnmsg::throw_exception);
}

xfsm_state_of(diag_fsm_t, initializing) {
  store_current_state(current_state_);
  if (check_current_state() == false) {
    xlogd << "entered initializing state";
  }

  if (this->do_initializing() == false) {
    last_error_.set_error(exception_data_t::diag_fsm_fault);
    oneself().send(dnmsg::throw_exception(last_error_.get_error()));
  }

  mesg()
      .wait(std::chrono::milliseconds(100))
      .handle(dispatch, dnmsg::initialized)
      .handle(dispatch, cnmsg::shutdown_subsystem)
      .handle(dispatch, dnmsg::throw_exception);
}

xfsm_state_of(diag_fsm_t, pending) {
  store_current_state(current_state_);
  if (check_current_state() == false) {
    xlogd << "entered pending state";
  }

  // do nothing

  mesg()
      .wait(std::chrono::milliseconds(100))
      .handle(dispatch, cnmsg::start_diag)
      .handle(dispatch, cnmsg::shutdown_subsystem)
      .handle(dispatch, dnmsg::throw_exception);
}

xfsm_state_of(diag_fsm_t, waiting) {
  store_current_state(current_state_);
  if (check_current_state() == false) {
    xlogd << "entered waiting state";
  }

  if (this->wait_for_client() == false) {
    last_error_.set_error(exception_data_t::diag_fsm_fault);
    oneself().send(dnmsg::throw_exception(last_error_.get_error()));
  }

  mesg()
      .wait(std::chrono::milliseconds(200))
      .handle(dispatch, dnmsg::connected)
      .handle(dispatch, cnmsg::shutdown_subsystem)
      .handle(dispatch, dnmsg::throw_exception);
}

xfsm_state_of(diag_fsm_t, active) {
  store_current_state(current_state_);
  if (check_current_state() == false) {
    xlogd << "entered active state";
  }

#if ENABLE_AUTOMATION_EMUL
  this->automation_emul();
#else
  if (this->do_active() == false) {
    last_error_.set_error(exception_data_t::diag_fsm_fault);
    oneself().send(dnmsg::throw_exception(last_error_.get_error()));
  }
#endif

  mesg()
      .wait(std::chrono::microseconds(period_us_))
      .handle(dispatch, dnmsg::disconnected)
      .handle(dispatch, cnmsg::stop_diag)
      .handle(dispatch, cnmsg::shutdown_subsystem)
      .handle(dispatch, dnmsg::throw_exception);
}

xfsm_state_of(diag_fsm_t, exception) {
  store_current_state(current_state_);
  if (check_current_state() == false) {
    xlogd << "entered exception state";
  }

  if (this->do_exception() == false) {
    ;
  }

  mesg().wait().handle(dispatch, cnmsg::shutdown_subsystem);
}

xfsm_state_of(diag_fsm_t, shutdown) {
  store_current_state(current_state_);
  if (check_current_state() == false) {
    xlogd << "entered shutdown state";
  }

  if (this->do_shutdown() == false) {
    ;
  }

  mesg().wait().handle(dispatch, dnmsg::terminate);
}

xfsm_state_of(diag_fsm_t, terminating) {
  store_current_state(current_state_);
  if (check_current_state() == false) {
    xlogd << "entered terminating state";
  }

  this->do_terminating();

  xlogv << "spin forever...";
  mesg().wait(std::chrono::milliseconds(500));
}

/*
 * dispatch function
 */
void diag_fsm_t::dispatch(dnmsg::start const &msg) {
  xlogv << "got dnmsg::start";
  update_state(xfsm_state_ref(diag_fsm_t, creating), msg);
}

void diag_fsm_t::dispatch(dnmsg::created const &msg) {
  xlogv << "got dnmsg::created";
  update_state(xfsm_state_ref(diag_fsm_t, initializing), msg);
}

void diag_fsm_t::dispatch(dnmsg::initialized const &msg) {
  xlogv << "got dnmsg::initialized";
  update_state(xfsm_state_ref(diag_fsm_t, pending), msg);
}

void diag_fsm_t::dispatch(cnmsg::start_diag const &msg) {
  xlogv << "got cnmsg::start_diag";
  update_state(xfsm_state_ref(diag_fsm_t, waiting), msg);
}

void diag_fsm_t::dispatch(cnmsg::stop_diag const &msg) {
  xlogv << "got cnmsg::stop_diag";
  update_state(xfsm_state_ref(diag_fsm_t, pending), msg);
}

void diag_fsm_t::dispatch(dnmsg::connected const &msg) {
  xlogv << "got dnmsg::connected";
  update_state(xfsm_state_ref(diag_fsm_t, active), msg);
}

void diag_fsm_t::dispatch(dnmsg::disconnected const &msg) {
  xlogv << "got dnmsg::disconnected";
  update_state(xfsm_state_ref(diag_fsm_t, waiting), msg);
}

void diag_fsm_t::dispatch(dnmsg::throw_exception const &msg) {
  xlogv << "got dnmsg::throw_exception";
  update_state(xfsm_state_ref(diag_fsm_t, exception), msg);
}

void diag_fsm_t::dispatch(cnmsg::shutdown_subsystem const &msg) {
  xlogv << "got cnmsg::shutdown_subsystem";
  update_state(xfsm_state_ref(diag_fsm_t, shutdown), msg);
}

void diag_fsm_t::dispatch(dnmsg::terminate const &msg) {
  xlogv << "got dnmsg::terminate";
  update_state(xfsm_state_ref(diag_fsm_t, terminating), msg);
}

/*
 * do action
 */
bool diag_fsm_t::do_idle() {
  xlogv << "send dnmsg::start";
  oneself().send(dnmsg::start());

  return true;
}

bool diag_fsm_t::do_creating() {
  xlogv << "send dnmsg::created";
  oneself().send(dnmsg::created());
  return true;
}

bool diag_fsm_t::do_initializing() {
  xlogv << "send dnmsg::initiallized";
  oneself().send(dnmsg::initialized());
  return true;
}

bool diag_fsm_t::wait_for_client() {
  xlogv << "send dnmsg:connected to core_fsm_";
  oneself().send(dnmsg::connected());
  return true;
}

bool diag_fsm_t::do_active() {
  //oneself().send(dnmsg::disconnected());
  xlogv << "done";
  return true;
}

bool diag_fsm_t::do_exception() {
  core_fsm_.send(dnmsg::exception_raised(last_error_.get_error()));
  return true;
}

bool diag_fsm_t::do_shutdown() {
  xlogv << "send dnmsg::terminate";

  oneself().send(dnmsg::terminate());
  return true;
}

bool diag_fsm_t::do_terminating() {
  xlogv << "state machine soon destroy...";
  return true;
}

/*
 * local helper
 */
bool diag_fsm_t::check_current_state() {
  if (prev_state_ == current_state_) {
    return true;
  } else {
    prev_state_ = current_state_;
    return false;
  }
}
