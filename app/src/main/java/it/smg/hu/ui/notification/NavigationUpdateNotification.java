package it.smg.hu.ui.notification;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.widget.ImageView;
import android.widget.TextView;

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;

import it.smg.hu.R;
import it.smg.hu.ui.PlayerActivity;

import it.smg.libs.aasdk.projection.INavigationStatusEvent.DistanceEvent;
import it.smg.libs.aasdk.projection.INavigationStatusEvent.TurnEvent;
import it.smg.libs.common.Log;

public class NavigationUpdateNotification extends Notification {

    private static final String TAG = "NavigationUpdateNotification";

    private static NavigationUpdateNotification instance_;

    public static NavigationUpdateNotification instance(Context ctx){
        if (instance_ == null){
            instance_ = new NavigationUpdateNotification(ctx);
        }

        return instance_;
    }

    private NavigationUpdateNotification(Context ctx){
        super(ctx);

        layoutResId_ = R.layout.navigation_update_notification;
        contentResId_ = R.id.navigation_update_layout;
        viewResId_ = R.id.navigation_update_content;
        toggleDismissBtnResId_ = R.id.navigation_update_toggle_dismiss;
        dismissBtnResId_ = R.id.navigation_update_dismiss;
        dismissLayoutResId_ = R.id.navigation_update_dismiss_layout;

        setOnClickListener(view -> {
            Intent i = new Intent(ctx_.getApplicationContext(), PlayerActivity.class);
            i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
            ctx_.getApplicationContext().startActivity(i);
            dismiss();
        });
    }

    @Override
    protected void createUi(INotificationEvent event){
        NavigationUpdateEvent navUpdateEvent = (NavigationUpdateEvent) event;
        TurnEvent turnEvent = navUpdateEvent.turnEvent;
        DistanceEvent distanceEvent = navUpdateEvent.distanceEvent;

        // set album art image
        ImageView imageView = dialog_.findViewById(R.id.navigation_update_image);
        if (turnEvent.image != null){
            ByteBuffer buffer = turnEvent.image.asReadOnlyBuffer();
            byte[] image = new byte[buffer.limit()];
            buffer.get(image);
            BitmapFactory.Options opts = new BitmapFactory.Options();
//            opts.inPreferredConfig = Bitmap.Config.ARGB_8888;
//            opts.inDither = false;
            Bitmap bmp = BitmapFactory.decodeByteArray(image, 0, image.length, opts);
            if (bmp != null) {
//                ByteArrayOutputStream out = new ByteArrayOutputStream(image.length);
//                bmp.compress(Bitmap.CompressFormat.PNG, 100, out);
//                byte[] imgOk = out.toByteArray();
//
//                Bitmap bmpOk = BitmapFactory.decodeByteArray(imgOk, 0, imgOk.length);
//                if (bmpOk != null) {
                imageView.setImageBitmap(Bitmap.createScaledBitmap(bmp, 256, 256, false));
//                    imageView.setImageBitmap(bmpOk);
//                }
            }
        }

        // set street
        TextView streetTxt = dialog_.findViewById(R.id.navigation_update_street);
        streetTxt.setText(turnEvent.street);

        // set distance
        TextView distanceTxt = dialog_.findViewById(R.id.navigation_update_dist);
        distanceTxt.setText(distanceEvent.meters + " " + distanceEvent.distanceUnit);

        // set time
        TextView timeTxt = dialog_.findViewById(R.id.navigation_update_time);
        timeTxt.setText(formatTime(distanceEvent.timeToStepSeconds));

    }

    private String formatTime(long timeInSeconds){

        // set time to step
        long minutes = timeInSeconds / 60;
        long seconds = timeInSeconds % 60;
        long hours = minutes / 60;
        minutes = minutes % 60;

        StringBuilder formatted = new StringBuilder();
        if (hours > 0){
            formatted.append(hours).append("h ");
        }

        if (minutes > 0){
            formatted.append(minutes).append("m ");
        }

        if (seconds > 0){
            formatted.append(seconds).append("s");
        }

        return formatted.toString();
    }

    @Override
    protected String tag() {
        return TAG;
    }
}
