#pragma once

#include <stdint.h>
#include <proto/ChannelOpenRequestMessage.pb.h>
#include <proto/BindingRequestMessage.pb.h>


namespace aasdk
{
namespace channel
{
namespace input
{

class IInputServiceChannelEventHandler
{
public:
    typedef IInputServiceChannelEventHandler* Pointer;

    IInputServiceChannelEventHandler() = default;
    virtual ~IInputServiceChannelEventHandler() = default;

    virtual void onChannelOpenRequest(const proto::messages::ChannelOpenRequest& request) = 0;
    virtual void onBindingRequest(const proto::messages::BindingRequest& request) = 0;
    virtual void onChannelError(const error::Error& e) = 0;
};

}
}
}
