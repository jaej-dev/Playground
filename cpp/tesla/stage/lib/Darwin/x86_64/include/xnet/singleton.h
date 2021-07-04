/*
 * Title      :   singleton.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#include <memory>

namespace xnet {

namespace internal {

template <typename t_t>
class singleton_t {
 public:
  static std::shared_ptr<t_t> get() {
    if (!instance_) {
      instance_ = std::make_shared<t_t>();
    }
    return instance_;
  }

  singleton_t(singleton_t const &) = delete;
  singleton_t &operator=(singleton_t const &) = delete;

 protected:
  singleton_t();

 private:
  static std::shared_ptr<t_t> instance_;
};

template <typename t_t>
std::shared_ptr<t_t> singleton_t<t_t>::instance_;

}  // namespace internal

}  // namespace xnet

