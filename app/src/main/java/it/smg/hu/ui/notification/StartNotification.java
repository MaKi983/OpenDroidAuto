package it.smg.hu.ui.notification;

import android.content.Context;
import android.content.Intent;
import android.view.Gravity;

import it.smg.hu.R;
import it.smg.hu.service.ODAService;
import it.smg.hu.ui.PlayerActivity;

public class StartNotification extends Notification {

    private static final String TAG = "StartNotification";

    private static StartNotification instance_;

    public static StartNotification instance(Context ctx){
        if (instance_ == null){
            instance_ = new StartNotification(ctx);
        }

        return instance_;
    }

    private StartNotification(Context ctx){
        super(ctx);

        setDismissible(false);
        setPosition(Gravity.TOP);

        layoutResId_ = R.layout.usb_start_notification;
        viewResId_ = R.id.usb_start_content;

        setOnClickListener(view -> {
            Intent i = new Intent(ctx_.getApplicationContext(), PlayerActivity.class);
            i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
            i.putExtra("mode", ODAService.MODE_USB);
            ctx_.getApplicationContext().startActivity(i);
            dialog_.dismiss();
        });
    }

    @Override
    protected void createUi(INotificationEvent event) {}

    @Override
    protected String tag() {
        return TAG;
    }
}
