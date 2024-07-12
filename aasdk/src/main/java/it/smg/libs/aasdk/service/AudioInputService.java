package it.smg.libs.aasdk.service;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IAudioInput;
import it.smg.libs.common.Log;

public class AudioInputService implements IService {

    private static final String TAG = "AudioInputService";

    private IAudioInput audioInput_;
    private final IAndroidAutoEntityEventHandler eventHandler_;

    public AudioInputService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler, IAudioInput audioInput) {
        handle_ = nativeSetup(messenger, audioInput);
        eventHandler_ = eventHandler;
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

    @Override
    public void onError(String error, int code){
        Log.v(TAG, "onError " + error + "/" + code);
        eventHandler_.onAndroidAutoQuitOnError(error, code);
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
