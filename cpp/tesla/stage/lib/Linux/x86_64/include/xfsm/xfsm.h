/*
 * Projet     :   XFSM, The finite state machine framework
 * Title      :   xfsm.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 * Revision   :   1.0.0
 */

#pragma once

#if defined(linux)
#include <cxxabi.h>
#include <stdint.h>
#include <string.h>
#include <sys/timerfd.h>
#include <unistd.h>
#endif

#include <functional>
#include <iostream>
//#include <map>
//#include <thread>

#include <xfsm/logger_adaptor.h>
#include <xfsm/base_message.h>
#include <xfsm/dispatcher.h>
#include <xfsm/timed_dispatcher.h>
#include <xfsm/queue.h>
#include <xfsm/receiver.h>
#include <xfsm/sender.h>
#include <xfsm/singleton.h>
#include <xfsm/state_machine.h>
#include <xfsm/precision_timer.h>

#define xfsm_set_init_state(class_, state_)                                    \
  xfsm::state_machine_t<class_>(&class_::xfsm_state_##state_)

#define xfsm_declare_state(state_) void xfsm_state_##state_()

#define xfsm_state_of(class_, state_) void class_::xfsm_state_##state_()

#define xfsm_state_ref(class_, state_) (&class_::xfsm_state_##state_)

#define xfsm_enable_state(class_, state_)                                  \
  do {                                                                         \
    register_state_name(get_state_index(&class_::xfsm_state_##state_),         \
                        #state_);                                              \
  } while (0)

#define xfsm_entry_action(class_, state_, cb_)                                 \
  do {                                                                         \
    register_entry_callback(get_state_index(&class_::xfsm_state_##state_),     \
                            std::bind(&class_::always, this), cb_,             \
                            std::bind(&class_::null_callback, this));          \
  } while (0)

#define xfsm_exit_action(class_, state_, cb_)                                  \
  do {                                                                         \
    register_exit_callback(get_state_index(&class_::xfsm_state_##state_),      \
                           std::bind(&class_::always, this), cb_,              \
                           std::bind(&class_::null_callback, this));           \
  } while (0)

#define xfsm_entry_action_handle_failure(class_, state_, pred_, cb_)           \
  do {                                                                         \
    register_entry_callback(get_state_index(&class_::xfsm_state_##state_),     \
                            pred_, std::bind(&class_::null_callback, this),    \
                            cb_);                                              \
  } while (0)

#define xfsm_exit_action_handle_failure(class_, state_, pred_, cb_)            \
  do {                                                                         \
    register_exit_callback(get_state_index(&class_::xfsm_state_##state_),      \
                           pred_, std::bind(&class_::null_callback, this),     \
                           cb_);                                               \
  } while (0)

#define handle(func_, msg_)                                                    \
  handle<msg_>([=](msg_ const &msg) { func_(msg); })

#define handle_user_data(func_, msg_, udata_)                                  \
  handle<msg_>([=, &udata_](msg_ const &msg) { func_(msg, udata_); })

#define xfsm_get_thread(class_, shared_ptr_obj_, err_code_)                    \
  class_::get_thread(&class_::run, shared_ptr_obj_, err_code_)

#define xfsm_get_thread_prio(class_, shared_ptr_obj_, err_code_, prio_)        \
  class_::get_thread(&class_::run, shared_ptr_obj_, err_code_, prio_)

