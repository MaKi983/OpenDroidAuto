#pragma once

#include <JNIBase.h>
#include <service/NavigationStatusService.h>
#include "it_smg_libs_aasdk_service_Service.h"

using namespace service;

class JNavigationStatusService : public JService, JNIBase {
public:
    typedef JNavigationStatusService* Pointer;

    JNavigationStatusService(JNIEnv *env, jobject jnavigationstatusservice, jobject jmessenger, jobject jnavigationstatusevent);
    ~JNavigationStatusService();

    void initJavaMethods() override;

    static jfieldID handleId;
    static JNavigationStatusService::Pointer getJNavigationStatusService(JNIEnv* env, jobject jnavigationstatusservice);
    IService::Pointer getService() override;

    void start();
    void stop();
    void onError(const aasdk::error::Error& e) override;
private:
    NavigationStatusService::Pointer navigationStatusService_;
//    jmethodID onErrorMethodId_;
};