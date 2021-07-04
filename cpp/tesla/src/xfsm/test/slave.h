
#pragma once

#include <xfsm/xfsm.h>
#include "message.h"

namespace mmsg = master_msg;
namespace smsg = slave_msg;

class slave : public xfsm::state_machine_t<slave> {
 public:
  explicit slave(xfsm::sender_t);
  ~slave();

 private:
  slave(slave const &);
  slave &operator=(slave const &);

 private:
  // declare state
  xfsm_declare_state(state_a);
  xfsm_declare_state(state_b);
  xfsm_declare_state(state_c);

  // dispatch
  void dispatch(smsg::slave_msg_a const &msg);
  void dispatch(smsg::slave_msg_b const &msg);
  void dispatch(smsg::slave_msg_c const &msg);
  void dispatch(xfsm::alarm const &msg);
  void dispatch(mmsg::master_msg_b const &msg);
  void dispatch(mmsg::master_msg_c const &msg);

  // entry action
  void entry_state_b();

  // do action
  void do_state_a();
  void do_state_b();
  void do_state_c();

  // exit action
  void exit_state_b();

  // local helper
  bool check_current_state();
  bool register_timer();

 private:
  xfsm::sender_t master_;
  state_t prev_state_, current_state_;
};
