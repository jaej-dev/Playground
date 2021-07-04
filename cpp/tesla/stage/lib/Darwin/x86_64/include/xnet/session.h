/*
 * Title      :   session.h
 * Author     :   JaeWoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#define DEBUG_XNET_SESSION true

#include <cstdlib>
#include <functional>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/steady_timer.hpp>
#include <xlog/xlog.h>

using boost::asio::ip::tcp;

namespace xnet {

class session_t : public std::enable_shared_from_this<session_t> {
public:
  static constexpr std::size_t buf_size = 16 * 1024; // limited boost ssl stream

  explicit session_t(tcp::socket socket, boost::asio::ssl::context &context)
      : socket_(std::move(socket), context) {
#if DEBUG_XNET_SESSION
    xlogv << "constructed at" << this;
#endif
      }

  virtual ~session_t() {
#if DEBUG_XNET_SESSION
    xlogv << "destructed at" << this;
#endif
  }

  void start() { do_handshake(); }

private:
  void do_handshake() {
    auto self(shared_from_this());
    socket_.async_handshake(
        boost::asio::ssl::stream_base::server,
        [this, self](const boost::system::error_code &ec) {
          if (!ec) {
            do_read();
          }
        });
  }

  void do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(
        boost::asio::buffer(data_),
        [this, self](const boost::system::error_code &ec, std::size_t length) {
          if (!ec) {
            std::cout << "Received: ";
            std::cout.write(data_, length);
            std::cout << "\n";
            std::cout << "Sending back again\n";
            do_write(length);
          }
        });
  }

  void do_write(std::size_t length) {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                             [this, self](const boost::system::error_code &ec,
                                          std::size_t /*length*/) {
                               if (!ec) {
                                 do_read();
                               }
                             });
  }

  boost::asio::ssl::stream<tcp::socket> socket_;
  char data_[1024];
};

}  // namespace xnet
