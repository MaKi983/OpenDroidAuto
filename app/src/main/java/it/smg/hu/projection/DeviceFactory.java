package it.smg.hu.projection;

import android.content.Context;
import android.os.Build;
import android.view.SurfaceView;

import java.util.HashMap;
import java.util.Map;

import it.smg.hu.config.Settings;
import it.smg.hu.service.ODAService;
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
import it.smg.libs.aasdk.service.InputService;
import it.smg.libs.aasdk.service.MediaAudioService;
import it.smg.libs.aasdk.service.MediaStatusService;
import it.smg.libs.aasdk.service.NavigationStatusService;
import it.smg.libs.aasdk.service.SensorService;
import it.smg.libs.aasdk.service.SpeechAudioService;
import it.smg.libs.aasdk.service.SystemAudioService;
import it.smg.libs.aasdk.service.VideoService;
import it.smg.libs.common.Log;

public class DeviceFactory {

    private static final String TAG = "DeviceFactory";

    public static IAudioOutput createMediaAudioOutput(){
        if (Log.isInfo()) Log.i(TAG, "create media audio output");
        return new MediaAudioOutput();
    }

    public static IAudioOutput createSpeechAudioOutput(){
        if (Log.isInfo()) Log.i(TAG, "create speech audio output");
        return new SpeechAudioOutput();
    }

    public static IAudioOutput createSystemAudioOutput(){
        if (Log.isInfo()) Log.i(TAG, "create system audio output");
        return new SystemAudioOutput();
    }

    public static IAudioInput createAudioInput(){
        if (Log.isInfo()) Log.i(TAG, "create audio input");
        return new AudioInput();
    }

    public static IInputDevice createInputDevice(Context ctx, SurfaceView surfaceView, InputDevice.OnKeyHolder keyHolder){
        if (Log.isInfo()) Log.i(TAG, "create input device");
        return new InputDevice(ctx, surfaceView, keyHolder);
    }

    public static IVideoOutput createVideoOutput(SurfaceView surfaceView){
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
        return videoOutput;
    }

    public static ISensor createSensor(Context ctx){
        if (Log.isInfo()) Log.i(TAG, "create sensor");
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

        return sensor;
    }

    public static IBluetoothDevice createBluetoothDevice(){
        if (Log.isInfo()) Log.i(TAG, "create bluetooth device");
        return new BluetoothDevice();
    }

    public static INavigationStatusEvent createNavigationStatusEvent(){
        if (Log.isInfo()) Log.i(TAG, "create navigation status event");
        return new NavigationStatusEvent();
    }

    public static IMediaStatusEvent createMediaStatusEvent(){
        if (Log.isInfo()) Log.i(TAG, "create navigation status event");
        return new MediaStatusEvent();
    }

}
