#include <signal.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <Poco/Logger.h>
#include "client.h"

using Poco::AutoPtr;
using Poco::ConsoleChannel;
using Poco::Logger;

int main(int argc, char** argv) {
  // create log instance
  AutoPtr<ConsoleChannel> pCons(new ConsoleChannel);
  Logger::root().setChannel(pCons);
  Logger& logger = Logger::get("client_logger");
  logger.information("start client program...");

  // create client instance
  IPAddress ip("127.0.0.1");
  uint16_t port = 1976;
  client_t client(ip, port);
  RunnableAdapter<client_t> client_runnable(client, &client_t::send_loop);
  Thread client_thread;

  // set os signal
  int signum;
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGINT);
  sigaddset(&sigset, SIGTERM);
  pthread_sigmask(SIG_BLOCK, &sigset, NULL);

  volatile bool run;
  try {
    client_thread.start(client_runnable);
    run = true;
    while (run) {
      sigwait(&sigset, &signum);
      switch (signum) {
        case SIGINT:
        case SIGTERM:
          logger.information("os signal occured, send clinet exit packet.");
          client_t::send_message(ip, port, "\\comm_exit");
          run = false;
          break;
      };
    }
  } catch (std::exception& e) {
    std::cerr << __func__ << ": " << e.what() << std::endl;
  }

  // delete server instance
  if (client_thread.current()) {
    client_thread.join();
  }
  logger.information("exit client program...");
}
