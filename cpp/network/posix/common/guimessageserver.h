#ifndef _GUI_MSG_SERVER_H_
#define _GUI_MSG_SERVER_H_

#include <mutex>
#include "masterGuiMsg.h"
#include "xnet/xnet.h"
#include "xbuf/xbuf.h"

enum class GUIMessageServerError
{
    Disconnected = -6,
    ParsingError = -5,
    NetworkError = -4,
    ErrorMessageReturned = -3,
    MemoryAllocationFailed = -2,
    UnknownError = -1,
    OK = 0,
};

struct GUIMessageServerResult
{
    GUIMessageServerError error;
    int networkErrno;

    GUIMessageServerResult();
    GUIMessageServerResult(GUIMessageServerError error, int networkErrno = 0);
    bool isOK();
    std::string description() const;
};

std::ostream& operator<<(std::ostream& os, const GUIMessageServerResult& obj);

class GUIMessageServer
{
public:
    static const size_t BUFFER_EXPONENT = 15;
    static const size_t BUFFER_SIZE = 1 << BUFFER_EXPONENT;
    GUIMessageServer();
    virtual ~GUIMessageServer();
    void setPort(const std::string& port);
    void setAcceptTimeout(int timeInMillisec);
    void setReadTimeout(int timeInMillisec);
    void setNoDelayOption(bool noDelay);
    const std::string getAddress();
    const std::string getPort();
    int accept();
    void close();
    bool isConnected();
    GUIMessageServerResult send(uint32_t messageID, msgpack::sbuffer *pPackedObject);
    GUIMessageServerResult receive(uint32_t *pMessageID, msgpack::unpacked *pUnpackedObject);
    GUIMessageServerResult sendNormalResponse(uint32_t messageID);
    char * findStartFlag(char *pCursor, const size_t collectedSize, const std::string startFlag);

    void initBuffer();
private:
    xnet_t *pNetworkManager_;
    xbuf_t buffer_;
    std::mutex mutex_;
};

#endif // _GUI_MSG_SERVER_H_
