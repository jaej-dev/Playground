#pragma once

#include <iostream>
#include <cstring>
#include <chrono>
#include <Poco/Logger.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Thread.h>
#include <Poco/RunnableAdapter.h>

using Poco::Logger;
using namespace Poco;
using namespace Poco::Net;

class client_t {
 public:
  client_t(const IPAddress& ip, const uint16_t& port) {
    socket_ = SocketAddress(ip, port);
    tick_ = 0;
  }

  client_t() { 
    socket_ = SocketAddress("127.0.0.1", 1976); 
    tick_ = 0; 
  }

  ~client_t() {}

  void send_loop() {
    DatagramSocket datagram;
    datagram.connect(socket_);
    std::string message;

    logger_.information("start client instance");
    while (!datagram.available()) {
      message = "packet-" + std::to_string(++tick_) + ": test packet";
      datagram.sendBytes(message.data(), int32_t(message.size()));
      if (message == "\\comm_exit") {
        datagram.close();
      }
      std::this_thread::sleep_for(std::chrono::microseconds(1000));
    }
    logger_.information("exit client instance");
  }

  std::string ip() { return socket_.host().toString(); }
  uint16_t port() { return socket_.port(); }

  static void send_message(const IPAddress& ip, const uint16_t port,
                           const std::string& message) {
    SocketAddress socket(ip, port);
    DatagramSocket datagram;
    datagram.connect(socket);
    datagram.sendBytes(message.data(), int32_t(message.size()));
  }

 private:
  SocketAddress socket_;
  uint32_t tick_;
  Logger& logger_ = Logger::get("client_logger");
};

