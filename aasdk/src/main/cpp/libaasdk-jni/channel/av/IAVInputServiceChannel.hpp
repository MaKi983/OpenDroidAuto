#pragma once

#include <memory>
#include <proto/AVChannelSetupResponseMessage.pb.h>
#include <proto/AVInputOpenResponseMessage.pb.h>
#include <proto/ChannelOpenResponseMessage.pb.h>
#include <messenger/ChannelId.hpp>
#include <messenger/Timestamp.hpp>
#include <channel/Promise.hpp>
#include <channel/av/IAVInputServiceChannelEventHandler.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class IAVInputServiceChannel
{
public:
    typedef std::shared_ptr<IAVInputServiceChannel> Pointer;

    IAVInputServiceChannel() = default;
    virtual ~IAVInputServiceChannel() = default;

    virtual void receive(IAVInputServiceChannelEventHandler::Pointer eventHandler) = 0;
    virtual void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) = 0;
    virtual void sendAVChannelSetupResponse(const proto::messages::AVChannelSetupResponse& response, SendPromise::Pointer promise) = 0;
    virtual void sendAVMediaWithTimestampIndication(messenger::Timestamp::ValueType, const common::Data& data, SendPromise::Pointer promise) = 0;
    virtual void sendAVInputOpenResponse(const proto::messages::AVInputOpenResponse& response, SendPromise::Pointer promise) = 0;
    virtual messenger::ChannelId getId() const = 0;
};

}
}
}
