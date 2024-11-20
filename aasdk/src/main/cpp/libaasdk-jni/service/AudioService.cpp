#include <Log.h>
#include <service/AudioService.hpp>
#include <inttypes.h>

namespace service
{

AudioService::AudioService(boost::asio::io_service& ioService, aasdk::channel::av::IAudioServiceChannel::Pointer channel, projection::IAudioOutput::Pointer audioOutput, IServiceEventHandler::Pointer serviceEventHandler)
        : strand_(ioService)
        , channel_(std::move(channel))
        , serviceEventHandler_(std::move(serviceEventHandler))
        , audioOutput_(std::move(audioOutput))
        , session_(-1)
        , isRunning_(false)
{
}

void AudioService::start()
{
    isRunning_ = true;
//    strand_.dispatch([this, self = this->shared_from_this()]() {
//    strand_.post([this, self = this->shared_from_this()]() {
        if(Log::isInfo()) Log_i("%s / start", aasdk::messenger::channelIdToString(channel_->getId()).c_str());
        channel_->receive(this->shared_from_this());
//    });
}

void AudioService::stop()
{
    if(Log::isInfo()) Log_i("%s / stop", aasdk::messenger::channelIdToString(channel_->getId()).c_str());
    isRunning_ = false;
    audioOutput_->stop();
}

void AudioService::fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response)
{
    if(Log::isInfo()) Log_i("%s / fill features", aasdk::messenger::channelIdToString(channel_->getId()).c_str());

    auto* channelDescriptor = response.add_channels();
    channelDescriptor->set_channel_id(static_cast<uint32_t>(channel_->getId()));

    auto* audioChannel = channelDescriptor->mutable_av_channel();
    audioChannel->set_stream_type(aasdk::proto::enums::AVStreamType::AUDIO);

    switch(channel_->getId())
    {
        case aasdk::messenger::ChannelId::SYSTEM_AUDIO:
            audioChannel->set_audio_type(aasdk::proto::enums::AudioType::SYSTEM);
            break;

        case aasdk::messenger::ChannelId::MEDIA_AUDIO:
            audioChannel->set_audio_type(aasdk::proto::enums::AudioType::MEDIA);
            break;

        case aasdk::messenger::ChannelId::SPEECH_AUDIO:
            audioChannel->set_audio_type(aasdk::proto::enums::AudioType::SPEECH);
            break;
        default:
            break;
    }

    audioChannel->set_available_while_in_call(true);

    auto* audioConfig = audioChannel->add_audio_configs();
    audioConfig->set_sample_rate(audioOutput_->getSampleRate());
    audioConfig->set_bit_depth(audioOutput_->getSampleSize());
    audioConfig->set_channel_count(audioOutput_->getChannelCount());

    if(Log::isVerbose() && Log::logProtocol()) Log_v("%s / %s", aasdk::messenger::channelIdToString(channel_->getId()).c_str(), channelDescriptor->Utf8DebugString().c_str());
}

void AudioService::onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("%s / onChannelOpenRequest: %s", aasdk::messenger::channelIdToString(channel_->getId()).c_str(), request.Utf8DebugString().c_str());
    if(Log::isDebug()) Log_d("%s / open request, priority %d", aasdk::messenger::channelIdToString(channel_->getId()).c_str(), request.priority());

    if(Log::isDebug()) Log_d("%s / audio output sample rate: %d, sample size: %d, channel count: %d",
          aasdk::messenger::channelIdToString(channel_->getId()).c_str(), audioOutput_->getSampleRate(), audioOutput_->getSampleSize(), audioOutput_->getChannelCount());

    const aasdk::proto::enums::Status::Enum status = audioOutput_->open() ? aasdk::proto::enums::Status::OK : aasdk::proto::enums::Status::FAIL;
    if(Log::isDebug()) Log_d("%s / open status: %s", aasdk::messenger::channelIdToString(channel_->getId()).c_str(), aasdk::proto::enums::Status::Enum_Name(status).c_str());

    aasdk::proto::messages::ChannelOpenResponse response;
    response.set_status(status);

    auto promise = aasdk::channel::SendPromise::defer(strand_, "AudioService_channelOpen");
    promise->then([]() {}, std::bind(&AudioService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendChannelOpenResponse(response, std::move(promise));
//    channel_->receive(this->shared_from_this());
}

void AudioService::onAVChannelSetupRequest(const aasdk::proto::messages::AVChannelSetupRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("%s / onAVChannelSetupRequest: %s", aasdk::messenger::channelIdToString(channel_->getId()).c_str(), request.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("%s / setup request config index: %d", aasdk::messenger::channelIdToString(channel_->getId()).c_str(), request.config_index());

    const aasdk::proto::enums::AVChannelSetupStatus::Enum status = aasdk::proto::enums::AVChannelSetupStatus::OK;
    if(Log::isDebug()) Log_d("%s / setup status: %s", aasdk::messenger::channelIdToString(channel_->getId()).c_str(), aasdk::proto::enums::AVChannelSetupStatus::Enum_Name(status).c_str());


    aasdk::proto::messages::AVChannelSetupResponse response;
    response.set_media_status(status);
    response.set_max_unacked(1);
    response.add_configs(0);

    auto promise = aasdk::channel::SendPromise::defer(strand_, "AudioService_avChannelSetup");
    promise->then([]() {}, std::bind(&AudioService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendAVChannelSetupResponse(response, std::move(promise));
//    channel_->receive(this->shared_from_this());
}

void AudioService::onAVChannelStartIndication(const aasdk::proto::messages::AVChannelStartIndication& indication)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("%s / onAVChannelStartIndication: %s", aasdk::messenger::channelIdToString(channel_->getId()).c_str(), indication.Utf8DebugString().c_str());
    if(Log::isDebug()) Log_d("%s / start indication, session: %d", aasdk::messenger::channelIdToString(channel_->getId()).c_str(), indication.session());

    session_ = indication.session();
    audioOutput_->start();
//    channel_->receive(this->shared_from_this());
}

void AudioService::onAVChannelStopIndication(const aasdk::proto::messages::AVChannelStopIndication& indication)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("%s / onAVChannelStopIndication: %s", aasdk::messenger::channelIdToString(channel_->getId()).c_str(), indication.Utf8DebugString().c_str());
    if(Log::isDebug()) Log_d("%s / stop indication, session: %d", aasdk::messenger::channelIdToString(channel_->getId()).c_str(), session_);

    session_ = -1;
    audioOutput_->suspend();
//    channel_->receive(this->shared_from_this());
}

void AudioService::onAVMediaWithTimestampIndication(aasdk::messenger::Timestamp::ValueType timestamp, const aasdk::common::DataConstBuffer& buffer)
{
    if (Log::isVerbose()) Log_v("onAVMediaWithTimestampIndication");
    audioOutput_->write(timestamp, buffer);

    aasdk::proto::messages::AVMediaAckIndication indication;
    indication.set_session(session_);
    indication.set_value(1);

    auto promise = aasdk::channel::SendPromise::defer(strand_, "AudioService_avMediaAck");
    promise->then([]() {}, std::bind(&AudioService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendAVMediaAckIndication(indication, std::move(promise));

//    channel_->receive(this->shared_from_this());
}

void AudioService::onAVMediaIndication(const aasdk::common::DataConstBuffer& buffer)
{
    this->onAVMediaWithTimestampIndication(0, buffer);
}

void AudioService::onChannelError(const aasdk::error::Error& e)
{
    if (!isRunning_){
        if (Log::isWarn()) Log_w("Received error %s but is not running (maybe is stopping?), ignore it", e.what());
        return;
    }
//    Log_e("%s / channel error: %s", aasdk::messenger::channelIdToString(channel_->getId()).c_str(), e.what());
    serviceEventHandler_->onError(e);
}

}
