#include "channel/av/SpeechAudioServiceChannel.hpp"
#include "service/SpeechAudioService.h"

namespace service
{

SpeechAudioService::SpeechAudioService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IAudioOutput::Pointer audioOutput)
        : AudioService(ioService, new aasdk::channel::av::SpeechAudioServiceChannel(strand_, std::move(messenger)), std::move(audioOutput))
{
}

SpeechAudioService::~SpeechAudioService(){
    delete channel_;
}

}