package com.fujitsu_ten.displayaudio.steeringmenuservice.service;

import com.fujitsu_ten.displayaudio.steeringmenuservice.service.ISteeringMenuServiceCallback;

interface ISteeringMenuService {

    void doModeMgrRequest(int i);

    IBinder getIMidIf();

    void notifySteeringMenuDispMode(int i, int i2);

    void registerCallback(ISteeringMenuServiceCallback iSteeringMenuServiceCallback);

    void registerCallbackEx(ISteeringMenuServiceCallback iSteeringMenuServiceCallback, int i);

    void requestFinishView();

    void requestFinishViewEx(boolean z);

    void startAudioLauncher();

    void startAudioLauncherPopup();

    void unregisterCallback(ISteeringMenuServiceCallback iSteeringMenuServiceCallback);

    void unregisterCallbackEx(ISteeringMenuServiceCallback iSteeringMenuServiceCallback, int i);

}