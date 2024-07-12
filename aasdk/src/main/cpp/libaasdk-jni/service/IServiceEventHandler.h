#pragma once

class IServiceEventHandler {
public:
    typedef IServiceEventHandler* Pointer;

    virtual ~IServiceEventHandler() = default;

    virtual void onError(const aasdk::error::Error& e) = 0;
};
