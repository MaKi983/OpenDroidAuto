#pragma once

#include <projection/IMediaStatusEvent.h>
#include "channel/av/MediaStatusServiceChannel.hpp"
#include "IService.hpp"

namespace service
{
    class MediaStatusService: public aasdk::channel::av::IMediaStatusServiceChannelEventHandler, public IService
    {
    public:
        typedef MediaStatusService* Pointer;

        MediaStatusService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::IMediaStatusEvent::Pointer mediaEvent);
        ~MediaStatusService();

        void start() override;
        void stop() override;
        void fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response) override;
        void onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request) override;
        void onChannelError(const aasdk::error::Error& e) override;
        void onMetadataUpdate(const aasdk::proto::messages::MediaInfoChannelMetadataData& metadata) override;
        void onPlaybackUpdate(const aasdk::proto::messages::MediaInfoChannelPlaybackData& playback) override;

    private:
        boost::asio::io_service::strand strand_;
        aasdk::channel::av::MediaStatusServiceChannel::Pointer channel_;
        projection::IMediaStatusEvent::Pointer mediaEvent_ = nullptr;
    };

}
