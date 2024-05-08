package it.smg.libs.aasdk.service;

public interface IAndroidAutoEntity {
    void start(IAndroidAutoEntityEventHandler eventHandler);
    void releaseFocus();
    void gainFocus();
    void stop();
}
