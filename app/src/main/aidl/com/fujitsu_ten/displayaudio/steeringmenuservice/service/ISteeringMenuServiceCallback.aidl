package com.fujitsu_ten.displayaudio.steeringmenuservice.service;

interface ISteeringMenuServiceCallback {

    boolean onFinishView(boolean z, boolean z2);

    void onShowView();

    boolean onSteeringSWDown(int i);

}