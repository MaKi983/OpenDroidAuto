#include <Log.h>
#include <google/protobuf/stubs/common.h>
#include <boost/asio/signal_set.hpp>
#include <boost/stacktrace.hpp>
#include "it_smg_libs_aasdk_Runtime.h"

jfieldID JRuntime::handleId = nullptr;
JRuntime::Pointer JRuntime::instance_ = nullptr;
//jclass JRuntime::cls_ = NULL;

JRuntime::JRuntime(JNIEnv *env, jobject androidApp) : JNIBase(env, androidApp, "JRuntime"),
                                                      /*work_(ioService_),*/ i(0){

//    jclass cls = env->FindClass("it/smg/libs/aasdk/Runtime");
//    cls_ = (jclass)env->NewGlobalRef(cls);

    if (instance_ != nullptr){
        if(Log::isInfo()) Log_i("JRuntime already instanciated, delete it");
        delete instance_;
    }

//    startIOServiceWorkers();

    instance_ = this;
}

JRuntime::Pointer JRuntime::getJRuntime(JNIEnv *env, jobject jruntime) {
    return (JRuntime::Pointer)env->GetLongField(jruntime, JRuntime::handleId);;
}

//void JRuntime::initJavaExecptionHandler(JNIEnv* env) {
//    jmethodID exceptionHandlerID  = env->GetStaticMethodID(cls_,  "initExceptionHanlder", "()V");
//    env->CallStaticVoidMethod(cls_, exceptionHandlerID);
//}

JRuntime::~JRuntime() {
//    stopIOServiceWorkers();
//    getJniEnv()->DeleteGlobalRef(cls_);
    if(Log::isDebug()) Log_d("all done");
    instance_ = nullptr;
}

void JRuntime::stopIOServiceWorkers() {
    if(Log::isDebug()) Log_d("stopping IOService Workers");
    i = 0;

    if (!ioService_.stopped()) {
        if(Log::isDebug()) Log_d("stop ioService");
        work_.reset();
        ioService_.stop();
        ioService_.reset();
    }

    if(Log::isDebug()) Log_d("stop all threads");
    auto iter = threadPool_.begin();
    while (iter != threadPool_.end()){
        iter->detach();
        if (iter->joinable()){
            if(Log::isDebug()) Log_d("join thread");
            iter->join();
        }
        threadPool_.erase(iter);
    }

    if(Log::isDebug()) Log_d("clear threadpool");
    threadPool_.clear();

    std::this_thread::sleep_for (std::chrono::seconds(1));

//    ioService_.reset();
}

void JRuntime::startIOServiceWorkers(int threads) {
    if(Log::isDebug()) Log_d("starting IOService Workers");

    work_ = std::make_unique<boost::asio::io_service::work>(ioService_);

    auto ioServiceWorker = [this]() {
        // Attach jni thread
        JNIEnv* env;
        JNIBase::javaAttachThread("IOService Worker", &env);

//        initJavaExecptionHandler(env);

        if(Log::isVerbose()) Log_v("start ioService");
        ioService_.run();
        if(Log::isVerbose()) Log_v("ioService stopped");
        // Detach thread
        JNIBase::javaDetachThread();

        if(Log::isInfo()) Log_i("ioservice worker thread complete");
    };

    if(Log::isInfo()) Log_i("Starting %d threads", threads);

    for (int i = 1; i <= threads; i++) {
        threadPool_.emplace_back(ioServiceWorker);
    }
    std::this_thread::sleep_for (std::chrono::seconds(1));
    if(Log::isInfo()) Log_i("all ioservice thread started");
}

aasdk::io::ioService &JRuntime::ioService() {
    return instance_->ioService_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_Runtime_nativeInit(JNIEnv *env, jclass clazz) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    jclass _class = env->FindClass("it/smg/libs/aasdk/Runtime");
    JRuntime::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;

//    //env->GetJavaVM(&JNIBase::javaVM);
//    auto jruntime = new JRuntime(env, clazz);
//    return (jlong)((size_t)jruntime);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_Runtime_nativeSetup(JNIEnv *env, jobject thiz) {
    auto jruntime = new JRuntime(env, thiz);
    return (jlong)((size_t)jruntime);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_Runtime_nativeDelete(JNIEnv *env, jobject thiz) {
    auto jRuntime = JRuntime::getJRuntime(env, thiz);
    delete jRuntime;
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_Runtime_nativeStartIOServiceWorker(JNIEnv *env, jobject thiz, jint threads) {
    auto jRuntime = JRuntime::getJRuntime(env, thiz);
    jRuntime->startIOServiceWorkers(threads);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_Runtime_nativeStopIOServiceWorker(JNIEnv *env, jobject thiz) {
    auto jRuntime = JRuntime::getJRuntime(env, thiz);
    jRuntime->stopIOServiceWorkers();
}

//extern "C"
//JNIEXPORT void JNICALL
//Java_it_smg_libs_aasdk_Runtime_nativeCrash(JNIEnv *env, jclass clazz) {
//    abort();
//}
