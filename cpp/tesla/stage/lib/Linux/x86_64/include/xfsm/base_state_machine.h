/*
 * Title      :   base_state_machine_t.h
 * Author     :   JaeWoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#include <string>

namespace xfsm {

class base_state_machine_t {
 public:
  base_state_machine_t() {}
  virtual ~base_state_machine_t() {}

  virtual sender_t oneself() = 0;
  virtual void done() = 0;
  virtual void run() = 0;
  virtual std::string get_current_state_string() { return std::string(); }
};

}  // namespace xfsm

