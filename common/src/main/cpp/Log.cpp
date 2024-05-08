#include "Log.h"
#include <sys/types.h>
#include <cstdio>
#include <pthread.h>
#include <cstdarg>
#include <sys/prctl.h>
#include <jni.h>

bool Log::logProtocol_ = false;
int Log::logLevel_ = ANDROID_LOG_INFO;

void Log::init(int logLevel, bool logProtocol) {
    logLevel_ = logLevel;
    logProtocol_ = logProtocol;

    if(isVerbose()) Log_v("Log: logLevel %d, logProtocol %s", logLevel_, logProtocol_ ? "true" : "false");
}

void Log::print(int prio, const char * tag, const char * fmt, ...) {
    if (prio < logLevel_){
        return;
    }

    va_list ap;
    va_start (ap, fmt);

    char tag_str [512] = {0};
    snprintf (tag_str, sizeof (tag_str), "ODA/%s", tag);
    __android_log_vprint (prio, tag_str, fmt, ap);

    va_end(ap);
}

void Log::print_splitted(int prio, std::vector<char> tag, std::vector<char> msg, size_t length){
    int chunks = length / 4000;
    for (int i = 0; i <= chunks; i++){
        int max = 4000 * (i + 1);
        std::vector<char> msg_chunk;
        if (max >= length){
            msg_chunk = {msg.begin() + (4000*i), msg.end()};
        } else {
            msg_chunk = {msg.begin() + (4000*i), msg.begin() + max};
        }
        std::string msg_str = std::to_string(i) + "/" + std::to_string(chunks) + ": " + msg_chunk.data();
        __android_log_write(prio, tag.data(), msg_str.c_str());
    }
}

bool Log::isVerbose() {
    return logLevel_ <= ANDROID_LOG_VERBOSE;
}

bool Log::isDebug() {
    return logLevel_ <= ANDROID_LOG_DEBUG;
}

bool Log::isInfo() {
    return logLevel_ <= ANDROID_LOG_INFO;
}

bool Log::isWarn() {
    return logLevel_ <= ANDROID_LOG_WARN;
}

bool Log::isError() {
    return logLevel_ <= ANDROID_LOG_ERROR;
}

bool Log::logProtocol() {
    return logProtocol_;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_common_Log_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/common/Log");

    jmethodID logLevelMethodId = env->GetStaticMethodID(_class, "logLevel", "()I");
    jmethodID logProtocolMethodId = env->GetStaticMethodID(_class, "logProtocol", "()Z");

    jint logLevel = env->CallStaticIntMethod(_class, logLevelMethodId);
    jboolean logProtocol = env->CallStaticBooleanMethod(_class, logProtocolMethodId);

    Log::init(logLevel, logProtocol == JNI_TRUE);

    env->DeleteLocalRef(_class);
    _class = nullptr;
}