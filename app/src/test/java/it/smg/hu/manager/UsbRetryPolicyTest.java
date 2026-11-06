package it.smg.hu.manager;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

public class UsbRetryPolicyTest {
    @Test
    public void negotiationRetriesAreShortAndBounded() {
        assertEquals(3, UsbRetryPolicy.NEGOTIATION_ATTEMPTS);
        assertEquals(250L, UsbRetryPolicy.negotiationDelayMs(0));
        assertEquals(600L, UsbRetryPolicy.negotiationDelayMs(1));
    }

    @Test
    public void reenumerationWatchdogUsesBoundedBackoff() {
        long totalDelay = 0L;
        long previousDelay = 0L;
        for (int attempt = 0; attempt < UsbRetryPolicy.REENUMERATION_ATTEMPTS; attempt++) {
            long delay = UsbRetryPolicy.reenumerationDelayMs(attempt);
            assertTrue(delay >= previousDelay);
            assertTrue(delay <= 1350L);
            previousDelay = delay;
            totalDelay += delay;
        }
        assertTrue(totalDelay < 12000L);
    }
}
