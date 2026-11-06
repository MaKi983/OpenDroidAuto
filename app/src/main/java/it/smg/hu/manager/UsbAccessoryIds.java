package it.smg.hu.manager;

import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;

/** Android Open Accessory identifiers accepted by the legacy head unit. */
final class UsbAccessoryIds {
    static final int GOOGLE_VENDOR_ID = 0x18D1;
    static final int ACCESSORY = 0x2D00;
    static final int ACCESSORY_ADB = 0x2D01;
    static final int ACCESSORY_AUDIO = 0x2D04;
    static final int ACCESSORY_AUDIO_ADB = 0x2D05;

    private UsbAccessoryIds() {}

    static boolean isAoap(int vendorId, int productId) {
        if (vendorId != GOOGLE_VENDOR_ID) {
            return false;
        }
        return productId == ACCESSORY || productId == ACCESSORY_ADB
                || productId == ACCESSORY_AUDIO || productId == ACCESSORY_AUDIO_ADB;
    }

    /**
     * Whether it is worth asking this device to switch into accessory mode.
     *
     * The head unit's own USB port is shared with mass storage: the log volume,
     * media sticks and card readers all show up here. Sending the AOAP
     * GET_PROTOCOL control request to those always fails, and because every
     * attach/recover pass retried it the failures surfaced to the user as a
     * stream of USB connection errors. Devices that only expose interfaces a
     * phone never uses are skipped instead.
     */
    static boolean maybeSupportsAoap(UsbDevice device) {
        if (device == null) {
            return false;
        }

        int interfaceCount = device.getInterfaceCount();
        int[] interfaceClasses = new int[interfaceCount];
        for (int i = 0; i < interfaceCount; i++) {
            interfaceClasses[i] = device.getInterface(i).getInterfaceClass();
        }
        return maybeSupportsAoap(interfaceClasses);
    }

    /** @see #maybeSupportsAoap(UsbDevice) */
    static boolean maybeSupportsAoap(int[] interfaceClasses) {
        if (interfaceClasses == null || interfaceClasses.length == 0) {
            // Nothing to judge by - let the negotiation decide.
            return true;
        }

        for (int interfaceClass : interfaceClasses) {
            if (!isNonPhoneInterface(interfaceClass)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Deliberately a very short list. A phone can legitimately appear as almost
     * anything depending on its USB mode - MTP and MIDI look vendor-specific,
     * PTP is still-image class, and USB tethering shows up as CDC/RNDIS - so
     * excluding those classes would risk ignoring a real phone. Only classes a
     * phone never presents on its own are listed here.
     */
    private static boolean isNonPhoneInterface(int interfaceClass) {
        return interfaceClass == UsbConstants.USB_CLASS_MASS_STORAGE
                || interfaceClass == UsbConstants.USB_CLASS_HUB
                || interfaceClass == UsbConstants.USB_CLASS_PRINTER;
    }
}
