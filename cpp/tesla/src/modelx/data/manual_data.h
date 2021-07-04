/*
 * Title      :   manual_data.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _HCD_DATA_H_
#define _HCD_DATA_H_

#include <string>
#include <memory>
#include <mutex>
#include <cmath>
#include "data/vehicle_data.h"

typedef struct _gripper_data_t {
  uint8_t status;
  uint16_t value;
} gripper_data_t;

typedef struct _steering_data_t {
  double_t px;
  double_t py;
  double_t pz;
  double_t oa;
  double_t ob;
  double_t og;
} steering_data_t;

class manual_data_t : public vehicle_data_t {
 public:
  enum manual_model {
    m1 = 0,
    m2,
    manual_model_max
  };

  enum manual_type {
    lft = 0,
    rgt,
    manual_type_max
  };

  explicit manual_data_t(std::string const& n, manual_model const m,
                      manual_type const t);
  manual_data_t();
  virtual ~manual_data_t();
 
  void set_model(manual_model const& m) { model_ = m; }
  manual_model get_model() { return model_; }

  void set_type(manual_type const& t) { type_ = t; }
  manual_type get_type() { return type_; }

  std::string get_name() const { return name_; }
  void set_name(std::string const& name) { name_ = name; }

  steering_data_t get_steering_data() const { return steering_data_; }
  void set_steering_data(steering_data_t const h);
  
 private:
  manual_model model_;
  manual_type type_;
  std::string name_;
  std::mutex mutex_;
  steering_data_t steering_data_;
};

#endif // !_HCD_DATA_H_
