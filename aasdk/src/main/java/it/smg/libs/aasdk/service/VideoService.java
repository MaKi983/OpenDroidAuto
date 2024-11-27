package it.smg.libs.aasdk.service;

import android.content.Context;

import androidx.annotation.Keep;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IVideoOutput;
import it.smg.libs.common.Log;

public class VideoService implements IService {

    private static final String TAG = "VideoService";

    private IVideoOutput videoOutput_;
    private final IAndroidAutoEntityEventHandler eventHandler_;

    public VideoService(IAndroidAutoEntityEventHandler eventHandler, Messenger messenger, IVideoOutput videoOutput) {
        eventHandler_ = eventHandler;
        handle_ = nativeSetup(messenger, videoOutput, eventHandler);

        videoOutput_ = videoOutput;
    }

    @Override
    public void start() {
        nativeStart();
    }

    @Override
    public void stop() {
        nativeStop();
        Log.v(TAG, "stopped");
    }

    @Override
    public void delete() {
        nativeDelete();
        videoOutput_.delete();
        videoOutput_ = null;
    }

    @Keep
    @Override
    public void onError(String error, int code){
        Log.v(TAG, "onError " + error + "/" + code);
        eventHandler_.onAndroidAutoQuitOnError(error, code);
    }

    public void releaseFocus(){
        nativeReleaseFocus();
        videoOutput_.stop();
    }

    public void gainFocus(){
        videoOutput_.init();
        nativeGainFocus();
    }

    static {
        nativeInit();
    }

    private static native void nativeInit();
    private native long nativeSetup(Messenger messenger, IVideoOutput videoOutput, IVideoEventHandler eventHandler);
    private native void nativeStart();
    private native void nativeStop();
    private native void nativeReleaseFocus();
    private native void nativeGainFocus();
    private native void nativeDelete();

    @Keep
    protected long handle_;
}
