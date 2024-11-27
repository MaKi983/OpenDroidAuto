package it.smg.libs.aasdk.service;

import androidx.annotation.Keep;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IMediaStatusEvent;
import it.smg.libs.aasdk.projection.INavigationStatusEvent;
import it.smg.libs.common.Log;

public class MediaStatusService implements IService {

    private static final String TAG = "MediaStatusService";

    private IMediaStatusEvent mediaStatusEvent_;
    private final IAndroidAutoEntityEventHandler eventHandler_;

    public MediaStatusService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler, IMediaStatusEvent mediaStatusEvent) {
        handle_ = nativeSetup(messenger, mediaStatusEvent);
        mediaStatusEvent_ = mediaStatusEvent;
        eventHandler_ = eventHandler;
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

    @Keep
    @Override
    public void onError(String error, int code){
        Log.v(TAG, "onError " + error + "/" + code);
        eventHandler_.onAndroidAutoQuitOnError(error, code);
    }

    static {
        nativeInit();
    }

    private static native void nativeInit();
    private native long nativeSetup(Messenger messenger, IMediaStatusEvent navigationEvent);
    private native void nativeStart();
    private native void nativeStop();
    private native void nativeDelete();

    @Keep
    protected long handle_;
}
