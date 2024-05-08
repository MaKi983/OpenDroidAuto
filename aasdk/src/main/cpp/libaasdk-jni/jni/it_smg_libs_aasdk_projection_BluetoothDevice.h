#pragma once

#include <JNIBase.h>
#include <projection/IBluetoothDevice.h>

namespace projection {

class JBluetoothDevice : JNIBase, public IBluetoothDevice {
public:
    typedef JBluetoothDevice* Pointer;

    JBluetoothDevice(JNIEnv *env, jobject jbluetoothdevice);

    static jfieldID handleId;
    static JBluetoothDevice::Pointer getJBluetoothDevice(JNIEnv* env, jobject jbluetoothdevice);

    void initJavaMethods() override;

    void stop() override;
    bool isPaired(const std::string& address) override;
    void pair(const std::string& address, PairingPromise::Pointer promise) override;
    std::string getLocalAddress() override;
    bool isAvailable() override;

private:
    jmethodID isPairedMethodId_;
    jmethodID stopMethodId_;
    jmethodID pairMethodId_;
    jmethodID getLocalAddressMethodId_;
    jmethodID isAvailableMethodId_;
};

}
