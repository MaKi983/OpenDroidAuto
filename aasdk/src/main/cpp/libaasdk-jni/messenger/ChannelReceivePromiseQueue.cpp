#include <messenger/ChannelReceivePromiseQueue.hpp>

namespace aasdk
{
namespace messenger
{

void ChannelReceivePromiseQueue::push(ChannelId channelId, ReceivePromise::Pointer promise)
{
    queue_[channelId].push(std::move(promise));
}

ReceivePromise::Pointer ChannelReceivePromiseQueue::pop(ChannelId channelId)
{
    auto& channelQueue = queue_.at(channelId);
    auto promise = std::move(channelQueue.front());
    channelQueue.pop();

    if(channelQueue.empty())
    {
        queue_.erase(channelId);
    }

    return promise;
}

bool ChannelReceivePromiseQueue::isPending(ChannelId channelId) const
{
    int count = queue_.count(channelId);
    if (count > 0) {
        auto item = queue_.at(channelId);
        return !item.empty();
    }
    return false;
}

size_t ChannelReceivePromiseQueue::size() const
{
    return queue_.size();
}

bool ChannelReceivePromiseQueue::empty() const
{
    return this->size() == 0;
}

void ChannelReceivePromiseQueue::clear()
{
    queue_.clear();
}

ReceivePromise::Pointer ChannelReceivePromiseQueue::pop()
{
    return this->pop(queue_.begin()->first);
}

}
}
