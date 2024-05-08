package it.smg.libs.aasdk.service;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IInputDevice;

public class InputService implements IService {

    private static final String TAG = "InputService";

    private IInputDevice inputDevice_;

    public InputService(Messenger messenger, IInputDevice inputDevice) {
        handle_ = nativeSetup(messenger, inputDevice);

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

    protected long handle_;
}
