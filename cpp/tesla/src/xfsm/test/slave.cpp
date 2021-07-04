
#include <iostream>
#include <functional>
#include <xlog/xlog.h>
#include "slave.h"

using_xfsm;

slave::slave(xfsm::sender_t s) : xfsm_set_init_state(slave, state_a), master_(s) {
  xfsm_entry_action(slave, state_b, std::bind(&slave::entry_state_b, this));
  xfsm_exit_action(slave, state_b, std::bind(&slave::exit_state_b, this));

  xlogv << "constructed: " << this;
}

slave::~slave() { xlogv << "destructed"; }

/*
 * states of fsm
 */
xfsm_state_of(slave, state_a) {

  // do action
  store_current_state(prev_state_);
  store_current_state(current_state_);

  //xlogv << "entered state_a state";

  this->do_state_a();

  // wait until 1sec and then check incoming message
  mesg()
      .wait(std::chrono::milliseconds(5))
      .handle(dispatch, smsg::slave_msg_a);
}

xfsm_state_of(slave, state_b) {
  store_current_state(current_state_);
  if (check_current_state() == false) {
    xlogv << "entered state_b state";
  }

  this->do_state_b();

  mesg()
      .wait()
      .handle(dispatch, mmsg::master_msg_c);
}

xfsm_state_of(slave, state_c) {
  store_current_state(current_state_);
  if (check_current_state() == false) {
    xlogv << "entered state_c state";
  }

  this->do_state_c();

  mesg()
      .wait()
      .handle(dispatch, mmsg::master_msg_b);
}

/*
 * dispatch function
 */
void slave::dispatch(smsg::slave_msg_a const &msg) {
  xlogv << "got smsg::slave_msg_a";
  update_state(xfsm_state_ref(slave, state_b), msg);
}

void slave::dispatch(smsg::slave_msg_b const &msg) {
  xlogv << "got mmsg::slave_msg_b, msg: " << msg.msg;
  //update_state(xfsm_state_ref(slave, state_c), msg);
}

void slave::dispatch(smsg::slave_msg_c const &msg) {
  xlogv << "got mmsg::slave_msg_c, msg: " << msg.msg;
  //update_state(xfsm_state_ref(slave, state_c), msg);
}

void slave::dispatch(mmsg::master_msg_b const &msg) {
  xlogv << "got mmsg::master_msg_b, msg: " << msg.msg;
  update_state(xfsm_state_ref(slave, state_b), msg);
}

void slave::dispatch(mmsg::master_msg_c const &msg) {
  xlogv << "got mmsg::master_msg_c, msg: " << msg.msg;
  update_state(xfsm_state_ref(slave, state_c), msg);
}

/*
 * entry action
 */

void slave::entry_state_b() {
  xlogv << "done";
}

/*
 * do action
 */
void slave::do_state_a() {
  static int count = 0;
  count++;
  if (count % 200 == 0) {
    xlogv << "count: " << count;
  }

  if (count == 1000) {
    master_.send(smsg::slave_msg_a(oneself()));
    oneself().send(smsg::slave_msg_a(oneself()));
  }
}

void slave::do_state_b() {
  xlogv << "done";
}

void slave::do_state_c() {
  xlogv << "done";
}

/*
 * exit action
 */
void slave::exit_state_b() {
  xlogv << "done";
}

/*
 * local helper
 */
bool slave::check_current_state() {
  if (prev_state_ == current_state_) {
    return true;
  } else {
    prev_state_ = current_state_;
    return false;
  }
}
