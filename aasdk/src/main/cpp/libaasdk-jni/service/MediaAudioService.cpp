#include <Log.h>
#include <channel/av/MediaAudioServiceChannel.hpp>
#include "service/MediaAudioService.hpp"

namespace service
{

MediaAudioService::MediaAudioService(aasdk::io::ioService& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IAudioOutput::Pointer audioOutput, IServiceEventHandler::Pointer serviceEventHandler)
        : AudioService(ioService, std::make_shared<aasdk::channel::av::MediaAudioServiceChannel>(strand_, std::move(messenger)), std::move(audioOutput), std::move(serviceEventHandler))
{
}

MediaAudioService::~MediaAudioService(){
    if (Log::isVerbose()) Log_v("destructor");
    channel_.reset();
}


}