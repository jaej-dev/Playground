/*
 * Title      :   vehicle_data.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _VEHICLE_DATA_H_
#define _VEHICLE_DATA_H_

#include <mutex>
#include <string>
#include "data/base_data.h"


class vehicle_data_t : public base_data_t {
 public:

  enum product_type {
    modelx = 0,
    modely,
    product_type_max
  };
  
  static const char* to_string(product_type const i) {
    static const char* const s[] = { 
                               "modelx", 
                               "modely", 
                               "product_type_max" };
    return s[i];
  };

  enum vehicle_type {
    dual = 0,
    tri,
    vehicle_model_max
  };

  static const char* to_string(vehicle_type const i) {
    static const char* const s[] = { 
                               "dual", 
                               "tri", 
                               "vehicle_type_max" };
    return s[i];
  };

  enum vehicle_class {
    manual = 0,
    autopilot,
    diag,
    vehicle_class_max
  };

  static const char* to_string(vehicle_class const i) {
    static const char* const s[] = { 
                               "manual", 
                               "autopilot", 
                               "diag",
                               "vehicle_class_max" };
    return s[i];
  };

  enum vehicle_mode { 
    normal = 0, 
    maintenance, 
    automation
  };

  enum automation_option {
    aut_opt_none      = 0x00000000,
    aut_opt_manual    = 0x00000001,
    aut_opt_autopilot = 0x00000100,
    aut_opt_all       = 0x00000111,
    aut_opt_mask      = 0xFFFFFFFF
  };

 public:
  explicit vehicle_data_t(std::string const& name);
  vehicle_data_t();
  virtual ~vehicle_data_t();

  std::string get_name() const { return name_; }
  void set_name(std::string const& name) { name_ = name; }

 private:
  std::string name_;
  mutable std::mutex mutex_;
};

#endif // !_VEHICLE_BASE_DATA_H_
