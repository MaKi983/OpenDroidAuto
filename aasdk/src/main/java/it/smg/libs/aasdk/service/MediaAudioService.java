package it.smg.libs.aasdk.service;

import it.smg.libs.aasdk.projection.IAudioOutput;
import it.smg.libs.aasdk.messenger.Messenger;

public class MediaAudioService extends AudioService {

    private static final String TAG = "MediaAudioService";

    public MediaAudioService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler, IAudioOutput audioOutput) {
        super(audioOutput, eventHandler);

        handle_ = nativeSetup(messenger, audioOutput);
    }

    @Override
    protected String tag() {
        return TAG;
    }

    static {
        nativeInit();
    }

    private static native void nativeInit();
    protected native long nativeSetup(Messenger messenger, IAudioOutput audioOutput);
    protected native void nativeStart();
    protected native void nativeStop();
    protected native void nativeDelete();

    protected long handle_;
}
