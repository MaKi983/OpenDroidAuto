#include <messenger/Message.hpp>
#include <Log.h>

namespace aasdk
{
namespace messenger
{

Message::Message(ChannelId channelId, EncryptionType encryptionType, MessageType type)
    : channelId_(channelId)
    , encryptionType_(encryptionType)
    , type_(type)
{
}

Message::Message(Message&& other)
    : channelId_(other.channelId_)
    , encryptionType_(other.encryptionType_)
    , type_(other.type_)
    , payload_(std::move(other.payload_))
{
}

Message::~Message() {
}

Message& Message::operator=(Message&& other)
{
    channelId_ = std::move(other.channelId_);
    encryptionType_ = std::move(other.encryptionType_);
    type_ = std::move(other.type_);
    payload_ = std::move(other.payload_);

    return *this;
}

ChannelId Message::getChannelId() const
{
    return channelId_;
}

EncryptionType Message::getEncryptionType() const
{
    return encryptionType_;
}

MessageType Message::getType() const
{
    return type_;
}

common::Data& Message::getPayload()
{
    return payload_;
}

const common::Data& Message::getPayload() const
{
    return payload_;
}

void Message::insertPayload(const common::Data& payload)
{
    payload_.insert(payload_.end(), payload.begin(), payload.end());
}

void Message::insertPayload(const google::protobuf::Message& message)
{
    auto offset = payload_.size();
    payload_.resize(payload_.size() + message.ByteSize());

    common::DataBuffer buffer(payload_, offset);
    message.SerializeToArray(buffer.data, buffer.size);
}

void Message::insertPayload(const common::DataConstBuffer& buffer)
{
    common::copy(payload_, buffer);
}

void Message::insertPayload(common::DataBuffer& buffer)
{
    common::copy(payload_, buffer);
}

std::string Message::toString() {
    return "Message[ channelId: " + channelIdToString(channelId_)
           + ", encryptionType: " + encryptionTypeToString(encryptionType_)
           + ", type: " + messageTypeToString(type_)
           + ", payload: " + std::to_string(payload_.size()) + " bytes ]";
}
}
}
