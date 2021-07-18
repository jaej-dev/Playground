#include "guimessageclient.h"
#include <msgpack.hpp>
#include "crc.h"
#include <errno.h>
#include <iostream>
#include <stdint.h>
#include "guimessageparser.h"
#include <sstream>

using namespace master_gui_protocol;
namespace mgp = master_gui_protocol;

using std::cout;
using std::endl;

GUIMessageClientResult::GUIMessageClientResult() :
    error(GUIMessageClientError::OK),
    networkErrno(0)
{}

GUIMessageClientResult::GUIMessageClientResult(GUIMessageClientError error, int networkErrno) :
    error(error),
    networkErrno(networkErrno)
{}

bool GUIMessageClientResult::isOK()
{
    return error == GUIMessageClientError::OK;
}

bool GUIMessageClientResult::hasError()
{
    return error != GUIMessageClientError::OK;
}

std::string GUIMessageClientResult::description() const
{
    switch (error)
    {
        case GUIMessageClientError::MessageNotMatched:
        {
            return "message not matched";
        }
        case GUIMessageClientError::NullPointerPassed:
        {
            return "null pointer passed";
        }
        case GUIMessageClientError::Disconnected:
        {
            return "disconnected";
        }
        case GUIMessageClientError::ParsingError:
        {
            return "parsing error";
        }
        case GUIMessageClientError::NetworkError:
        {
            std::ostringstream stream;
            stream << "network error(" << static_cast<int>(error) << ")";
            return stream.str();
        }
        case GUIMessageClientError::ErrorMessageReturned:
        {
            return "error response received";
        }
        case GUIMessageClientError::MemoryAllocationFailed:
        {
            return "memory allocation failed";
        }
        case GUIMessageClientError::UnknownError:
        {
            return "unknown error occured";
        }
        case GUIMessageClientError::OK:
        {
            return "no error";
        }
        default:
            return "no description";
    }
}

std::ostream& operator<<(std::ostream& os, const GUIMessageClientResult& obj)
{
    os << obj.description();
    return os;
}

GUIMessageClient::GUIMessageClient() :
    pNetworkManager_(nullptr),
    buffer_(BUFFER_EXPONENT),
    mutex_()
{
    pNetworkManager_ = new xnet_t();
    pNetworkManager_->setNoDelayOption(true);
}

GUIMessageClient::~GUIMessageClient()
{
    if (pNetworkManager_ != nullptr)
    {
        pNetworkManager_->close();
        delete pNetworkManager_;
    }
}

void GUIMessageClient::setEndPoint(const std::string& address, const std::string& port)
{
    pNetworkManager_->setEndPoint(address, port);
}

void GUIMessageClient::setPort(const std::string& port)
{
    pNetworkManager_->setPort(port);
}

const std::string GUIMessageClient::getAddress()
{
    return pNetworkManager_->getAddress();
}

const std::string GUIMessageClient::getPort()
{
    return pNetworkManager_->getPort();
}

int GUIMessageClient::connect()
{
    return pNetworkManager_->connect();
}

int GUIMessageClient::accept()
{
    return pNetworkManager_->accept();
}

void GUIMessageClient::close()
{
    pNetworkManager_->close();
}

bool GUIMessageClient::isConnected()
{
    return pNetworkManager_->isConnected();
}

void GUIMessageClient::initBuffer()
{
    buffer_.clear();
}

void GUIMessageClient::setConnectTimeout(int timeInMillisec)
{
    pNetworkManager_->setConnectTimeout(timeInMillisec);
}

void GUIMessageClient::setAcceptTimeout(int timeInMillisec)
{
    pNetworkManager_->setAcceptTimeout(timeInMillisec);
}

void GUIMessageClient::setReadTimeout(int timeInMillisec)
{
    pNetworkManager_->setReadTimeout(timeInMillisec);
}

void GUIMessageClient::setNoDelayOption(bool noDelay)
{
    pNetworkManager_->setNoDelayOption(noDelay);
}

GUIMessageClientResult GUIMessageClient::send(uint32_t messageID, msgpack::sbuffer *pPackedObject)
{
    char messageBuffer[BUFFER_SIZE] = {0, };

    ParsingResult parsingResult = GUIMessageParser::writeToBuffer(messageBuffer, BUFFER_SIZE, messageID, pPackedObject);

    size_t sizeToWrite = parsingResult.usedLength;

    errno = 0;
    ssize_t written = pNetworkManager_->write(messageBuffer, sizeToWrite);
    int result = errno;

    if (result != 0)
    {
        return GUIMessageClientResult(GUIMessageClientError::NetworkError, result);
    }
    else if (sizeToWrite > 0 && written < 1)
    {
        return GUIMessageClientResult(GUIMessageClientError::UnknownError);
    }
    else
    {
        return GUIMessageClientResult(GUIMessageClientError::OK);
    }
}

GUIMessageClientResult GUIMessageClient::receive(uint32_t *pMessageID, msgpack::unpacked *pUnpackedObject)
{
    int readSize;
    int error;
    size_t collectedSize = 0;

    GUIMessageClientResult result;
    ParsingResult parsingResult;

    int retryCount = 0;
    while (retryCount < 3)
    {
        char *pReadBuffer = static_cast<char *>(buffer_.alloc(BUFFER_SIZE));
        if (pReadBuffer == 0)
        {
            printf("Alloc failed\n");
            result.error = GUIMessageClientError::MemoryAllocationFailed;
            if (buffer_.data_len() > 0)
            {
                cout << "Ignore buffered data: " << buffer_.data_len() << endl;
                buffer_.clear();
            }
            break;
        }
        errno = 0;
        readSize = pNetworkManager_->read(pReadBuffer, BUFFER_SIZE);
        error = errno;

        if (readSize > -1)
        {
            buffer_.push(readSize);
        }
        else
        {
            buffer_.push(0);
        }

        if (error != 0)
        {
            printf("errno: %d(%s)\n", error, strerror(error));
            if (readSize > 0)
            {
                printf("Read data(%d bytes) will be ignored.", readSize);
                buffer_.pop(readSize);
            }
            result.error = GUIMessageClientError::NetworkError;
            result.networkErrno = error;
            break;
        }
        else if (readSize < 1)
        {
            printf("Disconnected. (readSize:%d)\n", readSize);
            result.error = GUIMessageClientError::Disconnected;
            close();
            break;
        }

        char *pCursor = static_cast<char *>(buffer_.peek(&collectedSize));
        parsingResult = GUIMessageParser::readFromBuffer(pCursor, collectedSize, pMessageID, pUnpackedObject);
        buffer_.pop(parsingResult.usedLength);

        if (parsingResult.error == ParsingError::NeedMoreData ||
            parsingResult.error == ParsingError::StartFlagNotFound)
        {
            retryCount++;
            continue;
        }
        else if (parsingResult.error == ParsingError::OK)
        {
            result.error = GUIMessageClientError::OK;
            break;
        }
        else
        {
            result.error = GUIMessageClientError::ParsingError;
            break;
        }
    }

    if (retryCount > 0)
    {
        cout << "Warning: Retry read count " << retryCount << endl;
    }

    if (retryCount == 3)
    {
        cout << "Ignore buffered data: " << buffer_.data_len() << endl;
        buffer_.clear();

        return GUIMessageClientResult(GUIMessageClientError::UnknownError);
    }
    else
    {
        return result;
    }
}

GUIMessageClientResult GUIMessageClient::sendReceive(uint32_t messageID,
                                      msgpack::sbuffer *packedForSend,
                                      msgpack::unpacked *unpackedForReceive)
{
    GUIMessageClientResult result;
    result = send(messageID, packedForSend);
    if (result.isOK())
    {
        uint32_t receivedMessageID = 0;
        result = receive(&receivedMessageID, unpackedForReceive);
        if (result.isOK())
        {
            if (receivedMessageID != messageID)
            {
                printf("Returned message id is not matched: 0X%08X\n", receivedMessageID);
                if (receivedMessageID == GUIMessage::GUI_MESSAGE_ERROR)
                {
                    result.error = GUIMessageClientError::ErrorMessageReturned;
                }
                else
                {
                    result.error = GUIMessageClientError::MessageNotMatched;
                }
            }
        }
        else
        {
            cout << "Receive failed: " << result.description() << endl;
        }
    }
    else
    {
        cout << "Send failed: " << result.description() << endl;
    }

    return result;
}


GUIMessageClientResult GUIMessageClient::getSpeakerVolume(SpeakerVolumeData *pData)
{
    std::lock_guard<std::mutex> lock(mutex_);
    GUIMessageClientResult result;
    if (pNetworkManager_->isConnected())
    {
        msgpack::sbuffer packed; // no data is needed

        uint32_t messageID = GUIMessage::GET_SPEAKER_VOLUME;
        msgpack::unpacked unpacked;
        result = sendReceive(messageID, &packed, &unpacked);
        if (result.isOK())
        {
            if (pData != nullptr)
            {
                unpacked.get().convert(pData);
            }
        }
        else
        {
            printf("sendReceive failed. IP: %s, sent: 0X%08X(%s), reason: %s, errno: %d\n", getAddress().c_str(),
                   messageID, GUIMessage::toString(messageID).c_str(),
                   result.description().c_str(), result.networkErrno);
        }
    }
    else
    {
        result.error = GUIMessageClientError::Disconnected;
    }

    return result;
}

GUIMessageClientResult GUIMessageClient::setSpeakerVolume(const SpeakerVolumeData &data)
{
    std::lock_guard<std::mutex> lock(mutex_);
    GUIMessageClientResult result;
    if (pNetworkManager_->isConnected())
    {
        msgpack::sbuffer packed;
        msgpack::pack(packed, data);

        uint32_t messageID = GUIMessage::SET_SPEAKER_VOLUME;
        msgpack::unpacked unpacked;
        result = sendReceive(messageID, &packed, &unpacked);
        if (result.isOK())
        {
            ResponseData response;
            unpacked.get().convert(&response);
//            cout << "Message: " << messageID << "Response: " << response.getData() << endl;
        }
        else
        {
            printf("sendReceive failed. IP: %s, sent: 0X%08X(%s), reason: %s, errno: %d\n", getAddress().c_str(),
                   messageID, GUIMessage::toString(messageID).c_str(),
                   result.description().c_str(), result.networkErrno);
        }
    }
    else
    {
        result.error = GUIMessageClientError::Disconnected;
    }

    return result;
}

GUIMessageClientResult GUIMessageClient::showMessage(const master_gui_protocol::MessageData &data)
{
    std::lock_guard<std::mutex> lock(mutex_);
    GUIMessageClientResult result;

    if (pNetworkManager_->isConnected())
    {
        msgpack::sbuffer packed;
        msgpack::pack(packed, data);
        uint32_t messageID = MasterMsg::SHOW_MESSAGE;
        msgpack::unpacked unpacked;
        result = sendReceive(messageID, &packed, &unpacked);
        if (result.isOK())
        {
            ResponseData response;
            unpacked.get().convert(&response);
        }
        else
        {
            printf("sendReceive failed. IP: %s, sent: 0X%08X(%s), reason: %s, errno: %d\n", getAddress().c_str(),
                   messageID, GUIMessage::toString(messageID).c_str(),
                   result.description().c_str(), result.networkErrno);
        }
    }
    else
    {
        result.error = GUIMessageClientError::Disconnected;
    }

    return result;
}

GUIMessageClientResult GUIMessageClient::hideMessage(const master_gui_protocol::MessageData &data)
{
    std::lock_guard<std::mutex> lock(mutex_);
    GUIMessageClientResult result;

    if (pNetworkManager_->isConnected())
    {
        msgpack::sbuffer packed;
        msgpack::pack(packed, data);
        uint32_t messageID = MasterMsg::HIDE_MESSAGE;
        msgpack::unpacked unpacked;
        result = sendReceive(messageID, &packed, &unpacked);
        if (result.isOK())
        {
            ResponseData response;
            unpacked.get().convert(&response);
        }
        else
        {
            printf("sendReceive failed. IP: %s, sent: 0X%08X(%s), reason: %s, errno: %d\n", getAddress().c_str(),
                   messageID, GUIMessage::toString(messageID).c_str(),
                   result.description().c_str(), result.networkErrno);
        }
    }
    else
    {
        result.error = GUIMessageClientError::Disconnected;
    }

    return result;
}

