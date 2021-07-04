
#ifndef _XBUF_H_
#define _XBUF_H_

#include <cstddef>
#include <mutex>

class xbuf_t {
public:
  xbuf_t(int order = 13); // default = 1 << 13 (8192)
  virtual ~xbuf_t();

  std::size_t data_len();
  std::size_t avail();
  void clear();

  // peek buffer data:
  // if buffer is empty, returns NULL, *len is 0.
  // else returns data pointer, *len is data length.
  void *peek(std::size_t *len);

  // remove data from buffer
  void pop(std::size_t len);

  // alloc buffer data:
  // if no space in buffer, returns NULL.
  // else returns a valid pointer.
  void *alloc(std::size_t len);

  // push data
  void push(std::size_t len);

  // get data:
  // data copying version.
  // returns copied data length.
  std::size_t get(void *data, std::size_t len);

  // checked runtime error: no space to put data
  // returns 0 on success, -1 on failure.
  int put(const void *data, size_t len);

private:
  int create(std::size_t capa, void **address);
  void destroy();
  inline void *write_address();
  inline void write_advance(std::size_t len);
  inline void *read_address();
  inline void read_advance(std::size_t len);
  inline std::size_t count_bytes();
  inline std::size_t count_free_bytes();
  inline std::size_t wrap_around();

  void *address;
  size_t n;
  size_t write_offset;
  size_t read_offset;
  bool alloc_in_progress;
  std::mutex _mutex;
};

#endif // _XBUF_H_
