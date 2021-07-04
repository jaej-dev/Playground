/*
 * Title      :   time.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef __QNXNTO__
#include <process.h>
#include <sys/time.h>
#else
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>
#endif

namespace xlog
{

inline void xlocaltime(struct tm *t, const time_t *time) {
  ::localtime_r(time, t);
}

inline void xgmtime(struct tm *t, const time_t *time) { ::gmtime_r(time, t); }

typedef struct _xtime_t {
  time_t time;
  unsigned short millitm;
} xtime_t;

inline void store_time(xtime_t *t) {
  timeval tv;
  ::gettimeofday(&tv, NULL);

  t->time = tv.tv_sec;
  t->millitm = static_cast<unsigned short>(tv.tv_usec / 1000);
}

} // namespace xlog
