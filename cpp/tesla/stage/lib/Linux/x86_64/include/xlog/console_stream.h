/*
 * Title      :   console_stream.h
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
#include <xlog/base_stream.h>
#include <xlog/chain_of_resp.h>
#include <xlog/severity.h>

namespace xlog
{

class console_stream_t : public xlog::base_stream_t {
public:
  console_stream_t(xlog::severity_e max_severity = xlog::severity_e::none,
                   const bool trim = false)
      : base_stream_t(max_severity), trim_(trim) {
#if XLOG_DEBUG
    std::cout << __func__ << ": constructed, max_severity: " << max_severity
              << std::endl;
#endif
  }

  virtual ~console_stream_t() {
#if XLOG_DEBUG
    std::cout << __func__ << ": destructed" << std::endl;
#endif
  }

  void open() {}
  void close() {}

  void set_trim(const bool trim) { trim_ = trim; }

  virtual void write(const char *s, const size_t n) override { std::cout << s; }

  void write(const std::string &str) {
    std::cout << " "; // for readability
    write(str.c_str(), str.size());
  }

  virtual void xhandle(const xlog::record_t &r) override {
#if XLOG_DEBUG
      std::cout << __func__ << ": severity: " << r.get_severity()
           << ", max severity: " << get_max_severity() << std::endl;
#endif
    if (r.get_severity() > get_max_severity()) {
      return;
    }

    switch (r.get_severity()) {
#ifdef __APPLE__
    case none: {
      std::cout << "\x1B[97m"; // white(97)
      break;
    }
    case fatal: {
      std::cout << "\x1B[97m\x1B[41m"; // white on red background
      break;
    }
    case error: {
      std::cout << "\x1B[91m"; // red(91)
      break;
    }
    case warning: {
      std::cout << "\x1B[93m"; // yellow(93)
      break;
    }
    case info: {
      std::cout << "\x1B[92m"; // green(92)
      break;
    }
    case debug: {
      std::cout << "\x1B[96m"; // cyan(96)
      break;
    }
    case verbose: {
      std::cout << "\x1B[90m"; // dark gray(90)
      break;
    }
    default: {
      break;
    }
#else
    case none: {
      std::cout << "\x1B[37m"; // white(37)
      break;
    }
    case fatal: {
      std::cout << "\x1B[37m\x1B[41m"; // white on red background
      break;
    }
    case error: {
      std::cout << "\x1B[31m"; // red(31)
      break;
    }
    case warning: {
      std::cout << "\x1B[33m"; // yellow(33)
      break;
    }
    case info: {
      std::cout << "\x1B[32m"; // green(32)
      break;
    }
    case debug: {
      std::cout << "\x1B[36m"; // cyan(36)
      break;
    }
    case verbose: {
      std::cout << "\x1B[30m"; // dark gray(30)
      break;
    }
    default: {
      break;
    }
#endif
    }

    if (trim_) {
      this->write(r.get_message());
    } else {
      this->write(r.get_stream().str().c_str(), r.get_stream().str().size());
    }

    // restore console color
    std::cout << "\x1B[0m\x1B[0K"
              << "" << std::flush;
  }

private:
  bool trim_;
};

} // namespace xlog

