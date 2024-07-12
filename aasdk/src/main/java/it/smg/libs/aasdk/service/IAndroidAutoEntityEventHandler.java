package it.smg.libs.aasdk.service;

public interface IAndroidAutoEntityEventHandler extends IVideoEventHandler {
    void onAndroidAutoQuit();
    void onAndroidAutoQuitOnError(String error, int nativeErrorCode);
}
