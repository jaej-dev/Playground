/*
 * Title      :   io_wrapper.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _IO_WRAPPER_H_
#define _IO_WRAPPER_H_

#include <thread>
#include <memory>
#include <mutex>
#include <functional>
#include <cmath>
#include "data/manual_data.h"
#include "data/autopilot_data.h"
#include "modelx_feature.h"

class io_t {
 public:
  enum scene {
    off = 0,
    on,
  };

  io_t();
  virtual ~io_t();

  bool io_init();
  bool io_deinit();

  bool get_emergency_btn_sts();
  bool get_rcvr_surg_btn_sts();
  bool get_power_btn_sts();
  bool get_autopilot_btn_sts();

  // emul
  bool init_emul();
  void deinit_emul();
  void start_emul();
  void emul_handler();

 private:
  std::thread emul_thread_;
  bool start_emul_;
  bool exit_emul_;
  std::mutex mutex_;
  std::mutex emul_mutex_;
  uint32_t emul_tick_;

  bool emergency_btn_sts_;
  bool power_btn_sts_;
  bool autopilot_btn_sts_;
};

#endif // !_IO_WRAPPER_H_
