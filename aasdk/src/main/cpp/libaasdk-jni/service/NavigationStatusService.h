#pragma once

#include "channel/navigation/NavigationStatusServiceChannel.hpp"
#include "projection/INavigationStatusEvent.h"
#include "IService.hpp"

namespace service
{
    class NavigationStatusService: public aasdk::channel::navigation::INavigationStatusServiceChannelEventHandler, public IService
    {
    public:
        typedef NavigationStatusService* Pointer;

        NavigationStatusService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::INavigationStatusEvent::Pointer navigationEvent);
        ~NavigationStatusService();

        void start() override;
        void stop() override;
        void fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response) override;
        void onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest& request) override;
        void onChannelError(const aasdk::error::Error& e) override;
        void onTurnEvent(const aasdk::proto::messages::NavigationTurnEvent& turnEvent) override;
        void onDistanceEvent(const aasdk::proto::messages::NavigationDistanceEvent& distanceEvent) override;
        void onStatusUpdate(const aasdk::proto::messages::NavigationStatus& navStatus) override;

    private:
        boost::asio::io_service::strand strand_;
        aasdk::channel::navigation::NavigationStatusServiceChannel::Pointer channel_;
        projection::INavigationStatusEvent::Pointer navigationEvent_ = nullptr;

    };

}
