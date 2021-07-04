/*
 * Title      :   modelx_system.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _MODELX_SYSTEM_H_
#define _MODELX_SYSTEM_H_

#include <exception>
#include <xcfg/xcfg.h>
#include "modelx_signal.h"
#include "core/core_fsm.h"
#include "core/vehicle_behavior.h"
#include "data/exception_data.h"
#include "data/terminate_data.h"
#include "hal/io/io_wrapper.h"
#include "hal/motor/motor_wrapper.h"

class modelx_system_t {
 private:
  modelx_system_t(modelx_system_t const&);
  modelx_system_t& operator=(modelx_system_t const&);

 public:
  explicit modelx_system_t();
  ~modelx_system_t();

  static void init();
  static void deinit(terminate_data_t::type const&);

 private:
  static modelx_system_t& get_instance();

  void init_();
  void deinit_(terminate_data_t::type const&);

  std::shared_ptr<io_t> io_;
  std::shared_ptr<motor_t> mt_;
  std::shared_ptr<vehicle_behavior_t> rb_;
  std::shared_ptr<xcfg_t> cfg_;
  std::shared_ptr<core_fsm_t> machine_;
  std::shared_ptr<std::thread> machine_thread_;
};

#endif // !_MODELX_SYSTEM_H_
