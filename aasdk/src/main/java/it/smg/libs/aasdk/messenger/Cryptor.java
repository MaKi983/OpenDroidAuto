package it.smg.libs.aasdk.messenger;

public class Cryptor {
    private static final String TAG = "Cryptor";

    public Cryptor(){
        handle_ = nativeSetup();
    }

    public void delete(){
        nativeDelete();
        handle_ = 0;
    }

    // Native context
    static {
        nativeInit();
    }

    private static native void nativeInit();
    private native long nativeSetup();
    private native void nativeDelete();

    private long handle_;
}
