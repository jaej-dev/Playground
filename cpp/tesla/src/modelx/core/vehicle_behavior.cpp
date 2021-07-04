/*
 * Title      :   vehicle_behavior.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <thread>
#include <exception>
#include <xlog/xlog.h>
#include "core/vehicle_behavior.h"

vehicle_behavior_t::vehicle_behavior_t() {
  for (int i = 0; i < vehicle_data_t::vehicle_class_max; i++) {
    vehicle_data_t::vehicle_class rc =
        static_cast<vehicle_data_t::vehicle_class>(i);
    std::shared_ptr<vehicle_component_t> comp(new vehicle_component_t);
    comp->set_vehicle_class(rc);
    vehicle_components_.push_back(comp);
    xlogv << vehicle_data_t::to_string(rc) << " is listed";
  }
  io_ = nullptr;
  prio_ = 15;
  xlogv << "constructed";
}

vehicle_behavior_t::~vehicle_behavior_t() { xlogv << "destructed"; }

bool vehicle_behavior_t::creating() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::manual: {
        std::shared_ptr<manual_data_t> data;
        data.reset(
            new manual_data_t("m1", manual_data_t::m1, manual_data_t::lft));
        comp->set_data(data);

        std::shared_ptr<manual_fsm_t> fsm;
        fsm.reset(new manual_fsm_t(get_core_fsm(), get_io_wrapper(),
                                    get_motor_wrapper(), data));
        if (fsm == nullptr) {
          xloge << "can not create manual_fsm_t initiate failure";
          return false;
        }
        comp->set_fsm(fsm);

        std::shared_ptr<std::thread> thread;
        std::error_code err;
#ifdef ARCH_QNX
        thread = xfsm_get_thread_prio(manual_fsm_t, fsm, err, prio_);
#else
        thread = xfsm_get_thread(manual_fsm_t, fsm, err);
#endif
        if (!thread) {
          xloge << "get_thread failure. error value: " << err.value()
               << ", message: " << err.message().c_str();
          return false;
        }
        comp->set_thread(thread);
        comp->set_creating_sts(true);

        xlogv << vehicle_data_t::to_string(comp->get_vehicle_class())
             << " component is created";
        break;
      }

      case vehicle_data_t::autopilot: {
        std::shared_ptr<autopilot_data_t> data;
        data.reset(new autopilot_data_t("a1"));
        comp->set_data(data);

        std::shared_ptr<autopilot_fsm_t> fsm;
        fsm.reset(new autopilot_fsm_t(get_core_fsm(), get_io_wrapper(),
                                     get_motor_wrapper(),
                                     data));
        if (fsm == nullptr) {
          xloge << "can not create autopilot_fsm_t initiate failure";
          return false;
        }
        comp->set_fsm(fsm);

        std::shared_ptr<std::thread> thread;
        std::error_code err;
#ifdef ARCH_QNX
        thread = xfsm_get_thread_prio(autopilot_fsm_t, fsm, err, prio_);
#else
        thread = xfsm_get_thread(autopilot_fsm_t, fsm, err);
#endif
        if (!thread) {
          xloge << "get_thread failure. error value: " << err.value()
               << ", message: " << err.message().c_str();
          return false;
        }
        comp->set_thread(thread);
        comp->set_creating_sts(true);

        xlogv << vehicle_data_t::to_string(comp->get_vehicle_class())
             << " component is created";
        break;
      }

      case vehicle_data_t::diag: {
        std::shared_ptr<diag_data_t> data;
        data.reset(new diag_data_t("diag"));
        comp->set_data(data);

        std::shared_ptr<diag_fsm_t> fsm;
        fsm.reset(new diag_fsm_t(get_core_fsm(), data));
        if (fsm == nullptr) {
          xloge << "can not create diag_fsm_t initiate failure";
          return false;
        }
        comp->set_fsm(fsm);

        std::shared_ptr<std::thread> thread;
        std::error_code err;
#ifdef ARCH_QNX
        thread = xfsm_get_thread_prio(diag_fsm_t, fsm, err, prio_);
#else
        thread = xfsm_get_thread(diag_fsm_t, fsm, err);
#endif
        if (!thread) {
          xloge << "get_thread failure. error value: " << err.value()
               << ", message: " << err.message().c_str();
          return false;
        }
        comp->set_thread(thread);
        comp->set_creating_sts(true);

        xlogv << vehicle_data_t::to_string(comp->get_vehicle_class())
             << " component is created";
        break;
      }

      default: { break; }
    }
  }
  xlogv << "done";
  return true;
}

bool vehicle_behavior_t::matching() {
  std::shared_ptr<manual_data_t> manual_data;
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::manual: {
        manual_data = comp->get_manual_data();
        if (manual_data == nullptr) {
          xloge << "manual_data is nullptr";
        }
        break;
      }
    }
  }

  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::autopilot: {
        std::shared_ptr<autopilot_data_t> data;
        data = comp->get_autopilot_data();
        if (data != nullptr) {
          data->set_manual_data(manual_data);
        }
        break;
      }
    }
  }
  xlogv << "done";
  return true;
}

bool vehicle_behavior_t::check_creating_sts(xfsm::sender_t s) {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    std::shared_ptr<xfsm::base_state_machine_t> fsm = comp->get_fsm();
    if (fsm == nullptr) {
      xloge << vehicle_data_t::to_string(comp->get_vehicle_class())
           << "'s fsm is null";
      return false;
    }

    if (comp->get_sender() == s) {
      comp->set_creating_sts(true);
      xlogv << vehicle_data_t::to_string(comp->get_vehicle_class())
           << "'s fsm is created";
      return true;
    } else {
      comp->set_creating_sts(false);
      xloge << vehicle_data_t::to_string(comp->get_vehicle_class())
           << "'s fsm is not created";
      return false;
    }
  }
  return false;
}

bool vehicle_behavior_t::check_creating_sts(std::string s) {
  std::shared_ptr<vehicle_component_t> component;
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    std::shared_ptr<xfsm::base_state_machine_t> fsm = comp->get_fsm();
    if (fsm != nullptr &&
        vehicle_data_t::to_string(comp->get_vehicle_class()) == s) {
      comp->set_creating_sts(true);
      xlogv << vehicle_data_t::to_string(comp->get_vehicle_class())
           << "'s fsm is created";
      return true;

    } else {
      if (fsm != nullptr) {
        comp->set_creating_sts(false);
      }
      continue;
    }
  }
  xloge << s << "'s fsm is not created";
  return false;
}

bool vehicle_behavior_t::start_manual_driving() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::manual: {
        comp->get_sender().send(cnmsg::start_manual_driving());
        break;
      }
    }
  }
  return true;
}

bool vehicle_behavior_t::stop_manual_driving() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::manual: {
        comp->get_sender().send(cnmsg::stop_manual_driving());
        break;
      }
    }
  }
  return true;
}

bool vehicle_behavior_t::start_diag() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::diag: {
        comp->get_sender().send(cnmsg::start_diag());
        break;
      }
    }
  }
  return true;
}

bool vehicle_behavior_t::stop_diag() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::diag: {
        comp->get_sender().send(cnmsg::stop_diag());
        break;
      }
    }
  }
  return true;
}

bool vehicle_behavior_t::start_autopilot_driving() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::manual: {
        comp->get_sender().send(cnmsg::stop_manual_driving());
        break;
      }
      case vehicle_data_t::autopilot: {
        comp->get_sender().send(cnmsg::start_autopilot_driving());
        break;
      }
    }
  }
  return true;
}

bool vehicle_behavior_t::stop_autopilot_driving() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::autopilot: {
        comp->get_sender().send(cnmsg::stop_autopilot_driving());
        break;
      }
      case vehicle_data_t::manual: {
        comp->get_sender().send(cnmsg::start_manual_driving());
        break;
      }
    }
  }
  return true;
}

bool vehicle_behavior_t::start_emergency() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::manual:
      case vehicle_data_t::autopilot:
      case vehicle_data_t::diag: {
        comp->get_sender().send(cnmsg::start_emergency());
        break;
      }
    }
  }
  return true;
}

bool vehicle_behavior_t::stop_emergency() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::manual:
      case vehicle_data_t::autopilot:
      case vehicle_data_t::diag: {
        comp->get_sender().send(cnmsg::stop_emergency());
        break;
      }
    }
  }
  return true;
}

bool vehicle_behavior_t::start_power_off() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::manual:
      case vehicle_data_t::autopilot:
      case vehicle_data_t::diag: {
        comp->get_sender().send(cnmsg::start_power_off());
        break;
      }
    }
  }
  xlogv << "done";
  return true;
}

bool vehicle_behavior_t::forced_break_subsystem() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::autopilot: {
        comp->get_sender().send(cnmsg::forced_break_subsystem());
        break;
      }
    }
  }
  return true;
}

// data gathering
std::shared_ptr<manual_data_t> vehicle_behavior_t::get_manual_data() {
  std::shared_ptr<manual_data_t> data;
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::manual: {
        data = comp->get_manual_data();
      } break;
    }
  }
  return data;
}

std::shared_ptr<autopilot_data_t> vehicle_behavior_t::get_autopilot_data() {
  std::shared_ptr<autopilot_data_t> data;
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    if (comp->get_vehicle_class() == vehicle_data_t::autopilot) {
      data = comp->get_autopilot_data();
      break;
    }
  }
  return data;
}

std::shared_ptr<diag_data_t> vehicle_behavior_t::get_diag_data() {
  std::shared_ptr<diag_data_t> data;
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    if (comp->get_vehicle_class() == vehicle_data_t::diag) {
      data = comp->get_diag_data();
      break;
    }
  }
  return data;
}

xfsm::sender_t vehicle_behavior_t::get_core_fsm() { return core_fsm_; }

bool vehicle_behavior_t::set_core_fsm(xfsm::sender_t s) {
  bool ret = true;
  core_fsm_ = s;
  return ret;
}

std::shared_ptr<io_t> vehicle_behavior_t::get_io_wrapper() {
  return io_;
}

bool vehicle_behavior_t::set_io_wrapper(std::shared_ptr<io_t> io) {
  bool ret = true;
  io_ = io;
  return ret;
}

std::shared_ptr<motor_t> vehicle_behavior_t::get_motor_wrapper() {
  return mt_;
}

bool vehicle_behavior_t::set_motor_wrapper(std::shared_ptr<motor_t> mt) {
  bool ret = true;
  mt_ = mt;
  return ret;
}

bool vehicle_behavior_t::check_state(std::string state) {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    std::shared_ptr<xfsm::base_state_machine_t> fsm = comp->get_fsm();
    if (fsm == nullptr) {
      xloge << vehicle_data_t::to_string(comp->get_vehicle_class())
           << "'s fsm is null";
      return false;
    }

    std::string s = fsm->get_current_state_string();
    if (s.compare(state) != 0) {
      xlogv << vehicle_data_t::to_string(comp->get_vehicle_class())
           << "'s state is " << s << ", expected state: " << state;
      return false;
    }
  }
  xlogv << "all subsystem's fsm state is " << state;
  return true;
}

// deinit
bool vehicle_behavior_t::shutdown_subsystem() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::manual:
      case vehicle_data_t::autopilot:
      case vehicle_data_t::diag: {
        if (comp->get_creating_sts()) {
          xlogv << "send cnmsg::shutdown_subsystem to "
               << vehicle_data_t::to_string(comp->get_vehicle_class());
          comp->get_sender().send(cnmsg::shutdown_subsystem());
        }
        break;
      }
    }
  }
  return true;
}

void vehicle_behavior_t::destroy() {
  for (std::shared_ptr<vehicle_component_t> comp : vehicle_components_) {
    switch (comp->get_vehicle_class()) {
      case vehicle_data_t::manual:
      case vehicle_data_t::autopilot:
      case vehicle_data_t::diag: {
        if (comp->get_creating_sts()) {
          xlogv << "try to destory sub fsm, fsm: "
               << vehicle_data_t::to_string(comp->get_vehicle_class());
          comp->get_fsm()->done();
          comp->get_thread()->join();
        }
        break;
      }
    }
  }
  xlogv << "done";
}

