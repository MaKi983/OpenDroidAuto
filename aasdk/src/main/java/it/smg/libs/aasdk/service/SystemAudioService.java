package it.smg.libs.aasdk.service;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IAudioOutput;

public class SystemAudioService extends AudioService {

    private static final String TAG = "SystemAudioService";

    public SystemAudioService(Messenger messenger, IAudioOutput audioOutput) {
        super(audioOutput);

        handle_ = nativeSetup(messenger, audioOutput);
    }

    @Override
    protected String tag() {
        return TAG;
    }

    static {
        nativeInit();
    }

    protected native long nativeSetup(Messenger messenger, IAudioOutput audioOutput);
    private static native void nativeInit();
    protected native void nativeStart();
    protected native void nativeStop();
    protected native void nativeDelete();

    protected long handle_;
}
