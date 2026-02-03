package it.smg.libs.aasdk.projection;

import androidx.annotation.Keep;

public abstract class InputDevice implements IInputDevice {
    private static final String TAG = "InputDevice";

    public InputDevice(){
        handle_ = nativeSetup();
    }

    static {
        nativeInit();
    }

    @Override
    public void delete() {
        nativeDelete();
    }

    protected native void sendTouchEvent(int action, int index, TouchAction[] actions);
    protected native void sendButtonEvent(int action, int code);
    private native long nativeSetup();
    private native void nativeDelete();
    private static native void nativeInit();

    @Keep
    private long handle_;
}
