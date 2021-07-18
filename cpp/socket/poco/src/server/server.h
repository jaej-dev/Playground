#pragma once

#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <Poco/Logger.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Thread.h>
#include <Poco/RunnableAdapter.h>

using Poco::Logger;
using namespace Poco;
using namespace Poco::Net;

class server_t {
 public:
  server_t(const uint16_t& port, const int32_t& bufferSize) {
    socket_ = SocketAddress(port);
    this->buffer_size_ = bufferSize;
  }

  server_t() : buffer_size_(1024) { socket_ = SocketAddress(1976); }

  ~server_t() {}

  void receive_loop() {
    DatagramSocket datagram(socket_);
    char buffer[buffer_size_];

    logger_.information("start server instance");
    while (true) {
      try {
        if (!datagram.available()) {
          SocketAddress sender;
          int32_t size = datagram.receiveFrom(buffer, buffer_size_, sender);
          buffer[size] = '\0';
          logger_.information("received packet: %s", std::string(buffer));

          if (std::string(buffer) == "\\comm_exit") {
            logger_.information("sender: %s, ended connection",
                                sender.toString());
            datagram.close();
            gen_signal_term();
          }
        } else {
          std::this_thread::sleep_for(std::chrono::microseconds(500000));
        }
      } catch (...) {
        std::cerr << "raised exception" << std::endl;
      }
    }
    logger_.information("exit server instance");
  }

  int32_t gen_signal_term() {
    std::stringstream ss;
    ss << "kill -TERM " << getpid();
    return system(ss.str().c_str());
  }

 private:
  int32_t buffer_size_;
  SocketAddress socket_;
  static bool debug_mode_enabled;
  Logger& logger_ = Logger::get("server_logger");
};

bool server_t::debug_mode_enabled = false;
