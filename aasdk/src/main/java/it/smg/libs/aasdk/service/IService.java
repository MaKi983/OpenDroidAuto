package it.smg.libs.aasdk.service;

public interface IService {

    void start();
    void stop();
    void delete();
    void onError(String error, int code);
}
