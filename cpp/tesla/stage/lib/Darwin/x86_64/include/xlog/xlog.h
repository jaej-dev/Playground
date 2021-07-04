/*
 * Projet     :   XLOG, Log library
 * Title      :   xlog.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#define XLOG_DEBUG false
#define XLOG_REMOTE_STREAM false
#define XLOG_FILE_STREAM false
#define XLOG_FILE_PATH "/Users/jaewoongjang/x/playground/cpp/tesla/build/xlog.txt"

#include <cerrno>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <xlog/singleton.h>
#include <xlog/base_stream.h>
#include <xlog/file_stream.h>
#include <xlog/console_stream.h>
#include <xlog/xrecord.h>
#include <xlog/severity.h>

#ifdef __QNXNTO__
#include <process.h>
#else
#include <sys/syscall.h>
#endif

namespace xlog
{

constexpr const char *xstrend(const char *str) {
  return *str ? xstrend(str + 1) : str;
}
constexpr const char *xstrchr(const char *str, const char c) {
  return *str == c ? str : (*str ? xstrchr(str + 1, c) : nullptr);
}
constexpr const char *xstrrchr(const char *str, const char c) {
  return *str == c ? (str + 1) : xstrrchr(str - 1, c);
}
constexpr const char *xlog_strip_path(const char *str) {
  return xstrchr(str, '/') ? xstrrchr(xstrend(str), '/') : str;
}

#define __FILENAME__ xlog::xlog_strip_path(__FILE__)

#define log_(severity)                                                         \
  if (xlog_t::get() != nullptr)                                                \
  xlog_t::get()->get_base_stream() += xlog::xrecord_t(                         \
      severity, xlog_t::get()->get_proc(), xlog_t::get()->get_pid(),          \
          xlog_t::get()->get_tid(), __FILENAME__, __FUNCTION__, __LINE__)

#define xlogn log_(xlog::none)
#define xlogv log_(xlog::verbose)
#define xlogd log_(xlog::debug)
#define xlogi log_(xlog::info)
#define xlogw log_(xlog::warning)
#define xloge log_(xlog::error)
#define xlogf log_(xlog::fatal)


class xlog_t {
public:
  explicit xlog_t()
#if XLOG_FILE_STREAM
      : file_stream_(XLOG_FILE_PATH),
        console_stream_(xlog::severity_e::verbose) {
    base_stream_.append(file_stream_).append(console_stream_);
#else
      : console_stream_(xlog::severity_e::verbose) {
    base_stream_.append(console_stream_);
#endif

#if XLOG_DEBUG
    std::cout << __func__ << ": constructed" << std::endl;
#endif
  }

  virtual ~xlog_t() {
#if XLOG_DEBUG
    std::cout << __func__ << ": destructed" << std::endl;
#endif
  }

  xlog::base_stream_t &get_base_stream() { return base_stream_; }

  xlog::console_stream_t &get_console_stream() { return console_stream_; }

  inline xlog_t* get() { return this; }

  

  inline const char *get_proc() {
#if defined(__APPLE__) || defined(__FreeBSD__)
    return getprogname();
#elif defined(_GNU_SOURCE)
    return program_invocation_name;
#elif defined(_WIN32)
    return __argv[0];
#else
    int32_t pid = this->get_pid();
    std::string s = std::to_string(pid);
    char const *ret = s.c_str();
    return ret;
#endif
  }

  inline int32_t get_pid() {
    pid_t pid = getpid();
    return static_cast<int32_t>(pid);
  }

  inline uint64_t get_tid() {
#ifdef __linux__
    return static_cast<uint64_t>(::syscall(__NR_gettid));
#elif defined(__APPLE__)
    uint64_t tid64;
    pthread_threadid_np(NULL, &tid64);
    return static_cast<uint64_t>(tid64);
#elif defined(__QNXNTO__)
    return gettid();
#endif
  }

private:
  xlog::base_stream_t base_stream_;
  xlog::console_stream_t console_stream_;
#if XLOG_FILE_STREAM
  xlog::file_stream_t file_stream_;
#endif
};

} // namespace xlog

#define using_xlog using xlog_t = xlog::internal::singleton_t<xlog::xlog_t>
using_xlog;
