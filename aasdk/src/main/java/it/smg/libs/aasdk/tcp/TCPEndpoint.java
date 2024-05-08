package it.smg.libs.aasdk.tcp;

public class TCPEndpoint {

    private static final String TAG = "TCPEndpoint";

    public TCPEndpoint(String ipAddress){
        handle_ = nativeSetup(ipAddress);
    }

    public void delete(){
        nativeDelete();
    }

    static {
        nativeInit();
    }

    private static native void nativeInit();
    private native long nativeSetup(String ipAddress);
    private native void nativeDelete();

    private long handle_;
}