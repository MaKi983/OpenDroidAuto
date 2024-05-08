#pragma once

#include <common/Data.hpp>
#include <messenger/ChannelId.hpp>
#include <messenger/FrameType.hpp>
#include <messenger/EncryptionType.hpp>
#include <messenger/MessageType.hpp>


namespace aasdk
{
namespace messenger
{

class FrameHeader
{
public:
    FrameHeader(const common::DataConstBuffer& buffer);
    FrameHeader(ChannelId channelId, FrameType frameType, EncryptionType encryptionType, MessageType messageType);
    ~FrameHeader();
    ChannelId getChannelId() const;
    FrameType getType() const;
    EncryptionType getEncryptionType() const;
    MessageType getMessageType() const;
    common::Data getData() const;

    static constexpr size_t getSizeOf() { return 2; }

    std::string toString();
private:
    ChannelId channelId_;
    FrameType frameType_;
    EncryptionType encryptionType_;
    MessageType messageType_;
};

}
}
