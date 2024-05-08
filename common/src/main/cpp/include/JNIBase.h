#pragma once

#include <jni.h>
#include <string>


class JNIBase {
public:
    JNIBase(JNIEnv* env, jobject androidApp, std::string name);
    ~JNIBase();

    static int javaAttachThread(const char* threadName, JNIEnv** pEnv);
    static int javaDetachThread(void);

protected:
    static JNIEnv* getJniEnv ();

    virtual void initJavaMethods();

    jobject androidClass_;

private:
    std::string name_;

};

