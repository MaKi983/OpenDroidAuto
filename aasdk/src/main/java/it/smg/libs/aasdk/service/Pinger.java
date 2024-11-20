package it.smg.libs.aasdk.service;

import androidx.annotation.Keep;

public class Pinger implements IPinger {

    private static final String TAG = "SensorService";

    public Pinger(int duration) {
        handle_ = nativeSetup(duration);
    }

    @Override
    public native void cancel();

    @Override
    public void delete() {
        nativeDelete();
    }

    static {
        nativeInit();
    }

    private static native void nativeInit();

    private native long nativeSetup(int duration);
    private native void nativeDelete();

    @Keep
    protected long handle_;

}
