/*
 * Title      :   io_wrapper.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <xlog/xlog.h>
#include "hal/io/io_wrapper.h"
#include "modelx_signal.h"
            
io_t::io_t() {
  emergency_btn_sts_ = false;
  power_btn_sts_ = false;

  start_emul_ = false;
  exit_emul_ = true;
  emul_tick_ = 0;
  xlogv << "constructed";
}

io_t::~io_t() { xlogv << "destructed"; }

bool io_t::io_init() {
  return true;
}

bool io_t::io_deinit() {
  return true;
}

bool io_t::get_emergency_btn_sts() {
  return emergency_btn_sts_;
}

bool io_t::get_power_btn_sts() {
  return power_btn_sts_;
}

bool io_t::get_autopilot_btn_sts() {
  return autopilot_btn_sts_;
}



// emul
bool io_t::init_emul() {
#if ENABLE_INPUT_EMUL
  std::lock_guard<std::mutex> guard(emul_mutex_);
  exit_emul_ = false;

  if (emul_thread_.joinable()) {
    return true;
  }

  try {
    emul_thread_ = std::thread(std::bind(&io_t::emul_handler, this));
  } catch (std::system_error const& e) {
    xloge << "error coccur : " << e.what();
    return false;
  }
#endif  // !ENABLE_INPUT_EMUL

  xlogv << "done";
  return true;
}

void io_t::start_emul() {
#if ENABLE_INPUT_EMUL
  start_emul_ = true;
#else
  start_emul_ = true;
  exit_emul_ = true;
#endif  // !ENABLE_INPUT_EMUL
}

void io_t::deinit_emul() {
  {
    std::lock_guard<std::mutex> guard(emul_mutex_);
    start_emul_ = true;
    exit_emul_ = true;
  }
#if ENABLE_INPUT_EMUL
  if (emul_thread_.joinable()) {
    emul_thread_.join();
  }
#endif  // !ENABLE_INPUT_EMUL

  xlogv << "done";
}

void io_t::emul_handler() {
  static const int32_t period_ms = 500;
  static const int32_t multiple = 1;
  emul_tick_ = 0;

  while (!exit_emul_) {
  if (!start_emul_) {
    continue;
  }

  if (emul_tick_ == 1) {
    emergency_btn_sts_ = false;
    power_btn_sts_ = false;
    autopilot_btn_sts_ = false;
  } else if (emul_tick_ == (multiple * 2)) {
    autopilot_btn_sts_ = true;
    xlogi << "emul_tick_: " << emul_tick_ << ", autopilot button is on";
  } else if (emul_tick_ == (multiple * 3)) {
    autopilot_btn_sts_ = false;
    xlogi << "emul_tick_: " << emul_tick_ << ", autopilot button is off";
  } else if (emul_tick_ == (multiple * 4)) {
    emergency_btn_sts_ = true;
    xlogi << "emul_tick_: " << emul_tick_ << ", emergency button is on";
  } else if (emul_tick_ == (multiple * 5)) {
    emergency_btn_sts_ = false;
    xlogi << "emul_tick_: " << emul_tick_ << ", emergency button is off";
  } else if (emul_tick_ == (multiple * 6)) {
    autopilot_btn_sts_ = true;
    xlogi << "emul_tick_: " << emul_tick_ << ", autopilot button is on";
  } else if (emul_tick_ == (multiple * 7)) {
    autopilot_btn_sts_ = false;
    xlogi << "emul_tick_: " << emul_tick_ << ", autopilot button is off";
  } else if (emul_tick_ == (multiple * 8)) {
    power_btn_sts_ = true;
    xlogi << "emul_tick_: " << emul_tick_ << ", power btn is pressed";
  } else if (emul_tick_ > (multiple * 9)) {
    power_btn_sts_ = false;
    xlogi << "emul_tick_: " << emul_tick_ << ", wait for shutdown...";
    //emul_tick_ = 0;
  }

  emul_tick_++;
  std::this_thread::sleep_for(std::chrono::milliseconds(period_ms));
  } // !while
}
