#pragma once

#include <queue>
#include <unordered_map>
#include <messenger/IMessenger.hpp>

namespace aasdk
{
namespace messenger
{

class ChannelReceivePromiseQueue
{
public:
    void push(ChannelId channelId, ReceivePromise::Pointer promise);
    ReceivePromise::Pointer pop(ChannelId channelId);
    bool isPending(ChannelId channelId) const;
    size_t size() const;
    bool empty() const;
    void clear();
    ReceivePromise::Pointer pop();

private:
    std::unordered_map<ChannelId, std::queue<ReceivePromise::Pointer>> queue_;
};

}
}
