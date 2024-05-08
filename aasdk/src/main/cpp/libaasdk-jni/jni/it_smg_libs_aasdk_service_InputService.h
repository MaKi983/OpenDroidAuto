#pragma once

#include <JNIBase.h>
#include <service/InputService.h>
#include "it_smg_libs_aasdk_service_Service.h"

using namespace service;

class JInputService : public JService, JNIBase {
public:
    typedef JInputService* Pointer;

    JInputService(JNIEnv *env, jobject jinputservice, jobject jmessenger, jobject jinputdevice);

    static jfieldID handleId;
    static JInputService::Pointer getJInputService(JNIEnv* env, jobject jinputservice);
    IService::Pointer getService() override;

    void start();
    void stop();
private:
    InputService::Pointer inputService_;

};
