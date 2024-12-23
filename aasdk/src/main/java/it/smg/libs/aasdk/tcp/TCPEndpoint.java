package it.smg.libs.aasdk.tcp;

import androidx.annotation.Keep;

import it.smg.libs.common.Log;

public class TCPEndpoint {

    private static final String TAG = "TCPEndpoint";

    private final String ipAddress_;

    public TCPEndpoint(String ipAddress) throws TCPConnectException{
        ipAddress_ = ipAddress;
    }

    public void connect() throws TCPConnectException {
        if (Log.isInfo()) Log.i(TAG, "Connect to ip " + ipAddress_);
        handle_ = nativeSetup(ipAddress_);
    }

    public void delete(){
        Log.v(TAG, "delete");
        nativeDelete();
    }

    static {
        nativeInit();
    }

    private static native void nativeInit();
    private native long nativeSetup(String ipAddress) throws TCPConnectException;
    private native void nativeDelete();

    @Keep
    private long handle_;
}