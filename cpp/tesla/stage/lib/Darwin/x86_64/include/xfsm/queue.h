/*
 * Title      :   queue.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#define DEBUG_XFSM_QUEUE false

#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <xlog/xlog.h>
#include <xfsm/base_message.h>

namespace xfsm {

class queue_t {
  mutable std::mutex m_;
  std::condition_variable c_;
  std::queue<std::shared_ptr<base_message_t>> q_;
  std::size_t qmax_;  // 0: unlimited
  bool blocked_;

 public:
  explicit queue_t(std::size_t qmax = 0)
      : m_(), c_(), q_(), qmax_(qmax), blocked_() {
#if DEBUG_XFSM_QUEUE
    xlogv << "constructed at " << this;
#endif
  }
  ~queue_t() {
#if DEBUG_XFSM_QUEUE
    xlogv << "destructed at" << this;
#endif
  }

  template <typename t_t>
  void push(t_t const &msg) {
    std::lock_guard<std::mutex> lk(m_);
    if (blocked_) {
      return;
    }

    if ((qmax_ != 0) && (q_.size() == qmax_)) {
      q_.pop();
    }

#if DEBUG_XFSM_QUEUE
    std::type_info const &ti = typeid(msg);
    const char *msgname = ti.name();
    xlogv << "push msg: " << msgname << " at" << this;
#endif  // DEBUG_XFSM_QUEUE
    q_.push(std::make_shared<wrapped_message_t<t_t>>(msg));
    c_.notify_all();
  }

  template <typename t_t>
  void thrust(t_t const &msg) {
    std::lock_guard<std::mutex> lk(m_);
    if ((qmax_ != 0) && (q_.size() == qmax_)) {
      q_.pop();
    }

    q_.push(std::make_shared<wrapped_message_t<t_t>>(msg));
    c_.notify_all();
#if DEBUG_XFSM_QUEUE
    std::type_info const &ti = typeid(msg);
    const char *msgname = ti.name();
    xlogv << "thrust msg: " << msgname << " at " << this;
#endif  // DEBUG_XFSM_QUEUE
  }

  std::shared_ptr<base_message_t> wait_and_pop() {
    std::unique_lock<std::mutex> lk(m_);
    while (q_.empty()) {
      c_.wait(lk);
    }
    std::shared_ptr<base_message_t> res = q_.front();
    q_.pop();
#if DEBUG_XFSM_QUEUE
    std::type_info const &ti = typeid(res);
    const char *msgname = ti.name();
    xlogv << "pop msg: " << msgname << " at " << this;
#endif  // DEBUG_XFSM_QUEUE
    return res;
  }

  template <typename duration_t>
  std::shared_ptr<base_message_t> wait_and_pop(duration_t timeout) {
    static_assert(is_chrono_duration_t<duration_t>::value,
                  "\033[91m type std::chrono::duration is required. \033[0m");

    std::unique_lock<std::mutex> lk(m_);
    auto now = std::chrono::system_clock::now();
    while (q_.empty()) {
      if (c_.wait_until(lk, now + timeout) == std::cv_status::timeout) {
#if DEBUG_XFSM_QUEUE
        xlogv << "msg is empty and timeout expired";
#endif  // DEBUG_XFSM_QUEUE
        return nullptr;
      }
    }
    std::shared_ptr<base_message_t> res = q_.front();
    q_.pop();
#if DEBUG_XFSM_QUEUE
    std::type_info const &ti = typeid(res);
    const char *msgname = ti.name();
    xlogv << "pop msg: " << msgname;
#endif  // DEBUG_XFSM_QUEUE
    return res;
  }

  void block() {
    std::lock_guard<std::mutex> lk(m_);
    blocked_ = true;
  }

  void unblock() {
    std::lock_guard<std::mutex> lk(m_);
    blocked_ = false;
  }

  bool is_blocked() const {
    std::lock_guard<std::mutex> lk(m_);
    return blocked_;
  }
};

class close_queue_t {};

}  // namespace xfsm

