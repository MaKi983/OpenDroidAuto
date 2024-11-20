#pragma once

#include "io/Promise.hpp"

namespace projection
{

class IBluetoothDevice
{
public:
    typedef aasdk::io::Promise<void, void> PairingPromise;
    typedef IBluetoothDevice* Pointer;

    virtual void stop() = 0;
    virtual bool isPaired(const std::string& address) = 0;
    virtual void pair(const std::string& address, PairingPromise::Pointer promise) = 0;
    virtual std::string getLocalAddress() = 0;
    virtual bool isAvailable() = 0;
    virtual bool isEnabledAd2p() = 0;
    virtual bool isEnabledHfp() = 0;
};

}

