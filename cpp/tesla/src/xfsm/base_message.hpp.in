/*
 * Title      :   base_message.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#include <chrono>

namespace xfsm {

struct base_message_t {
  virtual ~base_message_t() {}
};

template <typename msg_t>
struct wrapped_message_t : base_message_t {
  msg_t contents_;

  explicit wrapped_message_t(msg_t const& contents) : contents_(contents) {}
};

template <typename t_t>
struct is_chrono_duration_t {
  static constexpr bool value = false;
};

template <typename rep_t, typename period_t>
struct is_chrono_duration_t<std::chrono::duration<rep_t, period_t>> {
  static constexpr bool value = true;
};

}  // namespace xfsm

