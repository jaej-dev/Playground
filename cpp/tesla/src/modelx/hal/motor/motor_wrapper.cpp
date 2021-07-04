/*
 * Title      :   motor_wrapper.cpp
 * Author     :   Jaewoong Jang 
 * Date       :   Jun 20th 2021
 */

#include <xlog/xlog.h>
#include "hal/motor/motor_wrapper.h"
#include "modelx_signal.h"

motor_t::motor_t() { xlogv << "constructed"; }

motor_t::~motor_t() { xlogv << "destructed"; }

bool motor_t::motor_init(uint16_t chunk) {
  return true;
}

bool motor_t::motor_deinit(uint16_t chunk) {
  return true;
}

