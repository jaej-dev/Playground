/*
 * Title      :   manual_msg.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#ifndef _MANUAL_MSG_H_
#define _MANUAL_MSG_H_

#include <xfsm/xfsm.h>
#include "data/manual_data.h"
#include "data/terminate_data.h"
#include "data/exception_data.h"

namespace modelx_mnmsg {

struct start {};

struct created {
  explicit created(xfsm::sender_t const s,
                   std::shared_ptr<vehicle_data_t> const d)
      : sender_(s), data_(d) {}
  created() : sender_(nullptr), data_(nullptr) {}

 private:
  xfsm::sender_t sender_;
  std::shared_ptr<vehicle_data_t> data_;
};

struct initialized {};

struct throw_exception {
  explicit throw_exception(exception_data_t::type const _type) : type_(_type) {}
  throw_exception() : type_(exception_data_t::no_exception) {}
  exception_data_t::type get_type() const { return type_; }

 private:
  exception_data_t::type type_;
};

struct exception_raised {
  explicit exception_raised(exception_data_t::type const _type)
      : type_(_type) {}
  exception_raised() : type_(exception_data_t::no_exception) {}
  exception_data_t::type get_type() const { return type_; }

 private:
  exception_data_t::type type_;
};

struct terminate {};

struct terminated {};

}  // namespace modelx_mnmsg

#endif  // !_MANUAL_MSG_H_
