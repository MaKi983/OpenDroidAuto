package it.smg.hu.ui.notification;

import android.app.Notification;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

import androidx.core.app.NotificationCompat;

import it.smg.hu.R;
import it.smg.hu.ui.PlayerActivity;
import it.smg.libs.aasdk.projection.IMediaStatusEvent.*;
import it.smg.libs.aasdk.projection.INavigationStatusEvent.*;

public class NotificationFactory {

    private static final String TAG = "NotificationFactory";

    public static final int ODASERVICE_NOTIFICATION_ID = 100000;
    public static final int NAVIGATION_NOTIFICATION_ID = 100001;
    public static final int MEDIA_NOTIFICATION_ID = 100002;

    private Context ctx_;
    private static NotificationFactory factory_;
    private INotification mediaUpdateNotification_;
    private INotification navigationUpdateNotification_;
    private INotification startNotification_;
    private INotification appBadge_;

    private long lastTime_;
    private static final long NOTIFICATION_TIMEOUT = 1 * 1000;

    public static void init(Context ctx){
        if (factory_ == null){
            factory_ = new NotificationFactory(ctx);
        }
    }

    public static NotificationFactory instance() {
        return factory_;
    }

    private NotificationFactory(Context ctx){
        ctx_ = ctx;
        lastTime_ = System.currentTimeMillis();
        mediaUpdateNotification_ = MediaUpdateNotification.instance(ctx_);
        navigationUpdateNotification_ = NavigationUpdateNotification.instance(ctx_);
        startNotification_ = StartNotification.instance(ctx_);
        appBadge_ = AppBadge.instance();
    }

    public Notification create(){
        Intent i = new Intent(ctx_, PlayerActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(ctx_, 0, i, PendingIntent.FLAG_UPDATE_CURRENT);

        NotificationCompat.Builder builder = new NotificationCompat.Builder(ctx_, "OpenDroidAuto")
                .setSmallIcon(R.drawable.logo)
                .setContentTitle(ctx_.getText(R.string.main_notification_title))
                .setContentText(ctx_.getText(R.string.main_notification_content))
                .setContentIntent(pendingIntent)
                .setOngoing(true)
                .setPriority(NotificationCompat.PRIORITY_LOW);
        return builder.build();
    }

    public void notifyStartRequest(){
        startNotification_.show();
    }

    public void notifyNavigationEvent(TurnEvent turnEvent, DistanceEvent distanceEvent){
        if (canShowNotification()  && !navigationUpdateNotification_.isTurnedOff()){
            navigationUpdateNotification_.show(new NavigationUpdateEvent(turnEvent, distanceEvent));
        }
    }

    public void notifyMediaMetadataUpdate(PlaybackUpdateEvent playbackUpdateEvent, MetadataUpdateEvent metadataUpdateEvent){
        if (canShowNotification() && !navigationUpdateNotification_.isShowing() && !mediaUpdateNotification_.isTurnedOff()){
            mediaUpdateNotification_.show(new MediaUpdateEvent(playbackUpdateEvent, metadataUpdateEvent));
        }
    }

    private boolean canShowNotification(){
        if (lastTime_ < System.currentTimeMillis() - NOTIFICATION_TIMEOUT) {
            lastTime_ = System.currentTimeMillis();
            return true;
        }
        return false;
    }

    public void dismissAll(){
        mediaUpdateNotification_.dismiss();
        navigationUpdateNotification_.dismiss();
    }
}
