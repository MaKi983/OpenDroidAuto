#pragma once

#include <JNIBase.h>
#include <service/IPinger.h>

using namespace service;

class JPinger : JNIBase {
public:
    typedef JPinger* Pointer;

    JPinger(JNIEnv *env, jobject jpinger, int duration);
    ~JPinger();

    static jfieldID handleId;
    static JPinger::Pointer getJPinger(JNIEnv* env, jobject jpinger);

    IPinger::Pointer getPinger();

    void cancel();
private:
    IPinger::Pointer pinger_;

};
