#pragma once

#include <vector>
#include <memory>
#include <proto/ServiceDiscoveryResponseMessage.pb.h>
#include <messenger/Messenger.hpp>

class IService
{
public:
    typedef std::shared_ptr<IService> Pointer;

    virtual ~IService() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse& response) = 0;
};

typedef std::vector<IService::Pointer> ServiceList;

