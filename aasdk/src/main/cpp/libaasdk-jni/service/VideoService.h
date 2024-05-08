#pragma once

#include <memory>
#include "channel/av/VideoServiceChannel.hpp"
#include "channel/av/IVideoServiceChannelEventHandler.hpp"
#include "projection/IVideoOutput.h"
#include "IService.hpp"

namespace service
{

class VideoService: public aasdk::channel::av::IVideoServiceChannelEventHandler, public IService
{
public:
    typedef VideoService* Pointer;

    VideoService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IVideoOutput::Pointer videoOutput);
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
    void sendVideoFocusIndication();
    void sendVideoFocusReleaseIndication();

    boost::asio::io_service::strand strand_;
    aasdk::channel::av::VideoServiceChannel::Pointer channel_;
    projection::IVideoOutput::Pointer videoOutput_;
    int32_t session_;
};

}