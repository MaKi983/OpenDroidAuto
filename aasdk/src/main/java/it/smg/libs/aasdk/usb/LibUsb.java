package it.smg.libs.aasdk.usb;

import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;

import androidx.annotation.Keep;

public class LibUsb {

    static {
        nativeInit();
    }

    /** enum libusb_endpoint_direction **/
    /** Out: host-to-device */
    public static final int ENDPOINT_OUT = 0x00;
    /** In: device-to-host */
    public static final int ENDPOINT_IN = 0x80;

    public static final int ENDPOINT_DIR_MASK = 0x80;

    /****** libusb Error Codes, must match enum libusb_error in libusb.h ***/
    /** Success (no error) */
    public static final int SUCCESS = 0;
    /** Input/output error */
    public static final int ERROR_IO = -1;
    /** Invalid parameter */
    public static final int ERROR_INVALID_PARAM = -2;
    /** Access denied (insufficient permissions) */
    public static final int ERROR_ACCESS = -3;
    /** No such device (it may have been disconnected) */
    public static final int ERROR_NO_DEVICE = -4;
    /** Entity not found */
    public static final int ERROR_NOT_FOUND = -5;
    /** Resource busy */
    public static final int ERROR_BUSY = -6;
    /** Operation timed out */
    public static final int ERROR_TIMEOUT = -7;
    /** Overflow */
    public static final int ERROR_OVERFLOW = -8;
    /** Pipe error */
    public static final int ERROR_PIPE = -9;
    /** System call interrupted (perhaps due to signal) */
    public static final int ERROR_INTERRUPTED = -10;
    /** Insufficient memory */
    public static final int ERROR_NO_MEM = -11;
    /** Operation not supported or unimplemented on this platform */
    public static final int ERROR_NOT_SUPPORTED = -12;
    /** Other error */
    public static final int ERROR_OTHER = -99;
    /***********************************************************************/

    private static LibUsb instance_;

    @Keep
    private long handle_;

    private static native void nativeInit();
    private native long nativeSetup();

    public static void init(){
        instance_ = new LibUsb();
    }

    public static LibUsb instance() {
        return instance_;
    }

    private LibUsb(){
        handle_ = nativeSetup();
    }

    public LibUsbDevice createDevice(UsbDevice device, UsbManager usbManager){
        return new LibUsbDevice(this, device, usbManager);
    }
}
