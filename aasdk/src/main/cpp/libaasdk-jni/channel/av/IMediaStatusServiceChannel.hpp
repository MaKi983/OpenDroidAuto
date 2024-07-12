#pragma once

#include <memory>
#include <proto/ChannelOpenResponseMessage.pb.h>
#include <messenger/ChannelId.hpp>
#include <channel/Promise.hpp>
#include <channel/av/IMediaStatusServiceChannelEventHandler.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class IMediaStatusServiceChannel
{
public:
    typedef std::shared_ptr<IMediaStatusServiceChannel> Pointer;

    IMediaStatusServiceChannel() = default;
    virtual ~IMediaStatusServiceChannel() = default;

    virtual void receive(IMediaStatusServiceChannelEventHandler::Pointer eventHandler) = 0;
    virtual void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) = 0;
    virtual messenger::ChannelId getId() const = 0;
};

}
}
}
