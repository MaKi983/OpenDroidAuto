#include <channel/av/MediaAudioServiceChannel.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

MediaAudioServiceChannel::MediaAudioServiceChannel(boost::asio::io_service::strand& strand, messenger::IMessenger::Pointer messenger)
    : AudioServiceChannel(strand, std::move(messenger), messenger::ChannelId::MEDIA_AUDIO)
{

}

}
}
}
