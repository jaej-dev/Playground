
#ifndef MASTER_GUI_MESSAGE_H_
#define MASTER_GUI_MESSAGE_H_

#include <msgpack.hpp>
#include <string>
#include "guimessagedata.h"

#define DEFAULT_PORT ("1976")

namespace master_gui_protocol {

std::string toString(uint32_t messageId);

namespace GUIMessage
{
    enum Enum {
        UNDEFINED          = 0x00000000,
        GET_SPEAKER_VOLUME = 0x00000001,
        SET_SPEAKER_VOLUME = 0x00000002,
        GUI_MESSAGE_ERROR  = 0xF0000000,
    };

    inline std::string toString(uint32_t messageId)
    {
        return master_gui_protocol::toString(messageId);
    }

    inline std::ostream& operator<<(std::ostream& os, Enum enumValue)
    {
        os << toString(enumValue);
        return os;
    }
} // namespace GUIMessage

/////////////////////////////////////////////////////////////////////////////////////

namespace MasterMsg
{
    enum Enum {
        UNDEFINED           = 0x00000000,
        SHOW_MESSAGE        = 0x00010001,
        HIDE_MESSAGE        = 0x00010002,
        MASTER_MESSAGE_ERROR = 0xF0010000,
    };

    inline std::string toString(uint32_t messageId)
    {
        return master_gui_protocol::toString(messageId);
    }

    inline std::ostream& operator<<(std::ostream& os, Enum enumValue)
    {
        os << toString(enumValue);
        return os;
    }
} // namespace MasterMsg



} // namespace master_gui_protocol

#endif /* MASTER_GUI_MESSAGE_H_ */
