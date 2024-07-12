#pragma once

#include <JNIBase.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "OMXVideoCodec.h"

class JOMXVideoCodec : JNIBase {
public:
    typedef JOMXVideoCodec* Pointer;

    JOMXVideoCodec(JNIEnv *env, jobject jvideocodec);

    static jfieldID handleId;
    static JOMXVideoCodec::Pointer getJOMXVideoCodec(JNIEnv* env, jobject jvideocodec);

    void setNativeWindow(ANativeWindow* nativeWindow, int height, int width);
    status_t init(int fps);
    void shutdown();
    void queueBuffer(common::DataConstBuffer &b, int64_t timestamp);
    void setSps(common::DataConstBuffer &b);

    ~JOMXVideoCodec();
protected:
    void initJavaMethods() override;

private:
    OMXVideoCodec::Pointer omxVideoCodec_;
};

