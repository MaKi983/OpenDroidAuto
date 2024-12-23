#pragma once

#include "messenger/IMessenger.hpp"
#include "AudioService.hpp"

namespace service
{

    class SpeechAudioService: public AudioService
    {
    public:
        SpeechAudioService(aasdk::io::ioService& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IAudioOutput::Pointer audioOutput, IServiceEventHandler::Pointer serviceEventHandler);
        ~SpeechAudioService();
    };

}