package it.smg.hu.ui.notification;

public interface INotification {

    void show(INotificationEvent event);
    void show();
    void dismiss();
    boolean isShowing();
    boolean isTurnedOff();
}
