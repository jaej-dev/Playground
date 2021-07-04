/*
 * Title      :   manual_data.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <xlog/xlog.h>
#include "data/manual_data.h"

manual_data_t::manual_data_t(std::string const &n, manual_model const m,
                       manual_type const t) {
  this->set_name(n);
  this->set_model(m);
  this->set_type(t);
  xlogv << "constructed, " << get_name();
}

manual_data_t::manual_data_t() {
  xlogv << "constructed";
}

manual_data_t::~manual_data_t() {
  xlogv << "destructed";
}

void manual_data_t::set_steering_data(steering_data_t const h) {
  std::lock_guard<std::mutex> const lk(mutex_);
  steering_data_ = h; 
}
