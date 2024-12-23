#pragma once

#include <memory>
#include "channel/av/VideoServiceChannel.hpp"
#include "channel/av/IVideoServiceChannelEventHandler.hpp"
#include "projection/IVideoOutput.h"
#include "IService.hpp"
#include "IVideoEventHandler.h"
#include "IServiceEventHandler.h"

namespace service
{

class VideoService: public aasdk::channel::av::IVideoServiceChannelEventHandler, public IService, public std::enable_shared_from_this<VideoService>
{
public:
    typedef std::shared_ptr<VideoService> Pointer;

    VideoService(aasdk::io::ioService& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IVideoOutput::Pointer videoOutput, IVideoEventHandler::Pointer eventHandler, IServiceEventHandler::Pointer serviceEventHandler);
    ~VideoService();

    void start() override;
    void stop() override;
    void fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response) override;
    void onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request) override;
    void onAVChannelSetupRequest(const aasdk::proto::messages::AVChannelSetupRequest& request) override;
    void onAVChannelStartIndication(const aasdk::proto::messages::AVChannelStartIndication& indication) override;
    void onAVChannelStopIndication(const aasdk::proto::messages::AVChannelStopIndication& indication) override;
    void onAVMediaWithTimestampIndication(aasdk::messenger::Timestamp::ValueType timestamp, const aasdk::common::DataConstBuffer& buffer) override;
    void onAVMediaIndication(const aasdk::common::DataConstBuffer& buffer) override;
    void onVideoFocusRequest(const aasdk::proto::messages::VideoFocusRequest& request) override;
    void onChannelError(const aasdk::error::Error& e) override;

private:
    using std::enable_shared_from_this<VideoService>::shared_from_this;
    void sendVideoFocusIndication();
    void sendVideoFocusReleaseIndication();

    aasdk::io::strand strand_;
    aasdk::channel::av::VideoServiceChannel::Pointer channel_;
    projection::IVideoOutput::Pointer videoOutput_;
    int32_t session_;
    service::IVideoEventHandler::Pointer eventHandler_;
    IServiceEventHandler::Pointer serviceEventHandler_;
    bool isRunning_;
};

}