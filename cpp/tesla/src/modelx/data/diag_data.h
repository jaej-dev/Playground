/*
 * Title      :   diag_data.h
 * Author     :   Jaewoong Jang 
 * Date       :   Jun 20th 2021
 */

#ifndef _DIAG_DATA_H_
#define _DIAG_DATA_H_

#include <string>
#include "data/vehicle_data.h"

class diag_data_t : public vehicle_data_t {
 public:
  typedef struct _diag_cmd {
    vehicle_data_t::vehicle_mode mode;
    vehicle_data_t::automation_option option;
  } diag_cmd;

  explicit diag_data_t(std::string const& name);
  diag_data_t();
  virtual ~diag_data_t();

  std::string get_name() const { return name_; }
  void set_name(std::string const& name) { name_ = name; }

 private:
  std::string name_;
};

#endif  // !_DIAG_DATA_H_
