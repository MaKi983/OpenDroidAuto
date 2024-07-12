#pragma once

#include <memory>
#include <proto/AVChannelSetupResponseMessage.pb.h>
#include <proto/AVMediaAckIndicationMessage.pb.h>
#include <proto/ChannelOpenResponseMessage.pb.h>
#include <messenger/ChannelId.hpp>
#include <channel/Promise.hpp>
#include <channel/av/IAudioServiceChannelEventHandler.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class IAudioServiceChannel
{
public:
    typedef std::shared_ptr<IAudioServiceChannel> Pointer;

    IAudioServiceChannel() = default;
    virtual ~IAudioServiceChannel() = default;

    virtual void receive(IAudioServiceChannelEventHandler::Pointer eventHandler) = 0;
    virtual void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) = 0;
    virtual void sendAVChannelSetupResponse(const proto::messages::AVChannelSetupResponse& response, SendPromise::Pointer promise) = 0;
    virtual void sendAVMediaAckIndication(const proto::messages::AVMediaAckIndication& indication, SendPromise::Pointer promise) = 0;
    virtual messenger::ChannelId getId() const = 0;
};

}
}
}
