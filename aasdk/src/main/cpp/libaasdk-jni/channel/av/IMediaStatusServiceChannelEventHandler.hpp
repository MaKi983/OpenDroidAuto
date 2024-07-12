#pragma once

#include <proto/ChannelOpenRequestMessage.pb.h>
#include <proto/MediaInfoChannelMetadataData.pb.h>
#include <proto/MediaInfoChannelPlaybackData.pb.h>
#include <error/Error.hpp>


namespace aasdk
{
namespace channel
{
namespace av
{

class IMediaStatusServiceChannelEventHandler
{
public:
    typedef std::shared_ptr<IMediaStatusServiceChannelEventHandler> Pointer;

    IMediaStatusServiceChannelEventHandler() = default;
    virtual ~IMediaStatusServiceChannelEventHandler() = default;

    virtual void onChannelOpenRequest(const proto::messages::ChannelOpenRequest& request) = 0;
    virtual void onChannelError(const error::Error& e) = 0;
    virtual void onMetadataUpdate(const proto::messages::MediaInfoChannelMetadataData& metadata) = 0;
    virtual void onPlaybackUpdate(const proto::messages::MediaInfoChannelPlaybackData& playback) = 0;
};

}
}
}
