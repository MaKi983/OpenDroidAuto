#include "it_smg_libs_aasdk_transport_USBTransport.h"
#include "it_smg_libs_aasdk_usb_AOAPDevice.h"
#include "it_smg_libs_aasdk_Runtime.h"

jfieldID JUSBTransport::handleId = nullptr;

JUSBTransport::JUSBTransport(JNIEnv *env, jobject jusbtransport, jobject jaoapdevice)
        : JNIBase(env, jusbtransport, "JUSBTransport"){
    JAOAPDevice::Pointer jAOAPDevice = JAOAPDevice::getJAOAPDevice(env, jaoapdevice);

    transport_ = std::make_shared<USBTransport>(JRuntime::ioService(), jAOAPDevice->getAOAPDevice());
}

JUSBTransport::Pointer JUSBTransport::getJUSBTransport(JNIEnv *env, jobject jusbtransport) {
    return (JUSBTransport::Pointer)env->GetLongField(jusbtransport, JUSBTransport::handleId);
}

ITransport::Pointer JUSBTransport::getTransport() {
    return transport_;
}

JUSBTransport::~JUSBTransport() {
//    transport_.reset();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_transport_USBTransport_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/transport/USBTransport");
    JUSBTransport::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_transport_USBTransport_nativeSetup(JNIEnv *env, jobject thiz, jobject jaoapdevice) {
    auto jusbtransport = new JUSBTransport(env, thiz, jaoapdevice);
    return (jlong)((size_t)jusbtransport);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_transport_USBTransport_nativeStop(JNIEnv *env, jobject thiz) {
    JUSBTransport::Pointer jUSBTransport = JUSBTransport::getJUSBTransport(env, thiz);
    jUSBTransport->getTransport()->stop();
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_transport_USBTransport_nativeDelete(JNIEnv *env, jobject thiz) {
    JUSBTransport::Pointer jUSBTransport = JUSBTransport::getJUSBTransport(env, thiz);
    delete jUSBTransport;
}