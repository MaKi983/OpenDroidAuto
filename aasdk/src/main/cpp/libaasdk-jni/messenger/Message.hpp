#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include <google/protobuf/message.h>
#include <common/Data.hpp>
#include <messenger/ChannelId.hpp>
#include <messenger/EncryptionType.hpp>
#include <messenger/MessageType.hpp>
#include <messenger/MessageId.hpp>

namespace aasdk
{
namespace messenger
{

class Message: boost::noncopyable
{
public:
    typedef std::shared_ptr<Message> Pointer;

    Message(ChannelId channelId, EncryptionType encryptionType, MessageType type);
    Message(Message&& other);
    ~Message();
    Message& operator=(Message&& other);

    ChannelId getChannelId() const;
    EncryptionType getEncryptionType() const;
    MessageType getType() const;

    common::Data& getPayload();
    const common::Data& getPayload() const;
    void insertPayload(const common::Data& payload);
    void insertPayload(const google::protobuf::Message& message);
    void insertPayload(const common::DataConstBuffer& buffer);
    void insertPayload(common::DataBuffer& buffer);

    std::string toString();
private:
    ChannelId channelId_;
    EncryptionType encryptionType_;
    MessageType type_;
    common::Data payload_;
};

}
}
