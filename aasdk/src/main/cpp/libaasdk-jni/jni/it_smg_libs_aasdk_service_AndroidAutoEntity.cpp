#include <service/AudioInputService.h>
#include <configuration/Configuration.h>
#include "it_smg_libs_aasdk_service_AndroidAutoEntity.h"
#include "it_smg_libs_aasdk_messenger_Messenger.h"
#include "it_smg_libs_aasdk_Runtime.h"
#include "it_smg_libs_aasdk_projection_AudioInput.h"
#include "it_smg_libs_aasdk_messenger_Cryptor.h"
#include "it_smg_libs_aasdk_service_Pinger.h"
#include "it_smg_libs_aasdk_service_Service.h"

JAndroidAutoEntityEventHandler::JAndroidAutoEntityEventHandler(JNIEnv *env,
    jobject jandroidautoentityeventhandler)
    : JNIBase(env, jandroidautoentityeventhandler, "JAndroidAutoEntityEventHandler")  {

    initJavaMethods();
}

void JAndroidAutoEntityEventHandler::initJavaMethods() {
    JNIEnv* env = getJniEnv();
    jclass cls = env->GetObjectClass(androidClass_);

    onAndroidAutoQuitMethodId_ = env->GetMethodID(cls, "onAndroidAutoQuit", "()V");
    onAndroidAutoQuitOnErrorMethodId_ = env->GetMethodID(cls, "onAndroidAutoQuitOnError", "(Ljava/lang/String;I)V");

    env->DeleteLocalRef(cls);
}

void JAndroidAutoEntityEventHandler::onAndroidAutoQuit() {
    JNIEnv* env = getJniEnv();
    env->CallVoidMethod(androidClass_, onAndroidAutoQuitMethodId_);
}

void JAndroidAutoEntityEventHandler::onAndroidAutoQuitOnError(const aasdk::error::Error &e) {
    JNIEnv* env = getJniEnv();
    jstring jstr = env->NewStringUTF(aasdk::error::errorCodeToString(e.getCode()).c_str());
    env->CallVoidMethod(androidClass_, onAndroidAutoQuitOnErrorMethodId_, jstr, e.getNativeCode());
    env->DeleteLocalRef(jstr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

jfieldID JAndroidAutoEntity::handleId = nullptr;

JAndroidAutoEntity::JAndroidAutoEntity(JNIEnv *env, jobject jandroidautoentity, jobject jcryptor,
                                       jobject jmessenger, jobject jservicelist, jobject jconfig,
                                       jobject jpinger)
        : JNIBase(env, jandroidautoentity, "JAndroidAutoEntity")  {

    JMessenger::Pointer jMessenger = JMessenger::getJMessenger(env, jmessenger);

    JCryptor::Pointer jCryptor = JCryptor::getJCryptor(env, jcryptor);

    JPinger::Pointer jPinger = JPinger::getJPinger(env, jpinger);

    ServiceList serviceList = getServiceList(env, jservicelist);

    configuration::IConfiguration::Pointer config = getConfiguration(env, jconfig);

    androidAutoEntity_ = std::make_shared<AndroidAutoEntity>(JRuntime::ioService(), jCryptor->getCryptor(), jMessenger->getMessenger(), config, serviceList, jPinger->getPinger());
}

JAndroidAutoEntity::~JAndroidAutoEntity() {
    androidAutoEntity_.reset();
//    delete eventHandler_;
}

JAndroidAutoEntity::Pointer JAndroidAutoEntity::getJAndroidAutoEntity(JNIEnv *env, jobject jandroidautoentity) {
    return (JAndroidAutoEntity::Pointer)env->GetLongField(jandroidautoentity, JAndroidAutoEntity::handleId);;
}

ServiceList JAndroidAutoEntity::getServiceList(JNIEnv* env, jobject jservicelist) {
    ServiceList serviceList;

    jclass java_util_Collection = env->GetObjectClass(jservicelist);

    jmethodID java_util_Collection_size = env->GetMethodID(java_util_Collection, "size", "()I");
    jmethodID java_util_Collection_get = env->GetMethodID(java_util_Collection, "get", "(I)Ljava/lang/Object;");

    jint jsize = env->CallIntMethod(jservicelist, java_util_Collection_size);

    for (int i = 0; i<jsize; i++){
        jobject jservice = env->CallObjectMethod(jservicelist, java_util_Collection_get, i);
        jclass service_cls = env->GetObjectClass(jservice);
        jfieldID handleid = env->GetFieldID(service_cls, "handle_", "J");

        JService::Pointer jService = (JService::Pointer)env->GetLongField(jservice, handleid);
        serviceList.push_back(jService->getService());

        env->DeleteLocalRef(jservice);
        jservice = nullptr;

        env->DeleteLocalRef(service_cls);
        service_cls = nullptr;
    }

    env->DeleteLocalRef(java_util_Collection);
    java_util_Collection = nullptr;

    return serviceList;
}

configuration::IConfiguration::Pointer JAndroidAutoEntity::getConfiguration(JNIEnv* env, jobject jconfig){
    jclass config_cls = env->GetObjectClass(jconfig);

    jmethodID config_huName = env->GetMethodID(config_cls, "huName", "()Ljava/lang/String;");
    jmethodID config_model = env->GetMethodID(config_cls, "model", "()Ljava/lang/String;");
    jmethodID config_year = env->GetMethodID(config_cls, "year", "()Ljava/lang/String;");
    jmethodID config_huMake = env->GetMethodID(config_cls, "huMake", "()Ljava/lang/String;");
    jmethodID config_huModel = env->GetMethodID(config_cls, "huModel", "()Ljava/lang/String;");
    jmethodID config_swVersion = env->GetMethodID(config_cls, "swVersion", "()Ljava/lang/String;");
    jmethodID config_playMediaDuringVr = env->GetMethodID(config_cls, "playMediaDuringVr", "()Z");
    jmethodID config_leftHandDrive = env->GetMethodID(config_cls, "leftHandDrive", "()Z");

    auto config = new configuration::Configuration();
    config->huName(getStringFromJava(env, jconfig, config_huName));
    config->model(getStringFromJava(env, jconfig, config_model));
    config->year(getStringFromJava(env, jconfig, config_year));
    config->huMake(getStringFromJava(env, jconfig, config_huMake));
    config->huModel(getStringFromJava(env, jconfig, config_huModel));
    config->swVersion(getStringFromJava(env, jconfig, config_swVersion));
    config->playMediaDuringVr(env->CallBooleanMethod(jconfig, config_playMediaDuringVr) == JNI_TRUE);
    config->leftHandDrive(env->CallBooleanMethod(jconfig, config_leftHandDrive) == JNI_TRUE);

    env->DeleteLocalRef(config_cls);
    config_cls = nullptr;

    return config;
}

std::string JAndroidAutoEntity::getStringFromJava(JNIEnv* env, jobject object, jmethodID methodID){
    jstring jstr = (jstring) env->CallObjectMethod(object, methodID);

    const char *strReturn = env->GetStringUTFChars(jstr, 0);
    std::string str(strReturn);

    env->ReleaseStringUTFChars(jstr, strReturn);

    return str;
}

void JAndroidAutoEntity::stop() {
    androidAutoEntity_->stop();
}

void JAndroidAutoEntity::start(jobject jeventhandler) {
    eventHandler_ = new JAndroidAutoEntityEventHandler(getJniEnv(), jeventhandler);
    androidAutoEntity_->start(eventHandler_);
}

void JAndroidAutoEntity::requestShutdown() {
    androidAutoEntity_->sendShutdownRequest();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_AndroidAutoEntity_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/service/AndroidAutoEntity");
    JAndroidAutoEntity::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_service_AndroidAutoEntity_nativeSetup(JNIEnv *env, jobject thiz, jobject jcryptor, jobject jmessenger, jobject jservicelist, jobject jconfig,
                                                             jobject jpinger) {
    auto jAndroidAutoEntity = new JAndroidAutoEntity(env, thiz, jcryptor, jmessenger, jservicelist, jconfig, jpinger);
    return (jlong)((size_t)jAndroidAutoEntity);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_AndroidAutoEntity_nativeStop(JNIEnv *env, jobject thiz) {
    JAndroidAutoEntity::Pointer jAndroidAutoEntity = JAndroidAutoEntity::getJAndroidAutoEntity(env, thiz);
    jAndroidAutoEntity->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_AndroidAutoEntity_nativeStart(JNIEnv *env, jobject thiz, jobject jeventhandler) {
    JAndroidAutoEntity::Pointer jAndroidAutoEntity = JAndroidAutoEntity::getJAndroidAutoEntity(env, thiz);
    jAndroidAutoEntity->start(jeventhandler);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_AndroidAutoEntity_nativeDelete(JNIEnv *env, jobject thiz) {
    JAndroidAutoEntity::Pointer jAndroidAutoEntity = JAndroidAutoEntity::getJAndroidAutoEntity(env, thiz);
    delete jAndroidAutoEntity;
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_service_AndroidAutoEntity_nativeRequestShutdown(JNIEnv *env, jobject thiz) {
    JAndroidAutoEntity::Pointer jAndroidAutoEntity = JAndroidAutoEntity::getJAndroidAutoEntity(env, thiz);
    jAndroidAutoEntity->requestShutdown();
}