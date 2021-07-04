/*
 * Title      :   autopilot_data.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <xlog/xlog.h>
#include "data/autopilot_data.h"

autopilot_data_t::autopilot_data_t(std::string const &name) {
  set_name(name);
  manual_data_ = nullptr;
  xlogv << "constructed, " << get_name();
}

autopilot_data_t::autopilot_data_t() {
  xlogv << "constructed";
}

autopilot_data_t::~autopilot_data_t() {
  xlogv << "destructed";
}

void autopilot_data_t::set_manual_data(std::shared_ptr<manual_data_t> data) {
  std::lock_guard<std::mutex> const lk(mutex_);
  if (data == nullptr) {
    manual_data_ = nullptr;
    xloge << "autopilot data is nullptr";
  } else {
    manual_data_ = data;
    xlogv << "autopilot data name is " << manual_data_->get_name();
  }
}

std::shared_ptr<manual_data_t> autopilot_data_t::get_manual_data() {
  return manual_data_;
}

