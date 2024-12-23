package it.smg.hu.service;

import android.content.Context;
import android.view.SurfaceView;

import it.smg.hu.config.Settings;
import it.smg.hu.projection.DeviceFactory;
import it.smg.libs.aasdk.service.AndroidAutoEntity;
import it.smg.libs.aasdk.service.AndroidAutoEntityBuilder;
import it.smg.libs.aasdk.tcp.TCPEndpoint;
import it.smg.libs.aasdk.usb.LibUsbDevice;

public class AndroidAutoEntityFactory {

    private static final String TAG = "AndroidAutoEntityFactory";

    public static AndroidAutoEntity create(ODAService service, TCPEndpoint tcpEndpoint, SurfaceView surfaceView){
        AndroidAutoEntityBuilder builder = new AndroidAutoEntityBuilder(tcpEndpoint, service, Settings.instance().car);

        createServices(builder, service.getApplicationContext(), surfaceView);

        return builder.build();
    }

    public static AndroidAutoEntity create(ODAService service, LibUsbDevice device, SurfaceView surfaceView){
        AndroidAutoEntityBuilder builder = new AndroidAutoEntityBuilder(device, service, Settings.instance().car);

        createServices(builder, service.getApplicationContext(), surfaceView);

        return builder.build();
    }

    private static void createServices(AndroidAutoEntityBuilder builder, Context ctx, SurfaceView surfaceView){
        builder.createMediaAudioService(DeviceFactory.createMediaAudioOutput());
        builder.createSpeechAudioService(DeviceFactory.createSpeechAudioOutput());
        builder.createSystemAudioService(DeviceFactory.createSystemAudioOutput());
        builder.createAudioInputService(DeviceFactory.createAudioInput());
        builder.createVideoService(DeviceFactory.createVideoOutput(surfaceView));
        builder.createSensorService(DeviceFactory.createSensor(ctx));
        builder.createBluetoothService(DeviceFactory.createBluetoothDevice());
        builder.createInputService(DeviceFactory.createInputDevice(ctx, surfaceView));

        if (Settings.instance().video.showNavigationNotification()) {
            builder.createNavigationStatusService(DeviceFactory.createNavigationStatusEvent());
        }

        if (Settings.instance().video.showMediaNotification()) {
            builder.createMediaStatusService(DeviceFactory.createMediaStatusEvent());
        }

        builder.setThreadNum(Settings.instance().advanced.threads());
    }
}
