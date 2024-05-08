package it.smg.libs.aasdk.projection;

public interface ISensor {

    static final String DAY = "DAY";
    static final String NIGHT = "NIGHT";
    static final String TIME_GPS = "TIME/GPS";
    static final String LIGHT = "LIGHT";

    static final int IS_DAY = 0;
    static final int IS_NIGHT = 1;

    void stop();
    boolean isNight();
    void setListener(Listener listener);

    interface Listener {
        void onDayNightUpdate(boolean isNight);
    }
}
