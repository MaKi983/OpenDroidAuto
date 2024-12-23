package it.smg.libs.aasdk.service;

import java.util.HashMap;
import java.util.Map;

import it.smg.libs.aasdk.Runtime;
import it.smg.libs.aasdk.configuration.ICarConfiguration;
import it.smg.libs.aasdk.messenger.ChannelId;
import it.smg.libs.aasdk.messenger.Cryptor;
import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IAudioInput;
import it.smg.libs.aasdk.projection.IAudioOutput;
import it.smg.libs.aasdk.projection.IBluetoothDevice;
import it.smg.libs.aasdk.projection.IInputDevice;
import it.smg.libs.aasdk.projection.IMediaStatusEvent;
import it.smg.libs.aasdk.projection.INavigationStatusEvent;
import it.smg.libs.aasdk.projection.ISensor;
import it.smg.libs.aasdk.projection.IVideoOutput;
import it.smg.libs.aasdk.tcp.TCPEndpoint;
import it.smg.libs.aasdk.transport.TCPTransport;
import it.smg.libs.aasdk.transport.Transport;
import it.smg.libs.aasdk.transport.USBTransport;
import it.smg.libs.aasdk.usb.AOAPDevice;
import it.smg.libs.aasdk.usb.LibUsbDevice;
import it.smg.libs.common.Log;

public class AndroidAutoEntityBuilder  {

    private static final String TAG = "AndroidAutoEntityBuilder";

    private final Map<ChannelId, IService> serviceList_;
    private final Runtime runtime_;
    private final Transport transport_;
    private final Cryptor cryptor_;
    private final Messenger messenger_;
    private final IAndroidAutoEntityEventHandler eventHandler_;
    private final ICarConfiguration config_;

    private int threads_;

    public AndroidAutoEntityBuilder(TCPEndpoint tcpEndpoint, IAndroidAutoEntityEventHandler eventHandler, ICarConfiguration config){
        runtime_ = new Runtime();
        transport_ = new TCPTransport(tcpEndpoint);
        eventHandler_ = eventHandler;
        cryptor_ = new Cryptor();
        messenger_ = new Messenger((TCPTransport) transport_, cryptor_);
        config_ = config;
        threads_ = 4;

        serviceList_ = new HashMap<>();
    }

    public AndroidAutoEntityBuilder(LibUsbDevice device, IAndroidAutoEntityEventHandler eventHandler, ICarConfiguration config){
        runtime_ = new Runtime();
        if (Log.isInfo()) Log.i(TAG, "create AOAP device");
        AOAPDevice aoapDevice = AOAPDevice.create(device);
        transport_ = new USBTransport(aoapDevice);
        eventHandler_ = eventHandler;
        cryptor_ = new Cryptor();
        messenger_ = new Messenger((USBTransport) transport_, cryptor_);
        config_ = config;
        threads_ = 4;

        serviceList_ = new HashMap<>();
    }

    public void createMediaAudioService(IAudioOutput audioOutput){
        if (Log.isInfo()) Log.i(TAG, "create audio music service");
        serviceList_.put(ChannelId.MEDIA_AUDIO, new MediaAudioService(messenger_, eventHandler_, audioOutput));
    }

    public void createSpeechAudioService(IAudioOutput audioOutput){
        if (Log.isInfo()) Log.i(TAG, "create audio speech service");
        serviceList_.put(ChannelId.SPEECH_AUDIO, new SpeechAudioService(messenger_, eventHandler_, audioOutput));
    }

    public void createSystemAudioService(IAudioOutput audioOutput){
        if (Log.isInfo()) Log.i(TAG, "create audio system service");
        serviceList_.put(ChannelId.SYSTEM_AUDIO, new SystemAudioService(messenger_, eventHandler_, audioOutput));
    }

    public void createAudioInputService(IAudioInput audioInput){
        if (Log.isInfo()) Log.i(TAG, "create audio input service");
        serviceList_.put(ChannelId.AV_INPUT, new AudioInputService(messenger_, eventHandler_, audioInput));
    }

    public void createInputService(IInputDevice inputDevice){
        if (Log.isInfo()) Log.i(TAG, "create input service");
        serviceList_.put(ChannelId.INPUT, new InputService(messenger_, eventHandler_, inputDevice));
    }

    public void createVideoService(IVideoOutput videoOutput){
        if (Log.isInfo()) Log.i(TAG, "create video service");
        serviceList_.put(ChannelId.VIDEO, new VideoService(eventHandler_, messenger_, videoOutput));
    }

    public void createSensorService(ISensor sensor){
        if (Log.isInfo()) Log.i(TAG, "create sensor service");
        serviceList_.put(ChannelId.SENSOR, new SensorService(messenger_, eventHandler_, sensor));
    }

    public void createBluetoothService(IBluetoothDevice bluetoothDevice){
        if (Log.isInfo()) Log.i(TAG, "create bluetooth service");
        serviceList_.put(ChannelId.BLUETOOTH, new BluetoothService(messenger_, eventHandler_, bluetoothDevice));
    }

    public void createNavigationStatusService(INavigationStatusEvent navigationEvent){
        if (Log.isInfo()) Log.i(TAG, "create navigation status service");
        serviceList_.put(ChannelId.NAVIGATION, new NavigationStatusService(messenger_, eventHandler_, navigationEvent));
    }

    public void createMediaStatusService(IMediaStatusEvent mediaEvent){
        if (Log.isInfo()) Log.i(TAG, "create navigation status service");
        serviceList_.put(ChannelId.MEDIA_STATUS, new MediaStatusService(messenger_, eventHandler_, mediaEvent));
    }

    public void setThreadNum(int threads){
        threads_ = threads;
    }

    public AndroidAutoEntity build(){
        IPinger pinger = new Pinger(5000);

        return new AndroidAutoEntity(runtime_, cryptor_, transport_, messenger_, config_, serviceList_, pinger, threads_);
    }
}
