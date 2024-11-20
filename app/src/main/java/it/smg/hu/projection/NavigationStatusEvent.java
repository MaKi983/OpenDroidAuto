package it.smg.hu.projection;

import androidx.annotation.Keep;

import it.smg.hu.config.Settings;
import it.smg.hu.ui.PlayerActivity;
import it.smg.hu.ui.notification.NotificationFactory;
import it.smg.libs.common.Log;

public class NavigationStatusEvent extends it.smg.libs.aasdk.projection.NavigationStatusEvent {

    private static final String TAG = "NavigationStatusEvent";

    private final Settings settings_;
    private final NotificationFactory notificationFactory_;

    private TurnEvent turnEvent_;
    private DistanceEvent distanceEvent_;

    public NavigationStatusEvent(){
        settings_ = Settings.instance();
        notificationFactory_ = NotificationFactory.instance();
    }

    @Keep
    @Override
    public void navigationStatusUpdate(NavigationStatus navigationStatus) {
        if (!settings_.video.showNavigationNotification()) {
            return;
        }

        if (Log.isDebug()) Log.d(TAG, "navigationStatusUpdate: " + navigationStatus);
    }

    @Keep
    @Override
    public void navigationTurnEvent(TurnEvent turnEvent) {
        if (!settings_.video.showNavigationNotification()) {
            return;
        }

        turnEvent_ = turnEvent;

        if (distanceEvent_ != null){
            if (!PlayerActivity.isActive()){
                notificationFactory_.notifyNavigationEvent(turnEvent_, distanceEvent_);
            }
        }
    }

    @Keep
    @Override
    public void navigationDistanceEvent(DistanceEvent distanceEvent) {
        if (!settings_.video.showNavigationNotification()) {
            return;
        }

        distanceEvent_ = distanceEvent;

        if (turnEvent_ != null){
            if (!PlayerActivity.isActive()){
                notificationFactory_.notifyNavigationEvent(turnEvent_, distanceEvent_);
            }
        }
    }
}
