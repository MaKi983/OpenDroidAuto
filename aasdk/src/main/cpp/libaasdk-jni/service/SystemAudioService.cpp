#include "channel/av/SystemAudioServiceChannel.hpp"
#include "service/SystemAudioService.h"

namespace service
{

SystemAudioService::SystemAudioService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IAudioOutput::Pointer audioOutput)
        : AudioService(ioService, new aasdk::channel::av::SystemAudioServiceChannel(strand_, std::move(messenger)), std::move(audioOutput))
{
}

SystemAudioService::~SystemAudioService(){
    delete channel_;
}


}