package it.smg.libs.aasdk.service;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.INavigationStatusEvent;

public class NavigationStatusService implements IService {

    private static final String TAG = "NavigationStatusService";

    private INavigationStatusEvent navigationEvent_;

    public NavigationStatusService(Messenger messenger, INavigationStatusEvent navigationEvent) {
        handle_ = nativeSetup(messenger, navigationEvent);
        navigationEvent_ = navigationEvent;
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

    static {
        nativeInit();
    }

    private static native void nativeInit();
    private native long nativeSetup(Messenger messenger, INavigationStatusEvent navigationEvent);
    private native void nativeStart();
    private native void nativeStop();
    private native void nativeDelete();

    protected long handle_;
}
