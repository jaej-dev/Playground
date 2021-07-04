/*
 * Title      :   remote_stream.h
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
#include <xnet/server.h>
#include <xlog/base_stream.h>
#include <xlog/chain_of_resp.h>
#include <xlog/severity.h>

namespace xlog
{

class remote_stream_t : public xlog::base_stream_t {
public:
  typedef void (remote_stream_t::*state_t)();

  remote_stream_t() = delete;

  remote_stream_t(const std::string &port, const std::string &cert,
                  const std::string &key, const std::string &dh)
      : port_(port), cert_(cert), key_(key), dh_(dh),
        base_stream(xlog::severity_e::none) /*_server(port, cert, key, dh*/) {
  }

  virtual ~remote_stream_t() {
  }

  virtual void write(const char *s, const size_t n) override {
    if (n)
      _server.write(s, n);
  }

private:
  //xnet::server_t server_;
  std::string port_;
  std::string cert_;
  std::string key_;
  std::string dh_;
};

} // namespace xlog
