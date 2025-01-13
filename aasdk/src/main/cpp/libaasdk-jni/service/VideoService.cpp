#include <Log.h>
#include "VideoService.h"
#include "service/VideoService.h"
#include <inttypes.h>

namespace service
{

VideoService::VideoService(aasdk::io::ioService& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IVideoOutput::Pointer videoOutput, IVideoEventHandler::Pointer eventHandler, IServiceEventHandler::Pointer serviceEventHandler)
        : strand_(ioService)
        , channel_(std::make_shared<aasdk::channel::av::VideoServiceChannel>(strand_, std::move(messenger)))
        , videoOutput_(videoOutput)
        , eventHandler_(eventHandler)
        , serviceEventHandler_(serviceEventHandler)
        , session_(-1)
        , isRunning_(false)
{

}

VideoService::~VideoService(){
    if (Log::isVerbose()) Log_v("destructor");
    channel_.reset();
    delete eventHandler_;
    serviceEventHandler_ = nullptr;
    videoOutput_ = nullptr;
}

void VideoService::start()
{
    isRunning_ = true;
//    strand_.dispatch([this, self = this->shared_from_this()]() {
//    strand_.post([this, self = this->shared_from_this()]() {
        if(Log::isInfo()) Log_i("start.");
        channel_->receive(this->shared_from_this());
//    });
}

void VideoService::stop()
{
    if(Log::isInfo()) Log_i("Stop");
    isRunning_ = false;
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

    auto promise = aasdk::channel::SendPromise::defer(strand_, "VideoService_channelOpen");
    promise->then([]() {}, std::bind(&VideoService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendChannelOpenResponse(response, std::move(promise));

//    channel_->receive(this->shared_from_this());
}

void VideoService::onAVChannelSetupRequest(const aasdk::proto::messages::AVChannelSetupRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onAVChannelSetupRequest: %s", request.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("Setup request, config index: %d", request.config_index());
    const aasdk::proto::enums::AVChannelSetupStatus::Enum status = videoOutput_->init() ? aasdk::proto::enums::AVChannelSetupStatus::OK : aasdk::proto::enums::AVChannelSetupStatus::FAIL;
    if(Log::isDebug()) Log_d("Setup status: %s", aasdk::proto::enums::AVChannelSetupStatus::Enum_Name(status).c_str());

    aasdk::proto::messages::AVChannelSetupResponse response;
    response.set_media_status(status);
    response.set_max_unacked(1);
    response.add_configs(0);

    auto promise = aasdk::channel::SendPromise::defer(strand_, "VideoService_sendVideoFocus");
    promise->then(std::bind(&VideoService::sendVideoFocusIndication, this->shared_from_this()),
                  std::bind(&VideoService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendAVChannelSetupResponse(response, std::move(promise));
//    channel_->receive(this->shared_from_this());
}

void VideoService::onAVChannelStartIndication(const aasdk::proto::messages::AVChannelStartIndication& indication)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onAVChannelStartIndication: %s", indication.Utf8DebugString().c_str());
    if(Log::isDebug()) Log_d("start indication, session: %d", indication.session());

    session_ = indication.session();

//    channel_->receive(this->shared_from_this());
}

void VideoService::onAVChannelStopIndication(const aasdk::proto::messages::AVChannelStopIndication& indication)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onAVChannelStopIndication: %s", indication.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("stop indication");
    eventHandler_->onAVChannelStopIndication();

//    channel_->receive(this->shared_from_this());
}

void VideoService::onAVMediaWithTimestampIndication(aasdk::messenger::Timestamp::ValueType timestamp, const aasdk::common::DataConstBuffer& buffer)
{
    videoOutput_->write(timestamp, buffer);

    aasdk::proto::messages::AVMediaAckIndication indication;
    indication.set_session(session_);
    indication.set_value(1);

    auto promise = aasdk::channel::SendPromise::defer(strand_, "VideoService_avMediaAck");
    promise->then([]() {}, std::bind(&VideoService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendAVMediaAckIndication(indication, std::move(promise));

//    channel_->receive(this->shared_from_this());
}

void VideoService::onAVMediaIndication(const aasdk::common::DataConstBuffer& buffer)
{
    this->onAVMediaWithTimestampIndication(0, buffer);
}

void VideoService::onChannelError(const aasdk::error::Error& e)
{
    if (!isRunning_){
        if (Log::isWarn()) Log_w("Received error %s but is not running (maybe is stopping?), ignore it", e.what());
        return;
    }
//    Log_e("Channel error: %s", e.what());
    serviceEventHandler_->onError(e);
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
//    channel_->receive(this->shared_from_this());
}

void VideoService::sendVideoFocusIndication()
{
    if(Log::isDebug()) Log_d("Video focus indication.");

    aasdk::proto::messages::VideoFocusIndication videoFocusIndication;
    videoFocusIndication.set_focus_mode(aasdk::proto::enums::VideoFocusMode::FOCUSED);
    videoFocusIndication.set_unrequested(false);

    auto promise = aasdk::channel::SendPromise::defer(strand_, "VideoService_videoFocus");
    promise->then([]() {}, std::bind(&VideoService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendVideoFocusIndication(videoFocusIndication, std::move(promise));
}

void VideoService::sendVideoFocusReleaseIndication()
{
    if(Log::isDebug()) Log_d("Video focus release indication.");

    aasdk::proto::messages::VideoFocusIndication videoFocusIndication;
    videoFocusIndication.set_focus_mode(aasdk::proto::enums::VideoFocusMode::UNFOCUSED);
    videoFocusIndication.set_unrequested(false);

    auto promise = aasdk::channel::SendPromise::defer(strand_, "VideoService_videoFocusRelease");
    promise->then([]() {}, std::bind(&VideoService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendVideoFocusIndication(videoFocusIndication, std::move(promise));
}

}