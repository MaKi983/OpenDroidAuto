#include <time.h>
#include <Log.h>
#include <projection/IAudioInput.h>
#include "service/AudioInputService.h"

namespace service
{

AudioInputService::AudioInputService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IAudioInput::Pointer audioInput, IServiceEventHandler::Pointer serviceEventHandler)
        : strand_(ioService)
        , channel_(std::make_shared<aasdk::channel::av::AVInputServiceChannel>(strand_, std::move(messenger)))
        , serviceEventHandler_(std::move(serviceEventHandler))
        , audioInput_(std::move(audioInput))
        , session_(0)
        , isRunning_(false)
{

}

AudioInputService::~AudioInputService(){
}


void AudioInputService::start()
{
    isRunning_ = true;
    strand_.dispatch([this, self = this->shared_from_this()]() {
        if(Log::isInfo()) Log_i("start.");
        channel_->receive(this->shared_from_this());
    });
}

void AudioInputService::stop()
{
    if(Log::isInfo()) Log_i("stop.");
    isRunning_ = false;
    audioInput_->stop();
}

void AudioInputService::fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response)
{
    if(Log::isInfo()) Log_i("fill features.");

    auto* channelDescriptor = response.add_channels();
    channelDescriptor->set_channel_id(static_cast<uint32_t>(channel_->getId()));

    auto* avInputChannel = channelDescriptor->mutable_av_input_channel();
    avInputChannel->set_stream_type(aasdk::proto::enums::AVStreamType::AUDIO);

    auto audioConfig = avInputChannel->mutable_audio_config();
    audioConfig->set_sample_rate(audioInput_->getSampleRate());
    audioConfig->set_bit_depth(audioInput_->getSampleSize());
    audioConfig->set_channel_count(audioInput_->getChannelCount());

    if(Log::isVerbose() && Log::logProtocol()) Log_v("%s", channelDescriptor->Utf8DebugString().c_str());
}

void AudioInputService::onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onChannelOpenRequest: %s", request.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("open request, priority: %d", request.priority());
    const aasdk::proto::enums::Status::Enum status = audioInput_->open() ? aasdk::proto::enums::Status::OK : aasdk::proto::enums::Status::FAIL;
    if(Log::isDebug()) Log_d("open status: %s", aasdk::proto::enums::Status::Enum_Name(status).c_str());

    aasdk::proto::messages::ChannelOpenResponse response;
    response.set_status(status);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&AudioInputService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendChannelOpenResponse(response, std::move(promise));

    channel_->receive(this->shared_from_this());
}

void AudioInputService::onAVChannelSetupRequest(const aasdk::proto::messages::AVChannelSetupRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onAVChannelSetupRequest: %s", request.Utf8DebugString().c_str());

    if(Log::isDebug()) Log_d("setup request, config index: %d", request.config_index());
    const aasdk::proto::enums::AVChannelSetupStatus::Enum status = aasdk::proto::enums::AVChannelSetupStatus::OK;
    if(Log::isDebug()) Log_d("setup status: %s", aasdk::proto::enums::AVChannelSetupStatus::Enum_Name(status).c_str());

    aasdk::proto::messages::AVChannelSetupResponse response;
    response.set_media_status(status);
    response.set_max_unacked(1);
    response.add_configs(0);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&AudioInputService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendAVChannelSetupResponse(response, std::move(promise));

    channel_->receive(this->shared_from_this());
}

void AudioInputService::onAVInputOpenRequest(const aasdk::proto::messages::AVInputOpenRequest& request)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onAVInputOpenRequest: %s", request.Utf8DebugString().c_str());
    if(Log::isDebug()) Log_d("input open request, open: %d, anc: %d, ec: %d, max unacked: ", request.open(), request.anc(), request.ec(), request.max_unacked());

    if(request.open())
    {
        auto startPromise = projection::IAudioInput::StartPromise::defer(strand_);
        startPromise->then(std::bind(&AudioInputService::onAudioInputOpenSucceed, this->shared_from_this()),
                           [this, self = this->shared_from_this()]() {
                               Log_e("[AudioInputService] audio input open failed.");

                               aasdk::proto::messages::AVInputOpenResponse response;
                               response.set_session(session_);
                               response.set_value(1);

                               auto sendPromise = aasdk::channel::SendPromise::defer(strand_);
                               sendPromise->then([]() {}, std::bind(&AudioInputService::onChannelError, this->shared_from_this(), std::placeholders::_1));
                               channel_->sendAVInputOpenResponse(response, std::move(sendPromise));
                           });

        audioInput_->start(std::move(startPromise));
    }
    else
    {
        audioInput_->stop();

        aasdk::proto::messages::AVInputOpenResponse response;
        response.set_session(session_);
        response.set_value(0);

        auto sendPromise = aasdk::channel::SendPromise::defer(strand_);
        sendPromise->then([]() {}, std::bind(&AudioInputService::onChannelError, this->shared_from_this(), std::placeholders::_1));
        channel_->sendAVInputOpenResponse(response, std::move(sendPromise));
    }

    channel_->receive(this->shared_from_this());
}

void AudioInputService::onAVMediaAckIndication(const aasdk::proto::messages::AVMediaAckIndication& indication)
{
    if(Log::isVerbose() && Log::logProtocol()) Log_v("onAVMediaAckIndication: %s", indication.Utf8DebugString().c_str());

    channel_->receive(this->shared_from_this());
}

void AudioInputService::onChannelError(const aasdk::error::Error& e)
{
    if (!isRunning_){
        if (Log::isWarn()) Log_w("Received error %s but is not running (maybe is stopping?), ignore it", e.what());
        return;
    }
//    Log_e("channel error: %s", e.what());
//    serviceEventHandler_->onError(e);
}

void AudioInputService::onAudioInputOpenSucceed()
{
    if(Log::isDebug()) Log_d("audio input open succeed");

    aasdk::proto::messages::AVInputOpenResponse response;
    response.set_session(session_);
    response.set_value(0);

    auto sendPromise = aasdk::channel::SendPromise::defer(strand_);
    sendPromise->then([]() {}, std::bind(&AudioInputService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendAVInputOpenResponse(response, std::move(sendPromise));

    this->readAudioInput();
}

void AudioInputService::onAudioInputDataReady(aasdk::common::Data data)
{
    auto sendPromise = aasdk::channel::SendPromise::defer(strand_);
    sendPromise->then(std::bind(&AudioInputService::readAudioInput, this->shared_from_this()),
                      std::bind(&AudioInputService::onChannelError, this->shared_from_this(), std::placeholders::_1));

    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
    channel_->sendAVMediaWithTimestampIndication(timestamp.count(), std::move(data), std::move(sendPromise));
}

void AudioInputService::readAudioInput()
{

    if(audioInput_->isActive())
    {
        auto readPromise = projection::IAudioInput::ReadPromise::defer(strand_);
        readPromise->then(std::bind(&AudioInputService::onAudioInputDataReady, this->shared_from_this(), std::placeholders::_1),
                          [this, self = this->shared_from_this()]() {
                              if(Log::isInfo()) Log_i("audio input read rejected.");
                          });

        audioInput_->read(std::move(readPromise));
    }

}

}
