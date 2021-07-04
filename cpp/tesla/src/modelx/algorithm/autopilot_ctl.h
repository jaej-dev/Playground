/*
 * Title      :   autopilot_ctl.h
 * Author     :   Jaewoong Jang   
 * Date       :   Jun 20th 2021
 */

#ifndef _AUTOPILOT_CTL_H_
#define _AUTOPILOT_CTL_H_

#include <memory>
#include <math.h>
#include "data/autopilot_data.h"
#include "hal/io/io_wrapper.h"
#include "hal/motor/motor_wrapper.h"
#include "ctl_common.h"

class autopilot_ctl_t {
 public:
  explicit autopilot_ctl_t(autopilot::autopilot_type, 
                      std::shared_ptr<io_t>,
                      std::shared_ptr<motor_t>, 
                      std::shared_ptr<autopilot_data_t>, uint32_t);

  ~autopilot_ctl_t();

  uint32_t do_autopilot_driving();
  void do_stop_autopilot();

 private:
  autopilot_ctl_t(autopilot_ctl_t const &);
  autopilot_ctl_t &operator=(autopilot_ctl_t const &);
  
  // data gathering
  void get_last_steering_data();

  // algorithm
  
 private:
  std::shared_ptr<io_t> io_;
  std::shared_ptr<motor_t> mt_;
  std::shared_ptr<autopilot_data_t> data_;
  autopilot::autopilot_type autopilot_type_;
  vehicle_data_t::vehicle_type vehicle_type_;

  // raw data
  uint32_t period_us_; 
  steering_data_t steering_data_;
  uint32_t tick_count_ = 0;
  bool last_steering_data_update_;
};

#endif  // !_AUTOPILOT_CTL_H_
