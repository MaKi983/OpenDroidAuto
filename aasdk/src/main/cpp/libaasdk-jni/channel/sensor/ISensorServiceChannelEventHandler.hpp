#pragma once

#include <proto/SensorStartRequestMessage.pb.h>
#include <proto/ChannelOpenRequestMessage.pb.h>
#include <error/Error.hpp>


namespace aasdk
{
namespace channel
{
namespace sensor
{

class ISensorServiceChannelEventHandler
{
public:
    typedef std::shared_ptr<ISensorServiceChannelEventHandler> Pointer;

    ISensorServiceChannelEventHandler() = default;
    virtual ~ISensorServiceChannelEventHandler() = default;

    virtual void onChannelOpenRequest(const proto::messages::ChannelOpenRequest& request) = 0;
    virtual void onSensorStartRequest(const proto::messages::SensorStartRequestMessage& request) = 0;
    virtual void onChannelError(const error::Error& e) = 0;
};

}
}
}
