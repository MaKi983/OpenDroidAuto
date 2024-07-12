#pragma once

#include "messenger/IMessenger.hpp"
#include "AudioService.hpp"

namespace service
{

class MediaAudioService: public AudioService
{
public:
    MediaAudioService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IAudioOutput::Pointer audioOutput, IServiceEventHandler::Pointer serviceEventHandler);
    ~MediaAudioService();
};

}