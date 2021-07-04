/*
 * Title      :   autopilot_data.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _AUTOPILOT_DATA_H_
#define _AUTOPILOT_DATA_H_

#include <string>
#include <memory>
#include <cmath>
#include "data/vehicle_data.h"
#include "data/manual_data.h"

namespace autopilot {

enum autopilot_type { 
  unknown = -1,
  apx,
  apy,
  apz,
};

} // !namespace autopilot

class manual_data_t;
class autopilot_data_t : public vehicle_data_t {
 public:
  explicit autopilot_data_t(std::string const &name);
  autopilot_data_t();
  virtual ~autopilot_data_t();

  std::string get_name() const { return name_; }
  void set_name(std::string const& name) { name_ = name; }
  void set_manual_data(std::shared_ptr<manual_data_t>);
  std::shared_ptr<manual_data_t> get_manual_data();

 private:
  std::string name_;
  std::mutex mutex_;
  std::shared_ptr<manual_data_t> manual_data_;
};

#endif // !_AUTOPILOT_DATA_H_
