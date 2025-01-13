#pragma once

#include "libunwind.h"
#include "cxxabi.h"
#include <unwind.h>
#include <cassert>
#include <dlfcn.h>
#include <csignal>
#include <cstdlib>
#include <cstring>

// https://stackoverflow.com/questions/8115192/android-ndk-getting-the-backtrace
// https://github.com/alexeikh/android-ndk-backtrace-test

//char* __cxa_demangle(
//        const char* mangled_name,
//        char* output_buffer,
//        size_t* length,
//        int* status);

static const size_t address_count_max = 30;

struct BacktraceState {
    // On ARM32 architecture this context is needed
    // for getting backtrace of the before-crash stack,
    // not of the signal handler stack.
    const ucontext_t*   signal_ucontext;

    // On non-ARM32 architectures signal handler stack
    // seems to be "connected" to the before-crash stack,
    // so we only need to skip several initial addresses.
    size_t              address_skip_count;

    size_t              address_count;
    uintptr_t           addresses[address_count_max];

};
typedef struct BacktraceState BacktraceState;


class Backtrace {

public:
    static void init();

private:
    static void initState(BacktraceState* state, const ucontext_t* ucontext);
    static void setUpAltStack();
    static void setUpSigActionHandler();
    static void sigActionHandler(int sig, siginfo_t* info, void* ucontext);
    static void printBacktrace(BacktraceState* state);
    static bool addAddress(BacktraceState* state, uintptr_t ip);
    static void LibunwindWithRegisters(BacktraceState* state);
};
