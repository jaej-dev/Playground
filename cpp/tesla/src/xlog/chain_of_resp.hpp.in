/*
 * Title      :   chain_of_resp.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

namespace xlog
{

template <typename t_t> class sender_t;

template <typename t_t> class handler_t {
public:
  handler_t() : prev_(nullptr), next_(nullptr) {
#if XLOG_DEBUG
    std::cout << __func__ << ": constructed" << std::endl;
#endif
  }

  virtual ~handler_t() { 
    withdraw(); 
#if XLOG_DEBUG
    std::cout << __func__ << ": destructed" << std::endl;
#endif
  }

  virtual handler_t &append(handler_t &h) {
    next_ = &h;
    next_->prev_ = this;
    return h;
  }

  virtual void withdraw() {
    if (prev_ != nullptr) {
      prev_->next_ = next_;
    }
    prev_ = nullptr;
    next_ = nullptr;
  }

  virtual void request(t_t &r) {
    xhandle(r);
    if (this->next_) {
      this->next_->request(r);
    }
#if XLOG_DEBUG
    std::cout << __func__ << ": done" << std::endl;
#endif
  }

  virtual void xhandle(t_t &r) = 0;

private:
  handler_t *prev_;
  handler_t *next_;
};

} // namespace xlog

