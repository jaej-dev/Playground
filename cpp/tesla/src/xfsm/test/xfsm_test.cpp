/*
 * Title      :   xfsm_test.cpp
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
#include <xfsm/xfsm.h>
#include "master.h"
#include "slave.h"

using_xfsm;

std::shared_ptr<master> master_instance;
std::shared_ptr<slave> slave_instance;

int main(int argc, char *argv[]) {
  xlogw << "Hello xfsm example...";

#if 1
  auto timer = precision_timer_t::get();
  if (timer) {
    timer->start();
    xlogv << "start timer...";
  }
#endif

  master_instance = std::shared_ptr<master>(new master());
  slave_instance =
      std::shared_ptr<slave>(new slave(master_instance->oneself()));
  std::error_code err;
  std::shared_ptr<std::thread> master_thread =
      xfsm_get_thread(master, master_instance, err);
  if (!master_thread) {
    xloge << "get_thread failure. error value: " << err.value()
         << ", message: " << err.message().c_str();
    exit(EXIT_FAILURE);
  }
  std::shared_ptr<std::thread> slave_thread =
      xfsm_get_thread(slave, slave_instance, err);
  if (!master_thread) {
    xloge << "get_thread failure. error value: " << err.value()
         << ", message: " << err.message().c_str();
    exit(EXIT_FAILURE);
  }

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

  slave_instance->done();
  master_instance->done();
  if (slave_thread->joinable()) {
    slave_thread->join();
  }
  if (master_thread->joinable()) {
    master_thread->join();
  }
#if 1
  if (timer) {
    timer->stop();
  }
#endif
  sleep(1);
  xlogw << "Goodbye xfsm example...";
  return 0;
}
