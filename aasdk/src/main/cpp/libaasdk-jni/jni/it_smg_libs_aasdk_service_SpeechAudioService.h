#pragma once

#include <JNIBase.h>
#include <service/AudioService.hpp>
#include "it_smg_libs_aasdk_service_Service.h"

using namespace service;

class JSpeechAudioService : public JService, JNIBase {
public:
    typedef JSpeechAudioService* Pointer;

    JSpeechAudioService(JNIEnv *env, jobject jspeechaudioservice, jobject jmessenger, jobject jaudiooutput);
    ~JSpeechAudioService();

    void initJavaMethods() override;

    static jfieldID handleId;
    static JSpeechAudioService::Pointer getJSpeechAudioService(JNIEnv* env, jobject jspeechaudioservice);
    IService::Pointer getService() override;

    void start();
    void stop();
    void onError(const aasdk::error::Error& e) override;
private:
    AudioService::Pointer audioService_;
//    jmethodID onErrorMethodId_;
};
