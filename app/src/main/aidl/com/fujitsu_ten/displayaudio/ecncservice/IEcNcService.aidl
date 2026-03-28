package com.fujitsu_ten.displayaudio.ecncservice;

interface IEcNcService {
    int startVR(boolean enable);
    int endVr();
    int startSiri();
    int endSiri();
    int startHft();
    int endHft();
}
