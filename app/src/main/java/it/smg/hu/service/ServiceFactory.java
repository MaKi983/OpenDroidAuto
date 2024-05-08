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
import it.smg.libs.aasdk.service.IService;
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

    public static Map<ChannelId, IService> create(Messenger messenger, Context ctx, SurfaceView surfaceView){
        Map<ChannelId, IService> serviceList = new HashMap<>();

        Settings settings = Settings.instance();

        if (settings.audio.musicChannelEnabled()){
            IService musicAudioService = createMusicService(messenger);
            serviceList.put(ChannelId.MEDIA_AUDIO, musicAudioService);
        }

        if (settings.audio.speechAudioChannelEnabled()){
            IService speechAudioService = createSpeechService(messenger);
            serviceList.put(ChannelId.SPEECH_AUDIO, speechAudioService);
        }

        IService systemAudioService = createSystemService(messenger);
        serviceList.put(ChannelId.SYSTEM_AUDIO, systemAudioService);

        IService inputAudioService = createAudioInputService(messenger);
        serviceList.put(ChannelId.AV_INPUT, inputAudioService);

        IService videoService = createVideoService(messenger, ctx, surfaceView);
        serviceList.put(ChannelId.VIDEO, videoService);

        IService sensorService = createSensorService(messenger, ctx);
        serviceList.put(ChannelId.SENSOR, sensorService);

        IService bluetoothService = createBluetoothService(messenger);
        serviceList.put(ChannelId.BLUETOOTH, bluetoothService);

        IService inputService = createInputService(messenger, ctx, surfaceView);
        serviceList.put(ChannelId.INPUT, inputService);

        if (settings.video.showNavigationNotification()) {
            IService navigationStatusService = createNavigationStatusService(messenger);
            serviceList.put(ChannelId.NAVIGATION, navigationStatusService);
        }

        if (settings.video.showMediaNotification()) {
            IService mediaStatusService = createMediaStatusService(messenger);
            serviceList.put(ChannelId.MEDIA_STATUS, mediaStatusService);
        }

        return serviceList;
    }

    private static IService createMusicService(Messenger messenger){
        if (Log.isInfo()) Log.i(TAG, "create audio music service");
        IAudioOutput mediaAudioOutput = new MediaAudioOutput();
        return new MediaAudioService(messenger, mediaAudioOutput);
    }

    private static IService createSpeechService(Messenger messenger){
        if (Log.isInfo()) Log.i(TAG, "create audio speech service");
        IAudioOutput speechAudioOutput = new SpeechAudioOutput();
        return new SpeechAudioService(messenger, speechAudioOutput);
    }

    private static IService createSystemService(Messenger messenger){
        if (Log.isInfo()) Log.i(TAG, "create audio system service");
        IAudioOutput systemAudioOutput = new SystemAudioOutput();
        return new SystemAudioService(messenger, systemAudioOutput);
    }

    private static IService createAudioInputService(Messenger messenger){
        if (Log.isInfo()) Log.i(TAG, "create audio input service");
        IAudioInput audioInput = new AudioInput();
        return new AudioInputService(messenger, audioInput);
    }

    private static IService createInputService(Messenger messenger, Context ctx, SurfaceView surfaceView){
        if (Log.isInfo()) Log.i(TAG, "create input service");
        IInputDevice inputDevice = new InputDevice(ctx, surfaceView);
        return new InputService(messenger, inputDevice);
    }

    private static IService createVideoService(Messenger messenger, Context ctx, SurfaceView surfaceView){
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
        return new VideoService(messenger, videoOutput);
    }

    private static IService createSensorService(Messenger messenger, Context ctx){
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

        return new SensorService(messenger, sensor);
    }

    private static IService createBluetoothService(Messenger messenger){
        if (Log.isInfo()) Log.i(TAG, "create audio input service");
        IBluetoothDevice bluetoothDevice = new BluetoothDevice();
        return new BluetoothService(messenger, bluetoothDevice);
    }

    private static IService createNavigationStatusService(Messenger messenger){
        if (Log.isInfo()) Log.i(TAG, "create navigation status service");
        INavigationStatusEvent navigationEvent = new NavigationStatusEvent();
        return new NavigationStatusService(messenger, navigationEvent);
    }

    private static IService createMediaStatusService(Messenger messenger){
        if (Log.isInfo()) Log.i(TAG, "create navigation status service");
        IMediaStatusEvent mediaEvent = new MediaStatusEvent();
        return new MediaStatusService(messenger, mediaEvent);
    }
}
