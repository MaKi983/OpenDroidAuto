#pragma once

#include <service/IService.hpp>
#include <service/IServiceEventHandler.h>

class JService : public IServiceEventHandler
{
public:
    typedef JService* Pointer;

    virtual ~JService() = default;

    virtual IService::Pointer getService() = 0;
};

