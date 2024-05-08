#include <boost/endian/conversion.hpp>
#include <messenger/FrameHeader.hpp>

namespace aasdk
{
namespace messenger
{

FrameHeader::FrameHeader(const common::DataConstBuffer& buffer)
{
    channelId_ = static_cast<ChannelId>(buffer.cdata[0]);
    frameType_ = static_cast<FrameType>(buffer.cdata[1] & static_cast<uint32_t>(FrameType::BULK));
    encryptionType_ = static_cast<EncryptionType>(buffer.cdata[1] & static_cast<uint8_t>(EncryptionType::ENCRYPTED));
    messageType_ = static_cast<MessageType>(buffer.cdata[1] & static_cast<uint8_t>(MessageType::CONTROL));
}

FrameHeader::FrameHeader(ChannelId channelId, FrameType frameType, EncryptionType encryptionType, MessageType messageType)
    : channelId_(channelId)
    , frameType_(frameType)
    , encryptionType_(encryptionType)
    , messageType_(messageType)
{
}

FrameHeader::~FrameHeader() {
}

ChannelId FrameHeader::getChannelId() const
{
    return channelId_;
}

FrameType FrameHeader::getType() const
{
    return frameType_;
}

EncryptionType FrameHeader::getEncryptionType() const
{
    return encryptionType_;
}

MessageType FrameHeader::getMessageType() const
{
    return messageType_;
}

common::Data FrameHeader::getData() const
{
    common::Data data;

    data.push_back(static_cast<uint8_t>(channelId_));
    data.push_back(static_cast<uint8_t>(encryptionType_) | static_cast<uint8_t>(messageType_) | static_cast<uint8_t>(frameType_));

    return data;
}

std::string FrameHeader::toString() {
    return "FrameHeader[ channelId: "  + channelIdToString(channelId_)
            + ", frameType: " + frameTypeToString(frameType_)
            + ", encryptionType: " + encryptionTypeToString(encryptionType_)
            + ", messageType; " + messageTypeToString(messageType_) + " ]";
}

}
}
