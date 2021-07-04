#ifndef _GUI_MSG_CLIENT_H_
#define _GUI_MSG_CLIENT_H_

#include <mutex>
#include "masterGuiMsg.h"
#include "xnet/xnet.h"
#include "xbuf/xbuf.h"

enum class GUIMessageClientError
{
    MessageNotMatched = -8,
    NullPointerPassed = -7,
    Disconnected = -6,
    ParsingError = -5,
    NetworkError = -4,
    ErrorMessageReturned = -3,
    MemoryAllocationFailed = -2,
    UnknownError = -1,
    OK = 0,
};

struct GUIMessageClientResult
{
    GUIMessageClientError error;
    int networkErrno;

    GUIMessageClientResult();
    GUIMessageClientResult(GUIMessageClientError error, int networkErrno = 0);
    bool isOK();
    bool hasError();
    std::string description() const;
};

std::ostream& operator<<(std::ostream& os, const GUIMessageClientResult& obj);

class GUIMessageClient
{
public:
    static const size_t BUFFER_EXPONENT = 15;
    static const size_t BUFFER_SIZE = 1 << BUFFER_EXPONENT;
    GUIMessageClient();
    virtual ~GUIMessageClient();
    void setEndPoint(const std::string& address, const std::string& port);
    void setPort(const std::string& port);
    void setConnectTimeout(int timeInMillisec);
    void setAcceptTimeout(int timeInMillisec);
    void setReadTimeout(int timeInMillisec);
    void setNoDelayOption(bool noDelay);
    const std::string getAddress();
    const std::string getPort();
    int connect();
    int accept();
    void close();
    bool isConnected();
    void initBuffer();
    GUIMessageClientResult send(uint32_t messageID, msgpack::sbuffer *pPackedObject);
    GUIMessageClientResult receive(uint32_t *pMessageID, msgpack::unpacked *pUnpackedObject);
    char * findStartFlag(char *pCursor, const size_t collectedSize, const std::string startFlag);
    GUIMessageClientResult sendReceive(uint32_t messageID,
                    msgpack::sbuffer *packedForSend,
                    msgpack::unpacked *unpackedForReceive);

    // GUI -> Raphael Messages
    GUIMessageClientResult getSpeakerVolume(master_gui_protocol::SpeakerVolumeData *pData);
    GUIMessageClientResult setSpeakerVolume(const master_gui_protocol::SpeakerVolumeData &data);

    // Raphael -> GUI Messages
    GUIMessageClientResult showMessage(const master_gui_protocol::MessageData &data);
    GUIMessageClientResult hideMessage(const master_gui_protocol::MessageData &data);


    template <typename DataT, typename MessageIDT>
    GUIMessageClientResult setData(MessageIDT messageID, const DataT &data)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        GUIMessageClientResult result;

        if (pNetworkManager_->isConnected())
        {
            msgpack::sbuffer packed;
            msgpack::pack(packed, data);
            uint32_t id = messageID;
            msgpack::unpacked unpacked;
            result = sendReceive(id, &packed, &unpacked);
            if (result.isOK())
            {
                master_gui_protocol::ResponseData response;
                unpacked.get().convert(&response);
            }
            else
            {
                printf("sendReceive failed. IP: %s, sent: 0X%08X(%s), reason: %s, errno: %d\n", getAddress().c_str(),
                       id, master_gui_protocol::toString(id).c_str(),
                       result.description().c_str(), result.networkErrno);
            }
        }
        else
        {
            result.error = GUIMessageClientError::Disconnected;
        }

        return result;
    }

    template <typename MessageIDT, typename DataT>
    GUIMessageClientResult getData(MessageIDT messageID, DataT *pData)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        GUIMessageClientResult result;

        if (pNetworkManager_->isConnected())
        {
            msgpack::sbuffer packed;
            uint32_t id = messageID;
            msgpack::unpacked unpacked;
            result = sendReceive(id, &packed, &unpacked);
            if (result.isOK())
            {
                unpacked.get().convert(pData);
            }
            else
            {
                printf("sendReceive failed. IP: %s, sent: 0X%08X(%s), reason: %s, errno: %d\n", getAddress().c_str(),
                       id, master_gui_protocol::toString(id).c_str(),
                       result.description().c_str(), result.networkErrno);
            }
        }
        else
        {
            result.error = GUIMessageClientError::Disconnected;
        }

        return result;
    }

    template <typename MessageIDT, typename DataT, typename DataArgT>
    GUIMessageClientResult getDataArg(MessageIDT messageID, DataT *pData, DataArgT &dataArg)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        GUIMessageClientResult result;

        if (pNetworkManager_->isConnected())
        {
            msgpack::sbuffer packed;
            msgpack::pack(packed, dataArg);
            uint32_t id = messageID;
            msgpack::unpacked unpacked;
            result = sendReceive(id, &packed, &unpacked);
            if (result.isOK())
            {
                unpacked.get().convert(pData);
            }
            else
            {
                printf("sendReceive failed. IP: %s, sent: 0X%08X(%s), reason: %s, errno: %d\n", getAddress().c_str(),
                       id, master_gui_protocol::toString(id).c_str(),
                       result.description().c_str(), result.networkErrno);
            }
        }
        else
        {
            result.error = GUIMessageClientError::Disconnected;
        }

        return result;
    }

    template <typename MessageIDT>
    GUIMessageClientResult sendMessage(MessageIDT messageID)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        GUIMessageClientResult result;

        if (pNetworkManager_->isConnected())
        {
            msgpack::sbuffer packed;
            uint32_t id = messageID;
            msgpack::unpacked unpacked;
            result = sendReceive(id, &packed, &unpacked);
            if (result.isOK())
            {
                master_gui_protocol::ResponseData response;
                unpacked.get().convert(&response);
            }
            else
            {
                printf("sendReceive failed. IP: %s, sent: 0X%08X(%s), reason: %s, errno: %d\n", getAddress().c_str(),
                       id, master_gui_protocol::toString(id).c_str(),
                       result.description().c_str(), result.networkErrno);
            }
        }
        else
        {
            result.error = GUIMessageClientError::Disconnected;
        }

        return result;
    }



private:
    xnet_t *pNetworkManager_;
    xbuf_t buffer_;
    std::mutex mutex_;


};

#endif // _GUI_MSG_CLIENT_H_
