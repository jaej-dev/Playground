#include <iostream>
#include <chrono>
#include <thread>
#include <errno.h>
#include "masterGuiMsg.h"
#include "guimessageclient.h"
#include "crc.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;
using namespace master_gui_protocol;

GUIMessageClient controller;

GUIMessageClientResult testGetSpeakerVolume() {
  SpeakerVolumeData data;
  GUIMessageClientResult result = controller.getSpeakerVolume(&data);
  if (result.isOK()) {
    cout << "Get Speaker Volume: " << data.getVolume() << endl;
  } else {
    cout << "Error: " << result << endl;
  }

  return result;
}

GUIMessageClientResult testSetSpeakerVolume() {
  SpeakerVolumeData data;
  data.setVolume(60);
  GUIMessageClientResult result = controller.setSpeakerVolume(data);
  if (result.isOK()) {
    cout << "Set Speaker Volume: " << data.getVolume() << endl;
  } else {
    cout << "Error: " << result << endl;
  }

  return result;
}

GUIMessageClientResult testShowMessage(MessageCategory::Enum category)
{
    MessageData data;
    data.setCategory(category);
    data.setLevel(MessageLevel::Info);
//    data.setDurationMs(2000);
    data.setMessage("Test syste message");
    GUIMessageClientResult result = controller.showMessage(data);
    if (result.isOK())
    {
        cout << "testShowSystemMessage" << endl;
    }
    else
    {
        cout << "Error: " << result << endl;
    }

    return result;
}
GUIMessageClientResult testHideMessage(MessageCategory::Enum category) {
  MessageData data;
  data.setCategory(category);

  GUIMessageClientResult result = controller.hideMessage(data);
  if (result.isOK()) {
    cout << "testHideMessage" << endl;
  } else {
    cout << "Error: " << result << endl;
  }

  return result;
}

int main(int argc, char *argv[]) {
  // CLIENT

  int result = 0;
  int passCount = 0, failCount = 0;
  string localHost = "127.0.0.1";
  if (argc == 3) {
    controller.setEndPoint(argv[1], argv[2]);
    cout << "address: " << argv[1] << ", port: " << argv[2] << endl;
  } else {
    controller.setEndPoint(localHost, DEFAULT_PORT);
  }

  bool quit = false;
  while (quit == false) {
    cout << "Connecting to " << controller.getAddress() << ":"
         << controller.getPort() << endl;
    result = controller.connect();
    if (result == 0) {
      cout << "Connected." << endl;
    } else {
      cout << "Connect failed: " << result << endl;
    }

    bool stop = false;
    while (stop == false) {
      cout << "> ";
      string input;
      char ch = 0;
      cin >> ch;
      cout << "input: " << ch << endl;
      switch (ch) {
      // GUI to SYSTEM
      case '1':
        if (testGetSpeakerVolume().isOK())
          passCount++;
        else
          failCount++;
        break;
      case '2':
        if (testSetSpeakerVolume().isOK())
          passCount++;
        else
          failCount++;
        break;
        // SYSTEM to GUI
      case 'h':
        if (testHideMessage(MessageCategory::System).isOK())
          passCount++;
        else
          failCount++;
        break;
      case 's':
        if (testShowMessage(MessageCategory::System).isOK())
          passCount++;
        else
          failCount++;
        break;
      case 'q':
        stop = true;
        quit = true;
        break;
      default:
        cout << "wrong input" << endl;
        break;
      }
    }

    cout << "Pass: " << passCount << endl;
    cout << "Fail: " << failCount << endl;
  }

  //    getchar();

  cout << "Close connection.." << endl;
  controller.close();
  cout << "Done." << endl;

  return 0;
}
