/*
 * Title      :   core_fsm.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <functional>
#include <iostream>
#include <chrono>
#include <xlog/xlog.h>
#include "core/core_fsm.h"
#include "core/core_msg.h"
#include "modelx_signal.h"

core_fsm_t::core_fsm_t(std::shared_ptr<vehicle_behavior_t> rb,
                       std::shared_ptr<io_t> io,
                       std::shared_ptr<motor_t> mt,
                       std::shared_ptr<xcfg_t> cfg)
    : xfsm_set_init_state(core_fsm_t, idle),
      vehicle_(rb),
      io_(io),
      mt_(mt),
      cfg_(cfg) {
  xfsm_entry_action(core_fsm_t, creating,
                    std::bind(&core_fsm_t::entry_creating, this));
  xfsm_entry_action(core_fsm_t, pending,
                    std::bind(&core_fsm_t::entry_pending, this));
  xfsm_entry_action(core_fsm_t, manual,
                    std::bind(&core_fsm_t::entry_manual, this));
  xfsm_entry_action(core_fsm_t, autopilot, 
                    std::bind(&core_fsm_t::entry_autopilot, this));
  xfsm_entry_action(core_fsm_t, emergency,
                    std::bind(&core_fsm_t::entry_emergency, this));
  xfsm_entry_action(core_fsm_t, power_off,
                    std::bind(&core_fsm_t::entry_power_off, this));
  xfsm_entry_action(core_fsm_t, exception,
                    std::bind(&core_fsm_t::entry_exception, this));
  xfsm_entry_action(core_fsm_t, terminating,
                    std::bind(&core_fsm_t::entry_terminating, this));
  xfsm_exit_action(core_fsm_t, initializing,
                    std::bind(&core_fsm_t::exit_initializing, this));
  xfsm_exit_action(core_fsm_t, manual, 
                    std::bind(&core_fsm_t::exit_manual, this));
  xfsm_exit_action(core_fsm_t, autopilot, 
                    std::bind(&core_fsm_t::exit_autopilot, this));

  xfsm_enable_state(core_fsm_t, idle);
  xfsm_enable_state(core_fsm_t, creating);
  xfsm_enable_state(core_fsm_t, initializing);
  xfsm_enable_state(core_fsm_t, pending);
  xfsm_enable_state(core_fsm_t, autopilot);
  xfsm_enable_state(core_fsm_t, exception);
  xfsm_enable_state(core_fsm_t, power_off);
  xfsm_enable_state(core_fsm_t, emergency);
  xfsm_enable_state(core_fsm_t, shutdown);
  xfsm_enable_state(core_fsm_t, terminating);

  vehicle_start_tp_ = std::chrono::system_clock::now();
  boot_up_ = true;
  period_us_ = 50000;  // microseconds, 50ms
  tick_count_ = 0;
  power_off_detect_time_ = 1000;     // ms
  power_off_cancel_time_ = 5000;   // ms
  autopilot_btn_dbnc_time_ = 500;     // ms
  generated_os_signal_ = false;
  emergency_fl_ = false;
  power_off_fl_ = false;
  autopilot_detached_ = false;
  battery_validation_fl_ = false;
  autopilot_driving_fl_ = false;
  autopilot_first_using_fl_ = true;
  mars_link_fl_ = false;
  diag_link_fl_ = true;
  diag_cmd_.mode = vehicle_data_t::normal;
  diag_cmd_.option = vehicle_data_t::aut_opt_none;
  autopilot_type_ = autopilot::unknown;
  
  xcfg_fault_fl_ = false;
  vehicle_rcvr_fl_ = 0;
  battery_validation_cfg_ = true;
  battery_usage_threshold_ms_cfg_ = 60000; // 10 minutes
  battery_usage_limit_cfg_ = 10;
  autopilot_lifespan_threshold_ms_cfg_ = 10800000;  // 3 hours
  autopilot_lifespan_limit_cfg_ = 108000000; // 30 hours

  vehicle_->set_core_fsm(oneself());
  vehicle_->set_io_wrapper(io_);

  if (cfg_->init()) {
    this->load_xcfg();
    xcfg_fault_fl_ = false;
    // for enforced shutdown condition
    last_error_.set_error(exception_data_t::unknown_fault);
    terminate_reason_.set_reason(terminate_data_t::enforced_shutdown);
    cfg_->set_last_error(last_error_.get_error());
    cfg_->set_last_error_string(last_error_.to_string(last_error_.get_error()));
    cfg_->set_power_off_reason(terminate_reason_.get_reason());
    cfg_->set_power_off_reason_string(
      terminate_reason_.to_string(terminate_reason_.get_reason()));
  } else {
    xcfg_fault_fl_ = true;
  }

#if ENABLE_INPUT_EMUL
  io_->init_emul();  // to test
#endif               // !EANBLE_EMULATION
  

  xlogv << "constructed: " << this;
}

core_fsm_t::~core_fsm_t() {
  xlogv << "destructed";
}

/*
 * states of fsm
 */
xfsm_state_of(core_fsm_t, idle) {
  // do action
  store_current_state(prev_state_);
  store_current_state(current_state_);

  xlogd << "entered idle state, waiting to start vehicle";

  // do hardware something init 
  
  xlogd << "prepared hardware, start vehicle...";
  oneself().send(cnmsg::start_vehicle());

  mesg()
      .wait(std::chrono::milliseconds(500))
      .handle(dispatch, cnmsg::start_vehicle)
      .handle(dispatch, cnmsg::throw_exception);
}

xfsm_state_of(core_fsm_t, creating) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered creating state";
  }

  if (vehicle_->check_state("pending")) {
    xlogv << "send cnmsg::created";
    oneself().send(cnmsg::created());
  } else {
    xlogv << "subsystem does not the pending state";
  }

  mesg()
      .wait(std::chrono::milliseconds(500))
      .handle(dispatch, cnmsg::created)
      .handle(dispatch, mnmsg::exception_raised)
      .handle(dispatch, anmsg::exception_raised)
      .handle(dispatch, dnmsg::exception_raised)
      .handle(dispatch, cnmsg::throw_exception);
}

xfsm_state_of(core_fsm_t, initializing) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered initializing state";
  }

  static bool oneshot = false;
  if (!oneshot) {
    oneshot = true;
    if (vehicle_->matching()) {
      vehicle_->start_diag();
    }
    oneself().send(cnmsg::initialized());  // ori
  }

  mesg()
      .wait(std::chrono::milliseconds(500))
      .handle(dispatch, cnmsg::initialized)
      //.handle(dispatch, cnmsg::start_emergency)
      //.handle(dispatch, cnmsg::start_power_off)
      .handle(dispatch, mnmsg::exception_raised)
      .handle(dispatch, anmsg::exception_raised)
      .handle(dispatch, dnmsg::exception_raised)
      .handle(dispatch, cnmsg::throw_exception);
}

xfsm_state_of(core_fsm_t, pending) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered pending state";
  }

  this->check_event_on_pending();

  bool oneshot = false;
  if (!oneshot) {
    oneshot = true;
    oneself().send(cnmsg::start_manual_driving());
  }

  mesg()
      .wait(std::chrono::microseconds(period_us_))
      .handle(dispatch, cnmsg::start_emergency)
      .handle(dispatch, cnmsg::start_power_off)
      .handle(dispatch, cnmsg::start_manual_driving)
      .handle(dispatch, cnmsg::start_autopilot_driving)
      .handle(dispatch, mnmsg::exception_raised)
      .handle(dispatch, anmsg::exception_raised)
      .handle(dispatch, dnmsg::exception_raised)
      .handle(dispatch, cnmsg::throw_exception);
}

xfsm_state_of(core_fsm_t, manual) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered manual state";
  }

  this->check_event_on_manual();

  mesg()
      .wait(std::chrono::microseconds(period_us_))
      .handle(dispatch, cnmsg::start_emergency)
      .handle(dispatch, cnmsg::start_power_off)
      .handle(dispatch, cnmsg::start_autopilot_driving)
      .handle(dispatch, mnmsg::exception_raised)
      .handle(dispatch, anmsg::exception_raised)
      .handle(dispatch, dnmsg::exception_raised)
      .handle(dispatch, cnmsg::throw_exception);
}

xfsm_state_of(core_fsm_t, autopilot) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered autopilot state";
  }

  this->check_event_on_autopilot();

  mesg()
      .wait(std::chrono::microseconds(period_us_))
      .handle(dispatch, cnmsg::start_emergency)
      .handle(dispatch, cnmsg::start_power_off)
      .handle(dispatch, cnmsg::stop_autopilot_driving)
      .handle(dispatch, mnmsg::exception_raised)
      .handle(dispatch, anmsg::exception_raised)
      .handle(dispatch, dnmsg::exception_raised)
      .handle(dispatch, cnmsg::throw_exception);
}

xfsm_state_of(core_fsm_t, exception) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered exception state";
  }

#if 0
  this->gen_os_signal();

  mesg().wait().handle(dispatch, cnmsg::shutdown_vehicle);
#endif
#if 1
  static bool oneshot = false;
  if (last_error_.get_error() == exception_data_t::power_off_request) {
    if (!oneshot) {
      this->gen_os_signal();
      oneshot = true;
    }
  } else {
#if ENABLE_INPUT_EMUL
    if (!oneshot) {
      this->gen_os_signal();
      oneshot = true;
    }
#else
    if (io_->get_power_btn_sts()) {
      xlogw << "power btn is on, start shutdown procedure...";
      if (!oneshot) {
        this->gen_os_signal();
        oneshot = true;
      }
    }
#endif
  }

  mesg()
      .wait(std::chrono::microseconds(period_us_))
      .handle(dispatch, cnmsg::shutdown_vehicle);
#endif
}

xfsm_state_of(core_fsm_t, power_off) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered power_off state";
  }

  auto now = std::chrono::system_clock::now();
  auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
                   now - power_off_start_tp_)
                   .count();

  xlogv << "delta : " << delta;
  if ((delta >= power_off_detect_time_) && (delta < power_off_cancel_time_) &&
      (io_->get_power_btn_sts()) && (power_off_fl_)) {
    xlogw << "power btn is on before 10 seconds again!";
    power_off_cancel_tp_ = std::chrono::system_clock::now();
    oneself().send(cnmsg::cancel_power_off());
  } else if ((delta >= power_off_cancel_time_) && (power_off_fl_)) {
    xlogw << "5 seconds have past, start power off procedure";
    last_error_.set_error(exception_data_t::power_off_request);
    oneself().send(cnmsg::throw_exception(last_error_.get_error()));
  }

  this->check_event_on_power_off();

  mesg()
      .wait(std::chrono::microseconds(period_us_))
      .handle(dispatch, cnmsg::cancel_power_off)
      .handle(dispatch, cnmsg::start_emergency)
      .handle(dispatch, cnmsg::throw_exception)
      .handle(dispatch, mnmsg::exception_raised)
      .handle(dispatch, anmsg::exception_raised)
      .handle(dispatch, dnmsg::exception_raised)
      .handle(dispatch, cnmsg::shutdown_vehicle);
}

xfsm_state_of(core_fsm_t, emergency) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered emergency state";
  }

  if ((emergency_fl_) && (!io_->get_emergency_btn_sts())) {
    xlogw << "emergency btn is off";
    if (vehicle_->stop_emergency()) {
      oneself().send(cnmsg::stop_emergency());
    }
  }

  mesg()
      .wait(std::chrono::microseconds(period_us_))
      .handle(dispatch, cnmsg::stop_emergency)
      .handle(dispatch, cnmsg::shutdown_vehicle)
      .handle(dispatch, cnmsg::throw_exception);
}

xfsm_state_of(core_fsm_t, shutdown) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered shutdown state";
  }

  this->shutdown_system();

  mesg().wait().handle(dispatch, cnmsg::terminate);
}

xfsm_state_of(core_fsm_t, terminating) {
  store_current_state(current_state_);
  if (!check_current_state()) {
    xlogd << "entered terminating state";
  }

  this->terminate_system();

  mesg().wait(std::chrono::seconds(5));
}

/*
 * dispatch function
 */
void core_fsm_t::dispatch(cnmsg::start_vehicle const &msg) {
  xlogv << "got cnmsg::start_vehicle";
  update_state(xfsm_state_ref(core_fsm_t, creating), msg);
}

void core_fsm_t::dispatch(cnmsg::created const &msg) {
  xlogv << "got cnmsg::created";
  update_state(xfsm_state_ref(core_fsm_t, initializing), msg);
}

void core_fsm_t::dispatch(cnmsg::initialized const &msg) {
  xlogv << "got cnmsg::initialized";
  update_state(xfsm_state_ref(core_fsm_t, pending), msg);
}

void core_fsm_t::dispatch(cnmsg::start_manual_driving const &msg) {
  xlogv << "got cnmsg::start_manual_driving";
  if (vehicle_->start_manual_driving()) {
    update_state(xfsm_state_ref(core_fsm_t, manual), msg);
  }
}

void core_fsm_t::dispatch(cnmsg::stop_manual_driving const &msg) {
  xlogv << "got cnmsg::stop_manual_driving";
  if (vehicle_->stop_manual_driving()) {
    ;
  }
}

void core_fsm_t::dispatch(cnmsg::start_diag const &msg) {
  xlogv << "got cnmsg::start_diag";
  // just test
  if (!vehicle_->start_diag()) {
    ;
  }
}

void core_fsm_t::dispatch(cnmsg::stop_diag const &msg) {
  xlogv << "got cnmsg::stop_diag";
  // just test
  if (!vehicle_->stop_diag()) {
    ;
  }
}

void core_fsm_t::dispatch(dnmsg::connected const &msg) {
  xlogv << "got dnmsg::connected";
  diag_link_fl_ = true;
}

void core_fsm_t::dispatch(dnmsg::disconnected const &msg) {
  xlogv << "got dnmsg::disconnected";
  diag_link_fl_ = false;
}

void core_fsm_t::dispatch(cnmsg::start_autopilot_driving const &msg) {
  xlogv << "got cnmsg::start_autopilot_driving";
  autopilot_driving_fl_ = true;
  update_state(xfsm_state_ref(core_fsm_t, autopilot), msg);
}

void core_fsm_t::dispatch(cnmsg::stop_autopilot_driving const &msg) {
  xlogv << "got cnmsg::stop_autopilot_driving";
  autopilot_driving_fl_ = false;
  update_state(xfsm_state_ref(core_fsm_t, manual), msg);
}

void core_fsm_t::dispatch(cnmsg::throw_exception const &msg) {
  xloge << "got cnmsg::throw_exception, reason: "
        << exception_data_t::to_string(msg.get_type());

  last_error_.set_error(msg.get_type());
  switch (msg.get_type()) {
    case exception_data_t::terminal_request:
      terminate_reason_.set_reason(terminate_data_t::debug);
      update_state(xfsm_state_ref(core_fsm_t, shutdown), msg);
      break;

    case exception_data_t::power_off_request:
      terminate_reason_.set_reason(terminate_data_t::normal);
      update_state(xfsm_state_ref(core_fsm_t, exception), msg);
      break;

    case exception_data_t::system_license_fault:
    case exception_data_t::autopilot_motor_init_fault:
    case exception_data_t::autopilot_servo_on_fault:
    default:
      terminate_reason_.set_reason(terminate_data_t::abnormal);
      update_state(xfsm_state_ref(core_fsm_t, exception), msg);
      break;
  }
}

void core_fsm_t::dispatch(cnmsg::start_emergency const &msg) {
  xlogv << "got cnmsg::start_emergency";
  emergency_fl_ = true;
  update_state(xfsm_state_ref(core_fsm_t, emergency), msg);
}

void core_fsm_t::dispatch(cnmsg::stop_emergency const &msg) {
  xlogv << "got cnmsg::stop_emergency";
  emergency_fl_ = false;
  update_state(xfsm_state_ref(core_fsm_t, pending), msg);
}

void core_fsm_t::dispatch(cnmsg::start_power_off const &msg) {
  xlogv << "got cnmsg::start_power_off";
  power_off_start_tp_ = std::chrono::system_clock::now();
  power_off_fl_ = true;
  update_state(xfsm_state_ref(core_fsm_t, power_off), msg);
}

void core_fsm_t::dispatch(cnmsg::cancel_power_off const &msg) {
  xlogv << "got cnmsg::cancel_power_off";
  power_off_fl_ = false;
  update_state(xfsm_state_ref(core_fsm_t, pending), msg);
}

void core_fsm_t::dispatch(cnmsg::shutdown_vehicle const &msg) {
  xlogv << "got cnmsg::shutdown_vehicle";
  update_state(xfsm_state_ref(core_fsm_t, shutdown), msg);
}

void core_fsm_t::dispatch(cnmsg::terminate const &msg) {
  xlogv << "got cnmsg::terminate";
  update_state(xfsm_state_ref(core_fsm_t, terminating), msg);
}

void core_fsm_t::dispatch(mnmsg::exception_raised const &msg) {
  xlogv << "got mnmsg::exception_raised";
  last_error_.set_error(msg.get_type());
  oneself().send(cnmsg::throw_exception(last_error_.get_error()));
}

void core_fsm_t::dispatch(anmsg::exception_raised const &msg) {
  xlogv << "got anmsg::exception_raised";
  last_error_.set_error(msg.get_type());
  oneself().send(cnmsg::throw_exception(last_error_.get_error()));
}

void core_fsm_t::dispatch(dnmsg::exception_raised const &msg) {
  xlogv << "got dnmsg::exception_raised";
  last_error_.set_error(msg.get_type());
  oneself().send(cnmsg::throw_exception(last_error_.get_error()));
}

/*
 * entry action
 */

void core_fsm_t::entry_creating() {
  if (!vehicle_->creating()) {
    xlogv << "send cnmsg::throw_exception";
    last_error_.set_error(exception_data_t::core_fsm_fault);
    oneself().send(cnmsg::throw_exception(last_error_.get_error()));
  } else {
    xlogv << "done";
  }
}

void core_fsm_t::entry_pending() {
  // reset flag
  diag_cmd_.mode = vehicle_data_t::normal;
  diag_cmd_.option = vehicle_data_t::aut_opt_none;
  autopilot_driving_fl_ = false;
  autopilot_detached_ = false;

  if (boot_up_) {
    boot_up_ = false;
  }

#if ENABLE_GUI_MANAGER
#endif

#if ENABLE_INPUT_EMUL
  io_->start_emul();  // to test
#endif                    // !EANBLE_EMULATION

  xlogv << "done";
}

void core_fsm_t::entry_manual() {
  xlogv << "done";
}

void core_fsm_t::entry_autopilot() {
  if (autopilot_first_using_fl_) {
    autopilot_first_using_fl_ = false;
    this->update_battery_usage();
  }
  autopilot_start_tp_ = std::chrono::system_clock::now();
  xlogv << "done";
}

void core_fsm_t::entry_emergency() {
  autopilot_driving_fl_ = false;
  autopilot_detached_ = false;

  xlogv << "done";
}

void core_fsm_t::entry_power_off() {
  xlogv << "done";
}

void core_fsm_t::entry_exception() { xlogv << "done"; }

void core_fsm_t::entry_terminating() {
  xlogv << "done";
}

/*

  xlogv << "done";
}

/*
 * do action
 */
void core_fsm_t::check_event_on_pending() {
  auto now = std::chrono::system_clock::now();
  auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
                   now - power_off_cancel_tp_)
                   .count();

  if ((!emergency_fl_) && (io_->get_emergency_btn_sts()))  {
    xlogw << "console emergency btn is on";
    if (vehicle_->start_emergency()) {
      oneself().send(cnmsg::start_emergency());
    }
  } else if ((delta > power_off_detect_time_) && io_->get_power_btn_sts() &&
             (!power_off_fl_)) {
    xlogw << "power btn is on";
    if (vehicle_->forced_break_subsystem()) {
      oneself().send(cnmsg::start_power_off());
    }
  }
}

void core_fsm_t::check_event_on_manual() {
  auto now = std::chrono::system_clock::now();
  auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
                   now - power_off_cancel_tp_)
                   .count();

  if ((!emergency_fl_) && (io_->get_emergency_btn_sts()))  {
    xlogw << "console emergency btn is on";
    if (vehicle_->start_emergency()) {
      oneself().send(cnmsg::start_emergency());
    }
  } else if ((delta > power_off_detect_time_) && io_->get_power_btn_sts() &&
             (!power_off_fl_)) {
    xlogw << "power btn is on";
    if (vehicle_->forced_break_subsystem()) {
      oneself().send(cnmsg::start_power_off());
    }
  } else if ((!autopilot_driving_fl_) && (io_->get_autopilot_btn_sts())) {
    auto now = std::chrono::system_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
                     now - autopilot_btn_stop_tp_)
                     .count();
    if (delta > autopilot_btn_dbnc_time_) {
      autopilot_btn_start_tp_ = std::chrono::system_clock::now();
      xlogw << "autopilot button is on";
      if (vehicle_->start_autopilot_driving()) {
        oneself().send(cnmsg::start_autopilot_driving());
      }
    }
  }
}

void core_fsm_t::check_event_on_autopilot() {
  if ((!emergency_fl_) && (io_->get_emergency_btn_sts())) {
    xlogw << "console emergency btn is on";
    if (vehicle_->start_emergency()) {
      oneself().send(cnmsg::start_emergency());
    }
  } else if ((autopilot_driving_fl_) && (!io_->get_autopilot_btn_sts())) {
    auto now = std::chrono::system_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
                     now - autopilot_btn_start_tp_)
                     .count();
    if (delta > autopilot_btn_dbnc_time_) {
      xlogw << "autopilot button is off, delta: " << delta;
      autopilot_btn_stop_tp_ = std::chrono::system_clock::now();
      if (vehicle_->stop_autopilot_driving()) {
        oneself().send(cnmsg::stop_autopilot_driving());
      }
    }
  }
}

void core_fsm_t::check_event_on_power_off() {
  if ((!emergency_fl_) && (io_->get_emergency_btn_sts())) {
    xlogw << "console emergency btn is on";
    if (vehicle_->start_emergency()) {
      power_off_fl_ = false;
      oneself().send(cnmsg::start_emergency());
    }
  }
}

void core_fsm_t::shutdown_system() {

#if ENABLE_INPUT_EMUL
  io_->deinit_emul();
#endif
  if (!xcfg_fault_fl_) {
    this->store_xcfg();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  vehicle_->shutdown_subsystem();
  oneself().send(cnmsg::terminate());
}

void core_fsm_t::terminate_system() {
  xlogv << "sub fsm soon destroy";
  vehicle_->destroy();
  xlogv << "core fsm soon destroy...";
  this->done();
}

/*
 * exit action
 */
void core_fsm_t::exit_initializing() {
  if (xcfg_fault_fl_) {
    last_error_.set_error(exception_data_t::xcfg_fault);
    oneself().send(cnmsg::throw_exception(last_error_.get_error()));
  } else {
    xlogf << "Tesla model x is ready...";
  }
}

void core_fsm_t::exit_manual() {
  xlogv << "done";
}

void core_fsm_t::exit_autopilot() {
  auto now = std::chrono::system_clock::now();
  auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
                   now - autopilot_start_tp_)
                   .count();
  cfg_->set_autopilot_lifespan_ms(delta);
  xlogd << "increase autopilot lifespan ms: " << delta;
  xlogd << "autopilot lifepan ms: " << cfg_->get_autopilot_lifespan_ms();
}

/*
 * local helper
 */
void core_fsm_t::load_xcfg() {
  cfg_->set_total_run_cycle();
  vehicle_rcvr_fl_ = cfg_->get_recovery_flag();
  battery_validation_cfg_ = cfg_->get_battery_validation();
  battery_usage_threshold_ms_cfg_ = cfg_->get_battery_usage_threshold_ms();
  if (battery_usage_threshold_ms_cfg_ < 60000) {
    battery_usage_threshold_ms_cfg_ = 60000; // 10 minutes
  }
  battery_usage_limit_cfg_ = cfg_->get_battery_usage_limit();
  if (battery_usage_limit_cfg_ < 10) {
    battery_usage_limit_cfg_ = 10;
  }
  autopilot_lifespan_threshold_ms_cfg_ = cfg_->get_autopilot_lifespan_threshold_ms();
  if (autopilot_lifespan_threshold_ms_cfg_ < 10800000) {
    autopilot_lifespan_threshold_ms_cfg_ = 10800000;  // 3 hours
  }
  autopilot_lifespan_limit_cfg_ = cfg_->get_autopilot_lifespan_limit_ms();
  if (autopilot_lifespan_limit_cfg_ < 108000000) {
    autopilot_lifespan_limit_cfg_ = 108000000; // 30 hours
  }

  xlogv << "last error: " << cfg_->get_last_error_string();
  xlogv << "last power off reason: " << cfg_->get_power_off_reason_string();
  xlogv << "vehicle recovery flag: " << vehicle_rcvr_fl_;
  xlogv << "autopilot validation: " << battery_validation_cfg_;
  xlogv << "autopilot usage threshold ms: " << battery_usage_threshold_ms_cfg_;
  xlogv << "autopilot usage limit: " << battery_usage_limit_cfg_;
  xlogv << "autopilot lifespan threshold ms: " << autopilot_lifespan_threshold_ms_cfg_;
  xlogv << "autopilot lifespan limit: " << autopilot_lifespan_limit_cfg_;
}

void core_fsm_t::store_xcfg() {
  auto now = std::chrono::system_clock::now();
  auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(
                   now - vehicle_start_tp_)
                   .count();
  cfg_->set_last_run_time_ms(delta);
  cfg_->set_last_error(last_error_.get_error());
  cfg_->set_last_error_string(last_error_.to_string(last_error_.get_error()));
  cfg_->set_power_off_reason(terminate_reason_.get_reason());
  cfg_->set_power_off_reason_string(
      terminate_reason_.to_string(terminate_reason_.get_reason()));
  terminate_reason_.set_reason(terminate_data_t::abnormal);
  xlogv << "vehicle total run cycle: " << cfg_->get_total_run_cycle();
  xlogv << "vehicle last run time: " << cfg_->get_last_run_time_ms() << "ms";
  xlogv << "vehicle total run time: " << cfg_->get_total_run_time_ms() << "ms";
  xlogv << "vehicle last error: " << cfg_->get_last_error();
  xlogv << "vehicle last error: " << cfg_->get_last_error_string();
  xlogv << "vehicle power off reason: " << cfg_->get_power_off_reason();
  xlogv << "vehicle power off reason: " << cfg_->get_power_off_reason_string();
}

void core_fsm_t::battery_validation() {

  // battery usage count validation
  uint32_t usage = cfg_->get_battery_usage();  // must be read from eeprom
  if (usage >= battery_usage_limit_cfg_) {
    battery_validation_fl_ = false;
    xloge << "overflow battery usage: " << usage;
  } else {
    battery_validation_fl_ = true;
    xlogw << "battery attached, type: " << autopilot_type_
          << ", usage: " << usage;
  }

  // battery lifespan validation

  // ingnor battery validation
  if (!battery_validation_cfg_) {
    battery_validation_fl_ = true;
    xlogd << "ignore battery valdiation";
  }
}

bool core_fsm_t::update_battery_usage() {
  bool ret = true;
  uint64_t now = cfg_->get_current_timestamp_ms();
  uint64_t old = cfg_->get_autopilot_timestamp_ms();   // must be read from eeorom
  uint32_t usage = cfg_->get_battery_usage();        // must be read from eeprom

  if (old == 0) {
    old = now;
  }
  if (((now - old) > battery_usage_threshold_ms_cfg_) || (old == now)) {
    usage += 1;
    cfg_->set_battery_usage(usage);       // must write to eeprom
    cfg_->set_autopilot_timestamp_ms(now);  // must write to eeprom
    xlogw << "battery usage increased"
          << ", delta: " << now - old << "ms, usage: " << usage;
  }
  xlogv << "done";
  return ret;
}

bool core_fsm_t::check_current_state() {
  if (prev_state_ == current_state_) {
    return true;
  } else {
    prev_state_ = current_state_;
    //xlogv << "current state: " << this->get_current_state_string();
    return false;
  }
}

void core_fsm_t::gen_os_signal() {
  if (!generated_os_signal_) {
    tesla::gen_signal_term();
    generated_os_signal_ = true;
  }
}
