/*
 * Title      :   autopilot_ctl.cpp
 * Author     :   Jaewoong Jang   
 * Date       :   Jun 20th 2021
 */

#include <functional>
#include <iostream>
#include <chrono>
#include <xlog/xlog.h>
#include <modelx_feature.h>
#include "autopilot_ctl.h"


autopilot_ctl_t::autopilot_ctl_t(autopilot::autopilot_type w, 
                       std::shared_ptr<io_t> i,
                       std::shared_ptr<motor_t> m,
                       std::shared_ptr<autopilot_data_t> d, uint32_t t)
    : autopilot_type_(w), io_(i), mt_(m), data_(d), period_us_(t) {
  tick_count_ = 0;
  last_steering_data_update_ = true;
  xlogv << "constructed";

}

autopilot_ctl_t::~autopilot_ctl_t() {
  xlogv << "destructed";
}

uint32_t autopilot_ctl_t::do_autopilot_driving() {
  uint32_t ret = 0;
  auto now = std::chrono::system_clock::now();
  
  // update steering data
  if (last_steering_data_update_) {
    last_steering_data_update_ = false;
    get_last_steering_data();
  }

  // to write algorithm here
  
  // check spent time
  auto finish = std::chrono::system_clock::now();
  auto delta = std::chrono::duration_cast<std::chrono::microseconds>(
                   finish - now).count();
  if (delta > period_us_) {
    xloge << "algorithm took longer than " << period_us_
         << "us, spent time: " << delta << "us";
  } else {
    xlogv << "done, spent time: " << delta << "us";
  }
  return ret;
}

void autopilot_ctl_t::do_stop_autopilot() {
  last_steering_data_update_ = true;
}
// internal
void autopilot_ctl_t::get_last_steering_data() {
  steering_data_ = data_->get_manual_data()->get_steering_data();
  xlogv << "px: " << steering_data_.px;
  xlogv << "py: " << steering_data_.py;
  xlogv << "pz: " << steering_data_.pz;
  xlogv << "oa: " << steering_data_.oa;
  xlogv << "ob: " << steering_data_.ob;
  xlogv << "og: " << steering_data_.og;
}

