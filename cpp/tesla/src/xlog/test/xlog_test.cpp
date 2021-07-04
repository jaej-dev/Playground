/*
 * Title      :   xlog_test.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 21th 2020
 */

#include <signal.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <xlog/xlog.h>

int main(int argc, char *argv[]) {

  xlogn << "none";
  xlogv << "verbose";
  xlogd << "debug";
  xlogw << "warning";
  xlogi << "info";
  xloge << "error";
  xlogf << "fatal";

  int signum;
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGINT);
  pthread_sigmask(SIG_BLOCK, &sigset, NULL);
  volatile bool run;

  try {
    run = true;
    while (run) {
      sigwait(&sigset, &signum);
      switch (signum) {
        case SIGINT: run = false; break;
      };
    }
  } catch (std::exception &e) {
    std::cerr << __func__ << ": " << e.what() << std::endl;
  }

EXIT:
  sleep(1);
  return 0;
}
