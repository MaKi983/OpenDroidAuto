package it.smg.hu.projection;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import it.smg.hu.config.Settings;
import it.smg.hu.manager.HondaConnectManager;
import it.smg.libs.aasdk.projection.ISensor;
import it.smg.libs.common.Log;

public class DeviceLightSensor implements ISensor, SensorEventListener, ISensor.Listener {

    private static final String TAG = "LightSensor";

    private final Context context_;
    private Listener listener_;
    private int currentState_;

    public DeviceLightSensor(Context ctx){
        context_ = ctx;
        currentState_ = IS_DAY;

        Settings settings = Settings.instance();
        if (settings.advanced.hondaIntegrationEnabled()){
            if (Log.isDebug()) Log.d(TAG, "initHondaIntegration");
            HondaConnectManager.instance().setDayNightListener(this);

            boolean isNight = HondaConnectManager.instance().isNight();
            currentState_ = isNight ? IS_NIGHT : IS_DAY;
        } else {
            SensorManager sensorManager = (SensorManager)context_.getSystemService(Context.SENSOR_SERVICE);
            Sensor lightSensor = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT);
            if (lightSensor != null) {
                sensorManager.registerListener(this, lightSensor, SensorManager.SENSOR_DELAY_NORMAL);
            }
        }

    }

    @Override
    public void onDayNightUpdate(boolean isNight) {
        if (Log.isInfo()) Log.i(TAG, "Honda DayNight state changed, isNight: " + isNight);
        int state = isNight ? IS_NIGHT : IS_DAY;
        updateState(state);
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        if(sensorEvent.sensor.getType() == Sensor.TYPE_LIGHT){
            float value = sensorEvent.values[0];
            int state = value <= 5 ? IS_NIGHT : IS_DAY;
            updateState(state);
        }
    }

    private void updateState(int state){
        if (state != currentState_) {
            if (Log.isInfo()) Log.i(TAG, "state changed: " + state);
            currentState_ = state;
            if (listener_ != null) {
                listener_.onDayNightUpdate(isNight());
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {}

    @Override
    public void stop() {
        if (Log.isDebug()) Log.d(TAG, "stop");

        if (Settings.instance().advanced.hondaIntegrationEnabled()){
            HondaConnectManager.instance().setDayNightListener(null);
        } else {
            if (Log.isDebug()) Log.d(TAG, "remove sensorManager listener");
            SensorManager sensorManager = (SensorManager) context_.getSystemService(Context.SENSOR_SERVICE);
            sensorManager.unregisterListener(this);
        }
    }

    @Override
    public boolean isNight() {
        return currentState_ == IS_NIGHT;
    }

    @Override
    public void setListener(Listener listener) {
        listener_ = listener;
    }
}
