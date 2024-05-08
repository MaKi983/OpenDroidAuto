#include "it_smg_libs_aasdk_transport_Transport.h"

jfieldID JTransport::handleId = nullptr;

JTransport::Pointer JTransport::getJTransport(JNIEnv *env, jobject jtransport) {
    return (JTransport::Pointer)env->GetLongField(jtransport, JTransport::handleId);
}

ITransport::Pointer JTransport::getTransport() {
    return transport_;
}
