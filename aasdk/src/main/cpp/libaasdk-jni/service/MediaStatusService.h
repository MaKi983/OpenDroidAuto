#pragma once

#include <projection/IMediaStatusEvent.h>
#include "channel/av/MediaStatusServiceChannel.hpp"
#include "IService.hpp"
#include "IServiceEventHandler.h"

namespace service
{
    class MediaStatusService: public aasdk::channel::av::IMediaStatusServiceChannelEventHandler, public IService, public std::enable_shared_from_this<MediaStatusService>
    {
    public:
        typedef std::shared_ptr<MediaStatusService> Pointer;

        MediaStatusService(aasdk::io::ioService& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IMediaStatusEvent::Pointer mediaEvent, IServiceEventHandler::Pointer serviceEventHandler);
        ~MediaStatusService();

        void start() override;
        void stop() override;
        void fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response) override;
        void onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request) override;
        void onChannelError(const aasdk::error::Error& e) override;
        void onMetadataUpdate(const aasdk::proto::messages::MediaInfoChannelMetadataData& metadata) override;
        void onPlaybackUpdate(const aasdk::proto::messages::MediaInfoChannelPlaybackData& playback) override;

    private:
        using std::enable_shared_from_this<MediaStatusService>::shared_from_this;

        aasdk::io::strand strand_;
        aasdk::channel::av::MediaStatusServiceChannel::Pointer channel_;
        projection::IMediaStatusEvent::Pointer mediaEvent_ = nullptr;
        IServiceEventHandler::Pointer serviceEventHandler_;
        bool isRunning_;
    };

}
