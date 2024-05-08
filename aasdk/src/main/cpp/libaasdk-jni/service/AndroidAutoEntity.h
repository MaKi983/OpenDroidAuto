#pragma once

#include <boost/asio.hpp>
#include <configuration/IConfiguration.h>
#include "transport/ITransport.hpp"
#include "channel/control/IControlServiceChannel.hpp"
#include "channel/control/IControlServiceChannelEventHandler.hpp"
#include "channel/av/VideoServiceChannel.hpp"
#include "IAndroidAutoEntity.h"
#include "IService.hpp"
#include "IPinger.h"

namespace service {

class AndroidAutoEntity
        : public IAndroidAutoEntity,
          public aasdk::channel::control::IControlServiceChannelEventHandler {
public:
    typedef AndroidAutoEntity *Pointer;

    AndroidAutoEntity(boost::asio::io_service &ioService,
                      aasdk::messenger::ICryptor::Pointer cryptor,
                      aasdk::messenger::IMessenger::Pointer messenger,
                      configuration::IConfiguration::Pointer configuration,
                      ServiceList serviceList,
                      IPinger::Pointer pinger);

    ~AndroidAutoEntity() override;

    void start(IAndroidAutoEntityEventHandler &eventHandler) override;

    void stop() override;

    void sendShutdownRequest() override;

    void onVersionResponse(uint16_t majorCode, uint16_t minorCode,
                           aasdk::proto::enums::VersionResponseStatus::Enum status) override;

    void onHandshake(const aasdk::common::DataConstBuffer &payload) override;

    void onServiceDiscoveryRequest(
            const aasdk::proto::messages::ServiceDiscoveryRequest &request) override;

    void onAudioFocusRequest(const aasdk::proto::messages::AudioFocusRequest &request) override;

    void onShutdownRequest(const aasdk::proto::messages::ShutdownRequest &request) override;

    void onShutdownResponse(const aasdk::proto::messages::ShutdownResponse &response) override;

    void onNavigationFocusRequest(
            const aasdk::proto::messages::NavigationFocusRequest &request) override;

    void onPingRequest(const aasdk::proto::messages::PingRequest &request) override;

    void onPingResponse(const aasdk::proto::messages::PingResponse &response) override;

    void onChannelError(const aasdk::error::Error &e) override;

    void
    onVoiceSessionRequest(const aasdk::proto::messages::VoiceSessionRequest &request) override;

private:
    void triggerQuit();
    void triggerQuitOnError(const aasdk::error::Error& e);

    void schedulePing();

    void sendPing();

    boost::asio::io_service::strand strand_;
    aasdk::messenger::ICryptor::Pointer cryptor_;
    aasdk::transport::ITransport::Pointer transport_;
    aasdk::messenger::IMessenger::Pointer messenger_;
    aasdk::channel::control::IControlServiceChannel::Pointer controlServiceChannel_;
    configuration::IConfiguration::Pointer config_;
    ServiceList serviceList_;
    IPinger::Pointer pinger_;
    IAndroidAutoEntityEventHandler::Pointer eventHandler_;
};

}