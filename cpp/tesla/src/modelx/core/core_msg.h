/*
 * Title      :   core_msg.h
 * Author     :   Jaewoong Jang
 * Date       :   JUn 20th 2021
 */

#ifndef _CORE_MSG_H_
#define _CORE_MSG_H_

#include <xfsm/xfsm.h>
#include "data/diag_data.h"
#include "data/terminate_data.h"
#include "data/exception_data.h"

namespace modelx_cnmsg {

struct start_vehicle {
};

struct created {
};

struct initialized {
};

struct start_manual_driving {
};

struct stop_manual_driving {
};

struct start_diag {
};

struct stop_diag {
};

struct start_autopilot_linear_alignment {
};

struct start_autopilot_linear_alignment_rgt {
};

struct start_autopilot_driving {
};

struct stop_autopilot_driving {
};

struct throw_exception {
  explicit throw_exception(exception_data_t::type const _type) : type_(_type) {}
  throw_exception() : type_(exception_data_t::no_exception) {}
  exception_data_t::type get_type() const { return type_; }
 private:
  exception_data_t::type type_;
};

struct stop_emergency {
};

struct start_emergency {
};

struct start_power_off {
};

struct cancel_power_off {
};

struct shutdown_vehicle {
};

struct shutdown_subsystem {
};

struct terminate {
};

struct forced_break_subsystem {
};

struct shutdown_dfi_pc_by_paros {
};

struct reset_dfi_pc_by_paros {
};

struct shutdown_mars_by_paros {
};

struct reset_mars_by_paros {
};

struct show_mars_msg_critical {
};

struct show_mars_msg_ {
};

}  // namespace modelx_cnmsg

#endif // !_CORE_MSG_H_
