#include "masterGuiMsg.h"
#include <iostream>

using namespace master_gui_protocol;
using std::cout;
using std::endl;


std::string master_gui_protocol::toString(uint32_t messageId)
{
    switch (messageId)
    {
        case GUIMessage::UNDEFINED:
            return "UNDEFINED";
        case GUIMessage::GET_SPEAKER_VOLUME:
            return "GET_SPEAKER_VOLUME";
        case GUIMessage::SET_SPEAKER_VOLUME:
            return "SET_SPEAKER_VOLUME";
        case GUIMessage::GUI_MESSAGE_ERROR:
            return "GUI_MESSAGE_ERROR";

        case MasterMsg::SHOW_MESSAGE:
            return "SHOW_MESSAGE";
        case MasterMsg::HIDE_MESSAGE:
            return "HIDE_MESSAGE";
        case MasterMsg::MASTER_MESSAGE_ERROR:
            return "MASTER_MESSAGE_ERROR";
        default:
            return "unknown message id";
    }
}
