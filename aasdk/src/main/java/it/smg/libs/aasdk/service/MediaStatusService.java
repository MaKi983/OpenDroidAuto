package it.smg.libs.aasdk.service;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IMediaStatusEvent;
import it.smg.libs.aasdk.projection.INavigationStatusEvent;

public class MediaStatusService implements IService {

    private static final String TAG = "MediaStatusService";

    private IMediaStatusEvent mediaStatusEvent_;

    public MediaStatusService(Messenger messenger, IMediaStatusEvent mediaStatusEvent) {
        handle_ = nativeSetup(messenger, mediaStatusEvent);
        mediaStatusEvent_ = mediaStatusEvent;
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
        mediaStatusEvent_.delete();
        mediaStatusEvent_ = null;
    }

    static {
        nativeInit();
    }

    private static native void nativeInit();
    private native long nativeSetup(Messenger messenger, IMediaStatusEvent navigationEvent);
    private native void nativeStart();
    private native void nativeStop();
    private native void nativeDelete();

    protected long handle_;
}
