#include <io/PromiseLink.hpp>
#include <channel/ServiceChannel.hpp>


namespace aasdk
{
namespace channel
{

ServiceChannel::ServiceChannel(boost::asio::io_service::strand& strand,
                               messenger::IMessenger::Pointer messenger,
                               messenger::ChannelId channelId)
    : strand_(strand)
    , messenger_(std::move(messenger))
    , channelId_(channelId)
{

}

void ServiceChannel::send(messenger::Message::Pointer message, SendPromise::Pointer promise)
{
#if BOOST_VERSION < 106600
    auto sendPromise = messenger::SendPromise::defer(strand_.get_io_service());
#else
    auto sendPromise = messenger::SendPromise::defer(strand_.context());
#endif

    io::PromiseLink<>::forward(*sendPromise, std::move(promise));
    messenger_->enqueueSend(std::move(message), std::move(sendPromise));
}

}
}
