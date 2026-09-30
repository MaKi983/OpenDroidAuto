package it.smg.hu.manager;

import android.content.Context;

/** Keeps proprietary Honda APIs out of the generic-device startup path. */
public final class HondaPlatform {
    private static Context context_;
    private static Boolean available_;

    private HondaPlatform() {}

    public static void init(Context context) {
        context_ = context.getApplicationContext();
        available_ = null;
    }

    public static boolean isAvailable() {
        if (available_ != null) {
            return available_;
        }
        boolean available = false;
        try {
            Class.forName("com.fujitsu_ten.displayaudio.whitelist.common.IWhiteList");
            available = context_ != null && context_.getSystemService("ModeMgrService") != null;
        } catch (Throwable ignored) {
            available = false;
        }
        available_ = available;
        return available;
    }
}
