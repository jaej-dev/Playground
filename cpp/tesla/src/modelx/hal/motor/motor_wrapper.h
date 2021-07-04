/*
 * Title      :   motor_wrapper.h
 * Author     :   Jaewoong Jang 
 * Date       :   Jun 20th 2021
 */

#ifndef _MOTOR_WRAPPER_H_
#define _MOTOR_WRAPPER_H_

#include "data/manual_data.h"
#include "data/autopilot_data.h"
#include "modelx_feature.h"

class motor_t {
 public:
  enum motor_chunk {
    autopilot = 0,
  };

  motor_t();
  virtual ~motor_t();

  // motor general internface without motor control
  bool motor_init(uint16_t chunk);
  bool motor_deinit(uint16_t chunk);

 private:

};

#endif  // !_MOTOR_WRAPPER_H_
