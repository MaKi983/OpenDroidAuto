#pragma once

#include <JNIBase.h>
#include <service/VideoService.h>
#include "it_smg_libs_aasdk_service_Service.h"

using namespace service;

class JVideoEventHandler : JNIBase, public IVideoEventHandler {
public:
    typedef JVideoEventHandler* Pointer;

    JVideoEventHandler(JNIEnv *env, jobject jvideoeventhandler);

    void initJavaMethods() override;

    void onAVChannelStopIndication() override;

private:
    jmethodID onAVChannelStopIndicationMethodId_;
};

class JVideoService : public JService, JNIBase {
public:
    typedef JVideoService* Pointer;

    JVideoService(JNIEnv *env, jobject jvideoservice, jobject jmessenger, jobject jvideooutput, jobject jvideoeventhandler);
    ~JVideoService();

    void initJavaMethods() override;

    static jfieldID handleId;
    static JVideoService::Pointer getJVideoService(JNIEnv* env, jobject jvideoservice);
    IService::Pointer getService() override;

    void start();
    void stop();
    void releaseFocus();
    void gainFocus();
    void onError(const aasdk::error::Error& e) override;

private:
    VideoService::Pointer videoService_;
};