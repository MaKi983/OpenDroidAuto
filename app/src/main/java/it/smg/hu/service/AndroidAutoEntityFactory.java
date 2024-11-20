package it.smg.hu.service;

import android.view.SurfaceView;

import java.util.Map;

import it.smg.hu.config.Settings;
import it.smg.libs.aasdk.messenger.ChannelId;
import it.smg.libs.aasdk.messenger.Cryptor;
import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.service.AndroidAutoEntity;
import it.smg.libs.aasdk.service.IPinger;
import it.smg.libs.aasdk.service.IService;
import it.smg.libs.aasdk.service.Pinger;
import it.smg.libs.aasdk.transport.TCPTransport;
import it.smg.libs.aasdk.transport.USBTransport;

public class AndroidAutoEntityFactory {

    private static final String TAG = "AndroidAutoEntityFactory";

    public static AndroidAutoEntity create(ODAService service, TCPTransport transport, SurfaceView surfaceView){
        Cryptor cryptor = new Cryptor();

        Messenger messenger = new Messenger(transport, cryptor);

        Map<ChannelId, IService> serviceList = ServiceFactory.create(messenger, service, surfaceView);
        IPinger pinger = new Pinger(5000);

        return new AndroidAutoEntity(cryptor, transport, messenger, Settings.instance().car, serviceList, pinger, Settings.instance().advanced.threads());
    }

    public static AndroidAutoEntity create(ODAService service, USBTransport transport, SurfaceView surfaceView){
        Cryptor cryptor = new Cryptor();

        Messenger messenger = new Messenger(transport, cryptor);

        Map<ChannelId, IService> serviceList = ServiceFactory.create(messenger, service, surfaceView);
        IPinger pinger = new Pinger(5000);

        return new AndroidAutoEntity(cryptor, transport, messenger, Settings.instance().car, serviceList, pinger, Settings.instance().advanced.threads());
    }
}
