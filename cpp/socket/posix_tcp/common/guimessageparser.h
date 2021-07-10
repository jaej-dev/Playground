#ifndef _GUI_MSG_PARSER_H_
#define _GUI_MSG_PARSER_H_

#include <stdint.h>
#include <cstddef>
#include <msgpack.hpp>

namespace master_gui_protocol
{

enum class ParsingError : int
{
    OK,
    CRCError,
    StartFlagNotFound,
    NeedMoreData,
    Unknown,
};

struct ParsingResult
{
    ParsingError error;
    size_t usedLength;

    ParsingResult();
    ParsingResult(ParsingError error, size_t usedLength);
    bool isOK();
    std::string description() const;
};

std::ostream& operator<<(std::ostream& os, const ParsingResult& obj);

class GUIMessageParser
{
public:
    static ParsingResult readFromBuffer(char *pBuffer, const size_t bufferSize, uint32_t *pMessageID, msgpack::unpacked *pUnpackedObject);
    static ParsingResult writeToBuffer(char *pBuffer, const size_t bufferSize, uint32_t messageID, msgpack::sbuffer *pPackedObject);
    static char * findStartFlag(char *pBuffer, const size_t bufferSize, const std::string startFlag);
    static void setBufferPrint(bool on);
private:
    GUIMessageParser();
    static bool bufferPrintOn_;
};

}; // namespace master_gui_protocol

#endif // _GUI_MSG_PARSER_H_
