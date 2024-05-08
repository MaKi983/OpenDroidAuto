#pragma once

#include "proto/MediaInfoChannelMetadataData.pb.h"
#include "proto/MediaInfoChannelPlaybackData.pb.h"

namespace projection
{

    class IMediaStatusEvent
    {
    public:
        typedef IMediaStatusEvent* Pointer;

        virtual ~IMediaStatusEvent() = default;

        virtual void mediaPlaybackUpdate(const aasdk::proto::messages::MediaInfoChannelPlaybackData& playback) = 0;
        virtual void mediaMetadataUpdate(const aasdk::proto::messages::MediaInfoChannelMetadataData& metadata) = 0;
    };



}