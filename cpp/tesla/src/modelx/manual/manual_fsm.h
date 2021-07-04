/*
 * Title      :   manual_fsm.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _MANUAL_FSM_H_
#define _MANUAL_FSM_H_

#include <memory>
#include <mutex>
#include <cmath>
#include "core/core_fsm.h"
#include "core/core_msg.h"
#include "core/vehicle_behavior.h"
#include "data/manual_data.h"
#include "data/terminate_data.h"
#include "manual/manual_msg.h"
#include "hal/io/io_wrapper.h"
#include "hal/motor/motor_wrapper.h"

namespace mnmsg = modelx_mnmsg;
namespace cnmsg = modelx_cnmsg;

class vehicle_behavior_t;
class manual_data_t;
class manual_fsm_t : public xfsm::state_machine_t<manual_fsm_t> {
 public:
  explicit manual_fsm_t(xfsm::sender_t, 
                         std::shared_ptr<io_t>,
                         std::shared_ptr<motor_t>, 
                         std::shared_ptr<manual_data_t>);
  virtual ~manual_fsm_t();

 private:
  manual_fsm_t(manual_fsm_t const &);
  manual_fsm_t &operator=(manual_fsm_t const &);

 private:
  xfsm_declare_state(idle);          // 201
  xfsm_declare_state(creating);      // 202
  xfsm_declare_state(initializing);  // 203
  xfsm_declare_state(pending);       // 204
  xfsm_declare_state(active);        // 205
  xfsm_declare_state(emergency);     // 206
  xfsm_declare_state(exception);     // 208
  xfsm_declare_state(shutdown);      // 209
  xfsm_declare_state(terminating);   // 210

  void dispatch(mnmsg::start const &msg);
  void dispatch(mnmsg::created const &msg);
  void dispatch(mnmsg::initialized const &msg);
  void dispatch(cnmsg::start_manual_driving const &msg);
  void dispatch(cnmsg::stop_manual_driving const &msg);
  void dispatch(cnmsg::start_emergency const &msg);
  void dispatch(cnmsg::stop_emergency const &msg);
  void dispatch(mnmsg::throw_exception const &msg);
  void dispatch(cnmsg::shutdown_subsystem const &msg);
  void dispatch(mnmsg::terminate const &msg);

  bool do_idle();
  bool create_device();
  bool init_device();
  void deinit_device();
  void do_nothing();
  bool get_steering_data();
  bool do_exception();
  bool do_shutdown();
  bool do_terminating();

  bool check_current_state();

 private:
  xfsm::sender_t core_fsm_;
  std::mutex mutex_;
  state_t prev_state_;
  state_t current_state_;
  exception_data_t last_error_;
  uint32_t period_us_;
  xfsm::ptimer_t period_tick_;
  exception_data_t exception_type_;
  uint32_t tick_count_;
  std::shared_ptr<manual_data_t> manual_data_;
  std::shared_ptr<io_t> io_;
  std::shared_ptr<motor_t> mt_;
  steering_data_t steering_data_;
};

#endif  // !_MANUAL_FSM_H_
