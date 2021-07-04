/*
 * Title      :   autopilot_fsm.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _AUTOPILOT_FSM_H_
#define _AUTOPILOT_FSM_H_

#include <memory>
#include "core/core_fsm.h"
#include "core/core_msg.h"
#include "core/vehicle_behavior.h"
#include "data/autopilot_data.h"
#include "data/terminate_data.h"
#include "autopilot/autopilot_msg.h"
#include "algorithm/autopilot_ctl.h"
#include "hal/io/io_wrapper.h"
#include "hal/motor/motor_wrapper.h"

namespace anmsg = modelx_anmsg;
namespace cnmsg = modelx_cnmsg;

class vehicle_behavior_t;
class autopilot_data_t;
class autopilot_fsm_t : public xfsm::state_machine_t<autopilot_fsm_t> {
 public:
  explicit autopilot_fsm_t(xfsm::sender_t, 
                          std::shared_ptr<io_t>,
                          std::shared_ptr<motor_t>, 
                          std::shared_ptr<autopilot_data_t>);
  virtual ~autopilot_fsm_t();

 private:
  autopilot_fsm_t(autopilot_fsm_t const &);
  autopilot_fsm_t &operator=(autopilot_fsm_t const &);

 private:
  xfsm_declare_state(idle);          // 301
  xfsm_declare_state(creating);      // 302
  xfsm_declare_state(initializing);  // 303
  xfsm_declare_state(pending);       // 304
  xfsm_declare_state(active);        // 306
  xfsm_declare_state(emergency);     // 311
  xfsm_declare_state(exception);     // 313
  xfsm_declare_state(shutdown);      // 314
  xfsm_declare_state(terminating);   // 315

  void dispatch(anmsg::start const &msg);
  void dispatch(anmsg::created const &msg);
  void dispatch(anmsg::initialized const &msg);
  void dispatch(cnmsg::start_autopilot_driving const &msg);
  void dispatch(cnmsg::stop_autopilot_driving const &msg);
  void dispatch(cnmsg::start_emergency const &msg);
  void dispatch(cnmsg::stop_emergency const &msg);
  void dispatch(cnmsg::forced_break_subsystem const &msg);
  void dispatch(anmsg::throw_exception const &msg);
  void dispatch(cnmsg::shutdown_subsystem const &msg);
  void dispatch(anmsg::terminate const &msg);

  // entry action
  void entry_pending();
  void entry_active();
  void entry_emergency();

  // do action
  bool do_idle();
  bool create_control_instance();
  bool do_exception();
  bool do_shutdown();
  bool do_terminating();

  // exit action
  void exit_active();
  void exit_emergency();

  // motor
  bool motor_init();
  bool motor_deinit();
  bool servo_on();
  bool servo_off();
  
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
  bool driving_fl_;
  bool motor_fatal_fl_;
  std::shared_ptr<io_t> io_;
  std::shared_ptr<motor_t> mt_;
  std::shared_ptr<autopilot_data_t> data_;
  std::shared_ptr<autopilot_ctl_t> ctl_;
};

#endif  // !_AUTOPILOT_FSM_H_
