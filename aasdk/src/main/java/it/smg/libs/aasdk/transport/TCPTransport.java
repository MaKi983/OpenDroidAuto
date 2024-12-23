package it.smg.libs.aasdk.transport;

import androidx.annotation.Keep;

import it.smg.libs.aasdk.tcp.TCPConnectException;
import it.smg.libs.aasdk.tcp.TCPEndpoint;
import it.smg.libs.common.Log;

public class TCPTransport extends Transport {

    private final TCPEndpoint tcpEndpoint_;

    public TCPTransport(TCPEndpoint tcpEndpoint) throws TCPConnectException {
        tcpEndpoint_ = tcpEndpoint;
        tcpEndpoint_.connect();
        handle_ = nativeSetup(tcpEndpoint_);
    }

    @Override
    public void delete() {
        Log.v("TCPTransport", "delete");
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

    @Keep
    private long handle_;
}
