/*
 * Title      :   core_fsm.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _CORE_FSM_H_
#define _CORE_FSM_H_

#include <xcfg/xcfg.h>
#include "core/core_msg.h"
#include "core/vehicle_behavior.h"
#include "data/exception_data.h"
#include "data/terminate_data.h"
#include "manual/manual_msg.h"
#include "autopilot/autopilot_msg.h"
#include "diag/diag_msg.h"
#include "hal/io/io_wrapper.h"
#include "hal/motor/motor_wrapper.h"

namespace cnmsg = modelx_cnmsg;
namespace mnmsg = modelx_mnmsg;
namespace anmsg = modelx_anmsg;
namespace dnmsg = modelx_dnmsg;

class vehicle_behavior_t;
class core_fsm_t : public xfsm::state_machine_t<core_fsm_t> {
 public:
  explicit core_fsm_t(std::shared_ptr<vehicle_behavior_t>,
                      std::shared_ptr<io_t>,
                      std::shared_ptr<motor_t>,
                      std::shared_ptr<xcfg_t>);
  virtual ~core_fsm_t();

 private:
  core_fsm_t(core_fsm_t const &);
  core_fsm_t &operator=(core_fsm_t const &);

 private:
  // declare state
  xfsm_declare_state(idle);             // 101
  xfsm_declare_state(creating);         // 102 
  xfsm_declare_state(initializing);     // 103
  xfsm_declare_state(pending);          // 104
  xfsm_declare_state(manual);           // 105
  xfsm_declare_state(autopilot);        // 108
  xfsm_declare_state(exception);        // 113
  xfsm_declare_state(power_off);        // 114
  xfsm_declare_state(emergency);        // 115
  xfsm_declare_state(shutdown);         // 117
  xfsm_declare_state(terminating);      // 118

  // dispatch
  void dispatch(cnmsg::start_vehicle const &msg);
  void dispatch(cnmsg::created const &msg);
  void dispatch(cnmsg::initialized const &msg);
  void dispatch(cnmsg::start_manual_driving const &msg);
  void dispatch(cnmsg::stop_manual_driving const &msg);
  void dispatch(cnmsg::start_diag const &msg);
  void dispatch(cnmsg::stop_diag const &msg);
  void dispatch(dnmsg::connected const &msg);
  void dispatch(dnmsg::disconnected const &msg);
  void dispatch(cnmsg::start_autopilot_driving const &msg);
  void dispatch(cnmsg::stop_autopilot_driving const &msg);
  void dispatch(cnmsg::throw_exception const &msg);
  void dispatch(cnmsg::start_emergency const &msg);
  void dispatch(cnmsg::stop_emergency const &msg);
  void dispatch(cnmsg::start_power_off const &msg);
  void dispatch(cnmsg::cancel_power_off const &msg);
  void dispatch(cnmsg::shutdown_vehicle const &msg);
  void dispatch(cnmsg::terminate const &msg);
  void dispatch(mnmsg::exception_raised const &msg);
  void dispatch(anmsg::exception_raised const &msg);
  void dispatch(dnmsg::exception_raised const &msg);

  // entry action
  void entry_creating();
  void entry_pending();
  void entry_manual();
  void entry_autopilot();
  void entry_emergency();
  void entry_power_off();
  void entry_exception();
  void entry_terminating();

  // do action
  void check_event_on_pending();
  void check_event_on_manual();
  void check_event_on_autopilot();
  void check_event_on_power_off();
  void shutdown_system();
  void terminate_system();

  // exit action
  void exit_initializing();
  void exit_manual();
  void exit_autopilot();
  

  // local helper
  void load_xcfg();
  void store_xcfg();
  void battery_validation();
  bool update_battery_usage();
  bool check_current_state();
  void gen_os_signal();

 private:
  std::shared_ptr<vehicle_behavior_t> vehicle_;
  std::shared_ptr<io_t> io_;
  std::shared_ptr<motor_t> mt_;
  std::shared_ptr<xcfg_t> cfg_;
  state_t prev_state_;
  state_t current_state_;
  exception_data_t last_error_;
  terminate_data_t terminate_reason_;
  bool generated_os_signal_;
  bool xcfg_fault_fl_;
  uint32_t period_us_;
  std::chrono::system_clock::time_point power_off_start_tp_;
  std::chrono::system_clock::time_point power_off_cancel_tp_;
  std::chrono::system_clock::time_point autopilot_btn_start_tp_;
  std::chrono::system_clock::time_point autopilot_btn_stop_tp_;
  uint32_t power_off_detect_time_;
  uint32_t power_off_cancel_time_;
  uint32_t autopilot_btn_dbnc_time_;
  xfsm::ptimer_t period_tick_;
  exception_data_t exception_type_;
  uint32_t tick_count_;
  diag_data_t::diag_cmd diag_cmd_;
  bool boot_up_;
  bool debug_shutdown_fl_;
  bool spin_fl_;
  bool emergency_fl_;
  bool power_off_fl_;
  bool autopilot_detached_;
  bool battery_validation_fl_;
  bool autopilot_driving_fl_;
  bool autopilot_first_using_fl_;
  bool mars_link_fl_;
  bool diag_link_fl_;
  uint32_t autopilot_type_;
  bool vehicle_rcvr_fl_;
  bool battery_validation_cfg_;
  uint32_t battery_usage_threshold_ms_cfg_;
  uint32_t battery_usage_limit_cfg_;
  uint32_t autopilot_lifespan_threshold_ms_cfg_;
  uint32_t autopilot_lifespan_limit_cfg_;
  std::chrono::system_clock::time_point vehicle_start_tp_;
  std::chrono::system_clock::time_point autopilot_start_tp_;
};

#endif // !_CORE_FSM_H_
