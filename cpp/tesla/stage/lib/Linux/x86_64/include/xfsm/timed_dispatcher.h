/*
 * Title      :   timed_dispatcher.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#include <chrono>
#include <atomic>
#include <memory>
#include <utility>
#include <xlog/xlog.h>
#include <xfsm/queue.h>
#include <xfsm/base_message.h>
#include <xfsm/template_timed_dispatcher.h>

#define DEBUG_XFSM_TIMED_DISPATCHER false

namespace xfsm {

template <typename duration_t>
class timed_dispatcher_t {
  static_assert(is_chrono_duration_t<duration_t>::value,
                "type std::chrono::duration is required.");

  queue_t *q_;
  bool chained_;
  duration_t timeout_;
  std::atomic_flag &is_timed_out_;

  template <typename timed_dispatcher_t, typename msg_t, typename func_t,
            typename other_duration_t>
  friend class template_timed_dispatcher_t;

  void wait_and_dispatch() {
#if DEBUG_XFSM_TIMED_DISPATCHER
    xlogv << "timed_dispatcher_t pop msg from queue_t at " << this;
#endif
    std::shared_ptr<base_message_t> msg = q_->wait_and_pop(timeout_);
    if (msg) {
#if DEBUG_XFSM_TIMED_DISPATCHER
      xlogv << "timed_dispatcher_t wait_and_dispatch, remaining msg at " << this
           << " and now dispatch msg";
#endif
      dispatch(msg);
    } else {
#if DEBUG_XFSM_TIMED_DISPATCHER
      xlogv << "timed_dispatcher_t wait_and_dispatch, empty msg at " << this
           << " and spinlock acquire";
#endif
      is_timed_out_.test_and_set(std::memory_order_acquire);
    }
  }

  bool dispatch(std::shared_ptr<base_message_t> const &msg) {
#if DEBUG_XFSM_TIMED_DISPATCHER
    std::type_info const &ti = typeid(msg);
    const char *msgname = ti.name();
#endif
    if (dynamic_cast<wrapped_message_t<close_queue_t> *>(msg.get())) {
#if DEBUG_XFSM_TIMED_DISPATCHER
      xlogv << "timed_dispatcher_t dispatch, throw close_queue_t, msg: "
           << msgname;
#endif
      throw close_queue_t();
    }
#if DEBUG_XFSM_TIMED_DISPATCHER
    xlogv << "timed_dispatcher_t dispatch msg: " << msgname;
#endif
    return false;
  }

 public:
  timed_dispatcher_t(timed_dispatcher_t &&other)
      : q_(other.q_),
        chained_(other.chained_),
        timeout_(other.timeout_),
        is_timed_out_(other.is_timed_out_) {
#if DEBUG_XFSM_TIMED_DISPATCHER
    xlogv << "timed_dispatcher_t constructed with &&other at " << this
         << " and not chained";
#endif
    other.chained_ = true;
  }

  timed_dispatcher_t(queue_t *q, duration_t timeout,
                   std::atomic_flag &is_timed_out)
      : q_(q), chained_(false), timeout_(timeout), is_timed_out_(is_timed_out) {
#if DEBUG_XFSM_TIMED_DISPATCHER
    xlogv << "timed_dispatcher_t constructed at " << this << " and not chained";
#endif
  }

  template <typename msg_t, typename func_t>
  template_timed_dispatcher_t<timed_dispatcher_t, msg_t, func_t, duration_t> handle(
      func_t f) {
#if DEBUG_XFSM_TIMED_DISPATCHER
    xlogv << "timed_dispatcher_t handle at " << this;
#endif
    return template_timed_dispatcher_t<timed_dispatcher_t, msg_t, func_t,
                                     duration_t>(
        q_, this, std::forward<func_t>(f), timeout_, is_timed_out_);
  }

  ~timed_dispatcher_t() noexcept(false) {
    if (!chained_) {
      wait_and_dispatch();
    }
#if DEBUG_XFSM_TIMED_DISPATCHER
    xlogv << "timed_dispatcher_t destructed at " << this;
#endif
  }
};

}  // namespace xfsm