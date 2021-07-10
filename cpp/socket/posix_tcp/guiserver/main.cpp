#include <iostream>
#include <errno.h>
#include "masterGuiMsg.h"
#include "guimessageparser.h"
#include "guimessageserver.h"

using namespace std;
using namespace master_gui_protocol;

int main(int argc, char *argv[]) {
  // SERVER
  SpeakerVolumeData speakerVolume;
  speakerVolume.setVolume(80);

  GUIMessageServerResult result;
  int networkResult = 0;

  GUIMessageServer controller;

  if (argc == 2) {
    controller.setPort(argv[1]);
    cout << "port: " << argv[1] << endl;
  } else {
    controller.setPort(DEFAULT_PORT);
  }

  while (true) // accept loop
  {

    cout << "Wait on " << controller.getPort() << endl;
    networkResult = controller.accept();
    if (networkResult == 0) {
      cout << "Accepted." << endl;
    } else {
      cout << "Accept failed: " << result << endl;
    }

    while (controller.isConnected()) // receive loop
    {
      uint32_t messageID = 0;
      msgpack::unpacked unpacked;
      cout << "Wait for message.." << endl;
      result = controller.receive(&messageID, &unpacked);
      if (result.isOK()) {
        if (messageID != GUIMessage::UNDEFINED &&
            messageID != MasterMsg::UNDEFINED) {
          cout << master_gui_protocol::toString(messageID) << " received."
               << endl;
        }
        switch (messageID) {
        case GUIMessage::GET_SPEAKER_VOLUME: {
          msgpack::sbuffer packed;
          msgpack::pack(packed, speakerVolume);
          result = controller.send(messageID, &packed);
          if (result.isOK() == false) {
            cout << "Send failed: " << result << endl;
          }
          break;
        }
        case GUIMessage::SET_SPEAKER_VOLUME: {
          SpeakerVolumeData data;
          unpacked.get().convert(&data);
          speakerVolume.setVolume(data.getVolume());

          msgpack::sbuffer packed;
          ResponseData response;
          msgpack::pack(packed, response);
          result = controller.send(messageID, &packed);
          if (result.isOK() == false) {
            cout << "Send failed: " << result << endl;
          }
          break;
        }
        case MasterMsg::SHOW_MESSAGE: {

          MessageData data;
          unpacked.get().convert(&data);
          cout << "< show message >" << endl;
          cout << data;

          msgpack::sbuffer packed;
          ResponseData response;
          msgpack::pack(packed, response);
          result = controller.send(messageID, &packed);
          if (result.isOK() == false) {
            cout << "Send failed: " << result << endl;
          }
          break;
        }
        case MasterMsg::HIDE_MESSAGE: {

          MessageData data;
          unpacked.get().convert(&data);
          cout << "< hide message >" << endl;
          cout << data;

          msgpack::sbuffer packed;
          ResponseData response;
          msgpack::pack(packed, response);
          result = controller.send(messageID, &packed);
          if (result.isOK() == false) {
            cout << "Send failed: " << result << endl;
          }
          break;
        }
        case GUIMessage::UNDEFINED:
          // ignore message
          break;
        default: {
          printf("No matched message id: 0X%08X\n", messageID);
          msgpack::sbuffer packed;
          ErrorData errorData;
          msgpack::pack(packed, errorData);
          result = controller.send(GUIMessage::GUI_MESSAGE_ERROR, &packed);
          if (result.isOK() == false) {
            cout << "Send failed: " << result << endl;
          }
          break;
        }
        }
      } else {
        if (controller.isConnected() == false) {
          cout << "Disconnected." << endl;
        } else {
          cout << "Receive failed: " << result << endl;
        }
      }
    } // receive loop

    cout << "Close connection.." << endl;
    controller.close();
  }

  return 0;
}
