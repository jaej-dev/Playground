/*
 * Title      :   diag_fsm.h
 * Author     :   Jaewoong Jang 
 * Date       :   Jun 20th 2021
 */

#ifndef _DIAG_FSM_H_
#define _DIAG_FSM_H_

#include <memory>
#include <xfsm/xfsm.h>
#include "core/core_fsm.h"
#include "core/core_msg.h"
#include "core/vehicle_behavior.h"
#include "data/diag_data.h"
#include "data/terminate_data.h"
#include "diag/diag_msg.h"
//#include "diag/diag_server.h"

namespace dnmsg = modelx_dnmsg;
namespace cnmsg = modelx_cnmsg;

class vehicle_behavior_t;
class diag_data_t;
class diag_fsm_t : public xfsm::state_machine_t<diag_fsm_t> {
 public:
  explicit diag_fsm_t(xfsm::sender_t,
                      std::shared_ptr<diag_data_t>);
  virtual ~diag_fsm_t();


 private:
  diag_fsm_t(diag_fsm_t const &);
  diag_fsm_t &operator=(diag_fsm_t const &);

 private:
  xfsm_declare_state(idle);          // 801
  xfsm_declare_state(creating);      // 802
  xfsm_declare_state(initializing);  // 803
  xfsm_declare_state(pending);       // 804
  xfsm_declare_state(waiting);       // 805
  xfsm_declare_state(active);        // 806
  xfsm_declare_state(exception);     // 807
  xfsm_declare_state(shutdown);      // 808
  xfsm_declare_state(terminating);   // 809

  void dispatch(dnmsg::start const &msg);
  void dispatch(dnmsg::created const &msg);
  void dispatch(dnmsg::initialized const &msg);
  void dispatch(cnmsg::start_diag const &msg);
  void dispatch(cnmsg::stop_diag const &msg);
  void dispatch(dnmsg::connected const &msg);
  void dispatch(dnmsg::disconnected const &msg);
  void dispatch(dnmsg::throw_exception const &msg);
  void dispatch(cnmsg::shutdown_subsystem const &msg);
  void dispatch(dnmsg::terminate const &msg);

  bool do_idle();
  bool do_creating();
  bool do_initializing();
  bool wait_for_client();
  bool do_active();
  bool do_exception();
  bool do_shutdown();
  bool do_terminating();

  bool check_current_state();

 private:
  xfsm::sender_t core_fsm_;
  state_t prev_state_;
  state_t current_state_;
  exception_data_t last_error_;
  uint32_t period_us_;
  xfsm::ptimer_t period_tick_;
  exception_data_t exception_type_;
  uint32_t tick_count_;
  std::shared_ptr<diag_data_t> diag_data_;
};

#endif  // !_DIAG_FSM_H_
