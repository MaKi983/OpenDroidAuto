package it.smg.libs.aasdk.service;

public interface IAndroidAutoEntityEventHandler {
    void onAndroidAutoQuit();
    void onAndroidAutoQuitOnError(String error, int nativeErrorCode);
}
