/*
 * Title      :   exception_data.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _EXCEPTION_DATA_H_
#define _EXCEPTION_DATA_H_

class exception_data_t {
 public:
  enum type {
    no_exception = 0,
    unknown_fault,
    xcfg_fault,
    core_fsm_fault,
    mars_link_fault,
    diag_link_fault,
    manual_fsm_fault,
    manual_creation_fault,
    manual_initialization_fault,
    autopilot_fsm_fault,
    autopilot_motor_init_fault,
    autopilot_servo_on_fault,
    autopilot_servo_off_fault,
    autopilot_driving_fault,
    diag_fsm_fault,
    system_license_fault,
    os_fault,
    power_off_request,
    terminal_request,
    type_undefiend
  };

  static const char* to_string(exception_data_t::type const i) {
    static const char* const s[] = { 
                               "no_exception",
                               "unknown_fault",
                               "xcfg_fault",
                               "core_fsm_fault",
                               "mars_link_fault",
                               "diag_link_fault",
                               "manual_fsm_fault",
                               "manual_creation_fault",
                               "manual_initialization_fault",
                               "autopilot_fsm_fault",
                               "autopilot_motor_init_fault",
                               "autopilot_servo_on_fault",
                               "autopilot_servo_off_fault",
                               "autopilot_driving_fault",
                               "diag_fsm_fault",
                               "system_license_fault",
                               "os_fault",
                               "power_off_request",
                               "terminal_request",
                               "type_undefined"};
    return s[i];
  }

  void set_error(type error) { error_ = error; };
  type get_error() { return error_; }

 private:
  type error_;
};


#endif // !_EXCEPTION_DATA_H_
