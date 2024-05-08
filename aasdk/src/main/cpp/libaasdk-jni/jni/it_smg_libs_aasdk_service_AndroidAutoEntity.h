#pragma once

#include <JNIBase.h>
#include <service/AndroidAutoEntity.h>
#include <configuration/IConfiguration.h>
#include <service/IAndroidAutoEntityEventHandler.h>

using namespace service;

class JAndroidAutoEntityEventHandler : JNIBase, public IAndroidAutoEntityEventHandler {
public:
    typedef JAndroidAutoEntityEventHandler* Pointer;

    JAndroidAutoEntityEventHandler(JNIEnv *env, jobject jandroidautoentityeventhandler);

    void initJavaMethods() override;

    void onAndroidAutoQuit() override;
    void onAndroidAutoQuitOnError(const aasdk::error::Error& e) override;

private:
    jmethodID onAndroidAutoQuitMethodId_;
    jmethodID onAndroidAutoQuitOnErrorMethodId_;
};

class JAndroidAutoEntity : JNIBase {
public:
    typedef JAndroidAutoEntity* Pointer;

    JAndroidAutoEntity(JNIEnv *env, jobject jandroidautoentity, jobject jcryptor,
                       jobject jmessenger, jobject jservicelist, jobject jconfig,
                       jobject jpinger);
    ~JAndroidAutoEntity();

    static jfieldID handleId;
    static JAndroidAutoEntity::Pointer getJAndroidAutoEntity(JNIEnv* env, jobject jandroidautoentity);

    void start(jobject jeventhandler);
    void stop();
    void requestShutdown();
private:
    ServiceList getServiceList(JNIEnv* env, jobject jservicelist);
    configuration::IConfiguration::Pointer getConfiguration(JNIEnv* env, jobject jconfig);
    std::string getStringFromJava(JNIEnv* env, jobject object, jmethodID methodID);

    AndroidAutoEntity::Pointer androidAutoEntity_;
    JAndroidAutoEntityEventHandler::Pointer eventHandler_;

};