#include "guimessageparser.h"

#include <iostream>
#include <msgpack.hpp>
#include "crc.h"


using std::cout;
using std::endl;
using namespace master_gui_protocol;

bool GUIMessageParser::bufferPrintOn_ = false;

ParsingResult::ParsingResult() :
    error(ParsingError::OK),
    usedLength(0)
{}

ParsingResult::ParsingResult(ParsingError error, size_t usedLength) :
    error(error),
    usedLength(usedLength)
{}

bool ParsingResult::isOK()
{
    return error == ParsingError::OK;
}

std::string ParsingResult::description() const
{
    switch (error)
    {
        case ParsingError::OK:
        {
            return "no error";
        }
        case ParsingError::CRCError:
        {
            return "crc error";
        }
        case ParsingError::StartFlagNotFound:
        {
            return "start flag not found";
        }
        case ParsingError::NeedMoreData:
        {
            return "need more data";
        }
        case ParsingError::Unknown:
        {
            return "unknown error occured";
        }
        default:
            return "";
    }
}

std::ostream& operator<<(std::ostream& os, const ParsingResult& obj)
{
  os << obj.description();
  return os;
}

ParsingResult GUIMessageParser::readFromBuffer(char *pBuffer, const size_t bufferSize, uint32_t *pMessageID, msgpack::unpacked *pUnpackedObject)
{
    size_t usedLength = 0;
    char *pCursor = pBuffer;
    char *pStartOfCRCData = nullptr;
    size_t sizeOfCRCData = 0;
    uint32_t receivedMessageID = 0;
    uint32_t dataLength = 0;
    const char startFlag[] = "GUIM";
    size_t startFlagSize = 4;
    size_t headerLength = startFlagSize + sizeof(receivedMessageID) + sizeof(dataLength);
    if (bufferSize > headerLength)
    {
        char *pStartFlag = findStartFlag(pCursor, bufferSize, startFlag);
        if (pStartFlag != nullptr)
        {
            size_t invalidDataSize = pStartFlag - pCursor;
            usedLength = invalidDataSize;
            pCursor += invalidDataSize + startFlagSize;
        }
        else
        {
            printf("Start flag %s not found. Ignored data size: %zu\n", startFlag, bufferSize);
            return ParsingResult(ParsingError::StartFlagNotFound, bufferSize);
        }

        pStartOfCRCData = pCursor;
        memcpy(&receivedMessageID, pCursor, sizeof(receivedMessageID));
        receivedMessageID = ntohl(receivedMessageID);
        pCursor += sizeof(receivedMessageID);
        sizeOfCRCData += sizeof(receivedMessageID);

        memcpy(&dataLength, pCursor, sizeof(dataLength));
        dataLength = ntohl(dataLength);
        pCursor += sizeof(dataLength);
        sizeOfCRCData += sizeof(dataLength);

        sizeOfCRCData += dataLength;

        uint32_t receivedCRC = 0;
        uint32_t calculatedCRC = 0;
        memcpy(&receivedCRC, pCursor + dataLength, sizeof(receivedCRC));
        receivedCRC = ntohl(receivedCRC);

        calculatedCRC = crc32(pStartOfCRCData, sizeOfCRCData);

        size_t sizeOfPacket = startFlagSize + sizeOfCRCData + sizeof(receivedCRC);

        if (receivedCRC != calculatedCRC)
        {
            // ignore data with invalid crc
            usedLength += sizeOfPacket;
            return ParsingResult(ParsingError::CRCError, usedLength);
        }

        if (dataLength > 0)
        {
            msgpack::unpack(pUnpackedObject, pCursor, dataLength);
        }
        *pMessageID = receivedMessageID;

        usedLength += sizeOfPacket;

        if (bufferPrintOn_)
        {
            cout << "Receive data:" << endl;
            printBuffer(pBuffer, usedLength);
        }

        return ParsingResult(ParsingError::OK, usedLength);
    }
    else
    {
        if (bufferSize != 0)
        {
            cout << "Not enough data: " << bufferSize << endl;
        }
        return ParsingResult(ParsingError::NeedMoreData, usedLength);
    }
}

ParsingResult GUIMessageParser::writeToBuffer(char *pBuffer, const size_t bufferSize, uint32_t messageID, msgpack::sbuffer *pPackedObject)
{
    char *pCursor = pBuffer;
    char *pCRCData = nullptr;
    size_t CRCSize = 0;
    const size_t startFlagSize = 4;
    uint32_t networkByteOrder = 0;

    memcpy(pCursor, "GUIM", startFlagSize);
    pCursor += startFlagSize;

    pCRCData = pCursor;

    networkByteOrder = htonl(messageID);
    memcpy(pCursor, &networkByteOrder, sizeof(messageID));
    pCursor += sizeof(messageID);
    CRCSize += sizeof(messageID);

    uint32_t dataLength = pPackedObject->size();
    networkByteOrder = htonl(dataLength);
    memcpy(pCursor, &networkByteOrder, sizeof(dataLength));
    pCursor += sizeof(dataLength);
    CRCSize += sizeof(dataLength);

    memcpy(pCursor, pPackedObject->data(), dataLength);
    pCursor += dataLength;
    CRCSize += dataLength;

    uint32_t crc32data = crc32(pCRCData, CRCSize);
    networkByteOrder = htonl(crc32data);
    memcpy(pCursor, &networkByteOrder, sizeof(crc32data));
    pCursor += sizeof(crc32data);

    size_t sizeToWrite = startFlagSize + sizeof(messageID) + sizeof(dataLength) + dataLength + sizeof(crc32data);

    if (bufferPrintOn_)
    {
        cout << "Send data:" << endl;
        printBuffer(pBuffer, sizeToWrite);
    }

    return ParsingResult(ParsingError::OK, sizeToWrite);
}

char * GUIMessageParser::findStartFlag(char *pCursor, const size_t collectedSize, const std::string startFlag)
{
    char *pEnd = pCursor + collectedSize - startFlag.size();
    while (pCursor <= pEnd)
    {
        char currentData[startFlag.size()];

        memcpy(currentData, pCursor, startFlag.size());

        if (startFlag.c_str()[0] == currentData[0] &&
                startFlag.c_str()[1] == currentData[1] &&
                startFlag.c_str()[2] == currentData[2] &&
                startFlag.c_str()[3] == currentData[3])
        {
            return pCursor;
        }
        else
        {
            pCursor++;
        }
    }

    return nullptr;
}

void GUIMessageParser::setBufferPrint(bool on)
{
    bufferPrintOn_ = on;
}
