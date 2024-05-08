package it.smg.libs.aasdk.service;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IAudioOutput;

public abstract class AudioService implements IService {

    private IAudioOutput audioOutput_;

    public AudioService(IAudioOutput audioOutput){
        audioOutput_ = audioOutput;
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

    protected abstract String tag();

    protected abstract long nativeSetup(Messenger messenger, IAudioOutput audioOutput);
    protected abstract void nativeStart();
    protected abstract void nativeStop();
    protected abstract void nativeDelete();

}
