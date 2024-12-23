package it.smg.libs.aasdk.service;

import android.view.SurfaceView;

import androidx.annotation.Keep;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Map;

import it.smg.libs.aasdk.Runtime;
import it.smg.libs.aasdk.transport.TCPTransport;
import it.smg.libs.common.Log;
import it.smg.libs.aasdk.configuration.ICarConfiguration;
import it.smg.libs.aasdk.messenger.ChannelId;
import it.smg.libs.aasdk.messenger.Cryptor;
import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.transport.Transport;

public class AndroidAutoEntity implements IAndroidAutoEntity {

    private static final String TAG = "AndroidAutoEntity";

    private Cryptor cryptor_;
    private Transport transport_;
    private Messenger messenger_;
    private IPinger pinger_;
    private ICarConfiguration config_;
    private Map<ChannelId, IService> serviceList_;

    private Runtime runtime_;

    /*private*/ AndroidAutoEntity(Runtime runtime, Cryptor cryptor, Transport transport, Messenger messenger, ICarConfiguration config, Map<ChannelId, IService> serviceList, IPinger pinger, int threads) {
        cryptor_ = cryptor;
        transport_ = transport;
        messenger_ = messenger;
        serviceList_ = serviceList;
        config_ = config;
        pinger_ = pinger;
        runtime_ = runtime;

        handle_ = nativeSetup(cryptor_, messenger_, new ArrayList<>(serviceList_.values()), config_, pinger_);

//        Runtime.startThreads(threads);
        runtime_.startThreads(threads);
    }

    public void releaseFocus() {
        VideoService videoService = (VideoService) serviceList_.get(ChannelId.VIDEO);
        videoService.releaseFocus();

        InputService inputService = (InputService) serviceList_.get(ChannelId.INPUT);
        inputService.releaseFocus();
    }

    public void gainFocus() {
        VideoService videoService = (VideoService) serviceList_.get(ChannelId.VIDEO);
        videoService.gainFocus();

        InputService inputService = (InputService) serviceList_.get(ChannelId.INPUT);
        inputService.gainFocus();
    }

    public void shutdown(){
//        nativeRequestShutdown();
        stop();
    }

    @Override
    public void start(IAndroidAutoEntityEventHandler eventHandler) {
        if (Log.isInfo()) Log.i(TAG, "start");

        // start services
        for (IService service : serviceList_.values()) {
            service.start();
        }

        nativeStart(eventHandler);
    }

    public void onChannelError(String error) {
        Log.e(TAG, "onChannelError " + error);
//        triggerQuit();
    }

    @Override
    public void stop() {
        messenger_.stop();
        transport_.stop();

        for (IService service : serviceList_.values()) {
            service.stop();
        }

        if (pinger_ != null) {
            pinger_.cancel();
        }

        nativeStop();

        runtime_.stopThreads();
//        Runtime.stopThreads();
    }

    public void delete(){
        nativeDelete();

        for (IService service : serviceList_.values()) {
            service.delete();
        }

        if (pinger_ != null) {
            pinger_.delete();
        }
        pinger_ = null;
        messenger_.delete();
        messenger_ = null;
        cryptor_.delete();
        cryptor_ = null;
        transport_.delete();
        transport_ = null;

        runtime_.delete();
//        Runtime.stopThreads();
    }

    // Native context
    private native void nativeStart(IAndroidAutoEntityEventHandler eventHandler);
    private native void nativeStop();
    private native void nativeDelete();
    private native void nativeRequestShutdown();

    @Keep
    protected long handle_;

    private native long nativeSetup(Cryptor cryptor, Messenger messenger, Collection<IService> serviceList, ICarConfiguration config, IPinger pinger);
    private static native void nativeInit();

    static {
        nativeInit();
    }

}