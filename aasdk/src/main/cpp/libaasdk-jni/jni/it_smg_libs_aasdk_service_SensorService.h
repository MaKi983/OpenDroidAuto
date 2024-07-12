#pragma once

#include <JNIBase.h>
#include <service/SensorService.h>
#include "it_smg_libs_aasdk_service_Service.h"

using namespace service;

class JSensorService : public JService, JNIBase {
public:
    typedef JSensorService* Pointer;

    JSensorService(JNIEnv *env, jobject jsensorservice, jobject jmessenger, bool isnight);
    ~JSensorService();

    void initJavaMethods() override;

    static jfieldID handleId;
    static JSensorService::Pointer getJSensorService(JNIEnv* env, jobject jinputservice);
    IService::Pointer getService() override;

    void sendNightMode(bool isNight);
    void start();
    void stop();
    void onError(const aasdk::error::Error& e) override;
private:
    SensorService::Pointer sensorService_;
//    jmethodID onErrorMethodId_;
};
