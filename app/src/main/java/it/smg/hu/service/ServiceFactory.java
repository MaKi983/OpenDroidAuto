package it.smg.hu.service;

import android.content.Context;
import android.os.Build;
import android.view.SurfaceView;

import java.util.HashMap;
import java.util.Map;

import it.smg.hu.config.Settings;
import it.smg.hu.projection.AudioInput;
import it.smg.hu.projection.BluetoothDevice;
import it.smg.hu.projection.DayNightSensor;
import it.smg.hu.projection.FakeVideoOutput;
import it.smg.hu.projection.InputDevice;
import it.smg.hu.projection.LightSensor;
import it.smg.hu.projection.MediaAudioOutput;
import it.smg.hu.projection.MediaStatusEvent;
import it.smg.hu.projection.NativeVideoOutput;
import it.smg.hu.projection.NavigationStatusEvent;
import it.smg.hu.projection.OMXVideoOutput;
import it.smg.hu.projection.SpeechAudioOutput;
import it.smg.hu.projection.SystemAudioOutput;
import it.smg.hu.util.DeviceUtils;
import it.smg.libs.aasdk.messenger.ChannelId;
import it.smg.libs.aasdk.messenger.Messenger;
import it.smg.libs.aasdk.projection.IAudioInput;
import it.smg.libs.aasdk.projection.IAudioOutput;
import it.smg.libs.aasdk.projection.IBluetoothDevice;
import it.smg.libs.aasdk.projection.IInputDevice;
import it.smg.libs.aasdk.projection.IMediaStatusEvent;
import it.smg.libs.aasdk.projection.INavigationStatusEvent;
import it.smg.libs.aasdk.projection.ISensor;
import it.smg.libs.aasdk.projection.IVideoOutput;
import it.smg.libs.aasdk.service.AudioInputService;
import it.smg.libs.aasdk.service.BluetoothService;
import it.smg.libs.aasdk.service.IAndroidAutoEntityEventHandler;
import it.smg.libs.aasdk.service.IService;
import it.smg.libs.aasdk.service.IVideoEventHandler;
import it.smg.libs.aasdk.service.InputService;
import it.smg.libs.aasdk.service.MediaAudioService;
import it.smg.libs.aasdk.service.MediaStatusService;
import it.smg.libs.aasdk.service.NavigationStatusService;
import it.smg.libs.aasdk.service.SensorService;
import it.smg.libs.aasdk.service.SpeechAudioService;
import it.smg.libs.aasdk.service.SystemAudioService;
import it.smg.libs.aasdk.service.VideoService;
import it.smg.libs.common.Log;

public class ServiceFactory {

    private static final String TAG = "ServiceFactory";

    public static Map<ChannelId, IService> create(Messenger messenger, ODAService service, SurfaceView surfaceView){
        Map<ChannelId, IService> serviceList = new HashMap<>();

        Settings settings = Settings.instance();

        if (settings.audio.musicChannelEnabled()){
            IService musicAudioService = createMusicService(messenger, service);
            serviceList.put(ChannelId.MEDIA_AUDIO, musicAudioService);
        }

        if (settings.audio.speechAudioChannelEnabled()){
            IService speechAudioService = createSpeechService(messenger, service);
            serviceList.put(ChannelId.SPEECH_AUDIO, speechAudioService);
        }

        IService systemAudioService = createSystemService(messenger, service);
        serviceList.put(ChannelId.SYSTEM_AUDIO, systemAudioService);

        IService inputAudioService = createAudioInputService(messenger, service);
        serviceList.put(ChannelId.AV_INPUT, inputAudioService);

        IService videoService = createVideoService(messenger, service, surfaceView);
        serviceList.put(ChannelId.VIDEO, videoService);

        IService sensorService = createSensorService(messenger, service.getApplicationContext(), service);
        serviceList.put(ChannelId.SENSOR, sensorService);

        IService bluetoothService = createBluetoothService(messenger, service);
        serviceList.put(ChannelId.BLUETOOTH, bluetoothService);

        IService inputService = createInputService(messenger, service.getApplicationContext(), service, surfaceView);
        serviceList.put(ChannelId.INPUT, inputService);

        if (settings.video.showNavigationNotification()) {
            IService navigationStatusService = createNavigationStatusService(messenger, service);
            serviceList.put(ChannelId.NAVIGATION, navigationStatusService);
        }

        if (settings.video.showMediaNotification()) {
            IService mediaStatusService = createMediaStatusService(messenger, service);
            serviceList.put(ChannelId.MEDIA_STATUS, mediaStatusService);
        }

        return serviceList;
    }

    private static IService createMusicService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler){
        if (Log.isInfo()) Log.i(TAG, "create audio music service");
        IAudioOutput mediaAudioOutput = new MediaAudioOutput();
        return new MediaAudioService(messenger, eventHandler, mediaAudioOutput);
    }

    private static IService createSpeechService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler){
        if (Log.isInfo()) Log.i(TAG, "create audio speech service");
        IAudioOutput speechAudioOutput = new SpeechAudioOutput();
        return new SpeechAudioService(messenger, eventHandler, speechAudioOutput);
    }

    private static IService createSystemService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler){
        if (Log.isInfo()) Log.i(TAG, "create audio system service");
        IAudioOutput systemAudioOutput = new SystemAudioOutput();
        return new SystemAudioService(messenger, eventHandler, systemAudioOutput);
    }

    private static IService createAudioInputService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler){
        if (Log.isInfo()) Log.i(TAG, "create audio input service");
        IAudioInput audioInput = new AudioInput();
        return new AudioInputService(messenger, eventHandler, audioInput);
    }

    private static IService createInputService(Messenger messenger, Context ctx, IAndroidAutoEntityEventHandler eventHandler, SurfaceView surfaceView){
        if (Log.isInfo()) Log.i(TAG, "create input service");
        IInputDevice inputDevice = new InputDevice(ctx, surfaceView);
        return new InputService(messenger, eventHandler, inputDevice);
    }

    private static IService createVideoService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler, SurfaceView surfaceView){
        if (Log.isInfo()) Log.i(TAG, "create video service");
        IVideoOutput videoOutput;
        if (DeviceUtils.isX86) {
            if (Log.isInfo()) Log.i(TAG, "Fake Video Output");
            videoOutput = new FakeVideoOutput(surfaceView);
        } else if (Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN){
            if (Log.isInfo()) Log.i(TAG, "OMX Video Output Legacy");
            videoOutput = new OMXVideoOutput(surfaceView);
        } else {
            if (Log.isInfo()) Log.i(TAG, "Native Video Output");
            videoOutput = new NativeVideoOutput(surfaceView);
        }
        return new VideoService(eventHandler, messenger, videoOutput);
    }

    private static IService createSensorService(Messenger messenger, Context ctx, IAndroidAutoEntityEventHandler eventHandler){
        if (Log.isInfo()) Log.i(TAG, "create sensor service");
        Settings settings = Settings.instance();

        ISensor sensor;
        switch (settings.video.nightMode()){
            case ISensor.LIGHT:
                sensor = new LightSensor(ctx);
                break;
            case ISensor.DAY:
            case ISensor.NIGHT:
            case ISensor.TIME_GPS:
            default:
                sensor = new DayNightSensor(ctx);
                break;
        }

        return new SensorService(messenger, eventHandler, sensor);
    }

    private static IService createBluetoothService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler){
        if (Log.isInfo()) Log.i(TAG, "create audio input service");
        IBluetoothDevice bluetoothDevice = new BluetoothDevice();
        return new BluetoothService(messenger, eventHandler, bluetoothDevice);
    }

    private static IService createNavigationStatusService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler){
        if (Log.isInfo()) Log.i(TAG, "create navigation status service");
        INavigationStatusEvent navigationEvent = new NavigationStatusEvent();
        return new NavigationStatusService(messenger, eventHandler, navigationEvent);
    }

    private static IService createMediaStatusService(Messenger messenger, IAndroidAutoEntityEventHandler eventHandler){
        if (Log.isInfo()) Log.i(TAG, "create navigation status service");
        IMediaStatusEvent mediaEvent = new MediaStatusEvent();
        return new MediaStatusService(messenger, eventHandler, mediaEvent);
    }
}
