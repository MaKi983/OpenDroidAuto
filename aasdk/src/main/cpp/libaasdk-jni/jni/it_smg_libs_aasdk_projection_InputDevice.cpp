#include <Log.h>
#include <proto/InputEventIndicationMessage.pb.h>
#include "it_smg_libs_aasdk_projection_InputDevice.h"
#include "projection/IInputDeviceEventHandler.h"

using namespace projection;

jfieldID JInputDevice::handleId = nullptr;

JInputDevice::JInputDevice(JNIEnv *env, jobject jinputdevice) :
        JNIBase(env, jinputdevice, "JInputDevice"){

    initJavaMethods();
}

JInputDevice::Pointer JInputDevice::getJInputDevice(JNIEnv *env, jobject jinputdevice) {
    return (JInputDevice::Pointer)env->GetLongField(jinputdevice, JInputDevice::handleId);
}

void JInputDevice::initJavaMethods() {
    JNIEnv* env = getJniEnv();
    jclass cls = env->GetObjectClass(androidClass_);

    startMethodId_ = env->GetMethodID(cls, "start", "()V");
    stopMethodId_ = env->GetMethodID(cls, "stop", "()V");
    getSupportedButtonCodesMethodId_ = env->GetMethodID(cls, "getSupportedButtonCodes", "()[I");
    hasTouchscreenMethodId_ = env->GetMethodID(cls, "hasTouchscreen", "()Z" );
    getTouchscreenGeometryMethodId_ = env->GetMethodID(cls, "getTouchscreenGeometry", "()Lit/smg/libs/aasdk/projection/IInputDevice$Rect;");

    env->DeleteLocalRef(cls);
}

void JInputDevice::start(IInputDeviceEventHandler& eventHandler) {
    JNIEnv* env = getJniEnv();
    env->CallVoidMethod(androidClass_, startMethodId_);
    eventHandler_ = &eventHandler;
}

void JInputDevice::stop() {
    JNIEnv* env = getJniEnv();
    env->CallVoidMethod(androidClass_, stopMethodId_);
    eventHandler_ = nullptr;
}

projection::IInputDevice::ButtonCodes JInputDevice::getSupportedButtonCodes() {
    JNIEnv* env = getJniEnv();

    ButtonCodes c;
    jintArray jarr = (jintArray) env->CallObjectMethod(androidClass_, getSupportedButtonCodesMethodId_);
    jint len = env->GetArrayLength(jarr);
    if (len > 0){
        jint* jarrElems = env->GetIntArrayElements(jarr, JNI_FALSE);
        for (int i = 0; i < len; i++){
            aasdk::proto::enums::ButtonCode_Enum code = int2ButtonCode(jarrElems[i]);
            if (code != aasdk::proto::enums::ButtonCode::NONE){
                c.push_back(code);
            }
        }
    }

    return c;
}

aasdk::proto::enums::ButtonCode::Enum JInputDevice::int2ButtonCode(int c){
    switch (c) {
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_MICROPHONE_2:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_MICROPHONE_2;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_MENU:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_MENU;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_HOME:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_HOME;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_BACK:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_BACK;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_PHONE:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_PHONE;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_CALL_END:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_CALL_END;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_UP:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_UP;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_DOWN:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_DOWN;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_LEFT:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_LEFT;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_RIGHT:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_RIGHT;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_ENTER:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_ENTER;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_MICROPHONE_1:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_MICROPHONE_1;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_TOGGLE_PLAY:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_TOGGLE_PLAY;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_NEXT:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_NEXT;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_PREV:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_PREV;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_PLAY:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_PLAY;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_PAUSE:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_PAUSE;
        case aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_SCROLL_WHEEL:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_SCROLL_WHEEL;
        default:
            return aasdk::proto::enums::ButtonCode::Enum::ButtonCode_Enum_NONE;
    }
}

bool JInputDevice::hasTouchscreen() {
    JNIEnv* env = getJniEnv();
    jboolean ret = env->CallBooleanMethod(androidClass_, hasTouchscreenMethodId_);
    return ret == JNI_TRUE;
}

Rect JInputDevice::getTouchscreenGeometry() {
    JNIEnv* env = getJniEnv();
    jobject ret = env->CallObjectMethod(androidClass_, getTouchscreenGeometryMethodId_);

    jclass rectClass = env->GetObjectClass(ret);
    jfieldID widthFieldId = env->GetFieldID(rectClass,"width","I");
    jfieldID heightFieldId = env->GetFieldID(rectClass,"height","I");

    jint width = env->GetIntField(ret, widthFieldId);
    jint height = env->GetIntField(ret, heightFieldId);

    Rect geom;
    geom.width = width;
    geom.height = height;

    env->DeleteLocalRef(ret);
    ret = nullptr;

    env->DeleteLocalRef(rectClass);
    rectClass = nullptr;

    return geom;
}

void JInputDevice::sendTouchEvent(aasdk::proto::messages::InputEventIndication inputEventIndication) {
    if (eventHandler_ != nullptr){
        eventHandler_->onTouchEvent(inputEventIndication);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_InputDevice_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/projection/InputDevice");
    JInputDevice::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_projection_InputDevice_nativeSetup(JNIEnv *env, jobject thiz) {
    JInputDevice::Pointer jInputDevice = new JInputDevice(env, thiz);
    return (jlong)((size_t)jInputDevice);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_InputDevice_sendTouchEvent(JNIEnv *env, jobject thiz, jint action,
                           jint index, jobjectArray actions) {
    JInputDevice::Pointer jInputDevice = JInputDevice::getJInputDevice(env, thiz);

    aasdk::proto::enums::TouchAction::Enum type;
    auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
    aasdk::proto::messages::InputEventIndication inputEventIndication;
    inputEventIndication.set_timestamp(timestamp.count());
    auto touchEvent = inputEventIndication.mutable_touch_event();

    switch(action){
        case 0: // ACTION_DOWN
            type = aasdk::proto::enums::TouchAction::PRESS;
            break;
        case 1: // ACTION_UP
        case 3: // ACTION_CANCEL
            type = aasdk::proto::enums::TouchAction::RELEASE;
            break;
        case 2: // ACTION_MOVE
            type = aasdk::proto::enums::TouchAction::DRAG;
            break;
        case 5: // ACTION_POINTER_DOWN
            type = aasdk::proto::enums::TouchAction::POINTER_DOWN;
            break;
        case 6: // ACTION_POINTER_UP
            type = aasdk::proto::enums::TouchAction::POINTER_UP;
            break;
        default:
            if(Log::isWarn()) Log_w("unknown action %d", action);
    }

    touchEvent->set_touch_action(type);
    touchEvent->set_action_index(index);

    jint len = env->GetArrayLength(actions);
    for (int i = 0; i < len; i++){
        jobject jtouch = env->GetObjectArrayElement(actions, i);

        jclass taClass = env->GetObjectClass(jtouch);
        jfieldID xFieldId = env->GetFieldID(taClass,"x","I");
        jfieldID yFieldId = env->GetFieldID(taClass,"y","I");
        jfieldID pointerIdFieldId = env->GetFieldID(taClass,"pointerId","I");

        jint x = env->GetIntField(jtouch, xFieldId);
        jint y = env->GetIntField(jtouch, yFieldId);
        jint pointerId = env->GetIntField(jtouch, pointerIdFieldId);

        if(Log::isVerbose()) Log_v("Touch action: [x= %d, y= %d, pointerId= %d, action= %d, actionIndex= %d, timestamp= %ld", x, y, pointerId, action, index, timestamp);

        auto touchLocation = touchEvent->add_touch_location();
        touchLocation->set_x(x);
        touchLocation->set_y(y);
        touchLocation->set_pointer_id(pointerId);

        env->DeleteLocalRef(taClass);
        taClass = nullptr;

        env->DeleteLocalRef(jtouch);
        jtouch = nullptr;
    }

    jInputDevice->sendTouchEvent(inputEventIndication);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_InputDevice_nativeDelete(JNIEnv *env, jobject thiz) {
    JInputDevice::Pointer jInputDevice = JInputDevice::getJInputDevice(env, thiz);
    delete jInputDevice;
}