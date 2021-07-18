
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <errno.h>
#include "xbuf.h"

using namespace std;

xbuf_t::xbuf_t(int order)
    : address(NULL), n(0), write_offset(0), read_offset(0),
      alloc_in_progress(false) {
  assert(order > 0);

#ifdef __MINGW32__
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  size_t page_size = si.dwPageSize;
#else
  size_t page_size = sysconf(_SC_PAGESIZE);
#endif

  n = 1 << order;
  if (n < page_size)
    n = page_size;

  if (create(n, &address))
    abort();
}

xbuf_t::~xbuf_t() { destroy(); }

size_t xbuf_t::data_len() {
  size_t x;
  _mutex.lock();
  x = count_bytes();
  _mutex.unlock();
  return x;
}

size_t xbuf_t::avail() {
  size_t x;
  _mutex.lock();
  x = count_free_bytes();
  _mutex.unlock();
  return x;
}

void xbuf_t::clear() {
  _mutex.lock();
  write_offset = 0;
  read_offset = 0;
  _mutex.unlock();
}

void *xbuf_t::peek(size_t *len) {
  void *x;

  assert(len != NULL);
  _mutex.lock();
  *len = count_bytes();
  if (*len > 0)
    x = read_address();
  else
    x = NULL;
  _mutex.unlock();
  return x;
}

void xbuf_t::pop(size_t len) {
  _mutex.lock();
  if (count_bytes() < len) {
    _mutex.unlock();
    return;
#if 0
		abort(); // invariant broken..
#endif
  }
  read_advance(len);
  _mutex.unlock();
}

void *xbuf_t::alloc(size_t len) {
  void *x;

  if (alloc_in_progress) {
    printf("Alloc failed: Already in progress.\n");
    return NULL;
  }

  _mutex.lock();
  if (count_free_bytes() < len) {
    printf("Alloc failed: Not enough buffer(free: %u, request: %u.\n",
           count_free_bytes(), len);
    x = NULL;
  } else {
    x = write_address();
    alloc_in_progress = true;
  }
  _mutex.unlock();
  return x;
}

void xbuf_t::push(size_t len) {
  _mutex.lock();
  if (count_free_bytes() < len) {
    _mutex.unlock();
    return;
#if 0
		abort(); // invariant broken..
#endif
  }
  write_advance(len);
  alloc_in_progress = false;
  _mutex.unlock();
}

size_t xbuf_t::get(void *data, size_t len) {
  size_t x;
  size_t d;
  _mutex.lock();
  d = count_bytes();
  if (d == 0) {
    x = 0;
  } else {
    if (d < len)
      x = d;
    else
      x = len;

    memcpy(data, read_address(), x);
    read_advance(x);
  }
  _mutex.unlock();
  return x;
}

int xbuf_t::put(const void *data, size_t len) {
  int ret = 0;
  size_t d;
  _mutex.lock();
  d = count_free_bytes();
  if (d < len) {
    ret = -1;
  } else {
    memcpy(write_address(), data, len);
    write_advance(len);
  }
  _mutex.unlock();
  return ret;
}

int xbuf_t::create(size_t capa, void **address) {
  *address = malloc((capa << 1));
  if (*address == NULL) {
    return -1;
  }
  return 0;
}

void xbuf_t::destroy() {
  if (address != NULL) {
    free(address);
  }
}

void *xbuf_t::write_address() { return (char *)address + write_offset; }

void xbuf_t::write_advance(size_t len) {
  write_offset += len;

  // check to see if read_offset is in the redundant area
  // (always read_offset <= write_offset),
  // wrap around to the beginning of the underlying buffer.
  if (n <= read_offset) {
    wrap_around();
  }
}

void *xbuf_t::read_address() { return (char *)address + read_offset; }

void xbuf_t::read_advance(size_t len) {
  read_offset += len;

  // if another thread in 'alloc - write data - push' progress,
  // read_advance() does not call wrap_around().
  if ((n <= read_offset) && (!alloc_in_progress)) {
    wrap_around();
  }
}

size_t xbuf_t::count_bytes() { return write_offset - read_offset; }

size_t xbuf_t::count_free_bytes() {
  if (n <= read_offset) {
    return (n << 1) - write_offset;
  } else {
    return n - count_bytes();
  }
}

size_t xbuf_t::wrap_around() {
  size_t rest = count_bytes();
  memmove(address, ((char *)address + read_offset), rest);
  read_offset = 0;
  write_offset = rest;
  return rest;
}
