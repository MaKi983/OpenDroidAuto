#pragma once

#include <error/Error.hpp>
#include <common/Data.hpp>
#include <proto/ShutdownRequestMessage.pb.h>
#include <proto/ShutdownResponseMessage.pb.h>
#include <proto/VersionResponseStatusEnum.pb.h>
#include <proto/ServiceDiscoveryRequestMessage.pb.h>
#include <proto/AudioFocusRequestMessage.pb.h>
#include <proto/NavigationFocusRequestMessage.pb.h>
#include <proto/PingRequestMessage.pb.h>
#include <proto/PingResponseMessage.pb.h>
#include <proto/VoiceSessionRequestMessage.pb.h>



namespace aasdk
{
namespace channel
{
namespace control
{

class IControlServiceChannelEventHandler
{
public:
    typedef std::shared_ptr<IControlServiceChannelEventHandler> Pointer;

    IControlServiceChannelEventHandler() = default;
    virtual ~IControlServiceChannelEventHandler() = default;

    virtual void onVersionResponse(uint16_t majorCode, uint16_t minorCode, proto::enums::VersionResponseStatus::Enum status) = 0;
    virtual void onHandshake(const common::DataConstBuffer& payload) = 0;
    virtual void onServiceDiscoveryRequest(const proto::messages::ServiceDiscoveryRequest& request) = 0;
    virtual void onAudioFocusRequest(const proto::messages::AudioFocusRequest& request) = 0;
    virtual void onShutdownRequest(const proto::messages::ShutdownRequest& request) = 0;
    virtual void onShutdownResponse(const proto::messages::ShutdownResponse& response) = 0;
    virtual void onNavigationFocusRequest(const proto::messages::NavigationFocusRequest& request) = 0;
    virtual void onPingRequest(const proto::messages::PingRequest& request) = 0;
    virtual void onPingResponse(const proto::messages::PingResponse& response) = 0;
    virtual void onChannelError(const error::Error& e) = 0;
    virtual void onVoiceSessionRequest(const proto::messages::VoiceSessionRequest& request) = 0;
};

}
}
}
