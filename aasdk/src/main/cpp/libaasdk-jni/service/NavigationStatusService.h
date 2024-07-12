#pragma once

#include "channel/navigation/NavigationStatusServiceChannel.hpp"
#include "projection/INavigationStatusEvent.h"
#include "IService.hpp"
#include "IServiceEventHandler.h"

namespace service
{
    class NavigationStatusService: public aasdk::channel::navigation::INavigationStatusServiceChannelEventHandler, public IService, public std::enable_shared_from_this<NavigationStatusService>
    {
    public:
        typedef std::shared_ptr<NavigationStatusService> Pointer;

        NavigationStatusService(boost::asio::io_service& ioService, aasdk::messenger::IMessenger::Pointer messenger, projection::INavigationStatusEvent::Pointer navigationEvent, IServiceEventHandler::Pointer serviceEventHandler);
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
    	using std::enable_shared_from_this<NavigationStatusService>::shared_from_this;
    	
        boost::asio::io_service::strand strand_;
        aasdk::channel::navigation::NavigationStatusServiceChannel::Pointer channel_;
        projection::INavigationStatusEvent::Pointer navigationEvent_ = nullptr;
        IServiceEventHandler::Pointer serviceEventHandler_;
        bool isRunning_;
    };

}
