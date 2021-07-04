/*
 * Title      :   xrecord_t.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#define XLOG_DEBUG false

#include <cerrno>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <array>
#include <xlog/xlog.h>
#include <xlog/base_stream.h>
#include <xlog/severity.h>
#include <xlog/time.h>

namespace xlog
{

class xrecord_t : public xlog::record_t {
public:
  explicit xrecord_t(const xlog::severity_e severity, const char *proc,
                     const int32_t pid, const uint64_t tid, const char *file,
                     const char *func, size_t line)
      : xlog::record_t(severity) {
    static std::array<const char *, 7> min_severity = { "[f]",
                                                        "[e]", 
                                                        "[w]", 
                                                        "[i]",
                                                        "[d]", 
                                                        "[v]",
                                                        "[n]" };

    tm t;
    //xlog::xgmtime(&t, &get_time().time); // utc
    xlog::xlocaltime(&t, &get_time().time); // local
    stream_ << "[" << proc << ":" << pid << "] " 
            << "[" << t.tm_year + 1900 << "-" << std::setfill('0') 
            << std::setw(2) << t.tm_mon + 1 << "-" << std::setfill('0') 
            << std::setw(2) << t.tm_mday << " " << std::setfill('0') 
            << std::setw(2) << t.tm_hour << ":" << std::setfill('0') 
            << std::setw(2) << t.tm_min << ":" << std::setfill('0')
            << std::setw(2) << t.tm_sec << "." << std::setfill('0') 
            << std::setw(3) << get_time().millitm << "] "
            << "[" << tid << "] "
            << min_severity[static_cast<size_t>(severity)] << " "
            << "[" << file << "/" << func << "@" << line << "] ";
    delimit();
#if XLOG_DEBUG
    std::cout << __func__ << ": constructed" << std::endl;
#endif
  }

  ~xrecord_t() {
#if XLOG_DEBUG
    std::cout << __func__ << ": destructed" << std::endl;
#endif
  }
};

} // namespace xlog
