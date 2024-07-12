#pragma once

#include <memory>
#include "IAndroidAutoEntityEventHandler.h"

namespace service
{

    class IAndroidAutoEntity
    {
    public:
        typedef std::shared_ptr<IAndroidAutoEntity> Pointer;

        virtual ~IAndroidAutoEntity() = default;

        virtual void start(IAndroidAutoEntityEventHandler::Pointer eventHandler) = 0;
        virtual void stop() = 0;
        virtual void sendShutdownRequest() = 0;
    };

}
