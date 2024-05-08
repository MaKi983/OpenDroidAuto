#pragma once

#include <service/IService.hpp>

class JService
{
public:
    typedef JService* Pointer;

    virtual ~JService() = default;

    virtual IService::Pointer getService() = 0;
};

