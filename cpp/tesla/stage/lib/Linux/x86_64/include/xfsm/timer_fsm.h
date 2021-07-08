/*
 * Title      :   timer_fsm.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#define DEBUG_XFSM_TIMER_FSM false

#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <map>
#include <chrono>
#include <xlog/xlog.h>
#include <xfsm/base_timer.h>
#include <xfsm/state_machine.h>

#define internal_set_init_state(class_, state_) \
  xfsm::state_machine_t<class_>(&class_::xfsm_state_##state_)

#define internal_declare_state(state_) void xfsm_state_##state_()

#define internal_state_of(state_) void xfsm_state_##state_()

#define internal_state_ref(class_, state_) (&class_::xfsm_state_##state_)

#define internal_enable_log_state(class_, state_)                      \
  do {                                                                 \
    register_state_name(get_state_index(&class_::xfsm_state_##state_), \
                        #state_);                                      \
  } while (0)

#define internal_handle(func_, msg_) \
  handle<msg_>([=](msg_ const &msg) { func_(msg); })

#define internal_handle_user_data(func_, msg_, udata_) \
  handle<msg_>([=, &udata_](msg_ const &msg) { func_(msg, udata_); })

#define internal_get_thread(class_, shared_ptr_obj_, err_code_) \
  class_::get_thread(&class_::run, shared_ptr_obj_, err_code_)

#define internal_get_thread_prio(class_, shared_ptr_obj_, err_code_, prio_) \
  class_::get_thread(&class_::run, shared_ptr_obj_, err_code_, prio_)

namespace xfsm {

namespace internal {

class timer_fsm_t : public state_machine_t<timer_fsm_t> {
  enum { default_sleep_ms = 10 };

  struct client_t {
    ptimer_t id;
    sender_t target;
    bool is_one_shot;

    client_t() : id(), target(), is_one_shot() {}

    client_t(sender_t _target, bool _is_one_shot)
        : id(), target(_target), is_one_shot(_is_one_shot) {}
  };

  std::map<xtimer_t, std::vector<timer_fsm_t::client_t *>> client_map_;
  using client_map_iter =
      std::map<xtimer_t, std::vector<timer_fsm_t::client_t *>>::iterator;
  using client_map_item =
      std::pair<xtimer_t, std::vector<timer_fsm_t::client_t *>>;
  using client_vec = std::vector<timer_fsm_t::client_t *>;
  using client_vec_iter = std::vector<timer_fsm_t::client_t *>::iterator;
  std::mutex client_map_mutex_;

  bool run_;
  std::mutex run_mutex_;
  bool update_trigger_time_;
  std::chrono::microseconds short_test_period_;
  std::chrono::microseconds static_period_;
#ifdef linux
  int tfd_;
#endif

  void simulate() {
    std::chrono::microseconds period;
    {
      std::lock_guard<std::mutex> lock(client_map_mutex_);

      period =
          (static_period_.count() != 0) ? static_period_ : short_test_period_;

      if (update_trigger_time_) {
        std::chrono::time_point<std::chrono::high_resolution_clock> tp;
        tp = std::chrono::high_resolution_clock::now();

        for (client_map_iter it = client_map_.begin(); it != client_map_.end();
             ++it) {
          xtimer_t const &t = it->first;
          t.tp = tp;
        }
        update_trigger_time_ = false;
      }

      std::chrono::time_point<std::chrono::high_resolution_clock> current_time;
      current_time = std::chrono::high_resolution_clock::now();

      for (client_map_iter it = client_map_.begin(); it != client_map_.end();) {
        xtimer_t const &t = it->first;
        client_vec &cv = it->second;
        if (t.tp < current_time) {
          t.tp += t.period;

          if (t.tp <= current_time) {
            t.tp = current_time + t.period;
          }

          for (client_vec_iter ci = cv.begin(); ci != cv.end();) {
            alarm te;
            te.tp = current_time;
            te.us = t.us;
            te.period_us = t.period.count();
            te.id = (*ci)->id;
#if DEBUG_XFSM_TIMER_FSM
      xlogv << "send alarm msg";
#endif
            (*ci)->target.send(te);

            if ((*ci)->is_one_shot) {
              delete *ci;
              ci = cv.erase(ci);
            } else {
              ++ci;
            }
          }
        }
        if (cv.empty()) {
          client_map_.erase(it++);
        } else {
          ++it;
        }
      }
    }

#ifdef linux
    if (tfd_ < 0) {
      std::this_thread::sleep_for(std::chrono::microseconds(period));
    } else {
      struct itimerspec timeout;
      memset(&timeout, 0, sizeof(timeout));
      unsigned int nsec =
          std::chrono::duration_cast<std::chrono::nanoseconds>(period).count();
      timeout.it_value.tv_sec = nsec / 1000000000;
      timeout.it_value.tv_nsec = nsec % 1000000000;

      int ret = timerfd_settime(tfd_, 0, &timeout, NULL);
      if (ret < 0) {
        std::this_thread::sleep_for(std::chrono::microseconds(period));
      } else {
        uint64_t expirations = 0;
        ret = read(tfd_, &expirations, sizeof(expirations));
        if (expirations > 1) {
          fprintf(stderr, "warning: timer expirations > 1.\n");
        }
      }
    }
#else
    std::this_thread::sleep_for(std::chrono::microseconds(period));
#endif
  }

  void dispatch(xwakeup_t const &msg) {
    update_trigger_time_ = true;
    update_state(internal_state_ref(timer_fsm_t, check_expiration), msg);
  }

  internal_state_of(check_expiration) {
    bool run;
    {
      std::lock_guard<std::mutex> lock(run_mutex_);
      run = run_;
    }

    if (run) {
#if DEBUG_XFSM_TIMER_FSM
      xlogv << "call simulate";
#endif
      simulate();
    } else {
#if DEBUG_XFSM_TIMER_FSM
      xlogv << "send xwakeup_t";
#endif
      oneself().send(xwakeup_t());
      update_state(internal_state_ref(timer_fsm_t, wait_close));
    }
  }

  internal_state_of(wait_close) {
#if DEBUG_XFSM_TIMER_FSM
      xlogv << "called";
#endif
    std::this_thread::sleep_for(std::chrono::microseconds(default_sleep_ms));
    mesg().wait().internal_handle(dispatch, xwakeup_t);
  }

  ptimer_t do_set_timer(xtimer_t const &timer, sender_t client_id,
                        bool is_one_shot = false) {
    std::lock_guard<std::mutex> lock(client_map_mutex_);

    client_map_iter it = client_map_.find(timer);
    if (it != client_map_.end()) {
      ptimer_t ret = nil_timer_id();

      client_vec &cv = it->second;
      client_vec_iter ci;

      if (!is_one_shot) {
        for (ci = cv.begin(); ci != cv.end(); ++ci) {
          if ((*ci)->target == client_id) {
            break;
          }
        }
      } else {
        ci = cv.end();
      }

      if (ci == cv.end()) {
        timer_fsm_t::client_t *client = new (std::nothrow) timer_fsm_t::client_t;
        if (client) {
          client->id = client;
          client->target = client_id;
          client->is_one_shot = is_one_shot;

          cv.push_back(client);
          ret = client;
        }
      } else {
        ret = (*ci)->id;
      }
      return ret;
    } else {
      ptimer_t ret = nil_timer_id();

      timer.tp = std::chrono::high_resolution_clock::now() + timer.period;

      client_vec cv;
      timer_fsm_t::client_t *client = new (std::nothrow) timer_fsm_t::client_t;
      if (client) {
        client->id = client;
        client->target = client_id;
        client->is_one_shot = is_one_shot;

        cv.push_back(client);

        client_map_.insert(client_map_item(timer, cv));

        short_test_period_ = get_short_test_period();
        ret = client;
      }
      return ret;
    }
  }

  std::chrono::microseconds get_short_test_period() {
    if (!client_map_.empty()) {
      client_map_iter it;
      it = client_map_.begin();
      auto short_test = it->first.period;
      auto first = short_test;
      ++it;
      for (; it != client_map_.end(); ++it) {
        auto diff = it->first.period - first;
        if (diff < short_test) {
          short_test = diff;
        }
      }
      return short_test;
    }
    return std::chrono::microseconds(default_sleep_ms);
  }

 public:
  timer_fsm_t()
      : internal_set_init_state(timer_fsm_t, check_expiration),
        client_map_(),
        client_map_mutex_(),
        run_(),
        run_mutex_(),
        update_trigger_time_(true),
        short_test_period_(default_sleep_ms),
        static_period_(0)
#ifdef linux
        ,
        tfd_()
#endif
  {
#if DEBUG_XFSM_TIMER_FSM
    xlogv << "constructed";
#endif
    internal_enable_log_state(timer_fsm_t, check_expiration);
    internal_enable_log_state(timer_fsm_t, wait_close);

#ifdef linux
    tfd_ = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
#endif
  }

  ~timer_fsm_t() {
#ifdef linux
    close(tfd_);
#endif
    for (client_map_iter it = client_map_.begin(); it != client_map_.end();) {
      client_vec &cv = it->second;

      for (client_vec_iter ci = cv.begin(); ci != cv.end();) {
        delete *ci;
        ci = cv.erase(ci);
      }
      client_map_.erase(it++);
    }
#if DEBUG_XFSM_TIMER_FSM
    xlogv << "destructed";
#endif
  }

  timer_fsm_t(timer_fsm_t const &) = delete;
  timer_fsm_t &operator=(timer_fsm_t const &) = delete;

  template <typename period_t>
  void set_static_period(period_t period) {
    std::lock_guard<std::mutex> lock(run_mutex_);
    static_period_ =
        std::chrono::duration_cast<std::chrono::microseconds>(period);
  }

  void start() {
    std::lock_guard<std::mutex> lock(run_mutex_);
    run_ = true;
  }

  void stop() {
    std::lock_guard<std::mutex> lock(run_mutex_);
    run_ = false;
  }

  ptimer_t set_timer(float us, xfsm::sender_t client_id) {
    xtimer_t timer(us);
    return do_set_timer(timer, client_id);
  }

  template <typename timeout_t>
  ptimer_t set_timer(timeout_t timeout, xfsm::sender_t client_id) {
    xtimer_t timer(timeout);
    return do_set_timer(timer, client_id);
  }

  template <typename timeout_t>
  ptimer_t set_timer_oneshot(timeout_t timeout, xfsm::sender_t client_id) {
    xtimer_t timer(timeout);
    return do_set_timer(timer, client_id, true);
  }

  void cancel_timer(ptimer_t timer_id) {
    std::lock_guard<std::mutex> lock(client_map_mutex_);

    for (client_map_iter it = client_map_.begin(); it != client_map_.end();
         ++it) {
      client_vec &cv = it->second;
      for (client_vec_iter ci = cv.begin(); ci != cv.end(); ++ci) {
        if ((*ci)->id == timer_id) {
          delete *ci;
          cv.erase(ci);
          break;
        }
      }
      if (cv.empty()) {
        client_map_.erase(it);
        break;
      }
    }
  }
};

}  // namespace internal

}  // namespace xfsm
