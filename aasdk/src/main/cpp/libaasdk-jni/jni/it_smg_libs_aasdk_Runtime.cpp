#include <Log.h>
#include <google/protobuf/stubs/common.h>
#include <boost/asio/signal_set.hpp>
#include <boost/stacktrace.hpp>
#include "it_smg_libs_aasdk_Runtime.h"

JRuntime::Pointer JRuntime::instance_ = nullptr;
jclass JRuntime::cls_ = NULL;

JRuntime::JRuntime(JNIEnv *env, jobject androidApp) : JNIBase(env, androidApp, "JRuntime"),
                                                      work_(ioService_), i(0){

    jclass cls = env->FindClass("it/smg/libs/aasdk/Runtime");
    cls_ = (jclass)env->NewGlobalRef(cls);

    if (instance_ != nullptr){
        if(Log::isInfo()) Log_i("JRuntime already instanciated, delete it");
        delete instance_;
    }

//    startIOServiceWorkers();

    instance_ = this;
}

void JRuntime::initJavaExecptionHandler(JNIEnv* env) {
    jmethodID exceptionHandlerID  = env->GetStaticMethodID(cls_,  "initExceptionHanlder", "()V");
    env->CallStaticVoidMethod(cls_, exceptionHandlerID);
}

JRuntime::~JRuntime() {
    stopIOServiceWorkers();
    getJniEnv()->DeleteGlobalRef(cls_);
    if(Log::isDebug()) Log_d("all done");
}

void JRuntime::stopIOServiceWorkers() {
    if(Log::isDebug()) Log_d("stopping IOService Workers");
    i = 0;

    if (!ioService_.stopped()) {
        if(Log::isDebug()) Log_d("stop ioService");
        ioService_.stop();
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

    ioService_.reset();
}

void JRuntime::startIOServiceWorkers(int threads) {
    if(Log::isDebug()) Log_d("starting IOService Workers");

    auto ioServiceWorker = [this]() {
        // Attach jni thread
        JNIEnv* env;
        JNIBase::javaAttachThread("IOService Worker", &env);

//        initJavaExecptionHandler(env);

        if(Log::isVerbose()) Log_v("start ioService");
        ioService_.run();

        // Detach thread
        JNIBase::javaDetachThread();

        if(Log::isInfo()) Log_i("ioservice stopped");
    };

    if(Log::isInfo()) Log_i("Starting %d threads", threads);

    for (int i = 1; i <= threads; i++) {
        threadPool_.emplace_back(ioServiceWorker);
    }
    std::this_thread::sleep_for (std::chrono::seconds(1));
    if(Log::isInfo()) Log_i("all ioservice thread started");
}

boost::asio::io_service &JRuntime::ioService() {
    return instance_->ioService_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_Runtime_nativeInit(JNIEnv *env, jclass clazz) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    //env->GetJavaVM(&JNIBase::javaVM);
    auto jruntime = new JRuntime(env, clazz);
    return (jlong)((size_t)jruntime);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_Runtime_nativeFinalize(JNIEnv *env, jclass clazz, jlong handle) {
    auto jRuntime = (JRuntime::Pointer) handle;
    delete jRuntime;
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_Runtime_nativeStartIOServiceWorker(JNIEnv *env, jclass clazz, jlong handle, jint threads) {
    auto jRuntime = (JRuntime::Pointer) handle;
    jRuntime->startIOServiceWorkers(threads);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_Runtime_nativeStopIOServiceWorker(JNIEnv *env, jclass clazz, jlong handle) {
    auto jRuntime = (JRuntime::Pointer) handle;
    jRuntime->stopIOServiceWorkers();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_Runtime_nativeCrash(JNIEnv *env, jclass clazz) {
    abort();
}
