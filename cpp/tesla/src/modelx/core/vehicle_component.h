/*
 * Title      :   vehicle_component.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _ROBOT_COMPONENT_H_
#define _ROBOT_COMPONENT_H_

#include <cstring>
#include <xfsm/xfsm.h>
#include <iostream>
#include <memory>
#include <vector>
#include "data/core_data.h"

class manual_data_t;
class autopilot_data_t;
class vehicle_component_t {
 public:
  vehicle_component_t();
  ~vehicle_component_t();

  vehicle_data_t::vehicle_class get_vehicle_class();
  void set_vehicle_class(vehicle_data_t::vehicle_class rc);
  std::shared_ptr<std::thread> get_thread();
  void set_thread(std::shared_ptr<std::thread> thread);
  std::shared_ptr<xfsm::base_state_machine_t> get_fsm();
  void set_fsm(std::shared_ptr<xfsm::base_state_machine_t> fsm);
  xfsm::sender_t get_sender();
  bool get_creating_sts();
  void set_creating_sts(bool sts);
  bool get_initializing_sts();
  void set_initializing_sts(bool sts);
  bool get_terminating_sts();
  void set_terminating_sts(bool sts);
  void set_data(std::shared_ptr<vehicle_data_t> data);
  std::shared_ptr<vehicle_data_t> get_data();
  std::shared_ptr<manual_data_t> get_manual_data();
  std::shared_ptr<autopilot_data_t> get_autopilot_data();
  std::shared_ptr<diag_data_t> get_diag_data();

 private:
  std::shared_ptr<std::thread> thread_;
  std::shared_ptr<vehicle_data_t> data_;
  std::shared_ptr<xfsm::base_state_machine_t> fsm_;
  std::string xml_path_;
  std::string xsd_path_;
  vehicle_data_t::vehicle_class vehicle_class_;
  bool created_;
  bool initialized_;
  bool terminated_;
};

#endif // !_ROBOT_COMPONENT_H_
