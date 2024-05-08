package it.smg.libs.aasdk.service;

import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IVideoOutput;

public class VideoService implements IService {

    private static final String TAG = "VideoService";

    private IVideoOutput videoOutput_;

    public VideoService(Messenger messenger, IVideoOutput videoOutput) {
        handle_ = nativeSetup(messenger, videoOutput);

        videoOutput_ = videoOutput;
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
        videoOutput_.delete();
        videoOutput_ = null;
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
    private native long nativeSetup(Messenger messenger, IVideoOutput videoOutput);
    private native void nativeStart();
    private native void nativeStop();
    private native void nativeReleaseFocus();
    private native void nativeGainFocus();
    private native void nativeDelete();

    protected long handle_;
}
