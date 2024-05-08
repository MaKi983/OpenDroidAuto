#pragma once

#include <unordered_map>
#include <queue>
#include <messenger/Message.hpp>

namespace aasdk
{
namespace messenger
{

class ChannelReceiveMessageQueue
{
public:
    void push(Message::Pointer message);
    Message::Pointer pop(ChannelId channelId);
    bool empty(ChannelId channelId) const;
    void clear();

private:
    typedef std::queue<Message::Pointer> MessageQueue;
    std::unordered_map<ChannelId, MessageQueue> queue_;
};

}
}
