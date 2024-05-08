package it.smg.hu.ui.notification;

import android.app.Dialog;
import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;

import androidx.coordinatorlayout.widget.CoordinatorLayout;

import com.google.android.material.behavior.SwipeDismissBehavior;

import it.smg.hu.R;
import it.smg.libs.common.Log;

abstract class Notification implements INotification {

    protected Dialog dialog_;
    protected Handler handler_;
    protected Context ctx_;

    protected int layoutResId_;
    protected int viewResId_;
    protected int toggleDismissBtnResId_;
    protected int dismissBtnResId_;
    protected int dismissLayoutResId_;
    protected int contentResId_;

    private boolean isDismissible_;
    private boolean isTurnedOff_;
    private int position_;

    private boolean isDismissing;

    protected View.OnClickListener onClickListener_;

    /* package */ Notification(Context ctx){
        ctx_ = ctx;
        handler_ = new Handler(Looper.getMainLooper());
        isDismissing = false;
        isDismissible_ = true;
        isTurnedOff_ = false;
        position_ = Gravity.BOTTOM;
    }

    protected void initDismiss(){
        Button toggleDismissBtn = dialog_.findViewById(toggleDismissBtnResId_);
        toggleDismissBtn.setOnClickListener(view -> {
            if (dialog_!= null) {
                if (!isDismissing) {
                    dialog_.findViewById(contentResId_).setVisibility(View.GONE);
                    dialog_.findViewById(dismissLayoutResId_).setVisibility(View.VISIBLE);
                } else {
                    dialog_.findViewById(contentResId_).setVisibility(View.VISIBLE);
                    dialog_.findViewById(dismissLayoutResId_).setVisibility(View.GONE);
                }
                isDismissing = true;
            }
        });

        Button dismiss = dialog_.findViewById(dismissBtnResId_);
        dismiss.setOnClickListener(view -> {
            if (Log.isDebug()) Log.d(tag(), "turn off notification");
            isTurnedOff_ = true;
            dialog_.dismiss();
        });
    }

    protected void initPosition(){
        WindowManager.LayoutParams params = dialog_.getWindow().getAttributes();
        params.gravity = position_; //Gravity.BOTTOM;
        params.verticalMargin = 0.01f;
    }

    protected void initWipeAction(){
        View view = dialog_.findViewById(viewResId_);
        CoordinatorLayout.LayoutParams params = (CoordinatorLayout.LayoutParams) view.getLayoutParams();
        SwipeDismissBehavior<View> behavior = new SwipeDismissBehavior<>();
        behavior.setSwipeDirection(SwipeDismissBehavior.SWIPE_DIRECTION_ANY);

        behavior.setListener(new SwipeDismissBehavior.OnDismissListener() {
            @Override
            public void onDismiss(final View view) {
                if (dialog_ != null) {
                    dialog_.dismiss();
                }
            }

            @Override
            public void onDragStateChanged(int i) {}
        });

        params.setBehavior(behavior);
    }

    public void show(){
        show(null);
    }

    public void show(INotificationEvent event) {
        handler_.post(() -> {

            if (dialog_ == null) {
                dialog_ = new Dialog(ctx_, R.style.notificationPopup);
                dialog_.setContentView(layoutResId_);
                dialog_.getWindow().setFlags(WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL, WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL);
                dialog_.getWindow().setType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
                dialog_.getWindow().getAttributes().windowAnimations = R.style.animation;
                dialog_.setCancelable(true);

                initWipeAction();
                initPosition();

                dialog_.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_DIM_BEHIND);

                if (isDismissible_) {
                    initDismiss();
                }

                dialog_.setOnCancelListener(dialogInterface -> {
                });

                dialog_.setOnDismissListener(dialogInterface -> {
                    if (Log.isDebug()) Log.d(tag(), "onDismiss");
                    dialog_ = null;
                });

                View view = dialog_.findViewById(viewResId_);
                view.setOnClickListener(onClickListener_);
            }

            createUi(event);

            if (!dialog_.isShowing()) {
                dialog_.show();
            }

        });

    }

    public void dismiss(){
        if (dialog_ != null) {
            dialog_.dismiss();
        }
        // restore notification in case of it has been turned off
        isTurnedOff_ = false;
    }

    public void setDismissible(boolean isDismissible){
        this.isDismissible_ = isDismissible;
    }

    public void setPosition(int position){
        this.position_ = position;
    }

    @Override
    public boolean isShowing(){
        if (dialog_ != null){
            return dialog_.isShowing();
        }
        return false;
    }

    @Override
    public boolean isTurnedOff(){
        return isTurnedOff_;
    }

    protected abstract void createUi(INotificationEvent event);

    protected abstract String tag();

    public void setOnClickListener(View.OnClickListener l){
        this.onClickListener_ = l;
    }

}
