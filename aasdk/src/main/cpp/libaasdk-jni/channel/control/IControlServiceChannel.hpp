#pragma once

#include <proto/ShutdownRequestMessage.pb.h>
#include <proto/ShutdownResponseMessage.pb.h>
#include <proto/AuthCompleteIndicationMessage.pb.h>
#include <proto/ServiceDiscoveryResponseMessage.pb.h>
#include <proto/AudioFocusResponseMessage.pb.h>
#include <proto/NavigationFocusResponseMessage.pb.h>
#include <proto/StatusEnum.pb.h>
#include <proto/PingRequestMessage.pb.h>
#include <proto/VoiceSessionRequestMessage.pb.h>
#include <common/Data.hpp>
#include <channel/Promise.hpp>
#include <channel/control/IControlServiceChannelEventHandler.hpp>


namespace aasdk
{
namespace channel
{
namespace control
{

class IControlServiceChannel
{
public:
    typedef IControlServiceChannel* Pointer;

    IControlServiceChannel() = default;
    virtual ~IControlServiceChannel() = default;

    virtual void receive(IControlServiceChannelEventHandler::Pointer eventHandler) = 0;

    virtual void sendVersionRequest(SendPromise::Pointer promise) = 0;
    virtual void sendHandshake(common::Data handshakeBuffer, SendPromise::Pointer promise) = 0;
    virtual void sendAuthComplete(const proto::messages::AuthCompleteIndication& response, SendPromise::Pointer promise) = 0;
    virtual void sendServiceDiscoveryResponse(const proto::messages::ServiceDiscoveryResponse& response, SendPromise::Pointer promise) = 0;
    virtual void sendAudioFocusResponse(const proto::messages::AudioFocusResponse& response, SendPromise::Pointer promise) = 0;
    virtual void sendShutdownRequest(const proto::messages::ShutdownRequest& request, SendPromise::Pointer promise) = 0;
    virtual void sendShutdownResponse(const proto::messages::ShutdownResponse& response, SendPromise::Pointer promise) = 0;
    virtual void sendNavigationFocusResponse(const proto::messages::NavigationFocusResponse& response, SendPromise::Pointer promise) = 0;
    virtual void sendPingRequest(const proto::messages::PingRequest& request, SendPromise::Pointer promise) = 0;
    virtual void sendPingResponse(const proto::messages::PingResponse& response, SendPromise::Pointer promise) = 0;
};

}
}
}
