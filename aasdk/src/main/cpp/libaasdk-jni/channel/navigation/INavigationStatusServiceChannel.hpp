#pragma once

#include <memory>
#include <proto/ChannelOpenResponseMessage.pb.h>
#include <messenger/ChannelId.hpp>
#include <channel/Promise.hpp>
#include <channel/navigation/INavigationStatusServiceChannelEventHandler.hpp>


namespace aasdk
{
namespace channel
{
namespace navigation
{

class INavigationStatusServiceChannel
{
public:
    typedef std::shared_ptr<INavigationStatusServiceChannel> Pointer;

    INavigationStatusServiceChannel() = default;
    virtual ~INavigationStatusServiceChannel() = default;

    virtual void receive(INavigationStatusServiceChannelEventHandler::Pointer eventHandler) = 0;
    virtual void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) = 0;
    virtual messenger::ChannelId getId() const = 0;
};

}
}
}
