/*
 * Title      :   base_steram.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#define XLOG_DEBUG false

#include <future>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <xlog/chain_of_resp.h>
#include <xlog/record.h>
#include <xlog/severity.h>

namespace xlog 
{

class base_stream_t : public xlog::handler_t<const xlog::record_t> {
public:
  base_stream_t(severity_e max_severity = severity_e::verbose)
      : max_severity_(max_severity) {
#if XLOG_DEBUG
    std::cout << __func__ << ": constructed, max_severity: " << max_severity_
              << std::endl;
#endif
  }
  virtual ~base_stream_t() {
#if XLOG_DEBUG
    std::cout << __func__ << ": destructed" << std::endl;
#endif
  }

  severity_e get_max_severity() { return max_severity_; }
  severity_e set_max_severity(const severity_e s) { return max_severity_ = s; }

  void operator+=(xlog::record_t &r) {
    std::lock_guard<std::mutex> lk(mutex_);
    r << '\n';
    this->request(r);
  }

  virtual void write(const char *s, const size_t n) {}

  virtual void xhandle(const xlog::record_t &r) override {
    if (r.get_severity() > get_max_severity()) {
#if XLOG_DEBUG
      std::cout << __func__ << ": severity: " << r.get_severity()
           << ", max severity: " << get_max_severity() << std::endl;
#endif
      return;
    }
    this->write(r.get_stream().str().c_str(), r.get_stream().str().size());
  }

protected:
  std::mutex mutex_;
  severity_e max_severity_;
};

} // namespace xlog
