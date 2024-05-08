#include <channel/av/SystemAudioServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

SystemAudioServiceChannel::SystemAudioServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
    : AudioServiceChannel(strand, std::move(messenger), messenger::ChannelId::SYSTEM_AUDIO)
{

}

}
}
}
