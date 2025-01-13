#include "AndroidAutoEntity.h"
#include <channel/control/ControlServiceChannel.hpp>
#include <Log.h>
#include <boost/stacktrace.hpp>

namespace service
{

AndroidAutoEntity::AndroidAutoEntity(aasdk::io::ioService& ioService,
                                     aasdk::messenger::ICryptor::Pointer cryptor,
                                     aasdk::messenger::IMessenger::Pointer messenger,
                                     configuration::IConfiguration::Pointer config,
                                     ServiceList serviceList,
                                     IPinger::Pointer pinger)
        : strand_(ioService)
        , cryptor_(std::move(cryptor))
        , messenger_(std::move(messenger))
        , controlServiceChannel_(std::make_shared<aasdk::channel::control::ControlServiceChannel>(strand_, messenger_))
        , config_(std::move(config))
        , serviceList_(std::move(serviceList))
        , pinger_(std::move(pinger))
        , eventHandler_(nullptr)
{
}

AndroidAutoEntity::~AndroidAutoEntity()
{
    if(Log::isDebug()) Log_d("destroy.");
    delete config_;
//    delete controlServiceChannel_;
}

void AndroidAutoEntity::start(IAndroidAutoEntityEventHandler::Pointer eventHandler)
{
    strand_->dispatch([this, self = this->shared_from_this(), eventHandler = eventHandler]() {
        if(Log::isInfo()) Log_i("start");
        eventHandler_ = eventHandler;

        controlServiceChannel_->receive(this->shared_from_this());

        this->schedulePing();

        auto versionRequestPromise = aasdk::channel::SendPromise::defer(strand_, "AndroidAutoEntity_versionRequest");
        versionRequestPromise->then([]() {}, std::bind(&AndroidAutoEntity::onChannelError, this->shared_from_this(), std::placeholders::_1));
        controlServiceChannel_->sendVersionRequest(std::move(versionRequestPromise));
    });
}

void AndroidAutoEntity::stop()
{
    if(Log::isInfo()) Log_i("stop");

    delete eventHandler_;
    eventHandler_ = nullptr;
}

void AndroidAutoEntity::sendShutdownRequest() {
    if(Log::isInfo()) Log_i("Shutdown request");

    aasdk::proto::messages::ShutdownRequest shutdownRequest;
    shutdownRequest.set_reason(aasdk::proto::enums::ShutdownReason::QUIT);

    auto sendShutdownRequestPromise = aasdk::channel::SendPromise::defer(strand_, "AndroidAutoEntity_shutdown");
    sendShutdownRequestPromise->then([]() {}, std::bind(&AndroidAutoEntity::onChannelError, this->shared_from_this(), std::placeholders::_1));
    controlServiceChannel_->sendShutdownRequest(shutdownRequest, std::move(sendShutdownRequestPromise));
//    controlServiceChannel_->receive(this->shared_from_this());
}

void AndroidAutoEntity::onVersionResponse(uint16_t majorCode, uint16_t minorCode, aasdk::proto::enums::VersionResponseStatus::Enum status)
{
    if(Log::isInfo()) Log_i("onVersionResponse: version: %d.%d, %s", majorCode, minorCode, aasdk::proto::enums::VersionResponseStatus::Enum_Name(status).c_str());

    if(status == aasdk::proto::enums::VersionResponseStatus::MISMATCH)
    {
        Log_e("version mismatch.");
        this->triggerQuit();
    }
    else
    {
        if(Log::isInfo()) Log_i("Begin handshake");

        try
        {
            cryptor_->doHandshake();

            auto handshakePromise = aasdk::channel::SendPromise::defer(strand_, "AndroidAutoEntity_sendHandshake");
            handshakePromise->then([]() {}, std::bind(&AndroidAutoEntity::onChannelError, this->shared_from_this(), std::placeholders::_1));
            controlServiceChannel_->sendHandshake(cryptor_->readHandshakeBuffer(), std::move(handshakePromise));
//            controlServiceChannel_->receive(this->shared_from_this());
        }
        catch(const aasdk::error::Error& e)
        {
            this->onChannelError(e);
        }
    }
}

void AndroidAutoEntity::onHandshake(const aasdk::common::DataConstBuffer& payload)
{
    try
    {
        cryptor_->writeHandshakeBuffer(payload);

        if(!cryptor_->doHandshake())
        {
            if(Log::isInfo()) Log_i("continue handshake");

            auto handshakePromise = aasdk::channel::SendPromise::defer(strand_, "AndroidAutoEntity_continueHandshake");
            handshakePromise->then([]() {}, std::bind(&AndroidAutoEntity::onChannelError, this->shared_from_this(), std::placeholders::_1));
            controlServiceChannel_->sendHandshake(cryptor_->readHandshakeBuffer(), std::move(handshakePromise));
        }
        else
        {
            if(Log::isInfo()) Log_i("Auth completed");

            aasdk::proto::messages::AuthCompleteIndication authCompleteIndication;
            authCompleteIndication.set_status(aasdk::proto::enums::Status::OK);

            auto authCompletePromise = aasdk::channel::SendPromise::defer(strand_, "AndroidAutoEntity_authComplete");
            authCompletePromise->then([]() {}, std::bind(&AndroidAutoEntity::onChannelError, this->shared_from_this(), std::placeholders::_1));
            controlServiceChannel_->sendAuthComplete(authCompleteIndication, std::move(authCompletePromise));
        }

//        controlServiceChannel_->receive(this->shared_from_this());
    }
    catch(const aasdk::error::Error& e)
    {
        this->onChannelError(e);
    }
}

void AndroidAutoEntity::onServiceDiscoveryRequest(const aasdk::proto::messages::ServiceDiscoveryRequest& request)
{
    if(Log::isInfo()) Log_i("Discovery request, device name: %s, brand: %s", request.device_name().c_str(), request.device_brand().c_str());

    aasdk::proto::messages::ServiceDiscoveryResponse serviceDiscoveryResponse;
    serviceDiscoveryResponse.mutable_channels()->Reserve(256);
    serviceDiscoveryResponse.set_head_unit_name(config_->huName());
    serviceDiscoveryResponse.set_car_model(config_->model());
    serviceDiscoveryResponse.set_car_year(config_->year());
    serviceDiscoveryResponse.set_car_serial("20180301");
    serviceDiscoveryResponse.set_left_hand_drive_vehicle(config_->leftHandDrive());
    serviceDiscoveryResponse.set_headunit_manufacturer(config_->huMake());
    serviceDiscoveryResponse.set_headunit_model(config_->huModel());
    serviceDiscoveryResponse.set_sw_build(config_->swVersion());
    serviceDiscoveryResponse.set_sw_version(config_->swVersion());
    serviceDiscoveryResponse.set_can_play_native_media_during_vr(config_->playMediaDuringVr());
    serviceDiscoveryResponse.set_hide_clock(config_->hideClock());

    std::for_each(serviceList_.begin(), serviceList_.end(), std::bind(&IService::fillFeatures, std::placeholders::_1, std::ref(serviceDiscoveryResponse)));

    if(Log::isInfo()) Log_i("Fill features done");

    auto promise = aasdk::channel::SendPromise::defer(strand_, "AndroidAutoEntity_serviceDiscoveryResponse");
    promise->then([]() {}, std::bind(&AndroidAutoEntity::onChannelError, this->shared_from_this(), std::placeholders::_1));
    controlServiceChannel_->sendServiceDiscoveryResponse(serviceDiscoveryResponse, std::move(promise));
//    controlServiceChannel_->receive(this->shared_from_this());
}

void AndroidAutoEntity::onAudioFocusRequest(const aasdk::proto::messages::AudioFocusRequest& request)
{
    aasdk::proto::enums::AudioFocusState::Enum audioFocusState =
            request.audio_focus_type() == aasdk::proto::enums::AudioFocusType::RELEASE ? aasdk::proto::enums::AudioFocusState::LOSS
                                                                                       : aasdk::proto::enums::AudioFocusState::GAIN;

    if(Log::isDebug()) Log_d("audio focus state: %s", aasdk::proto::enums::AudioFocusState::Enum_Name(audioFocusState).c_str());

    aasdk::proto::messages::AudioFocusResponse response;
    response.set_audio_focus_state(audioFocusState);

    auto promise = aasdk::channel::SendPromise::defer(strand_, "AndroidAutoEntity_audioFocus");
    promise->then([]() {}, std::bind(&AndroidAutoEntity::onChannelError, this->shared_from_this(), std::placeholders::_1));
    controlServiceChannel_->sendAudioFocusResponse(response, std::move(promise));
//    controlServiceChannel_->receive(this->shared_from_this());
}

void AndroidAutoEntity::onShutdownRequest(const aasdk::proto::messages::ShutdownRequest& request)
{
    if(Log::isInfo()) Log_i("Shutdown request, reason: %s", aasdk::proto::enums::ShutdownReason::Enum_Name(request.reason()).c_str());

    aasdk::proto::messages::ShutdownResponse response;
    auto promise = aasdk::channel::SendPromise::defer(strand_, "AndroidAutoEntity_triggerQuit");
    promise->then(std::bind(&AndroidAutoEntity::triggerQuit, this->shared_from_this()),
                  std::bind(&AndroidAutoEntity::onChannelError, this->shared_from_this(), std::placeholders::_1));

    controlServiceChannel_->sendShutdownResponse(response, std::move(promise));
}

void AndroidAutoEntity::onShutdownResponse(const aasdk::proto::messages::ShutdownResponse& response)
{
    this->triggerQuit();
}

void AndroidAutoEntity::onNavigationFocusRequest(const aasdk::proto::messages::NavigationFocusRequest& request)
{
    if(Log::isDebug()) Log_d("navigation focus request, type: %d", request.type());

    aasdk::proto::messages::NavigationFocusResponse response;
    response.set_type(2);

    auto promise = aasdk::channel::SendPromise::defer(strand_, "AndroidAutoEntity_navigationFocus");
    promise->then([]() {}, std::bind(&AndroidAutoEntity::onChannelError, this->shared_from_this(), std::placeholders::_1));
    controlServiceChannel_->sendNavigationFocusResponse(response, std::move(promise));
//    controlServiceChannel_->receive(this->shared_from_this());
}

void AndroidAutoEntity::onVoiceSessionRequest(const aasdk::proto::messages::VoiceSessionRequest& request)
{
    if(Log::isDebug()) Log_d("Voice session request, type: %s", ((request.type() == 1) ? "START" : ((request.type() == 2) ? "STOP" : "UNKNOWN")));

    auto promise = aasdk::channel::SendPromise::defer(strand_, "AndroidAutoEntity_voiceSession");
    promise->then([]() {}, std::bind(&AndroidAutoEntity::onChannelError, this->shared_from_this(), std::placeholders::_1));
//    controlServiceChannel_->receive(this->shared_from_this());
}

void AndroidAutoEntity::onPingRequest(const aasdk::proto::messages::PingRequest& request)
{
    if(Log::isVerbose()) Log_v("on ping request");
    aasdk::proto::messages::PingResponse response;
    response.set_timestamp(request.timestamp());
    auto promise = aasdk::channel::SendPromise::defer(strand_, "AndroidAutoEntity_pong");
    promise->then([]() {}, std::bind(&AndroidAutoEntity::onChannelError, this->shared_from_this(), std::placeholders::_1));
    controlServiceChannel_->sendPingResponse(response, std::move(promise));
//    controlServiceChannel_->receive(this->shared_from_this());
}

void AndroidAutoEntity::onPingResponse(const aasdk::proto::messages::PingResponse& response)
{
    pinger_->pong();
//    controlServiceChannel_->receive(this->shared_from_this());
}

void AndroidAutoEntity::onChannelError(const aasdk::error::Error& e)
{
    Log_e("onChannelError %s", e.what());
    this->triggerQuitOnError(e);
}

void AndroidAutoEntity::triggerQuitOnError(const aasdk::error::Error& e)
{
    if(eventHandler_ != nullptr)
    {
        eventHandler_->onAndroidAutoQuitOnError(e);
    }
}

void AndroidAutoEntity::triggerQuit()
{
    if(eventHandler_ != nullptr)
    {
        eventHandler_->onAndroidAutoQuit();
    }
}

void AndroidAutoEntity::schedulePing()
{
    auto promise = IPinger::Promise::defer(strand_, "AndroidAutoEntity_ping");
    promise->then([this, self = this->shared_from_this()]() {
                      this->sendPing();
                      this->schedulePing();
                  },
                  [this](auto error) {
                      if(error != aasdk::error::ErrorCode::OPERATION_ABORTED &&
                         error != aasdk::error::ErrorCode::OPERATION_IN_PROGRESS)
                      {
                          Log_e("ping timer exceeded");
                          this->triggerQuit();
                      }
                  });

    pinger_->ping(std::move(promise));
}

void AndroidAutoEntity::sendPing()
{
    if(Log::isVerbose()) Log_v("send ping");
    auto promise = aasdk::channel::SendPromise::defer(strand_, "AndroidAutoEntity_ping");
    promise->then([]() {}, std::bind(&AndroidAutoEntity::onChannelError, this->shared_from_this(), std::placeholders::_1));

    aasdk::proto::messages::PingRequest request;
    auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
    request.set_timestamp(timestamp.count());
    controlServiceChannel_->sendPingRequest(request, std::move(promise));
}

}
