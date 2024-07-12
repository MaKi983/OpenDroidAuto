#pragma once

#include "messenger/IMessenger.hpp"
#include "AudioService.hpp"

namespace service
{

class SystemAudioService: public AudioService
{
public:
    SystemAudioService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IAudioOutput::Pointer audioOutput, IServiceEventHandler::Pointer serviceEventHandler);
    ~SystemAudioService();
};

}