package it.smg.hu.ui.notification;

import android.content.Context;
import android.content.Intent;
import android.view.DragEvent;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import androidx.coordinatorlayout.widget.CoordinatorLayout;

import it.smg.hu.R;
import it.smg.hu.config.Settings;
import it.smg.hu.ui.PlayerActivity;
import it.smg.libs.common.Log;

public class AppBadge extends Notification{

    private static final String TAG = "AppBadge";

    private static AppBadge instance_;
    private int x_;
    private int y_;
    private int oriX_;
    private int oriY_;

    public static void init(Context ctx){
        if (instance_ == null){
            instance_ = new AppBadge(ctx);
        }
    }

    public static AppBadge instance(){
        return instance_;
    }

    private AppBadge(Context ctx){
        super(ctx);

        x_ = -1;
        y_ = -1;

        setDismissible(false);

        layoutResId_ = R.layout.app_badge;
        viewResId_ = R.id.app_badge_content;

        setOnClickListener(view -> {
            Intent i = new Intent(ctx_, PlayerActivity.class);
            i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
            ctx_.startActivity(i);
            dismiss();
        });
    }

    @Override
    protected void initPosition() {
        WindowManager.LayoutParams params = dialog_.getWindow().getAttributes();
        params.gravity = Gravity.TOP | Gravity.LEFT; //Gravity.BOTTOM;
        params.horizontalMargin = 0.01f;
        params.verticalMargin = 0.01f;
    }

    @Override
    protected void createUi(INotificationEvent event) {
        CoordinatorLayout l = dialog_.findViewById(R.id.app_badge_root);
        View v = dialog_.findViewById(R.id.app_badge_content);


        v.setOnLongClickListener(view -> {
            int[] pos = new int[2];
            l.getLocationOnScreen(pos);
            oriX_ = pos[0];
            oriY_ = pos[1];
            if (Log.isVerbose()) Log.v(TAG, "start drag position= x: " + oriX_ + " y: " + oriY_);
            View.DragShadowBuilder myShadow = new View.DragShadowBuilder(l);
            v.startDrag(null,myShadow,null,0);
            return true;
        });

        v.setOnDragListener(new View.OnDragListener() {
            @Override
            public boolean onDrag(View view, DragEvent dragEvent) {

                switch (dragEvent.getAction()){
                    case DragEvent.ACTION_DROP:
                        x_ = (int) dragEvent.getX();
                        y_ = (int) dragEvent.getY();
                        if (Log.isDebug()) Log.d(TAG, "(ACTION_DROP) dragEvent new position x= " + x_ + " y= " + y_);
                        return true;
                    case DragEvent.ACTION_DRAG_STARTED:
                    case DragEvent.ACTION_DRAG_LOCATION:
                        x_ = (int) (dragEvent.getX());
                        y_ = (int) (dragEvent.getY());
                        if (Log.isDebug()) Log.d(TAG, "dragEvent new position x= " + x_ + " y= " + y_);
                        return true;
//                        break;
                    case DragEvent.ACTION_DRAG_ENDED:
                        if (Log.isDebug()) Log.d(TAG, "dragEvent set new position x= " + x_ + " y= " + y_);
//                        CoordinatorLayout.LayoutParams params = (CoordinatorLayout.LayoutParams)view.getLayoutParams();
                        Window w = dialog_.getWindow();
                        WindowManager.LayoutParams params = w.getAttributes();
                        params.x = x_;
                        params.y = y_;
                        w.setAttributes(params);
                        w.clearFlags(WindowManager.LayoutParams.FLAG_DIM_BEHIND);
                        return true;
//                        break;
                }

                return false;
            }
        });
    }

    @Override
    protected void initWipeAction() {}

    @Override
    public void show() {
        if (Settings.instance().video.showAppBadge()) {

            Intent odaServiceIntent = new Intent(ctx_, AppBadge.class);
            ctx_.startService(odaServiceIntent);
            super.show();
        }
    }

    @Override
    public void dismiss() {
        if (Settings.instance().video.showAppBadge()) {
            super.dismiss();
        }
    }

    @Override
    protected String tag() {
        return TAG;
    }
}
