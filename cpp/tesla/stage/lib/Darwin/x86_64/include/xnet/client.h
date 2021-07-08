/*
 * Title      :   client.h
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#define DEBUG_XNET_CLIENT true

#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/steady_timer.hpp>
#include <xnet/logger_adaptor.h>
#include <xlog/xlog.h>

using boost::asio::ip::tcp;
using std::placeholders::_1;
using std::placeholders::_2;

namespace xnet {

class client_t {
public:
  static constexpr std::size_t max_length = 1024; // 16 * 1024 is ssl limited

  explicit client_t(boost::asio::io_context &io_context,
         boost::asio::ssl::context &context,
         const tcp::resolver::results_type &endpoints)
      : socket_(io_context, context) {
    socket_.set_verify_mode(boost::asio::ssl::verify_peer);
    socket_.set_verify_callback(
        std::bind(&client_t::verify_certificate, this, _1, _2));

#if DEBUG_XNET_CLIENT
  xlogv << "constructed at" << this;
#endif
    connect(endpoints);
  }

  virtual ~client_t() {
#if DEBUG_XNET_CLIENT
  xlogv << "destructed at" << this;
#endif
  }

private:
  bool verify_certificate(bool preverified,
                          boost::asio::ssl::verify_context &ctx) {
    char subject_name[256];
    X509 *cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
#if DEBUG_XNET_CLIENT
    xlogd << "Verifying " << subject_name;
#endif

    return preverified;
  }

  void connect(const tcp::resolver::results_type &endpoints) {
    boost::asio::async_connect(socket_.lowest_layer(), endpoints,
                               [this](const boost::system::error_code &ec,
                                      const tcp::endpoint & endpoint) {
                                 if (!ec) {
                                  xlogd << "connected " << endpoint;
                                   handshake();
                                 } else {
                                   xloge << "Connect failed: "
                                        << ec.message();
                                 }
                               });
  }

  void handshake() {
    socket_.async_handshake(boost::asio::ssl::stream_base::client,
                            [this](const boost::system::error_code &ec) {
                              if (!ec) {
                                send_request();
                              } else {
                                xloge << "Handshake failed: " << ec.message();
                              }
                            });
  }

  void send_request() {
    std::cout << "Enter message: ";
    std::cin.getline(request_, max_length);
    size_t request_length = std::strlen(request_);

    boost::asio::async_write(
        socket_, boost::asio::buffer(request_, request_length),
        [this](const boost::system::error_code &ec, std::size_t length) {
          if (!ec) {
            receive_response(length);
          } else {
            xloge << "Write failed: " << ec.message() << "\n";
          }
        });
  }

  void receive_response(std::size_t length) {
    boost::asio::async_read(
        socket_, boost::asio::buffer(reply_, length),
        [this](const boost::system::error_code &ec, std::size_t length) {
          if (!ec) {
            std::cout << "Reply: ";
            std::cout.write(reply_, length);
            std::cout << "\n";
          } else {
            xloge << "Read failed: " << ec.message() << "\n";
          }
        });
  }

  boost::asio::ssl::stream<tcp::socket> socket_;
  char request_[max_length];
  char reply_[max_length];
};

}  // namespace xnet
