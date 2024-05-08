package it.smg.libs.aasdk.messenger;

import it.smg.libs.aasdk.transport.TCPTransport;
import it.smg.libs.aasdk.transport.USBTransport;

public class Messenger {
    private static final String TAG = "Messenger";

    public Messenger(TCPTransport transport, Cryptor cryptor){
        handle_ = nativeSetupTCP(transport, cryptor);
    }

    public Messenger(USBTransport transport, Cryptor cryptor){
        handle_ = nativeSetupUSB(transport, cryptor);
    }

    public void stop(){
        nativeStop();
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
    private native long nativeSetupTCP(TCPTransport transport, Cryptor cryptor);
    private native long nativeSetupUSB(USBTransport transport, Cryptor cryptor);
    private native void nativeStop();
    private native void nativeDelete();

    private long handle_;

}
