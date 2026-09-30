package it.smg.hu.manager;

/** Bounded timings for legacy USB negotiation and AOAP re-enumeration. */
final class UsbRetryPolicy {
    static final int NEGOTIATION_ATTEMPTS = 3;
    static final int REENUMERATION_ATTEMPTS = 9;

    private UsbRetryPolicy() {}

    static long negotiationDelayMs(int failedAttempt) {
        return failedAttempt <= 0 ? 250L : 600L;
    }

    static long reenumerationDelayMs(int scanAttempt) {
        return 350L + Math.min(Math.max(scanAttempt, 0) * 250L, 1000L);
    }
}
