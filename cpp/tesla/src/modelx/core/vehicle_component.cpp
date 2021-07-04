/*
 * Title      :   vehicle_component.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <xlog/xlog.h>
#include "core/vehicle_component.h"

vehicle_component_t::vehicle_component_t()
    : thread_(nullptr),
      data_(nullptr),
      fsm_(nullptr),
      xml_path_(""),
      xsd_path_(""),
      vehicle_class_(vehicle_data_t::vehicle_class_max),
      created_(false),
      initialized_(false) {
  xlogv << "constructed";
}

vehicle_component_t::~vehicle_component_t() {
  thread_.reset();
  data_.reset();
  fsm_.reset();
  xlogv << "destructed";
}

vehicle_data_t::vehicle_class vehicle_component_t::get_vehicle_class() {
  return vehicle_class_;
}

void vehicle_component_t::set_vehicle_class(vehicle_data_t::vehicle_class rc) {
  vehicle_class_ = rc;
  xlogv << "class: " << vehicle_class_;
}

std::shared_ptr<std::thread> vehicle_component_t::get_thread() { return thread_; }

void vehicle_component_t::set_thread(std::shared_ptr<std::thread> thread) {
  thread_ = thread;
  xlogv << "thread: " << thread_;
}

std::shared_ptr<xfsm::base_state_machine_t> vehicle_component_t::get_fsm() {
  return fsm_;
}

void vehicle_component_t::set_fsm(
    std::shared_ptr<xfsm::base_state_machine_t> fsm) {
  fsm_ = fsm;
  xlogv << "fsm: " << fsm_;
}

xfsm::sender_t vehicle_component_t::get_sender() { return fsm_->oneself(); }

bool vehicle_component_t::get_creating_sts() { return created_; }

void vehicle_component_t::set_creating_sts(bool sts) { created_ = sts; }

bool vehicle_component_t::get_initializing_sts() { return initialized_; }

void vehicle_component_t::set_initializing_sts(bool sts) {
  initialized_ = sts;
}

bool vehicle_component_t::get_terminating_sts() { return terminated_; }

void vehicle_component_t::set_terminating_sts(bool sts) {
  terminated_ = sts;
}

void vehicle_component_t::set_data(std::shared_ptr<vehicle_data_t> data) {
  if (data == nullptr) {
    xloge << "data is nullptr";
  }
  data_ = data;
}

std::shared_ptr<vehicle_data_t> vehicle_component_t::get_data() {
  return std::dynamic_pointer_cast<vehicle_data_t>(data_);
}

std::shared_ptr<manual_data_t>
vehicle_component_t::get_manual_data() {
  return std::dynamic_pointer_cast<manual_data_t>(data_);
}

std::shared_ptr<autopilot_data_t> vehicle_component_t::get_autopilot_data() {
  return std::dynamic_pointer_cast<autopilot_data_t>(data_);
}

std::shared_ptr<diag_data_t> vehicle_component_t::get_diag_data() {
  return std::dynamic_pointer_cast<diag_data_t>(data_);
}

