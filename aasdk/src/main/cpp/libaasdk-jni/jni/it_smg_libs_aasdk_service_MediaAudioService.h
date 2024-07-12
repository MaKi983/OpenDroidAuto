#pragma once

#include <JNIBase.h>
#include <service/AudioService.hpp>
#include "it_smg_libs_aasdk_service_Service.h"

using namespace service;

class JMediaAudioService : public JService, JNIBase {
public:
    typedef JMediaAudioService* Pointer;

    JMediaAudioService(JNIEnv *env, jobject jmediaaudioservice, jobject jmessenger, jobject jaudiooutput);
    ~JMediaAudioService();

    void initJavaMethods() override;

    static jfieldID handleId;
    static JMediaAudioService::Pointer getJMediaAudioService(JNIEnv* env, jobject jmediaaudioservice);
    IService::Pointer getService() override;

    void start();
    void stop();
    void onError(const aasdk::error::Error& e) override;
private:
    AudioService::Pointer audioService_;
//    jmethodID onErrorMethodId_;
};