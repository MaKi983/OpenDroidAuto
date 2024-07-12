#pragma once

#include <JNIBase.h>
#include <service/AudioInputService.h>
#include "it_smg_libs_aasdk_service_Service.h"

using namespace service;

class JAudioInputService : public JService, JNIBase {
public:
    typedef JAudioInputService* Pointer;

    JAudioInputService(JNIEnv *env, jobject jaudioinputservice, jobject jmessenger, jobject jaudiooutput);
    ~JAudioInputService();

    void initJavaMethods() override;

    static jfieldID handleId;
    static JAudioInputService::Pointer getJAudioInputService(JNIEnv* env, jobject jaudioinputservice);
    IService::Pointer getService() override;

    void start();
    void stop();
    void onError(const aasdk::error::Error& e) override;
private:
    AudioInputService::Pointer audioInputService_;
//    jmethodID onErrorMethodId_;

};