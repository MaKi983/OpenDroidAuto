#include <Log.h>
#include "VideoService.h"
#include "service/VideoService.h"
#include <inttypes.h>

namespace service
{

VideoService::VideoService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IVideoOutput::Pointer videoOutput)
        : strand_(ioService)
        , channel_(new aasdk::channel::av::VideoServiceChannel(strand_, std::move(messenger)))
        , videoOutput_(std::move(videoOutput))
        , session_(-1)
{

}

VideoService::~VideoService(){
    delete channel_;
}

void VideoService::start()
{
    strand_.dispatch([this]() {
        if(Log::isInfo()) Log_i("start.");
        channel_->receive(this);
    });
}

void VideoService::stop()
{
    if(Log::isInfo()) Log_i("Stop");
    videoOutput_->stop();
}

void VideoService::onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onChannelOpenRequest: %s", request.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("open request, priority %d", request.priority());
    const aasdk::proto::enums::Status::Enum status = videoOutput_->open() ? aasdk::proto::enums::Status::OK : aasdk::proto::enums::Status::FAIL;
    if(Log::isDebug()) Log_d("open status: %s", aasdk::proto::enums::Status::Enum_Name(status).c_str());

    aasdk::proto::messages::ChannelOpenResponse response;
    response.set_status(status);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&VideoService::onChannelError, this, std::placeholders::_1));
    channel_->sendChannelOpenResponse(response, std::move(promise));

    channel_->receive(this);
}

void VideoService::onAVChannelSetupRequest(const aasdk::proto::messages::AVChannelSetupRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onAVChannelSetupRequest: %s", request.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("Setup request, config index: %d", request.config_index());
    const aasdk::proto::enums::AVChannelSetupStatus::Enum status = videoOutput_->init() ? aasdk::proto::enums::AVChannelSetupStatus::OK : aasdk::proto::enums::AVChannelSetupStatus::FAIL;
    if(Log::isDebug()) Log_d("Setup status: %d", status);

    aasdk::proto::messages::AVChannelSetupResponse response;
    response.set_media_status(status);
    response.set_max_unacked(1);
    response.add_configs(0);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then(std::bind(&VideoService::sendVideoFocusIndication, this),
                  std::bind(&VideoService::onChannelError, this, std::placeholders::_1));
    channel_->sendAVChannelSetupResponse(response, std::move(promise));
    channel_->receive(this);
}

void VideoService::onAVChannelStartIndication(const aasdk::proto::messages::AVChannelStartIndication& indication)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onAVChannelStartIndication: %s", indication.Utf8DebugString().c_str());
    if(Log::isDebug()) Log_d("start indication, session: %d", indication.session());

    session_ = indication.session();

    channel_->receive(this);
}

void VideoService::onAVChannelStopIndication(const aasdk::proto::messages::AVChannelStopIndication& indication)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onAVChannelStopIndication: %s", indication.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("stop indication");

    channel_->receive(this);
}

void VideoService::onAVMediaWithTimestampIndication(aasdk::messenger::Timestamp::ValueType timestamp, const aasdk::common::DataConstBuffer& buffer)
{
    if (Log::isVerbose()) Log_v("t= %" PRIu64 " buffer= %hhu", timestamp, buffer.size);
    videoOutput_->write(timestamp, buffer);
    if (Log::isVerbose()) Log_v("wrote ok");
    aasdk::proto::messages::AVMediaAckIndication indication;
    indication.set_session(session_);
    indication.set_value(1);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&VideoService::onChannelError, this, std::placeholders::_1));
    channel_->sendAVMediaAckIndication(indication, std::move(promise));

    channel_->receive(this);
}

void VideoService::onAVMediaIndication(const aasdk::common::DataConstBuffer& buffer)
{
    videoOutput_->write(0, buffer);

    aasdk::proto::messages::AVMediaAckIndication indication;
    indication.set_session(session_);
    indication.set_value(1);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&VideoService::onChannelError, this, std::placeholders::_1));
    channel_->sendAVMediaAckIndication(indication, std::move(promise));

    channel_->receive(this);
}

void VideoService::onChannelError(const aasdk::error::Error& e)
{
    Log_e("Channel error: %s", e.what());
}

void VideoService::fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response)
{
    if(Log::isInfo()) Log_i("fill features");

    auto* channelDescriptor = response.add_channels();
    channelDescriptor->set_channel_id(static_cast<uint32_t>(channel_->getId()));

    auto* videoChannel = channelDescriptor->mutable_av_channel();
    videoChannel->set_stream_type(aasdk::proto::enums::AVStreamType::VIDEO);
    videoChannel->set_available_while_in_call(true);

    auto* videoConfig1 = videoChannel->add_video_configs();
    videoConfig1->set_video_resolution(videoOutput_->getVideoResolution());
    videoConfig1->set_video_fps(videoOutput_->getVideoFPS());

    const auto& videoMargins = videoOutput_->getVideoMargins();
    videoConfig1->set_margin_height(videoMargins.height);
    videoConfig1->set_margin_width(videoMargins.width);
    videoConfig1->set_dpi(videoOutput_->getScreenDPI());

    if(Log::isVerbose() && Log::logProtocol()) Log_v("%s", channelDescriptor->Utf8DebugString().c_str());
}

void VideoService::onVideoFocusRequest(const aasdk::proto::messages::VideoFocusRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onVideoFocusRequest: %s", request.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("Video focus request, display index: %d, focus mode: %s, focus reason: %s", request.disp_index(), aasdk::proto::enums::VideoFocusMode_Enum_Name(request.focus_mode()).c_str(), aasdk::proto::enums::VideoFocusReason_Enum_Name(request.focus_reason()).c_str());

    if (request.focus_mode() == aasdk::proto::enums::VideoFocusMode::FOCUSED) {
        this->sendVideoFocusIndication();
    } else if (request.focus_mode() == aasdk::proto::enums::VideoFocusMode::UNFOCUSED){
        this->sendVideoFocusReleaseIndication();
    }
    channel_->receive(this);
}

void VideoService::sendVideoFocusIndication()
{
    if(Log::isDebug()) Log_d("Video focus indication.");

    aasdk::proto::messages::VideoFocusIndication videoFocusIndication;
    videoFocusIndication.set_focus_mode(aasdk::proto::enums::VideoFocusMode::FOCUSED);
    videoFocusIndication.set_unrequested(false);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&VideoService::onChannelError, this, std::placeholders::_1));
    channel_->sendVideoFocusIndication(videoFocusIndication, std::move(promise));
}

void VideoService::sendVideoFocusReleaseIndication()
{
    if(Log::isDebug()) Log_d("Video focus release indication.");

    aasdk::proto::messages::VideoFocusIndication videoFocusIndication;
    videoFocusIndication.set_focus_mode(aasdk::proto::enums::VideoFocusMode::UNFOCUSED);
    videoFocusIndication.set_unrequested(false);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&VideoService::onChannelError, this, std::placeholders::_1));
    channel_->sendVideoFocusIndication(videoFocusIndication, std::move(promise));
}

}