#pragma once

#include <android/log.h>
#include <string>
#include <vector>

#define __FILENAME__ strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__

#define  Log_v(...)  Log::print(ANDROID_LOG_VERBOSE, __FILENAME__ ,__VA_ARGS__)
#define  Log_d(...)  Log::print(ANDROID_LOG_DEBUG, __FILENAME__ ,__VA_ARGS__)
#define  Log_i(...)  Log::print(ANDROID_LOG_INFO, __FILENAME__ ,__VA_ARGS__)
#define  Log_w(...)  Log::print(ANDROID_LOG_WARN, __FILENAME__ ,__VA_ARGS__)
#define  Log_e(...)  Log::print(ANDROID_LOG_ERROR, __FILENAME__ ,__VA_ARGS__)

class Log {
public:
    static bool isVerbose();
    static bool isDebug();
    static bool isInfo();
    static bool isWarn();
    static bool isError();
    static void print (int prio, const char * tag, const char * fmt, ...);
    static void print_splitted(int prio, std::vector<char> tag, std::vector<char> msg, size_t length);
    static bool logProtocol();
    static void init(int logLevel, bool logProtocol);
private:
    static bool logProtocol_;
    static int logLevel_;
};
