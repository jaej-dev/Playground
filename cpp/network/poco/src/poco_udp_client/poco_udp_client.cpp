#include <signal.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <Poco/Logger.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/DateTime.h>
#include "poco_udp_client.h"

using Poco::AutoPtr;
using Poco::ConsoleChannel;
using Poco::Logger;

int main(int argc, char** argv) {
  // create log instance
  AutoPtr<SplitterChannel> splitterChannel(new SplitterChannel());
  AutoPtr<Channel> consoleChannel(new ConsoleChannel());
  splitterChannel->addChannel(consoleChannel);
  AutoPtr<Formatter> formatter(new PatternFormatter("%d-%m-%Y %H:%M:%S %s: %t"));
  AutoPtr<Channel> formattingChannel(new FormattingChannel(formatter, splitterChannel));
  Logger& logger = Logger::create("client_logger", formattingChannel, Message::PRIO_TRACE);
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
