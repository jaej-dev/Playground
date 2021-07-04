/*
 * Title      :   precision_timer.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#define DEBUG_XFSM_PRECISON_TIMER false

#include <memory>
#include <mutex>
#include <thread>
#include <exception>
#include <xlog/xlog.h>
#include <xfsm/timer_fsm.h>

namespace xfsm {

namespace internal {

class precision_timer_t {
  std::mutex m_;
  bool started_;
  std::shared_ptr<timer_fsm_t> sm_;
  std::shared_ptr<std::thread> thread_;

 public:
  precision_timer_t() : m_(), started_(), sm_(), thread_() {
#if DEBUG_XFSM_PRECISION_TIMER
    xlogv << "constructed";
#endif
    try {
      sm_ = std::make_shared<timer_fsm_t>();
    } catch (std::exception const& e) {
      std::cout << e.what() << std::endl;
      return;
    }
  }

  ~precision_timer_t() {
    xlogv << "destructed";
  }

  template <typename period_t>
  void set_static_period(period_t period) {
    sm_->set_static_period(period);
  }

  bool start() {
    std::lock_guard<std::mutex> lk(m_);

    if (!started_) {
      std::error_code ec;
      thread_ = internal_get_thread(timer_fsm_t, sm_, ec);
      if (thread_) {
        sm_->start();
        started_ = true;
      }
    }
    return started_;
  }

  bool start(int priority) {
    std::lock_guard<std::mutex> lk(m_);

    if (!started_) {
      std::error_code ec;
      thread_ = internal_get_thread_prio(timer_fsm_t, sm_, ec, priority);
      if (thread_) {
        sm_->start();
        started_ = true;
      }
    }
    return started_;
  }

  void stop() {
    std::lock_guard<std::mutex> lk(m_);

    if (started_) {
      sm_->stop();
      sm_->done();
      thread_->join();
      thread_.reset();
      started_ = false;
    }
    return;
  }

  ptimer_t set_timer(float us, xfsm::sender_t client_id) {
    return sm_->set_timer(us, client_id);
  }

  template <typename timeout_t>
  ptimer_t set_timer(timeout_t timeout, xfsm::sender_t client_id) {
    return sm_->set_timer(timeout, client_id);
  }

  template <typename timeout_t>
  ptimer_t set_timer_oneshot(timeout_t timeout, xfsm::sender_t client_id) {
    return sm_->set_timer_oneshot(timeout, client_id);
  }

  void cancel_timer(ptimer_t timer_id) { return sm_->cancel_timer(timer_id); }
};

}  // namespace internal

}  // namespace xfsm

#define using_xfsm                                                \
  using precision_timer_t =                                         \
      xfsm::internal::singleton_t<xfsm::internal::precision_timer_t>; \
  using namespace xfsm::operators

