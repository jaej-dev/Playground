// region includes
#include "Poco/Logger.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/AutoPtr.h"
// endregion

// region using namespaces
using Poco::Logger;
using Poco::ConsoleChannel;
using Poco::AutoPtr;
// endregion

// region main
int main(int argc, char** argv)
{
    AutoPtr<ConsoleChannel> pCons(new ConsoleChannel); // Create a console channel to send the log messages to 
    Logger::root().setChannel(pCons); // assign the channel to the logger
    Logger& logger = Logger::get("HelloWorldLogger"); // get a Logger reference with any name you like. You can reuse by name for advanced functionality
    logger.information("Hello World!"); // send a log message through the logger to the console channel
    return 0;
}
// endregion
