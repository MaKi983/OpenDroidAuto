package it.smg.libs.aasdk.tcp;

import androidx.annotation.Keep;

@Keep
public class TCPConnectException extends RuntimeException {
    public TCPConnectException(String msg){
        super(msg);
    }
}
