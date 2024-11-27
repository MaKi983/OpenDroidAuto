package it.smg.libs.aasdk.service;

import androidx.annotation.Keep;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.INavigationStatusEvent;
import it.smg.libs.common.Log;

public class NavigationStatusService implements IService {

    private static final String TAG = "NavigationStatusService";

    private INavigationStatusEvent navigationEvent_;
    private final IAndroidAutoEntityEventHandler eventHandler_;

    public NavigationStatusService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler, INavigationStatusEvent navigationEvent) {
        handle_ = nativeSetup(messenger, navigationEvent);
        navigationEvent_ = navigationEvent;
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
        navigationEvent_.delete();
        navigationEvent_ = null;
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
    private native long nativeSetup(Messenger messenger, INavigationStatusEvent navigationEvent);
    private native void nativeStart();
    private native void nativeStop();
    private native void nativeDelete();

    @Keep
    protected long handle_;
}
