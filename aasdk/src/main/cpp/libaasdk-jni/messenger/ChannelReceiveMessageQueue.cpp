#include <messenger/ChannelReceiveMessageQueue.hpp>

namespace aasdk
{
namespace messenger
{

void ChannelReceiveMessageQueue::push(Message::Pointer message)
{
    const auto channelId = message->getChannelId();

    if(queue_.count(channelId) == 0)
    {
        queue_.emplace(std::make_pair(channelId, MessageQueue()));
    }

    auto& channelQueue = queue_.at(channelId);
    channelQueue.emplace(std::move(message));
}

Message::Pointer ChannelReceiveMessageQueue::pop(ChannelId channelId)
{
    auto& channelQueue = queue_.at(channelId);
    auto message(std::move(channelQueue.front()));
    channelQueue.pop();

    if(channelQueue.empty())
    {
        queue_.erase(channelId);
    }

    return message;
}

bool ChannelReceiveMessageQueue::empty(ChannelId channelId) const
{
    return queue_.count(channelId) == 0;
}

void ChannelReceiveMessageQueue::clear()
{
    queue_.clear();
}

}
}
