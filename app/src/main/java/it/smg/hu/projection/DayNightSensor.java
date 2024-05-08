package it.smg.hu.projection;

import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Handler;

import androidx.annotation.NonNull;

import java.util.List;

import it.smg.hu.config.Settings;
import it.smg.libs.common.Log;
import it.smg.libs.aasdk.projection.ISensor;

public class DayNightSensor implements ISensor, LocationListener {

    private static final String TAG = "DayNightSensor";

    private final Context context_;
    private int currentState_;
    private TwilightCalculator twilightCalculator_;
    private LocationManager locationManager_;
    private Handler handler_;
    private Runnable timeCallback_;
    private Listener listener_;

    public DayNightSensor(Context ctx){
        context_ = ctx;
        Settings settings = Settings.instance();

        switch (settings.video.nightMode()){
            case ISensor.NIGHT:
                currentState_ = IS_NIGHT;
                break;
            case ISensor.TIME_GPS:
                initTwilightCalculator();
                break;
            case ISensor.DAY:
                currentState_ = IS_DAY;
                break;
            default:
                if (Log.isWarn()) Log.w(TAG, "unknown night mode, use DAY");
                currentState_ = IS_DAY;
        }
    }

    private void initTwilightCalculator(){
        twilightCalculator_ = new TwilightCalculator();

        locationManager_ = (LocationManager) context_.getSystemService(Context.LOCATION_SERVICE);
        locationManager_.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, this);

        Location l = getLastKnownLocation();
        if (l != null){
            if (Log.isDebug()) Log.d(TAG, "Found last knonw location " + l.getLatitude() + " - " + l.getLongitude());
            twilightCalculator_.calculateTwilight(System.currentTimeMillis(), l.getLatitude(), l.getLongitude());
            currentState_ = twilightCalculator_.mState;
        } else {
            if (Log.isDebug()) Log.d(TAG, "last location not known, init to day");
            currentState_ = IS_DAY;
        }

        handler_ = new Handler();
        timeCallback_ = new Runnable() {
            @Override
            public void run() {
                Location l = getLastKnownLocation();
                if (Log.isVerbose()) Log.v(TAG, "location " + l);
                if (l != null){
                    onLocationChanged(l);
                }
                startTimeCheck();
            }
        };

        startTimeCheck();
    }

    private Location getLastKnownLocation() {
        List<String> providers = locationManager_.getProviders(true);
        Location bestLocation = null;
        for (String provider : providers) {
            Location l = locationManager_.getLastKnownLocation(provider);
            if (l == null) {
                continue;
            }
            if (bestLocation == null || l.getAccuracy() < bestLocation.getAccuracy()) {
                // Found best last known location: %s", l);
                bestLocation = l;
            }
        }
        return bestLocation;
    }

    private void startTimeCheck() {
        handler_.postDelayed(timeCallback_, 60*1000); // 10*60*1000
    }

    @Override
    public void onLocationChanged(@NonNull Location location) {
        if (Log.isVerbose()) Log.v(TAG, "time: " + System.currentTimeMillis() + ", lat: " + location.getLatitude() + ", log: " + location.getLongitude());
        twilightCalculator_.calculateTwilight(System.currentTimeMillis(), location.getLatitude(), location.getLongitude());

        if (currentState_ != twilightCalculator_.mState){
            if (Log.isInfo()) Log.i(TAG, "state changed, new state " + twilightCalculator_.mState);
            currentState_ = twilightCalculator_.mState;
            if (listener_ != null) {
                listener_.onDayNightUpdate(isNight());
            }
        }
    }

    @Override
    public void onStatusChanged(String s, int i, Bundle bundle) {

    }

    @Override
    public void onProviderEnabled(String s) {

    }

    @Override
    public void onProviderDisabled(String s) {

    }

    @Override
    public void stop() {
        if (Log.isInfo()) Log.i(TAG, "stop");
        if (locationManager_ != null){
            if (Log.isDebug()) Log.d(TAG, "remove location update");
            locationManager_.removeUpdates(this);
        }
        if (handler_ != null){
            if (Log.isDebug()) Log.d(TAG, "remove handler callbacks");
            handler_.removeCallbacks(timeCallback_);
        }
    }

    @Override
    public boolean isNight() {
        return currentState_ == TwilightCalculator.NIGHT;
    }

    @Override
    public void setListener(Listener listener) {
        listener_ = listener;
    }
}
