#include <unistd.h>
#include "Backtrace.h"
#include "Log.h"

void Backtrace::init(){
    setUpAltStack();
    setUpSigActionHandler();

    if (Log::isInfo()) Log_i("initialized");
}

void Backtrace::setUpAltStack() {
    // Set up an alternate signal handler stack.
    stack_t stack = {};
    stack.ss_size = 0;
    stack.ss_flags = 0;
    stack.ss_size = SIGSTKSZ;
    stack.ss_sp = malloc(stack.ss_size);
    assert(stack.ss_sp);

    sigaltstack(&stack, NULL);
}

void Backtrace::setUpSigActionHandler() {
    // Set up signal handler.
    struct sigaction action = {};
    memset(&action, 0, sizeof(action));
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = sigActionHandler;
    action.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;

    sigaction(SIGSEGV, &action, NULL);
    sigaction(SIGILL, &action, NULL);
    sigaction(SIGBUS, &action, NULL);
}

void Backtrace::sigActionHandler(int sig, siginfo_t* info, void* ucontext) {
    const ucontext_t* signal_ucontext = (const ucontext_t*)ucontext;
    assert(signal_ucontext);

    Log_e("******** Backtrace (%s) pid: %d, uid: %d ********", sys_signame[sig], info->_sifields._kill._pid, info->_sifields._kill._uid);
    BacktraceState backtrace_state;
    initState(&backtrace_state, signal_ucontext);
    LibunwindWithRegisters(&backtrace_state);
    printBacktrace(&backtrace_state);

//    raise(SIGUSR1);
    exit(0);
}

void Backtrace::printBacktrace(BacktraceState* state) {
    assert(state);

    size_t frame_count = state->address_count;
    for (size_t frame_index = 0; frame_index < frame_count; ++frame_index) {

        void* address = (void*)(state->addresses[frame_index]);
        assert(address);

        const char* symbol_name = "";

        Dl_info info = {};
        if (dladdr(address, &info) && info.dli_sname) {
            symbol_name = info.dli_sname;
        }

        // Relative address matches the address which "nm" and "objdump"
        // utilities give you, if you compiled position-independent code
        // (-fPIC, -pie).
        // Android requires position-independent code since Android 5.0.
        unsigned long relative_address = (char*)address - (char*)info.dli_fbase;

        char* demangled = NULL;

        int status = 0;
        demangled = __cxxabiv1::__cxa_demangle(symbol_name, NULL, NULL, &status);
        if (demangled)
            symbol_name = demangled;

        assert(symbol_name);
        Log_e("  #%02zu:  0x%lx  %s\n", frame_index, relative_address, symbol_name);

        free(demangled);
    }
}

void Backtrace::initState(BacktraceState *state, const ucontext_t *ucontext) {
    assert(state);
    assert(ucontext);
    memset(state, 0, sizeof(BacktraceState));
    state->signal_ucontext = ucontext;
    state->address_skip_count = 3;
}

bool Backtrace::addAddress(BacktraceState *state, uintptr_t ip) {
    assert(state);

    // No more space in the storage. Fail.
    if (state->address_count >= address_count_max)
        return false;

#if __thumb__
    // Reset the Thumb bit, if it is set.
    const uintptr_t thumb_bit = 1;
    ip &= ~thumb_bit;
#endif

    if (state->address_count > 0) {
        // Ignore null addresses.
        // They sometimes happen when using _Unwind_Backtrace()
        // with the compiler optimizations,
        // when the Link Register is overwritten by the inner
        // stack frames, like PreCrash() functions in this example.
        if (ip == 0)
            return true;

        // Ignore duplicate addresses.
        // They sometimes happen when using _Unwind_Backtrace()
        // with the compiler optimizations,
        // because we both add the second address from the Link Register
        // in ProcessRegisters() and receive the same address
        // in UnwindBacktraceCallback().
        if (ip == state->addresses[state->address_count - 1])
            return true;
    }

    // Finally add the address to the storage.
    state->addresses[state->address_count++] = ip;
    return true;
}

void Backtrace::LibunwindWithRegisters(BacktraceState *state) {
    assert(state);

    // Initialize unw_context and unw_cursor.
    unw_context_t unw_context = {};
    unw_getcontext(&unw_context);
    unw_cursor_t  unw_cursor = {};
    unw_init_local(&unw_cursor, &unw_context);

    // Get more contexts.
    const ucontext_t* signal_ucontext = state->signal_ucontext;
    assert(signal_ucontext);
    const struct sigcontext* signal_mcontext = &(signal_ucontext->uc_mcontext);
    assert(signal_mcontext);

    // Set registers.
    unw_set_reg(&unw_cursor, UNW_ARM_R0,  signal_mcontext->arm_r0);
    unw_set_reg(&unw_cursor, UNW_ARM_R1,  signal_mcontext->arm_r1);
    unw_set_reg(&unw_cursor, UNW_ARM_R2,  signal_mcontext->arm_r2);
    unw_set_reg(&unw_cursor, UNW_ARM_R3,  signal_mcontext->arm_r3);
    unw_set_reg(&unw_cursor, UNW_ARM_R4,  signal_mcontext->arm_r4);
    unw_set_reg(&unw_cursor, UNW_ARM_R5,  signal_mcontext->arm_r5);
    unw_set_reg(&unw_cursor, UNW_ARM_R6,  signal_mcontext->arm_r6);
    unw_set_reg(&unw_cursor, UNW_ARM_R7,  signal_mcontext->arm_r7);
    unw_set_reg(&unw_cursor, UNW_ARM_R8,  signal_mcontext->arm_r8);
    unw_set_reg(&unw_cursor, UNW_ARM_R9,  signal_mcontext->arm_r9);
    unw_set_reg(&unw_cursor, UNW_ARM_R10, signal_mcontext->arm_r10);
    unw_set_reg(&unw_cursor, UNW_ARM_R11, signal_mcontext->arm_fp);
    unw_set_reg(&unw_cursor, UNW_ARM_R12, signal_mcontext->arm_ip);
    unw_set_reg(&unw_cursor, UNW_ARM_R13, signal_mcontext->arm_sp);
    unw_set_reg(&unw_cursor, UNW_ARM_R14, signal_mcontext->arm_lr);
    unw_set_reg(&unw_cursor, UNW_ARM_R15, signal_mcontext->arm_pc);

    unw_set_reg(&unw_cursor, UNW_REG_IP,  signal_mcontext->arm_pc);
    unw_set_reg(&unw_cursor, UNW_REG_SP,  signal_mcontext->arm_sp);

    // unw_step() does not return the first IP,
    // the address of the instruction which caused the crash.
    // Thus let's add this address manually.
    addAddress(state, signal_mcontext->arm_pc);

    //printf("unw_is_signal_frame(): %i\n", unw_is_signal_frame(&unw_cursor));

    // Unwind frames one by one, going up the frame stack.
    while (unw_step(&unw_cursor) > 0) {
        unw_word_t ip = 0;
        unw_get_reg(&unw_cursor, UNW_REG_IP, &ip);

        bool ok = addAddress(state, ip);
        if (!ok)
            break;

        //printf("unw_is_signal_frame(): %i\n", unw_is_signal_frame(&unw_cursor));
    }
}
