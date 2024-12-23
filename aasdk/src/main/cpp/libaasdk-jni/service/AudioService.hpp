#pragma once

#include "channel/av/IAudioServiceChannel.hpp"
#include "channel/av/IAudioServiceChannelEventHandler.hpp"
#include "projection/IAudioOutput.h"
#include "IService.hpp"
#include "IServiceEventHandler.h"

namespace service
{

class AudioService: public aasdk::channel::av::IAudioServiceChannelEventHandler, public IService, public std::enable_shared_from_this<AudioService>
{
public:
    typedef std::shared_ptr<AudioService> Pointer;

    AudioService(aasdk::io::ioService& ioService, aasdk::channel::av::IAudioServiceChannel::Pointer channel, projection::IAudioOutput::Pointer audioOutput, IServiceEventHandler::Pointer serviceEventHandler);
    ~AudioService();
    void start() override;
    void stop() override;
    void fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response) override;
    void onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request) override;
    void onAVChannelSetupRequest(const aasdk::proto::messages::AVChannelSetupRequest& request) override;
    void onAVChannelStartIndication(const aasdk::proto::messages::AVChannelStartIndication& indication) override;
    void onAVChannelStopIndication(const aasdk::proto::messages::AVChannelStopIndication& indication) override;
    void onAVMediaWithTimestampIndication(aasdk::messenger::Timestamp::ValueType timestamp, const aasdk::common::DataConstBuffer& buffer) override;
    void onAVMediaIndication(const aasdk::common::DataConstBuffer& buffer) override;
    void onChannelError(const aasdk::error::Error& e) override;

protected:
    using std::enable_shared_from_this<AudioService>::shared_from_this;

    aasdk::io::strand strand_;
    aasdk::channel::av::IAudioServiceChannel::Pointer channel_;
    projection::IAudioOutput::Pointer audioOutput_;
    int32_t session_;
    IServiceEventHandler::Pointer serviceEventHandler_;
    bool isRunning_;
};

}
