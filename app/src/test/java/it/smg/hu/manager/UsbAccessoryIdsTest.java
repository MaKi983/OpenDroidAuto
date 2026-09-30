package it.smg.hu.manager;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Test;

public class UsbAccessoryIdsTest {
    @Test
    public void acceptsAllSupportedAoapV1AndV2Products() {
        assertTrue(UsbAccessoryIds.isAoap(0x18D1, 0x2D00));
        assertTrue(UsbAccessoryIds.isAoap(0x18D1, 0x2D01));
        assertTrue(UsbAccessoryIds.isAoap(0x18D1, 0x2D04));
        assertTrue(UsbAccessoryIds.isAoap(0x18D1, 0x2D05));
    }

    @Test
    public void rejectsOtherVendorsAndGoogleProducts() {
        assertFalse(UsbAccessoryIds.isAoap(0x18D1, 0x4EE7));
        assertFalse(UsbAccessoryIds.isAoap(0x2717, 0x2D00));
    }

    @Test
    public void skipsAoapNegotiationForStorageAndHubs() {
        assertFalse(UsbAccessoryIds.maybeSupportsAoap(new int[]{0x08}));
        assertFalse(UsbAccessoryIds.maybeSupportsAoap(new int[]{0x09}));
        assertFalse(UsbAccessoryIds.maybeSupportsAoap(new int[]{0x08, 0x08}));
    }

    @Test
    public void attemptsAoapNegotiationForEveryPhoneUsbMode() {
        assertTrue(UsbAccessoryIds.maybeSupportsAoap(new int[]{0xFF}));         // MTP / vendor specific
        assertTrue(UsbAccessoryIds.maybeSupportsAoap(new int[]{0x06}));         // PTP
        assertTrue(UsbAccessoryIds.maybeSupportsAoap(new int[]{0x02, 0x0A}));   // USB tethering (RNDIS/CDC)
        assertTrue(UsbAccessoryIds.maybeSupportsAoap(new int[]{0x01}));         // USB audio
        assertTrue(UsbAccessoryIds.maybeSupportsAoap(new int[]{0x03}));         // HID
        assertTrue(UsbAccessoryIds.maybeSupportsAoap(new int[]{0x00}));         // per-interface class
    }

    @Test
    public void attemptsAoapNegotiationWhenDeviceExposesStorageAlongsideOtherFunctions() {
        // A phone offering mass storage plus MTP must still be negotiated with.
        assertTrue(UsbAccessoryIds.maybeSupportsAoap(new int[]{0x08, 0xFF}));
    }

    @Test
    public void attemptsAoapNegotiationWhenInterfacesAreUnknown() {
        assertTrue(UsbAccessoryIds.maybeSupportsAoap(new int[0]));
        assertTrue(UsbAccessoryIds.maybeSupportsAoap((int[]) null));
    }
}
