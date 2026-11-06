#include "Log.h"
#include <sys/types.h>
#include <cstdio>
#include <pthread.h>
#include <cstdarg>
#include <sys/prctl.h>
#include <jni.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <ctime>

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
// Native crash capture: a Java UncaughtExceptionHandler never sees a native
// SIGSEGV/SIGABRT/SIGBUS/SIGILL - the process just dies with nothing in logcat.
// This handler writes a minimal marker to a file on external storage (tried in
// order below) before letting the default handler terminate the process, so a
// native crash can be diagnosed without adb access.

static const char *const kCrashLogPaths[] = {
        "/mnt/sdcard/oda_native_crash.log",
        "/sdcard/oda_native_crash.log",
        "/mnt/usbdrive2/logs/oda_native_crash.log",
        "/storage/emulated/0/oda_native_crash.log",
};

static struct sigaction g_prevHandlers[NSIG];

static void writeUnsigned(int fd, unsigned long value) {
    char buf[24];
    int i = sizeof(buf);
    buf[--i] = '\0';
    if (value == 0) {
        buf[--i] = '0';
    }
    while (value > 0 && i > 0) {
        buf[--i] = (char) ('0' + (value % 10));
        value /= 10;
    }
    write(fd, &buf[i], sizeof(buf) - i - 1);
}

static void nativeCrashHandler(int signalNumber, siginfo_t *info, void *context) {
    for (const char *path : kCrashLogPaths) {
        int fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd < 0) {
            continue;
        }
        static const char header[] = "\n=== ODA native crash === signal=";
        write(fd, header, sizeof(header) - 1);
        writeUnsigned(fd, (unsigned long) signalNumber);
        static const char pidTag[] = " pid=";
        write(fd, pidTag, sizeof(pidTag) - 1);
        writeUnsigned(fd, (unsigned long) getpid());
        static const char tidTag[] = " tid=";
        write(fd, tidTag, sizeof(tidTag) - 1);
        writeUnsigned(fd, (unsigned long) gettid());
        if (info != nullptr) {
            static const char addrTag[] = " fault_addr=";
            write(fd, addrTag, sizeof(addrTag) - 1);
            writeUnsigned(fd, (unsigned long) info->si_addr);
        }
        write(fd, "\n", 1);
        close(fd);
        break;
    }

    __android_log_write(ANDROID_LOG_ERROR, "ODA/NativeCrash", "native crash captured, re-raising signal");

    // Chain to whatever handler was previously installed (Android's own crash
    // handler writes the tombstone/logcat "Fatal signal" line) so we don't
    // suppress the normal debugging path, only add to it.
    if (signalNumber >= 0 && signalNumber < NSIG && g_prevHandlers[signalNumber].sa_sigaction != nullptr) {
        sigaction(signalNumber, &g_prevHandlers[signalNumber], nullptr);
    }
    raise(signalNumber);
}

static void installNativeCrashHandler() {
    static const int kSignals[] = {SIGSEGV, SIGABRT, SIGBUS, SIGILL, SIGFPE};

    struct sigaction action{};
    action.sa_sigaction = nativeCrashHandler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);

    for (int sig : kSignals) {
        sigaction(sig, &action, &g_prevHandlers[sig]);
    }
}

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    installNativeCrashHandler();
    return JNI_VERSION_1_6;
}

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