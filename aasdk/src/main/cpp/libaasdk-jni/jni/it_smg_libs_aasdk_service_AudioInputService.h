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

    static jfieldID handleId;
    static JAudioInputService::Pointer getJAudioInputService(JNIEnv* env, jobject jaudioinputservice);
    IService::Pointer getService() override;

    void start();
    void stop();
private:
    AudioInputService::Pointer audioInputService_;

};