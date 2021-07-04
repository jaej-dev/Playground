/*
 * Title      :   receiver.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#define DEBUG_XFSM_RECEIVER false

#include <memory>
#include <mutex>
#include <atomic>
#include <xlog/xlog.h>
#include <xfsm/queue.h>
#include <xfsm/sender.h>

namespace xfsm {

class receiver_t {
  std::unique_ptr<queue_t> q_;
  std::mutex m_;
  std::atomic_flag is_timed_out_;

 public:
  receiver_t(std::size_t qmax = 0)
      : q_(nullptr), m_(), is_timed_out_(ATOMIC_FLAG_INIT) {
#if DEBUG_XFSM_RECEIVER
    xlogv << "constructed at " << this;
#endif
    q_.reset(new queue_t(qmax));
    is_timed_out_.clear(std::memory_order_release);
  }

  ~receiver_t() {
#if DEBUG_XFSM_RECEIVER
    xlogv << "destructed at " << this;
#endif
  }

  operator sender_t() { 
#if DEBUG_XFSM_RECEIVER
    xlogv << "operator sender_t from receiver_t at " << this;
#endif
    return sender_t(q_.get()); 
  }

  dispatcher_t wait() { 
#if DEBUG_XFSM_RECEIVER
    xlogv << "call dispatcher_t";
#endif
    return dispatcher_t(q_.get()); 
  }

  template <typename duration_t>
  timed_dispatcher_t<duration_t> wait(duration_t timeout) {
    static_assert(is_chrono_duration_t<duration_t>::value,
                  "type std::chrono::duration is required.");
#if DEBUG_XFSM_RECEIVER
    xlogv << "call timed_dispatcher_t and spinlock release";
#endif
    is_timed_out_.clear(std::memory_order_release); // spinlock release
    return timed_dispatcher_t<duration_t>(q_.get(), timeout, is_timed_out_);
  }

  bool is_blocked() const { return q_->is_blocked(); }

  void block() { return q_->block(); }

  void unblock() { return q_->unblock(); }

  bool is_timed_out() {
    std::lock_guard<std::mutex> lk(m_);
    bool status = is_timed_out_.test_and_set(std::memory_order_acquire);
    if (status == false) {
      is_timed_out_.clear(std::memory_order_release);
    }
    return status;
  }
};

}  // namespace xfsm

