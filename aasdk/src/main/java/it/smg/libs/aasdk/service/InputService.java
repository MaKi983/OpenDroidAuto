package it.smg.libs.aasdk.service;

import androidx.annotation.Keep;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IInputDevice;
import it.smg.libs.common.Log;

public class InputService implements IService {

    private static final String TAG = "InputService";

    private IInputDevice inputDevice_;
    private final IAndroidAutoEntityEventHandler eventHandler_;

    public InputService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler, IInputDevice inputDevice) {
        handle_ = nativeSetup(messenger, inputDevice);
        eventHandler_ = eventHandler;
        inputDevice_ = inputDevice;
    }

    @Override
    public void start() {
        nativeStart();
    }

    @Override
    public void stop() {
        nativeStop();
    }

    @Override
    public void delete() {
        nativeDelete();
        inputDevice_.delete();
        inputDevice_ = null;
    }

    @Override
    public void onError(String error, int code){
        Log.v(TAG, "onError " + error + "/" + code);
        eventHandler_.onAndroidAutoQuitOnError(error, code);
    }

    public void releaseFocus(){
        inputDevice_.releaseFocus();
    }

    public void gainFocus(){
        inputDevice_.gainFocus();
    }

    // Native context
    static {
        nativeInit();
    }

    private static native void nativeInit();
    private native long nativeSetup(Messenger messenger, IInputDevice inputDevice);
    private native void nativeStart();
    private native void nativeStop();
    private native void nativeDelete();

    @Keep
    protected long handle_;
}
