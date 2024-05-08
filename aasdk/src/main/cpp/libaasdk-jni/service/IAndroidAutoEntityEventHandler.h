#pragma once

#include "error/Error.hpp"

namespace service
{

class IAndroidAutoEntityEventHandler
{
public:
    typedef IAndroidAutoEntityEventHandler* Pointer;

    virtual ~IAndroidAutoEntityEventHandler() = default;
    virtual void onAndroidAutoQuit() = 0;
    virtual void onAndroidAutoQuitOnError(const aasdk::error::Error& e) = 0;
};

}
