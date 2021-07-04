/*
 * Title      :   xnet_client.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <signal.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <xlog/xlog.h>
#include "xnet_client.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    xloge << "usage: xnet_client 127.0.0.1 1976";
    return 1;
  }

  xlogw << "Hello xnet client test...";

  int signum;
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGINT);
  pthread_sigmask(SIG_BLOCK, &sigset, NULL);
  
  volatile bool run;
  try {
    run = true;
    boost::asio::io_context io_context;
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
    ctx.load_verify_file("../../stage/certificate/certi_client.crt");
    xnet::client_t c(io_context, ctx, endpoints);
    io_context.run();

#if 0
    xlogd << "going to sigwait...";
    while (run) {
      sigwait(&sigset, &signum);
      switch (signum) {
        case SIGINT: run = false; break;
      };
    }
#endif
  } catch (std::exception &e) {
    std::cerr << __func__ << ": " << e.what() << std::endl;
  }

  sleep(1);
  xlogw << "Goodbye xnet client test...";
  return 0;
}
