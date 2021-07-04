/*
 * Title      :   xnet_server.cpp
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
#include <xnet/server.h>
#include "xnet_server.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    xloge << "usage: xnet_server 1976";
    return 1;
  }

  xlogw << "Hello xnet server test...";

  int signum;
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGINT);
  pthread_sigmask(SIG_BLOCK, &sigset, NULL);

  volatile bool run;
  try {
    run = true;
    boost::asio::io_context io_context;
    xnet::server_t s(io_context, std::atoi(argv[1]));
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
  xlogw << "Goodbye xnet server test...";
  return 0;
}
