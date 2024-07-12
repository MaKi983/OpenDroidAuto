#pragma once

#include <memory>
#include <proto/StatusEnum.pb.h>
#include <proto/BindingResponseMessage.pb.h>
#include <proto/InputEventIndicationMessage.pb.h>
#include <proto/ChannelOpenResponseMessage.pb.h>
#include <messenger/ChannelId.hpp>
#include <channel/Promise.hpp>
#include <channel/input/IInputServiceChannelEventHandler.hpp>


namespace aasdk
{
namespace channel
{
namespace input
{

class IInputServiceChannel
{
public:
    typedef std::shared_ptr<IInputServiceChannel> Pointer;

    IInputServiceChannel() = default;
    virtual ~IInputServiceChannel() = default;

    virtual void receive(IInputServiceChannelEventHandler::Pointer eventHandler) = 0;
    virtual void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) = 0;
    virtual void sendInputEventIndication(const proto::messages::InputEventIndication& indication, SendPromise::Pointer promise) = 0;
    virtual void sendBindingResponse(const proto::messages::BindingResponse& response, SendPromise::Pointer promise) = 0;
    virtual messenger::ChannelId getId() const = 0;
};

}
}
}
