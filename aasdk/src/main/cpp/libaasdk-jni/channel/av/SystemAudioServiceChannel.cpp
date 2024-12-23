#include <channel/av/SystemAudioServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

SystemAudioServiceChannel::SystemAudioServiceChannel(io::strand& strand, messenger::IMessenger::Pointer messenger)
    : AudioServiceChannel(strand, std::move(messenger), messenger::ChannelId::SYSTEM_AUDIO)
{

}

}
}
}
