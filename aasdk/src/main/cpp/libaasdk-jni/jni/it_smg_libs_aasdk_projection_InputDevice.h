#pragma once

#include <JNIBase.h>
#include <projection/IInputDevice.h>

namespace projection {

class JInputDevice : JNIBase, public IInputDevice {
public:
    typedef JInputDevice* Pointer;

    JInputDevice(JNIEnv *env, jobject jinputdevice);

    static jfieldID handleId;
    static JInputDevice::Pointer getJInputDevice(JNIEnv* env, jobject jinputdevice);

    void initJavaMethods() override;

    void sendTouchEvent(aasdk::proto::messages::InputEventIndication inputEventIndication);
    void start(IInputDeviceEventHandler& eventHandler) override;
    void stop() override;
    ButtonCodes getSupportedButtonCodes() override;
    bool hasTouchscreen() override;
    Rect getTouchscreenGeometry() override;
private:
    aasdk::proto::enums::ButtonCode::Enum int2ButtonCode(int c);

    jmethodID startMethodId_;
    jmethodID stopMethodId_;
    jmethodID getSupportedButtonCodesMethodId_;
    jmethodID hasTouchscreenMethodId_;
    jmethodID getTouchscreenGeometryMethodId_;

    IInputDeviceEventHandler* eventHandler_;
};

}
