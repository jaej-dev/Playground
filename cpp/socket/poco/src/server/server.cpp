#include <signal.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <Poco/Logger.h>
#include <Poco/SplitterChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/DateTime.h>
#include "server.h"

using Poco::AutoPtr;
using Poco::ConsoleChannel;
using Poco::Logger;

int main(int argc, char** argv) {
  // create log instance
#if 0
  AutoPtr<ConsoleChannel> pCons(new ConsoleChannel);
  Logger::root().setChannel(pCons);
  Logger& logger = Logger::get("server_logger");
  logger.information("start server program...");
#else
  AutoPtr<SplitterChannel> splitterChannel(new SplitterChannel());
  AutoPtr<Channel> consoleChannel(new ConsoleChannel());
  splitterChannel->addChannel(consoleChannel);
  AutoPtr<Formatter> formatter(new PatternFormatter("%d-%m-%Y %H:%M:%S %s: %t"));
  AutoPtr<Channel> formattingChannel(new FormattingChannel(formatter, splitterChannel));
  Logger& logger = Logger::create("server_logger", formattingChannel, Message::PRIO_TRACE);
  logger.information("start server program...");
#endif

  // create server instance
  uint16_t port = 1976;
  uint32_t buffer_size = 1024;
  server_t server(port, buffer_size);
  RunnableAdapter<server_t> server_runnable(server, &server_t::receive_loop);
  Thread server_thread;

  // set os signal
  int signum;
  sigset_t sigset;
  sigemptyset(&sigset);
  sigaddset(&sigset, SIGINT);
  sigaddset(&sigset, SIGTERM);
  pthread_sigmask(SIG_BLOCK, &sigset, NULL);

  volatile bool run;
  try {
    server_thread.start(server_runnable);
    run = true;
    while (run) {
      sigwait(&sigset, &signum);
      switch (signum) {
        case SIGINT:
        case SIGTERM:
          logger.information("os signal occured, force exit program.");
          run = false;
          break;
      };
    }
  } catch (std::exception& e) {
    std::cerr << __func__ << ": " << e.what() << std::endl;
  }

  // delete server instance
  if (server_thread.current()) {
    server_thread.join();
  }
  logger.information("exit server program...");
}
