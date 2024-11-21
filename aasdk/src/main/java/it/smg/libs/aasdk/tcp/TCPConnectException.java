package it.smg.libs.aasdk.tcp;

import androidx.annotation.Keep;

@Keep
public class TCPConnectException extends Exception {
    public TCPConnectException(String msg){
        super(msg);
    }
}
