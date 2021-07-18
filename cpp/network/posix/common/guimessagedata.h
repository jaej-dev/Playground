#ifndef _GUI_MSG_DATA_H_
#define _GUI_MSG_DATA_H_

#include <msgpack.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <array>

namespace master_gui_protocol {

class SpeakerVolumeData
{
private:
    int volume_;

public:
    MSGPACK_DEFINE(volume_)

    SpeakerVolumeData(int volume) : volume_(volume)
    {}

    SpeakerVolumeData() : volume_(0)
    {}

    inline int getVolume() const { return volume_; }
    inline void setVolume(int volume) { volume_ = volume; }
};

inline std::ostream& operator<<(std::ostream& os, SpeakerVolumeData &obj)
{
    os << obj.getVolume();
    return os;
}

namespace MessageCategory
{
enum Enum
{
    System,
    LeftMasterArm,
    RightMasterArm,
    LeftSlaveArmMain,
    LeftSlaveArmSub,
    RightSlaveArmMain,
    RightSlaveArmSub,
    CameraArm,
    Console,
    OperationCart,
    LeftSlaveArmCannula,
    RightSlaveArmCannula,
    LeftSlaveArmSubCannula,
    RightSlaveArmSubCannula,
    CameraArmCannula,
    ErrorMessageBox,
};
}

namespace MessageLevel
{
enum Enum
{
    Info = 0,
    Warning,
    RecoverableError,
    FatalError,
};
}

namespace MessageIcon
{
enum Enum
{
    NotDefined = 0,

    SystemInfo,
    SystemWarning,
    SystemFatalError,
};
}


class MessageData
{
private:
    MessageCategory::Enum category_;
    MessageLevel::Enum level_;
    std::string message_;
    MessageIcon::Enum icon_;
    int duration_;

public:
    MSGPACK_DEFINE((int&)category_, (int&)level_, message_, (int&)icon_, duration_)

    MessageData(MessageCategory::Enum category, MessageLevel::Enum level, std::string message, MessageIcon::Enum icon, int durationMs) :
        category_(category),
        level_(level),
        message_(message),
        icon_(icon),
        duration_(durationMs)
    {}

    MessageData() :
        category_(MessageCategory::System),
        level_(MessageLevel::Info),
        message_(),
        icon_(MessageIcon::NotDefined),
        duration_(0) // no timer
    {}

    MessageCategory::Enum getCategory() const { return category_; }
    void setCategory(MessageCategory::Enum category) { category_ = category; }

    MessageLevel::Enum getLevel() const { return level_; }
    void setLevel(MessageLevel::Enum level) { level_ = level; }

    std::string getMessage() const { return message_; }
    void setMessage(std::string message) { message_ = message; }

    MessageIcon::Enum getIcon() const { return icon_; }
    void setIcon(MessageIcon::Enum icon) { icon_ = icon; }

    int getDurationMs() const { return duration_; }
    void setDurationMs(int durationMs) { duration_ = durationMs; }

    std::string description() const;
};

inline std::ostream& operator<<(std::ostream& os, MessageData &obj)
{
    os << obj.description();
    return os;
}

enum RETURN_VALUE
{
    FAILED = 0,
    SUCCEED
};

class ErrorData
{
public:
    enum ErrorCode
    {
        UNKNOWN_MESSAGE = 0,
        NOT_YET_REALIZATION,
        UNKNOWN_ERROR
    };

    static const char* toString(ErrorCode i)
    {
        static const char* s[] = {
                "Unknown message id",
                "Not yet realization",
                "unknown"
        };
        return s[i];
    }

    MSGPACK_DEFINE(messageId_, (int&)error_)

    ErrorData(int messageId, ErrorCode error)
        : messageId_(messageId), error_(error) {}

    ErrorData()
        : messageId_(), error_() {}

    int getMessageId() const { return messageId_; }
    void setMessageId(int messageId) { messageId_ = messageId; }

    ErrorCode getErrorCode() const { return error_; }
    void setErrorCode(ErrorCode error) { error_ = error; }

private:
    int messageId_;
    ErrorCode error_;
};

inline std::ostream& operator<<(std::ostream& os, ErrorData &obj)
{
    os << "id: " << obj.getMessageId() << ", error: " << ErrorData::toString(obj.getErrorCode());
    return os;
}

class ResponseData
{
public:
    MSGPACK_DEFINE(data_)

    ResponseData(int data)
        : data_(data) {}

    ResponseData()
        : data_() {}

    int getData() const { return data_; }
    void setData(int data) { data_ = data; }

private:
    int data_;
};

inline std::ostream& operator<<(std::ostream& os, ResponseData &obj)
{
    os << obj.getData();
    return os;
}

} // namespace master_gui_protocol

#endif // _GUI_MSG_DATA_H_
