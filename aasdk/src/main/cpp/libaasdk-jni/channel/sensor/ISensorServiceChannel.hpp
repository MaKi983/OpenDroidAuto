#pragma once

#include <memory>
#include <messenger/ChannelId.hpp>
#include <channel/Promise.hpp>
#include <proto/SensorEventIndicationMessage.pb.h>
#include <proto/ChannelOpenResponseMessage.pb.h>
#include <proto/SensorStartResponseMessage.pb.h>
#include <channel/sensor/ISensorServiceChannelEventHandler.hpp>


namespace aasdk
{
namespace channel
{
namespace sensor
{

class ISensorServiceChannel
{
public:
    typedef ISensorServiceChannel* Pointer;

    ISensorServiceChannel() = default;
    virtual ~ISensorServiceChannel() = default;

    virtual void receive(ISensorServiceChannelEventHandler::Pointer eventHandler) = 0;
    virtual messenger::ChannelId getId() const = 0;
    virtual void sendChannelOpenResponse(const proto::messages::ChannelOpenResponse& response, SendPromise::Pointer promise) = 0;
    virtual void sendSensorEventIndication(const proto::messages::SensorEventIndication& indication, SendPromise::Pointer promise) = 0;
    virtual void sendSensorStartResponse(const proto::messages::SensorStartResponseMessage& response, SendPromise::Pointer promise) = 0;
};

}
}
}
