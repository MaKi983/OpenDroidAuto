#pragma once

#include <JNIBase.h>
#include <service/AudioService.hpp>
#include "it_smg_libs_aasdk_service_Service.h"

using namespace service;

class JSystemAudioService : public JService, JNIBase {
public:
    typedef JSystemAudioService* Pointer;

    JSystemAudioService(JNIEnv *env, jobject jsystemaudioservice, jobject jmessenger, jobject jaudiooutput);
    ~JSystemAudioService();

    static jfieldID handleId;
    static JSystemAudioService::Pointer getJSystemAudioService(JNIEnv* env, jobject jsystemaudioservice);
    IService::Pointer getService() override;

    void start();
    void stop();
private:
    AudioService::Pointer audioService_;

};
