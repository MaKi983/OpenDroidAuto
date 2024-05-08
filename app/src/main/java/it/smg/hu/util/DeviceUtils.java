package it.smg.hu.util;

public class DeviceUtils {

    public static final boolean isX86 = "i686".equalsIgnoreCase(System.getProperty("os.arch"));
}
