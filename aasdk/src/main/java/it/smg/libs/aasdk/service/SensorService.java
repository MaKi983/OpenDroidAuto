package it.smg.libs.aasdk.service;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.ISensor;

public class SensorService implements IService, ISensor.Listener {

    private static final String TAG = "SensorService";

    private ISensor sensor_;

    public SensorService(Messenger messenger, ISensor sensor) {
        handle_ = nativeSetup(messenger, sensor.isNight());
        sensor_ = sensor;
        sensor_.setListener(this);
    }

    @Override
    public void start() {
        nativeStart();
    }

    @Override
    public void stop() {
        nativeStop();
        sensor_.stop();
        sensor_.setListener(null);
    }

    @Override
    public void delete() {
        nativeDelete();
    }

    @Override
    public void onDayNightUpdate(boolean isNight) {
        sendNightMode(isNight);
    }

    static {
        nativeInit();
    }

    private static native void nativeInit();
    private native long nativeSetup(Messenger messenger, boolean isNight);
    private native void nativeStart();
    private native void nativeStop();
    private native void nativeDelete();
    private native void sendNightMode(boolean isNight);

    protected long handle_;

}
