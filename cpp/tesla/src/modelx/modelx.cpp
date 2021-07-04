/*
 * Title      :   modelx.cpp
 * Author     :   Jaewoong Jang
 * Date       :   Jun 20th 2021
 */

#include <cstdint>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <csignal>
#include <unistd.h>
#include <arch_config.h>
#include <xfsm/xfsm.h>
#include <xver/xver.h>
#include <xcfg/xcfg.h>
#include <xlog/xlog.h>
#include "modelx_system.h"
#ifdef ARCH_QNX
#include <sys/neutrino.h>
#include <ctime>
#endif

namespace tesla {

using_xfsm;

#ifdef ARCH_QNX
static void init_system_clock(uint32_t const nsec) {
  struct _clockperiod cp;
  struct timespec ts;
  cp.nsec = nsec;
  cp.fract = 0;

  if (clock_getres(CLOCK_REALTIME, &ts) != -1) {
    if (static_cast<std::uint64_t>(ts.tv_nsec) >
        static_cast<std::uint64_t>(cp.nsec)) {
      int32_t const ret = ClockPeriod(CLOCK_REALTIME, &cp, NULL, 0);
      if (ret != 0) {
        // std:cerr does not return anything
        std::cerr << "ClockPeriod() failure.\n";
      }
    }
  }
}
#endif  // !ARCH_QNX

static void check_myself() {
  int32_t const fd =
      open("/var/run/modelx.pid", static_cast<int32_t>(O_RDWR | O_CREAT),
           static_cast<mode_t>(0640));
  if (fd < 0) {
    fprintf(stderr, "can not open lock file.\n");
    exit(EXIT_FAILURE);
  }

  int32_t const ret = lockf(fd, F_TLOCK, 0);
  if (ret < 0) {
    fprintf(stderr, "modelx is already running.\n");
    exit(EXIT_FAILURE);
  }

  char pidbuf[64];
  ssize_t size;
  snprintf(&pidbuf[0], sizeof(pidbuf), "%d\n", getpid());
  if (pidbuf[0] != '0') {
    ;
  }
  size = write(fd, &pidbuf[0], strlen(&pidbuf[0]));
  if (size <= 0) {
    fprintf(stderr, "pid write failure.\n");
  }

  return;
}

}  // !namespace tesla

int32_t main(int32_t const argc, char** const argv) {
  if ((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
    std::cout << "The modelx " << xver_t::version() << "\n";
    std::cout << "Created at " << xver_t::build_date() << "\n";
    std::cout << xver_t::build_developer() << " @ " << xver_t::build_path()
              << "\n";
    std::cout << "Copyright (C) 2021 Jaewoong Jang / jaej.dev@gmail.com" << "\n";
    return 0;
  }

#ifdef ARCH_QNX
  tesla::check_myself();
#endif  // !ARCH_QNX

  xlogf << "Hello modelx...";
  xlogv << "modelx version " << xver_t::version();

  auto const modelx_timer = tesla::precision_timer_t::get();
  if (modelx_timer != nullptr) {
#ifdef ARCH_QNX
    modelx_timer->start(9);
#else
    modelx_timer->start();
#endif
  }

  volatile bool run;
  int32_t signum;
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGABRT);
  sigaddset(&sigset, SIGTERM);
  sigaddset(&sigset, SIGUSR1);
  sigaddset(&sigset, SIGUSR2);
  sigaddset(&sigset, SIGINT);
  pthread_sigmask(SIG_BLOCK, &sigset, NULL);

  try {
    modelx_system_t const sys;
    sys.init();

    run = true;
    while (run) {
      sigwait(&sigset, &signum);
      switch (signum) {
        case SIGABRT: 
        {
          xlogd << "SIGABRT catched!";
          sys.deinit(terminate_data_t::normal);
          run = false;
          break;
        }
        case SIGTERM: 
        {
          xlogd << "SIGTERM catched!";
          sys.deinit(terminate_data_t::normal);
          run = false;
          break;
        }
        case SIGUSR1:
        {
          xlogd << "SIGUSR1 catched!";
          sys.deinit(terminate_data_t::abnormal);
          run = false;
          break;
        }
        case SIGUSR2:
        {
          xlogd << "SIGUSR2 catched!";
          sys.deinit(terminate_data_t::critical);
          run = false;
          break;
        }
        case SIGINT:
        {
          xlogd << "SIGINT catched!\n";
          sys.deinit(terminate_data_t::debug);
          run = false;
          break;
        }
        default:
        {
          sleep(1);
          std::cout << __func__ << ": SIG '" << signum << "'' catched";
          xlogd << "SIG '" << signum << "'' catched!";
          sys.deinit(terminate_data_t::abnormal);
          run = false;
          break;
        }
      }
    }
  } catch (std::exception &e) {
    std::cerr << __func__ << ": " << e.what() << "\n";
  }


  if (modelx_timer != nullptr) {
    modelx_timer->stop();
  }

  sleep(1);
  xlogf << "Goodbye modelx...";
  return 0;
}
