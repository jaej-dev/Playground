/*
 * Title      :   vehicle_behavior.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _ROBOT_BEHAVIOR_H_
#define _ROBOT_BEHAVIOR_H_

#include <memory>
#include <vector>
#include "core/core_fsm.h"
#include "core/core_msg.h"
#include "core/vehicle_component.h"
#include "manual/manual_fsm.h"
#include "manual/manual_msg.h"
#include "autopilot/autopilot_fsm.h"
#include "autopilot/autopilot_msg.h"
#include "diag/diag_fsm.h"
#include "diag/diag_msg.h"
#include "data/core_data.h"
#include "hal/io/io_wrapper.h"
#include "hal/motor/motor_wrapper.h"

class core_fsm_t;
class manual_fsm_t;
class autopilot_fsm_t;
class diag_fsm_t;
class vehicle_behavior_t {
 public:
  vehicle_behavior_t();
  ~vehicle_behavior_t();

  // create and initialize
  bool creating();
  bool matching();
  bool check_creating_sts(xfsm::sender_t);
  bool check_creating_sts(std::string);

  // outside fsm 
  bool start_manual_driving();
  bool stop_manual_driving();

  bool start_diag();
  bool stop_diag();
  
  // driving
  bool start_autopilot_driving();
  bool stop_autopilot_driving();

  bool start_emergency();
  bool stop_emergency();

  bool start_power_off();
  bool forced_break_subsystem();
  
  // data gathering
  std::shared_ptr<manual_data_t> get_manual_data();
  std::shared_ptr<autopilot_data_t> get_autopilot_data();
  std::shared_ptr<diag_data_t> get_diag_data();

  // helper 
  xfsm::sender_t get_core_fsm();
  bool set_core_fsm(xfsm::sender_t);
  std::shared_ptr<io_t> get_io_wrapper();
  bool set_io_wrapper(std::shared_ptr<io_t>);
  std::shared_ptr<motor_t> get_motor_wrapper();
  bool set_motor_wrapper(std::shared_ptr<motor_t>);
  bool check_state(std::string state);
  
  // deinitialize
  bool shutdown_subsystem();
  void destroy();

 private:
  xfsm::sender_t core_fsm_;
  std::shared_ptr<io_t> io_;
  std::shared_ptr<motor_t> mt_;
  std::vector<std::shared_ptr<vehicle_component_t> > vehicle_components_;
  uint32_t prio_;
};

#endif // !_ROBOT_BEHAVIOR_H_
