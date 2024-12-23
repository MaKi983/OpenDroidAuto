#pragma once

#include "channel/av/AVInputServiceChannel.hpp"
#include "IService.hpp"
#include "projection/IAudioInput.h"
#include "IServiceEventHandler.h"

namespace service
{

class AudioInputService: public aasdk::channel::av::IAVInputServiceChannelEventHandler, public IService, public std::enable_shared_from_this<AudioInputService>
{
public:
    typedef std::shared_ptr<AudioInputService> Pointer;

    AudioInputService(aasdk::io::ioService& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IAudioInput::Pointer audioInput, IServiceEventHandler::Pointer serviceEventHandler);
    ~AudioInputService();

    void start() override;
    void stop() override;
    void fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response) override;
    void onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request) override;
    void onAVChannelSetupRequest(const aasdk::proto::messages::AVChannelSetupRequest& request) override;
    void onAVInputOpenRequest(const aasdk::proto::messages::AVInputOpenRequest& request) override;
    void onAVMediaAckIndication(const aasdk::proto::messages::AVMediaAckIndication& indication) override;
    void onChannelError(const aasdk::error::Error& e) override;

private:
    using std::enable_shared_from_this<AudioInputService>::shared_from_this;
    void onAudioInputOpenSucceed();
    void onAudioInputDataReady(aasdk::common::Data data);
    void readAudioInput();

    aasdk::io::strand strand_;
    aasdk::channel::av::AVInputServiceChannel::Pointer channel_;
    projection::IAudioInput::Pointer audioInput_;
    int32_t session_;
    IServiceEventHandler::Pointer serviceEventHandler_;
    bool isRunning_;
};

}
