/*
 * Title      :   diag_data.cpp
 * Author     :   Jaewoong Jang 
 * Date       :   Jun 20th 2021
 */

#include <xlog/xlog.h>
#include "data/diag_data.h"

diag_data_t::diag_data_t(std::string const &name) {
  set_name(name);
  xlogv << "constructed, " << get_name();
}

diag_data_t::diag_data_t() {
  xlogv << "constructed";
}

diag_data_t::~diag_data_t() {
  xlogv << "destructed";
}

