/*
 * Title      :   state_machine.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#define DEBUG_XFSM_STATE_MACHINE false

#include <algorithm>
#include <memory>
#include <thread>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <tuple>
#include <sched.h>
#include <xlog/xlog.h>
#include <xfsm/base_state_machine.h>


namespace xfsm {

template <typename t_t>
class state_machine_t : public base_state_machine_t {
 public:
  typedef void (t_t::*state_t)(); // void t_t::state(void);

  static std::shared_ptr<std::thread> get_thread(void (t_t::*entry)(),
                                                 std::shared_ptr<t_t> &obj,
                                                 std::error_code &ec,
                                                 int prio = -1) {
    std::shared_ptr<std::thread> thr;
    if (prio >= 0) {
      thr.reset(new std::thread(entry, obj.get()));

      int ret;
      struct sched_param schedp;
      int policy;

      int minp = sched_get_priority_min(SCHED_RR);
      int maxp = sched_get_priority_max(SCHED_RR);

      if (prio > maxp) {
        prio = maxp;
      }
      if (prio < minp) {
        prio = minp;
      }

      ret = pthread_getschedparam(thr->native_handle(), &policy, &schedp);
      if (ret == 0) {
        schedp.sched_priority = prio;
        ret = pthread_setschedparam(thr->native_handle(), SCHED_RR, &schedp);
        if (ret != 0) {
          ec = std::error_code(ret, std::generic_category());
          pthread_cancel(thr->native_handle());
          if (thr->joinable()) {
            thr->join();
          }
          thr.reset();
          return thr;
        }
      } else {
        ec = std::error_code(ret, std::generic_category());
        pthread_cancel(thr->native_handle());
        if (thr->joinable()) {
          thr->join();
        }
        thr.reset();
        return thr;
      }
      return thr;
    } else {
      thr.reset(new std::thread(entry, obj.get()));
      return thr;
    }
  }

 private:
  typedef std::vector<state_t> state_vector_t;
  typedef std::map<size_t, std::string> state_name_map_t;
  typedef std::function<bool()> predicate_t;
  typedef std::function<void()> callback_handler_t;
  typedef std::tuple<predicate_t, callback_handler_t, callback_handler_t>
      callback_record_t;
  typedef std::map<size_t, callback_record_t> state_func_map_t;

  std::unique_ptr<receiver_t> incoming_;
  state_t state_;
  std::string name_;
  mutable std::mutex mutex_name_;

  state_vector_t state_vector_;
  std::mutex state_vector_mutex_;

  state_name_map_t state_name_map_;
  std::mutex state_name_map_mutex_;

  bool log_enalbe_;
  std::mutex log_enalbe_mutex_;

  state_func_map_t state_entry_map_;
  std::mutex state_entry_map_mutex_;

  state_func_map_t state_exit_map_;
  std::mutex state_exit_map_mutex_;

 protected:
  receiver_t& mesg() { return *(incoming_.get()); }

  void dispatch_callback(callback_record_t const &cbr) {
    if ((std::get<0>(cbr))()) {
      if (std::get<1>(cbr)) {
        (std::get<1>(cbr))();
      }
    } else {
      if (std::get<2>(cbr)) {
        (std::get<2>(cbr))();
      }
    }
  }

  void update_state(state_t s) {
    std::stringstream log_stream;
    std::lock_guard<std::mutex> lock(log_enalbe_mutex_);
    if (log_enalbe_) {
      std::lock_guard<std::mutex> lock(state_name_map_mutex_);

      typename state_name_map_t::const_iterator it;
      it = state_name_map_.find(get_state_index(state_));
      if (it != state_name_map_.end()) {
        log_stream << it->second;
      }
    }

    {
      std::lock_guard<std::mutex> lock(state_exit_map_mutex_);

      typename state_func_map_t::const_iterator it;
      it = state_exit_map_.find(get_state_index(state_));
      if (it != state_exit_map_.end()) {
        dispatch_callback(it->second);
      }
    }

    state_ = s;

    if (log_enalbe_) {
      std::lock_guard<std::mutex> lock(state_name_map_mutex_);

      typename state_name_map_t::const_iterator it;
      it = state_name_map_.find(get_state_index(state_));
      if (it != state_name_map_.end()) {
        log_stream << " -> " << it->second;
      }
#if DEBUG_XFSM_STATE_MACHINE
      xlogv << log_stream.str();
#endif
    }

    {
      std::lock_guard<std::mutex> lock(state_entry_map_mutex_);

      typename state_func_map_t::const_iterator it;
      it = state_entry_map_.find(get_state_index(state_));
      if (it != state_entry_map_.end()) {
        dispatch_callback(it->second);
      }
    }
  }

  template <typename msg_t>
  void update_state(state_t s, msg_t const &msg) {
    std::stringstream log_stream;
    std::lock_guard<std::mutex> lock(log_enalbe_mutex_);

    if (log_enalbe_) {
      std::lock_guard<std::mutex> lock(state_name_map_mutex_);

      typename state_name_map_t::const_iterator it;
      it = state_name_map_.find(get_state_index(state_));
      if (it != state_name_map_.end()) {
        log_stream << it->second;
      }
    }

    {
      std::lock_guard<std::mutex> lock(state_exit_map_mutex_);

      typename state_func_map_t::const_iterator it;
      it = state_exit_map_.find(get_state_index(state_));
      if (it != state_exit_map_.end()) {
        dispatch_callback(it->second);
      }
    }

    state_ = s;

    if (log_enalbe_) {
      std::type_info const &ti = typeid(msg);
      const char *msgname = ti.name();
      {
        std::lock_guard<std::mutex> lock(state_name_map_mutex_);

        typename state_name_map_t::const_iterator it;
        it = state_name_map_.find(get_state_index(state_));
        if (it != state_name_map_.end()) {
          log_stream << " -> " << it->second << ", msg: "<< msgname;
        }
      }
#if DEBUG_XFSM_STATE_MACHINE
      xlogv << log_stream.str();
#endif
    }

    {
      std::lock_guard<std::mutex> lock(state_entry_map_mutex_);

      typename state_func_map_t::const_iterator it;
      it = state_entry_map_.find(get_state_index(state_));
      if (it != state_entry_map_.end()) {
        dispatch_callback(it->second);
      }
    }
  }

  bool current_state_equals(state_t s) { return (state_ == s); }

  void store_current_state(state_t &s) { s = state_; }

 public:
  explicit state_machine_t(state_t istate, std::size_t qmax = 0)
      : incoming_(nullptr),
        state_(istate),
        name_("state_machine_t"),
        mutex_name_(),
        state_vector_(),
        state_vector_mutex_(),
        state_name_map_(),
        state_name_map_mutex_(),
        log_enalbe_(),
        log_enalbe_mutex_(),
        state_entry_map_(),
        state_entry_map_mutex_(),
        state_exit_map_(),
        state_exit_map_mutex_() {
#if DEBUG_XFSM_STATE_MACHINE
    xlogv << "constructed, state_: " << istate << ", qmax: " << qmax << " at "
         << this;
#endif
    incoming_.reset(new receiver_t(qmax));
  }

  virtual ~state_machine_t() {
#if DEBUG_XFSM_STATE_MACHINE
    xlogv << "destructed at" << this;
#endif
  }

  size_t get_state_index(state_t state) {
    std::lock_guard<std::mutex> lock(state_vector_mutex_);

    typename state_vector_t::const_iterator it =
        find(state_vector_.begin(), state_vector_.end(), state);
    if (it != state_vector_.end()) {
      return (it - state_vector_.begin());
    }
    state_vector_.push_back(state);
    return (state_vector_.size() - 1);
  }

  size_t get_max_state_index() {
    std::lock_guard<std::mutex> lock(state_vector_mutex_);
    return (state_vector_.size() - 1);
  }

  void register_state_name(size_t index, std::string const &name) {
    std::lock_guard<std::mutex> lock(state_name_map_mutex_);

    typename state_name_map_t::iterator it;
    it = state_name_map_.find(index);
    if (it != state_name_map_.end()) {
      state_name_map_.erase(it);
    }

    state_name_map_.insert(std::pair<size_t, std::string>(index, name));
  }

  bool always() { return true; }

  void null_callback() {
    ;  // nothing to do here
  }

  void register_entry_callback(size_t index, predicate_t pred,
                               callback_handler_t on_true,
                               callback_handler_t on_false) {
    std::lock_guard<std::mutex> lock(state_entry_map_mutex_);

    typename state_func_map_t::iterator it;
    it = state_entry_map_.find(index);
    if (it != state_entry_map_.end()) {
      state_entry_map_.erase(it);
    }

    state_entry_map_.insert(std::pair<size_t, callback_record_t>(
        index, callback_record_t(pred, on_true, on_false)));
  }

  void register_exit_callback(size_t index, predicate_t pred,
                              callback_handler_t on_true,
                              callback_handler_t on_false) {
    std::lock_guard<std::mutex> lock(state_exit_map_mutex_);

    typename state_func_map_t::iterator it;
    it = state_exit_map_.find(index);
    if (it != state_exit_map_.end()) {
      state_exit_map_.erase(it);
    }

    state_exit_map_.insert(std::pair<size_t, callback_record_t>(
        index, callback_record_t(pred, on_true, on_false)));
  }

  void set_name(std::string const &name) {
    std::lock_guard<std::mutex> lock(mutex_name_);
    name_ = name;
    std::replace_if(
        name_.begin(), name_.end(),
        [](decltype(*(name_.begin())) c) { return (std::isspace(c) != 0); },
        '-');
  }

  std::string get_name() const {
    std::lock_guard<std::mutex> lock(mutex_name_);
    return name_;
  }

  void set_log_transition(bool enable) {
    std::lock_guard<std::mutex> lock(log_enalbe_mutex_);
    log_enalbe_ = enable;
  }

  size_t get_current_state_number() { return get_state_index(state_); }

  std::string get_state_string(size_t index) {
    std::string ret;
    std::lock_guard<std::mutex> lock(state_name_map_mutex_);

    typename state_name_map_t::const_iterator it;
    it = state_name_map_.find(index);
    if (it != state_name_map_.end()) {
      ret = it->second;
    }
    return ret;
  }

  std::string get_current_state_string() {
    return get_state_string(get_state_index(state_));
  }

  sender_t oneself() { return *incoming_; }

  void done() { oneself().thrust(close_queue_t()); }

  void run() {
    try {
      for (;;) {
        (static_cast<t_t *>(this)->*state_)();
      }
    } catch (close_queue_t const &) {
      ;
    }
  }
};

}  // namespace xfsm
