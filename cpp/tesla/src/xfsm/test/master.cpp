
#include <iostream>
#include <functional>
#include <xlog/xlog.h>
#include "master.h"

using_xfsm;

master::master() : xfsm_set_init_state(master, state_a) {
  xfsm_entry_action(master, state_b, std::bind(&master::entry_state_b, this));
  xfsm_exit_action(master, state_b, std::bind(&master::exit_state_b, this));

  xlogv << "constructed: " << this;
}

master::~master() { xlogv << "destructed"; }

/*
 * states of fsm
 */
xfsm_state_of(master, state_a) {

  // do action
  store_current_state(prev_state_);
  store_current_state(current_state_);

  xlogv << "entered state_a state";

  this->do_state_a();

  // wait until 1sec and then check incoming message
  mesg()
      .wait()
      .handle(dispatch, smsg::slave_msg_a);
}

xfsm_state_of(master, state_b) {
  store_current_state(current_state_);
  if (check_current_state() == false) {
    xlogv << "entered state_b state";
  }

  this->do_state_b();

  mesg()
      .wait(std::chrono::seconds(2))
      .handle(dispatch, mmsg::master_msg_b);
}

xfsm_state_of(master, state_c) {
  store_current_state(current_state_);
  if (check_current_state() == false) {
    xlogv << "entered state_c state";
  }

  this->do_state_c();

  mesg()
      .wait();
}

/*
 * dispatch function
 */
void master::dispatch(mmsg::master_msg_a const &msg) {
  xlogv << "got mmsg::master_msg_a";
  update_state(xfsm_state_ref(master, state_b), msg);
}

void master::dispatch(mmsg::master_msg_b const &msg) {
  xlogv << "got mmsg::master_msg_b, msg: " << msg.msg;
  update_state(xfsm_state_ref(master, state_c), msg);
}

void master::dispatch(mmsg::master_msg_c const &msg) {
  xlogv << "got mmsg::master_msg_c, msg: " << msg.msg;
  //update_state(xfsm_state_ref(master, state_c), msg);
}

void master::dispatch(smsg::slave_msg_a const &msg) {
  xlogv << "got smsg::slave_msg_a";
  slave_ = msg.slave;
  update_state(xfsm_state_ref(master, state_b), msg);
}

void master::dispatch(smsg::slave_msg_b const &msg) {
  xlogv << "got smsg::slave_msg_b, msg: " << msg.msg;
  //update_state(xfsm_state_ref(master, state_b), msg);
}

void master::dispatch(smsg::slave_msg_c const &msg) {
  xlogv << "got smsg::slave_msg_c, msg: " << msg.msg;
  //update_state(xfsm_state_ref(master, state_c), msg);
}

/*
 * entry action
 */

void master::entry_state_b() {
  xlogv << "done";
  //oneself().send(cmsg::activate_instrument_arm());
}

/*
 * do action
 */
void master::do_state_a() {
  xlogv << "done";
}

void master::do_state_b() {
  static int count = 0;
  count++;

  if (count % 2 == 0) {
    slave_.send(mmsg::master_msg_b("state change to b"));
  } else {
    slave_.send(mmsg::master_msg_c("state change to c"));
  }

  if (count == 10) {
    oneself().send(mmsg::master_msg_b());
  }
}

void master::do_state_c() {
  xlogv << "done";
}

/*
 * exit action
 */
void master::exit_state_b() {
  xlogv << "done";
}

/*
 * local helper
 */
bool master::check_current_state() {
  if (prev_state_ == current_state_) {
    return true;
  } else {
    prev_state_ = current_state_;
    return false;
  }
}

