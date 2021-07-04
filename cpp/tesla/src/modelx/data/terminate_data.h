/*
 * Title      :   terminate_data.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _TERMINATE_DATA_H_
#define _TERMINATE_DATA_H_


class terminate_data_t {
 public:
  enum type {
    normal = 0,
    abnormal,
    critical,
    debug,
    reboot_request,
    enforced_shutdown,
    factory_reset,
    type_undefiend
  };

  static const char* to_string(terminate_data_t::type const i) {
    static const char* const s[] = { "normal",
                                     "abnormal",   
                                     "critical",   
                                     "debug",      
                                     "reboot_request",
                                     "enforced_shutdown",
                                     "factory_reset",   
                                     "type_undefined" };
    return s[i];
  }

  void set_reason(type reason) { reason_ = reason; };
  type get_reason() { return reason_; }

 private:
  type reason_;
};

#endif // !_TERMINATE_DATA_H_
