#include "JNIBase.h"
#include "Log.h"
#include <android_runtime/AndroidRuntime.h>
#include <thread>
#include <sstream>
#include <utility>

JNIBase::JNIBase(JNIEnv* env, jobject androidApp, std::string name): name_(std::move(name)) {
    androidClass_ = env->NewGlobalRef(androidApp);
    if(Log::isVerbose()) Log_v("(%s) created global ref %p", name_.c_str(), androidClass_);
}

JNIBase::~JNIBase(){
    if(Log::isVerbose()) Log_v("(%s) delete global ref %p", name_.c_str(), androidClass_);
    JNIEnv* env = getJniEnv();
    if (env == NULL){
        if(Log::isWarn()) Log_w("(%s) env null skip DeleteGlobalRef");
    } else {
        getJniEnv()->DeleteGlobalRef(androidClass_);
    }
};

int JNIBase::javaAttachThread(const char* threadName, JNIEnv** pEnv)
{
    JavaVMAttachArgs args;
    JavaVM* vm;
    jint result;

    vm = android::AndroidRuntime::getJavaVM();
    assert(vm != NULL);

    args.version = JNI_VERSION_1_4;
    args.name = (char*) threadName;
    args.group = NULL;

    result = vm->AttachCurrentThread(pEnv, (void*) &args);
    if (result != JNI_OK) {
        if (Log::isWarn()) Log_w("NOTE: attach of thread '%s' failed", threadName);
    } else {
        if(Log::isInfo()) Log_i("attached thread '%s'", threadName);
    }

    return result;
}

int JNIBase::javaDetachThread(void)
{
    JavaVM* vm;
    jint result;

    vm = android::AndroidRuntime::getJavaVM();
    assert(vm != NULL);

    result = vm->DetachCurrentThread();
    if (result != JNI_OK) {
        if (Log::isError()) Log_e("thread detach failed");
    } else {
        if(Log::isInfo()) Log_i("detached thread");
    }

    return result;
}

JNIEnv* JNIBase::getJniEnv () {
    return android::AndroidRuntime::getJNIEnv();
}

void JNIBase::initJavaMethods() {}