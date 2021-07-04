/*
 * Title      :   base_timer.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#include <chrono>

namespace xfsm {

typedef void *ptimer_t;

inline ptimer_t nil_timer_id() { return 0; }

inline bool is_valid_id(ptimer_t id) { return (id != 0); }

struct alarm {
  ptimer_t id;
  std::chrono::time_point<std::chrono::high_resolution_clock> tp;
  float us;
  unsigned int period_us;
};

namespace operators {
constexpr float operator"" _US(long double us) { return us; }

constexpr float operator"" _US(unsigned long long int us) { return us; }
}  // namespace operators

namespace internal {

struct xwakeup_t {};

struct xtimer_t {
  mutable std::chrono::time_point<std::chrono::high_resolution_clock> tp;
  float us;
  std::chrono::microseconds period;

  explicit xtimer_t(float _us) : tp(), us(_us), period() {
    auto tickcount = static_cast<decltype(period.count())>(us);
    period = std::chrono::microseconds(tickcount);
  }

  template <typename period_t>
  explicit xtimer_t(period_t _period) : tp(), us(), period(_period) {
    static_assert(is_chrono_duration_t<period_t>::value,
                  "type std::chrono::duration is required.");

    period = std::chrono::duration_cast<std::chrono::microseconds>(_period);
    us = period.count();
  }

  bool operator<(const xtimer_t &other) const {
    if (period.count() < other.period.count()) {
      return true;
    }
    return false;
  }
};

}  // namespace internal

}  // namespace xfsm

