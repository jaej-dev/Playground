/*
 * Title      :   file_stream.h
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

class file_stream_t : public xlog::base_stream_t {
public:
  explicit file_stream_t(const std::string file)
      : file_name_(file), base_stream_t(xlog::severity_e::none) {
    this->open(file_name_);
#if XLOG_DEBUG
    std::cout << __func__ << ": constructed" << std::endl;
#endif
  }

  virtual ~file_stream_t() { 
    this->close(); 
#if XLOG_DEBUG
    std::cout << __func__ << ": destructed" << std::endl;
#endif
  }

  void open(const std::string file) {
    file_.open(file, std::fstream::in | std::fstream::out | std::fstream::app);
    if (file_.is_open()) {
      const char *tag = "XLOG new session: [pid] localtime (severity) tid [file/function@line] message...]\n";
      size_t size = strlen(tag);
      this->write(tag, size);
#if XLOG_DEBUG
      std::cout << __func__ << ": " << file << " is opened" << std::endl;
#endif
    } else {
#if XLOG_DEBUG
      std::cout << __func__ << ": can not open " << file << std::endl;
#endif
    }
  }

  void close() {
    if (file_.is_open()) {
      const char *tag = "\n";
      size_t size = strlen(tag);
      this->write(tag, size);
#if XLOG_DEBUG
      std::cout << __func__ << ": " << file_name_ << " is opened" << std::endl;
#endif
    } else {
#if XLOG_DEBUG
      std::cout << __func__ << ": can not open " << file_name_ << std::endl;
#endif
    }
    file_.close();
  }

  std::fstream &stream() { return file_; }

  virtual void write(const char *s, const size_t n) override {
    std::lock_guard<std::mutex> lk(mutex_);
    file_.write(s, n);
#if XLOG_DEBUG
    std::cout << __func__ << ": wrote > " << *s << std::endl;
#endif
  }

private:
  std::mutex mutex_;
  std::fstream file_;
  std::string file_name_;
};

} // namespace xlog
