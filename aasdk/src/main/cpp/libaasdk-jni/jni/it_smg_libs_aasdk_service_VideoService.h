#pragma once

#include <JNIBase.h>
#include <service/VideoService.h>
#include "it_smg_libs_aasdk_service_Service.h"

using namespace service;

class JVideoService : public JService, JNIBase {
public:
    typedef JVideoService* Pointer;

    JVideoService(JNIEnv *env, jobject jvideoservice, jobject jmessenger, jobject jvideooutput);
    ~JVideoService();

    static jfieldID handleId;
    static JVideoService::Pointer getJVideoService(JNIEnv* env, jobject jvideoservice);
    IService::Pointer getService() override;

    void start();
    void stop();
    void releaseFocus();
    void gainFocus();

private:
    VideoService::Pointer videoService_;

};