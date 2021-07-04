/*
 * Title      :   message.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#include <string>
#include <xfsm/xfsm.h>

namespace master_msg {

struct master_msg_a {
};

struct master_msg_b {
  explicit master_msg_b(std::string _msg) : msg(_msg) {}
  master_msg_b() : msg("") {}
  std::string msg;
};

struct master_msg_c {
  explicit master_msg_c(std::string _msg) : msg(_msg) {}
  master_msg_c() : msg("") {}
  std::string msg;
};

}

namespace slave_msg {

struct slave_msg_a {
  explicit slave_msg_a(xfsm::sender_t s)
     : slave(s) {}
  slave_msg_a() : slave() {}
  xfsm::sender_t slave;
};

struct slave_msg_b {
  explicit slave_msg_b(std::string _msg) : msg(_msg) {}
  slave_msg_b() : msg("") {}
  std::string msg;
};

struct slave_msg_c {
  explicit slave_msg_c(std::string _msg) : msg(_msg) {}
  slave_msg_c() : msg("") {}
  std::string msg;
};

}

