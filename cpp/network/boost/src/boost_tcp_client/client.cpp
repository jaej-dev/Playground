#include <signal.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include "client.h"

int main(int argc, char** argv) {

  //
  // set os signal
  int signum;
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGINT);
  sigaddset(&sigset, SIGTERM);
  pthread_sigmask(SIG_BLOCK, &sigset, NULL);

  volatile bool run;
  try {
    run = true;
    while (run) {
      sigwait(&sigset, &signum);
      switch (signum) {
        case SIGINT:
        case SIGTERM:
          run = false;
          break;
      };
    }
  } catch (std::exception& e) {
    std::cerr << __func__ << ": " << e.what() << std::endl;
  }

  // delete server instance
}
