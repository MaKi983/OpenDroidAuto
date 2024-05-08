package it.smg.libs.aasdk.transport;

import it.smg.libs.aasdk.tcp.TCPEndpoint;

public class TCPTransport extends Transport {

    private TCPEndpoint tcpEndpoint_;

    public TCPTransport(TCPEndpoint tcpEndpoint){
        handle_ = nativeSetup(tcpEndpoint);
        tcpEndpoint_ = tcpEndpoint;
    }

    @Override
    public void delete() {
        tcpEndpoint_.delete();
        nativeDelete();
        handle_ = 0;
    }

    @Override
    public void stop() {
        nativeStop();
    }

    static {
        nativeInit();
    }
    private static native void nativeInit();

    private native long nativeSetup(TCPEndpoint tcpEndpoint);
    private native void nativeStop();
    private native void nativeDelete();

    private long handle_;
}
