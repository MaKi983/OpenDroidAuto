package it.smg.libs.aasdk.service;

import androidx.annotation.Keep;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IAudioOutput;
import it.smg.libs.common.Log;

public abstract class AudioService implements IService {

    private IAudioOutput audioOutput_;
    private final IAndroidAutoEntityEventHandler eventHandler_;

    public AudioService(IAudioOutput audioOutput, IAndroidAutoEntityEventHandler eventHandler){
        audioOutput_ = audioOutput;
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
        audioOutput_.delete();
        audioOutput_ = null;
    }

    @Keep
    @Override
    public void onError(String error, int code){
        Log.v(tag(), "onError " + error + "/" + code);
        eventHandler_.onAndroidAutoQuitOnError(error, code);
    }

    protected abstract String tag();

    protected abstract long nativeSetup(Messenger messenger, IAudioOutput audioOutput);
    protected abstract void nativeStart();
    protected abstract void nativeStop();
    protected abstract void nativeDelete();

}
