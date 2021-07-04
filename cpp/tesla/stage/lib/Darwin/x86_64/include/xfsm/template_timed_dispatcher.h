/*
 * Title      :   template_timed_dispatcher.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#include <atomic>
#include <memory>
#include <utility>
#include <xlog/xlog.h>
#include <xfsm/queue.h>
#include <xfsm/base_message.h>

#define DEBUG_XFSM_TEMPLATE_TIMED_DISPATCHER \
  false

namespace xfsm {

template <typename prev_timed_dispatcher_t, typename msg_t, typename func_t,
          typename duration_t>
class template_timed_dispatcher_t {
  queue_t *q_;
  prev_timed_dispatcher_t *prev_;
  func_t f_;
  bool chained_;
  duration_t timeout_;
  std::atomic_flag &is_timed_out_;

  template <typename timed_dispatcher_t, typename other_msg_t,
            typename other_func_t, typename other_duration_t>
  friend class template_timed_dispatcher_t;

  void wait_and_dispatch() {
#if DEBUG_XFSM_TEMPLATE_TIMED_DISPATCHER
    xlogv << "template_timed_dispatcher_t pop msg from queue_t at " << this;
#endif
    std::shared_ptr<base_message_t> msg = q_->wait_and_pop(timeout_);
    if (msg) {
#if DEBUG_XFSM_TEMPLATE_TIMED_DISPATCHER
      xlogv
          << "template_timed_dispatcher_t wait_and_dispatch, remaining msg at "
          << this << " and now dispatch msg";
#endif
      dispatch(msg);
    } else {
#if DEBUG_XFSM_TEMPLATE_TIMED_DISPATCHER
      xlogv << "template_timed_dispatcher_t wait_and_dispatch, empty msg at "
           << this << " and spinlock acquire";
#endif
      is_timed_out_.test_and_set(std::memory_order_acquire);
    }
  }

  bool dispatch(std::shared_ptr<base_message_t> const &msg) {
#if DEBUG_XFSM_TEMPLATE_TIMED_DISPATCHER
    std::type_info const &ti = typeid(msg);
    const char *msgname = ti.name();
#endif
    if (wrapped_message_t<msg_t> *wrapper =
            dynamic_cast<wrapped_message_t<msg_t> *>(msg.get())) {
#if DEBUG_XFSM_TEMPLATE_TIMED_DISPATCHER
      xlogv << "template_timed_dispatcher_t dispatch msg: " << msgname
           << " and execute callback";
#endif
      f_(wrapper->contents_);
      return true;
    } else {
#if DEBUG_XFSM_TEMPLATE_TIMED_DISPATCHER
      xlogv << "template_timed_dispatcher_t previous dispatch msg: " << msgname;
#endif
      return prev_->dispatch(msg);
    }
  }

 public:
  template_timed_dispatcher_t(template_timed_dispatcher_t &&other)
      : q_(other.q_),
        prev_(other.prev_),
        f_(std::move(other.f_)),
        chained_(other.chained_),
        timeout_(other.timeout_),
        is_timed_out_(other.is_timed_out_) {
#if DEBUG_XFSM_TEMPLATE_TIMED_DISPATCHER
    xlogv << "template_timed_dispatcher_t constructed with &&other at " << this
         << " and chained";
#endif
    other.chained_ = true;
  }

  template_timed_dispatcher_t(queue_t *q, prev_timed_dispatcher_t *prev,
                               func_t f, duration_t timeout,
                               std::atomic_flag &is_timed_out)
      : q_(q),
        prev_(prev),
        f_(std::forward<func_t>(f)),
        chained_(false),
        timeout_(timeout),
        is_timed_out_(is_timed_out) {
#if DEBUG_XFSM_TEMPLATE_TIMED_DISPATCHER
    xlogv << "template_timed_dispatcher_t constructed at " << this
         << ", chained with " << prev;
#endif
    prev_->chained_ = true;
  }

  template <typename other_msg_t, typename other_func_t>
  template_timed_dispatcher_t<template_timed_dispatcher_t, other_msg_t,
                               other_func_t, duration_t>
  handle(other_func_t of) {
#if DEBUG_XFSM_TEMPLATE_TIMED_DISPATCHER
    xlogv << "template_timed_dispatcher_t handle at " << this;
#endif
    return template_timed_dispatcher_t<template_timed_dispatcher_t,
                                        other_msg_t, other_func_t, duration_t>(
        q_, this, std::forward<other_func_t>(of), timeout_, is_timed_out_);
  }

  ~template_timed_dispatcher_t() noexcept(false) {
    if (!chained_) {
      wait_and_dispatch();
    }
#if DEBUG_XFSM_TEMPLATE_TIMED_DISPATCHER
    xlogv << "template_timed_dispatcher_t destructed at " << this;
#endif
  }
};

}  // namespace xfsm
