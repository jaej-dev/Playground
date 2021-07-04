/*
 * Title      :   sender.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#define DEBUG_XFSM_SENDER false

#include <xlog/xlog.h>
#include <xfsm/queue.h>

namespace xfsm {

class sender_t {
  queue_t *q_;

 public:
  sender_t() : q_(nullptr) {
#if DEBUG_XFSM_SENDER
    xlogv << "constructed without queue_t at " << this;
#endif
  }

  ~sender_t() {
#if DEBUG_XFSM_SENDER
    xlogv << "destructed at " << this;
#endif
  }

  bool operator==(sender_t const &other) const { return (other.q_ == this->q_); }
  bool operator!=(sender_t const &other) const { return !(*this == other); }

  explicit sender_t(queue_t *q) : q_(q) {
#if DEBUG_XFSM_SENDER
    xlogv << "constructed with queue_t at " << this;
#endif
  }

  template <typename msg_t>
  void send(msg_t const &msg) {
    if (q_) {
#if DEBUG_XFSM_SENDER
      std::type_info const &ti = typeid(msg);
      const char *msgname = ti.name();
      xlogv << msgname << " push to queue_t at " << this;
#endif
      q_->push(msg);
    }
  }

  template <typename msg_t>
  void thrust(msg_t const &msg) {
    if (q_) {
#if DEBUG_XFSM_SENDER
      std::type_info const &ti = typeid(msg);
      const char *msgname = ti.name();
      xlogv << msgname << " thrust to queue_t";
#endif
      q_->thrust(msg);
    }
  }

  bool is_blocked() const {
    if (q_) {
      return q_->is_blocked();
    }
    return true;
  }
};

}  // namespace xfsm

