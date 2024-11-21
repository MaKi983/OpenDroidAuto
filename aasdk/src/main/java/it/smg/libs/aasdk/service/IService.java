package it.smg.libs.aasdk.service;

import androidx.annotation.Keep;

public interface IService {

    @Keep
    void start();
    @Keep
    void stop();
    void delete();
    @Keep
    void onError(String error, int code);
}
