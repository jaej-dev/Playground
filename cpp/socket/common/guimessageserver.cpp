#include "guimessageserver.h"
#include <msgpack.hpp>
#include "util.h"
#include <errno.h>
#include <iostream>
#include <stdint.h>
#include "guimessageparser.h"
#include <sstream>

using namespace master_gui_protocol;

using std::cout;
using std::endl;

GUIMessageServerResult::GUIMessageServerResult() :
    error(GUIMessageServerError::OK),
    networkErrno(0)
{}

GUIMessageServerResult::GUIMessageServerResult(GUIMessageServerError error, int networkErrno) :
    error(error),
    networkErrno(networkErrno)
{}

bool GUIMessageServerResult::isOK()
{
    return error == GUIMessageServerError::OK;
}

std::string GUIMessageServerResult::description() const
{
    switch (error)
    {
        case GUIMessageServerError::Disconnected:
        {
            return "disconnected";
        }
        case GUIMessageServerError::ParsingError:
        {
            return "parsing error";
        }
        case GUIMessageServerError::NetworkError:
        {
            std::ostringstream stream;
            stream << "network error(" << static_cast<int>(error) << ")";
            return stream.str();
        }
        case GUIMessageServerError::ErrorMessageReturned:
        {
            return "error response received";
        }
        case GUIMessageServerError::MemoryAllocationFailed:
        {
            return "memory allocation failed";
        }
        case GUIMessageServerError::UnknownError:
        {
            return "unknown error occured";
        }
        case GUIMessageServerError::OK:
        {
            return "no error";
        }
        default:
            return "";
    }
}

std::ostream& operator<<(std::ostream& os, const GUIMessageServerResult& obj)
{
    os << obj.description();
    return os;
}

GUIMessageServer::GUIMessageServer() :
    pNetworkManager_(nullptr),
    buffer_(BUFFER_EXPONENT),
    mutex_()
{
    pNetworkManager_ = new xnet_t();
    pNetworkManager_->setNoDelayOption(true);
}

GUIMessageServer::~GUIMessageServer()
{
    if (pNetworkManager_ != nullptr)
    {
        pNetworkManager_->close();
        delete pNetworkManager_;
    }
}

void GUIMessageServer::setPort(const std::string& port)
{
    pNetworkManager_->setPort(port);
}

const std::string GUIMessageServer::getAddress()
{
    return pNetworkManager_->getAddress();
}

const std::string GUIMessageServer::getPort()
{
    return pNetworkManager_->getPort();
}

int GUIMessageServer::accept()
{
    return pNetworkManager_->accept();
}

void GUIMessageServer::close()
{
    pNetworkManager_->close();
}

bool GUIMessageServer::isConnected()
{
    return pNetworkManager_->isConnected();
}

void GUIMessageServer::setAcceptTimeout(int timeInMillisec)
{
    pNetworkManager_->setAcceptTimeout(timeInMillisec);
}

void GUIMessageServer::setReadTimeout(int timeInMillisec)
{
    pNetworkManager_->setReadTimeout(timeInMillisec);
}

void GUIMessageServer::setNoDelayOption(bool noDelay)
{
    pNetworkManager_->setNoDelayOption(noDelay);
}

GUIMessageServerResult GUIMessageServer::send(uint32_t messageID, msgpack::sbuffer *pPackedObject)
{
    char messageBuffer[BUFFER_SIZE] = {0, };

    ParsingResult parsingResult = GUIMessageParser::writeToBuffer(messageBuffer, BUFFER_SIZE, messageID, pPackedObject);

    size_t sizeToWrite = parsingResult.usedLength;

    errno = 0;
    ssize_t written = pNetworkManager_->write(messageBuffer, sizeToWrite);
    int result = errno;

    if (result != 0)
    {
        return GUIMessageServerResult(GUIMessageServerError::NetworkError, result);
    }
    else if (sizeToWrite > 0 && written < 1)
    {
        return GUIMessageServerResult(GUIMessageServerError::UnknownError);
    }
    else
    {
        return GUIMessageServerResult(GUIMessageServerError::OK);
    }
}

GUIMessageServerResult GUIMessageServer::receive(uint32_t *pMessageID, msgpack::unpacked *pUnpackedObject)
{
    int readSize;
    int error;

    size_t collectedSize = 0;

    GUIMessageServerResult result;
    ParsingResult parsingResult;

    int retryCount = 0;
    while (retryCount < 3)
    {
        char *pCursor = static_cast<char *>(buffer_.peek(&collectedSize));
//        printf("collected: %d\n", collectedSize);
        if (collectedSize == 0 ||
            parsingResult.error == ParsingError::NeedMoreData ||
            parsingResult.error == ParsingError::StartFlagNotFound)
        {
            size_t availableSize = buffer_.avail();
            char *pReadBuffer = static_cast<char *>(buffer_.alloc(availableSize));
            if (pReadBuffer == 0)
            {
                printf("Alloc failed\n");
                result.error = GUIMessageServerError::MemoryAllocationFailed;
                break;
            }
            errno = 0;
            readSize = pNetworkManager_->read(pReadBuffer, availableSize);
            error = errno;

            if (readSize > -1)
            {
                buffer_.push(readSize);
//                printf("%d bytes read\n", readSize);
                pCursor = static_cast<char *>(buffer_.peek(&collectedSize));
            }
            else
            {
                buffer_.push(0);
            }

            if (error != 0)
            {
                printf("Errno: %d(%s)\n", error, strerror(error));
                result.error = GUIMessageServerError::NetworkError;
                result.networkErrno = error;
                break;
            }
            else if (readSize < 1)
            {
                printf("Disconnected. (readSize:%d)\n", readSize);
                result.error = GUIMessageServerError::Disconnected;
                close();
                break;
            }
        }

        parsingResult = GUIMessageParser::readFromBuffer(pCursor, collectedSize, pMessageID, pUnpackedObject);
        buffer_.pop(parsingResult.usedLength);
//        printf("%d bytes used\n", parsingResult.usedLength);

        if (parsingResult.error == ParsingError::NeedMoreData ||
            parsingResult.error == ParsingError::StartFlagNotFound)
        {
            retryCount++;
            continue;
        }
        else if (parsingResult.error == ParsingError::OK)
        {
            result.error = GUIMessageServerError::OK;
            break;
        }
        else
        {
            result.error = GUIMessageServerError::ParsingError;
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

        return GUIMessageServerResult(GUIMessageServerError::UnknownError);
    }
    else
    {
        return result;
    }
}

GUIMessageServerResult GUIMessageServer::sendNormalResponse(uint32_t messageID)
{
    msgpack::sbuffer packed;
    ResponseData response;
    msgpack::pack(packed, response);
    GUIMessageServerResult result = send(messageID, &packed);
    if (result.isOK() == false)
    {
        cout << "Send failed: " << result << endl;
    }

    return result;
}

void GUIMessageServer::initBuffer()
{
    buffer_.clear();
}
