#include "channel/av/SystemAudioServiceChannel.hpp"
#include "service/SystemAudioService.h"

namespace service
{

SystemAudioService::SystemAudioService(aasdk::io::ioService& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IAudioOutput::Pointer audioOutput, IServiceEventHandler::Pointer serviceEventHandler)
        : AudioService(ioService, std::make_shared<aasdk::channel::av::SystemAudioServiceChannel>(strand_, std::move(messenger)), std::move(audioOutput), std::move(serviceEventHandler))
{
}

SystemAudioService::~SystemAudioService(){
    if (Log::isVerbose()) Log_v("destructor");
    channel_.reset();
}


}