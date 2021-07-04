/*
 * Title      :   modelx_system.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <functional>
#include <xlog/xlog.h>
#include "modelx_signal.h"
#include "modelx_system.h"

modelx_system_t::modelx_system_t() : machine_thread_() {}

modelx_system_t::~modelx_system_t() {}

modelx_system_t& modelx_system_t::get_instance() {
  static modelx_system_t sys;
  return sys;
}

void modelx_system_t::init() { get_instance().init_(); }

void modelx_system_t::deinit(terminate_data_t::type const& type) {
  get_instance().deinit_(type);
}

void modelx_system_t::init_() {
  try {
    io_t* const hmem = static_cast<io_t*>(operator new(sizeof(io_t)));
    io_.reset(new (hmem) io_t());

    motor_t* const tmem = static_cast<motor_t*>(operator new(sizeof(motor_t)));
    mt_.reset(new (tmem) motor_t());

    if (!io_->io_init()) {
      xloge << "hardware initialization failure\n";
      tesla::gen_signal_usr2();
      return;
    }

    vehicle_behavior_t* const bmem =
        static_cast<vehicle_behavior_t*>(operator new(
            sizeof(vehicle_behavior_t)));
    rb_.reset(new (bmem) vehicle_behavior_t());

    xcfg_t* const xmem = static_cast<xcfg_t*>(operator new(sizeof(xcfg_t)));
    cfg_.reset(new (xmem) xcfg_t());

    core_fsm_t* const cmem =
        static_cast<core_fsm_t*>(operator new(sizeof(core_fsm_t)));
    machine_.reset(new (cmem) core_fsm_t(rb_, io_, mt_, cfg_));

    std::error_code err;
#ifdef ARCH_QNX
    machine_thread_ = xfsm_get_thread_prio(core_fsm_t, machine_, err, 20);
#else
    machine_thread_ = xfsm_get_thread(core_fsm_t, machine_, err);
#endif
    if (machine_ == nullptr) {
      std::cout << __func__ << ": machine_ is nullptr\n";
    }
    if (err.message() != "success") {
      std::cout << __func__ << ": error = " << err.message() << "\n";
    }

    if (!machine_thread_) {
      xloge << "get_thread failure. error value: " << err.value()
            << ", message: " << err.message().c_str();
    } else {
      xlogv << "thread created, thread: " << machine_thread_;
    }
  } catch (std::exception const& e) {
    std::cout << e.what() << "\n";
    tesla::gen_signal_term();
  }
}

void modelx_system_t::deinit_(terminate_data_t::type const& type) {
  switch (type) {
    case terminate_data_t::normal: {
      machine_->oneself().send(cnmsg::shutdown_vehicle());
      break;
    }
    case terminate_data_t::abnormal: {
      machine_->oneself().send(cnmsg::shutdown_vehicle());
      break;
    }
    case terminate_data_t::critical: {
      machine_->oneself().send(cnmsg::shutdown_vehicle());
      break;
    }
    case terminate_data_t::debug: {
      machine_->oneself().send(
          cnmsg::throw_exception(exception_data_t::terminal_request));
      break;
    }
    default: {
      machine_->oneself().send(
          cnmsg::throw_exception(exception_data_t::os_fault));
      break;
    }
  }

  xlogd << "reset core fsm...";
  if (machine_thread_->joinable()) {
    machine_thread_->join();
    machine_thread_.reset();
    machine_.reset();
  }

  io_->io_deinit();

  rb_.reset();

  xlogd << "reset motor wrapper...";
  mt_.reset();

  xlogd << "reset io wrapper...";
  io_.reset();

  xlogv << "done...";
}
