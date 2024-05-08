package it.smg.libs.aasdk.service;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IAudioInput;

public class AudioInputService implements IService {

    private static final String TAG = "AudioInputService";

    private IAudioInput audioInput_;

    public AudioInputService(Messenger messenger, IAudioInput audioInput) {
        handle_ = nativeSetup(messenger, audioInput);
        audioInput_ = audioInput;
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
        audioInput_.delete();
        audioInput_ = null;
    }

    static {
        nativeInit();
    }

    private static native void nativeInit();
    private native long nativeSetup(Messenger messenger, IAudioInput audioInput);
    private native void nativeStart();
    private native void nativeStop();
    private native void nativeDelete();

    protected long handle_;
}
