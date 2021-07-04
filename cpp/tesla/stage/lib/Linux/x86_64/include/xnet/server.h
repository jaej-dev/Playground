/*
 * Title      :   server.h
 * Author     :   JaeWoong Jang
 * Date       :   Jun 20th 2021
 */

#pragma once

#define DEBUG_XNET_SERVER true

#include <xlog/xlog.h>
#include <xnet/session.h>

using boost::asio::ip::tcp;

namespace xnet {

class server_t {
public:
  explicit server_t(boost::asio::io_context &io_context, unsigned short port)
      : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
        context_(boost::asio::ssl::context::sslv23) {
    context_.set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::single_dh_use);
    context_.set_password_callback(std::bind(&server_t::get_password, this));
    context_.use_certificate_chain_file("../../stage/certificate/certi_client.crt");
    context_.use_private_key_file("../../stage/certificate/certi_client.key",
                                  boost::asio::ssl::context::pem);
    context_.use_tmp_dh_file("../../stage/certificate/certi_server.dh2048");

#if DEBUG_XNET_SERVER
    xlogv << "constructed at" << this;
#endif

    do_accept();
  }

  virtual ~server_t() {
#if DEBUG_XNET_SESSION
    xlogv << "destructed at" << this;
#endif
  }

private:
  std::string get_password() const { return "test"; }

  void do_accept() {
    acceptor_.async_accept([this](const boost::system::error_code &ec,
                                  tcp::socket socket) {
      if (!ec) {
        std::make_shared<xnet::session_t>(std::move(socket), context_)->start();
      }

      do_accept();
    });
  }

  tcp::acceptor acceptor_;
  boost::asio::ssl::context context_;
};

} // namespace xnet
