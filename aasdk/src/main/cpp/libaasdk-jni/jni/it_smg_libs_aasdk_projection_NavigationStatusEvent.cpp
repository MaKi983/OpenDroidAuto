#include "it_smg_libs_aasdk_projection_NavigationStatusEvent.h"
#include <Log.h>

using namespace projection;

jfieldID JNavigationStatusEvent::handleId = nullptr;

JNavigationStatusEvent::JNavigationStatusEvent(JNIEnv *env, jobject jnavigationstatusevent) :
        JNIBase(env, jnavigationstatusevent, "JNavigationStatusEvent"){

    initJavaMethods();
}

JNavigationStatusEvent::~JNavigationStatusEvent(){
    JNIEnv* env = getJniEnv();
    env->DeleteGlobalRef(navigationStatusClass_);
    env->DeleteGlobalRef(maneuverTypeClass_);
    env->DeleteGlobalRef(maneuverDirectionClass_);
    env->DeleteGlobalRef(turnEventClass_);
    env->DeleteGlobalRef(distanceUnitClass_);
    env->DeleteGlobalRef(distanceEventClass_);
}

JNavigationStatusEvent::Pointer JNavigationStatusEvent::getJNavigationStatusEvent(JNIEnv *env, jobject jnavigationstatusevent) {
    return (JNavigationStatusEvent::Pointer)env->GetLongField(jnavigationstatusevent, JNavigationStatusEvent::handleId);;
}

void JNavigationStatusEvent::initJavaMethods() {
    JNIEnv* env = getJniEnv();
    jclass cls = env->GetObjectClass(androidClass_);

    navigationTurnEventMethodId_ = env->GetMethodID(cls, "navigationTurnEvent", "(Lit/smg/libs/aasdk/projection/INavigationStatusEvent$TurnEvent;)V");
    navigationStatusMethodId_ = env->GetMethodID(cls, "navigationStatusUpdate", "(Lit/smg/libs/aasdk/projection/INavigationStatusEvent$NavigationStatus;)V");
    navigationDistanceEventMethodId_ = env->GetMethodID(cls, "navigationDistanceEvent", "(Lit/smg/libs/aasdk/projection/INavigationStatusEvent$DistanceEvent;)V");

    jclass navigationStatusClass = env->FindClass("it/smg/libs/aasdk/projection/INavigationStatusEvent$NavigationStatus");
    navigationStatusClass_ = (jclass)env->NewGlobalRef(navigationStatusClass);

    jclass maneuverTypeClass = env->FindClass("it/smg/libs/aasdk/projection/INavigationStatusEvent$ManeuverType");
    maneuverTypeClass_ = (jclass)env->NewGlobalRef(maneuverTypeClass);

    jclass maneuverDirectionClass = env->FindClass("it/smg/libs/aasdk/projection/INavigationStatusEvent$ManeuverDirection");
    maneuverDirectionClass_ = (jclass)env->NewGlobalRef(maneuverDirectionClass);

    jclass turnEventClass = env->FindClass("it/smg/libs/aasdk/projection/INavigationStatusEvent$TurnEvent");
    turnEventClass_ = (jclass)env->NewGlobalRef(turnEventClass);

    jclass distanceUnitClass = env->FindClass("it/smg/libs/aasdk/projection/INavigationStatusEvent$DistanceUnit");
    distanceUnitClass_ = (jclass)env->NewGlobalRef(distanceUnitClass);

    jclass distanceEventClass = env->FindClass("it/smg/libs/aasdk/projection/INavigationStatusEvent$DistanceEvent");
    distanceEventClass_ = (jclass)env->NewGlobalRef(distanceEventClass);

    env->DeleteLocalRef(cls);
}

void JNavigationStatusEvent::navigationStatusUpdate(const aasdk::proto::messages::NavigationStatus& navStatus) {
    JNIEnv* env = getJniEnv();

    // create navigation status enum
    jfieldID navigationStatusID = NULL;
    if (navStatus.has_status()){
        std::string navigationStatus = aasdk::proto::messages::NavigationStatus_Enum_Name(navStatus.status());
        navigationStatusID = env->GetStaticFieldID(navigationStatusClass_, navigationStatus.c_str(), "Lit/smg/libs/aasdk/projection/INavigationStatusEvent$NavigationStatus;");
    }
    if (navigationStatusID == NULL){
        if (Log::isVerbose()) Log_v("navigationStatus null or unknown");
        navigationStatusID = env->GetStaticFieldID(navigationStatusClass_, "UNAVAILABLE", "Lit/smg/libs/aasdk/projection/INavigationStatusEvent$NavigationStatus;");
    }
    jobject jNavigationStatus = env->GetStaticObjectField(navigationStatusClass_, navigationStatusID);

    env->CallVoidMethod(androidClass_, navigationStatusMethodId_, jNavigationStatus);

    env->DeleteLocalRef(jNavigationStatus);
    jNavigationStatus = nullptr;
}

void JNavigationStatusEvent::navigationTurnEvent(const aasdk::proto::messages::NavigationTurnEvent& turnEvent) {
    JNIEnv* env = getJniEnv();

    // create bytebuffer obj for image
    jclass byteBufferClass = env->FindClass("java/nio/ByteBuffer");
    jmethodID orderID = env->GetMethodID(byteBufferClass, "order", "(Ljava/nio/ByteOrder;)Ljava/nio/ByteBuffer;");
    jclass byteOrderClass = env->FindClass("java/nio/ByteOrder");
    jmethodID nativeOrderID = env->GetStaticMethodID(byteOrderClass, "nativeOrder", "()Ljava/nio/ByteOrder;");
    jobject nativeByteOrderObj = env->CallStaticObjectMethod(byteOrderClass, nativeOrderID);
    jobject imageByteBuffer = env->NewDirectByteBuffer((uint8_t *)turnEvent.turnimage().data(), turnEvent.turnimage().size());
    jobject me = env->CallObjectMethod(imageByteBuffer, orderID, nativeByteOrderObj);

    env->DeleteLocalRef(me);
    me = nullptr;

    jfieldID maneuverTypeID = NULL;
    if (turnEvent.has_maneuvertype()){
        std::string maneuverType = aasdk::proto::enums::ManeuverType_Enum_Name(turnEvent.maneuvertype());
        maneuverTypeID = env->GetStaticFieldID(maneuverTypeClass_, maneuverType.c_str(), "Lit/smg/libs/aasdk/projection/INavigationStatusEvent$ManeuverType;");
    }
    if (maneuverTypeID == NULL){
        if (Log::isVerbose()) Log_v("maneuverType null or unknown");
        maneuverTypeID  = env->GetStaticFieldID(maneuverTypeClass_, "UNKNOWN", "Lit/smg/libs/aasdk/projection/INavigationStatusEvent$ManeuverType;");
    }
    jobject jManeuverType = env->GetStaticObjectField(maneuverTypeClass_, maneuverTypeID);

    jfieldID maneuverDirectionID = NULL;
    if (turnEvent.has_maneuverdirection()){
        if (Log::isVerbose()) Log_v("maneuverDirection null or unknown");
        std::string maneuverDirection = aasdk::proto::enums::ManeuverDirection_Enum_Name(turnEvent.maneuverdirection());
        maneuverDirectionID = env->GetStaticFieldID(maneuverDirectionClass_, maneuverDirection.c_str(), "Lit/smg/libs/aasdk/projection/INavigationStatusEvent$ManeuverDirection;");
    }
    if (maneuverDirectionID == NULL){
        maneuverDirectionID = env->GetStaticFieldID(maneuverDirectionClass_, "UNKNOWN", "Lit/smg/libs/aasdk/projection/INavigationStatusEvent$ManeuverDirection;");
    }
    jobject jManeuverDirection = env->GetStaticObjectField(maneuverDirectionClass_, maneuverDirectionID);

    // create streetName field
    jstring jstreetName = env->NewStringUTF(turnEvent.has_street_name() ? turnEvent.street_name().c_str() : "");

    // create roundaboutexitnumber field
    jlong roundaboutexitnumber = turnEvent.has_roundaboutexitnumber() ? turnEvent.roundaboutexitnumber() : -1;

    // create roundaboutexitangle field
    jlong roundaboutexitangle = turnEvent.has_roundaboutexitangle() ? turnEvent.roundaboutexitangle() : -1;

    jmethodID turnEventConst = env->GetMethodID(turnEventClass_, "<init>",
                                                "(Ljava/lang/String;Ljava/nio/ByteBuffer;Lit/smg/libs/aasdk/projection/INavigationStatusEvent$ManeuverType;Lit/smg/libs/aasdk/projection/INavigationStatusEvent$ManeuverDirection;JJ)V");

    jobject jTurnEvent = env->NewObject(turnEventClass_, turnEventConst, jstreetName, imageByteBuffer, jManeuverType, jManeuverDirection, roundaboutexitangle, roundaboutexitnumber);

    env->CallVoidMethod(androidClass_, navigationTurnEventMethodId_, jTurnEvent);

    env->DeleteLocalRef(jstreetName);

    env->DeleteLocalRef(jTurnEvent);
    jTurnEvent = nullptr;

    env->DeleteLocalRef(jManeuverDirection);
    jManeuverDirection = nullptr;

    env->DeleteLocalRef(jManeuverType);
    jManeuverType = nullptr;

    env->DeleteLocalRef(imageByteBuffer);
    imageByteBuffer = nullptr;

    env->DeleteLocalRef(nativeByteOrderObj);
    nativeByteOrderObj = nullptr;

    env->DeleteLocalRef(byteBufferClass);
    byteBufferClass = nullptr;

    env->DeleteLocalRef(byteOrderClass);
    byteOrderClass = nullptr;
}

void JNavigationStatusEvent::navigationDistanceEvent(const aasdk::proto::messages::NavigationDistanceEvent& distanceEvent) {
    JNIEnv* env = getJniEnv();

    jlong meters = distanceEvent.has_meters() ? distanceEvent.meters() : -1;

    jlong timeToStepSeconds = distanceEvent.has_timetostepseconds() ? distanceEvent.timetostepseconds() : -1;

    jlong distanceToStepMillis = distanceEvent.has_distancetostepmillis() ? distanceEvent.distancetostepmillis() : -1;

    jfieldID distanceUnitID = NULL;
    if (distanceEvent.has_distanceunit()){
        std::string distanceUnit = aasdk::proto::enums::DistanceUnit_Enum_Name(distanceEvent.distanceunit());
        distanceUnitID = env->GetStaticFieldID(distanceUnitClass_, distanceUnit.c_str(), "Lit/smg/libs/aasdk/projection/INavigationStatusEvent$DistanceUnit;");
    }
    if (distanceUnitID == NULL){
        distanceUnitID = env->GetStaticFieldID(distanceUnitClass_, "UNKNOWN", "Lit/smg/libs/aasdk/projection/INavigationStatusEvent$DistanceUnit;");
    }
    jobject jDistanceUnit = env->GetStaticObjectField(distanceUnitClass_, distanceUnitID);

    jmethodID distanceEventConst = env->GetMethodID(distanceEventClass_, "<init>","(JJJLit/smg/libs/aasdk/projection/INavigationStatusEvent$DistanceUnit;)V");
    jobject jDistanceEvent = env->NewObject(distanceEventClass_, distanceEventConst, meters, timeToStepSeconds, distanceToStepMillis, jDistanceUnit);

    env->CallVoidMethod(androidClass_, navigationDistanceEventMethodId_, jDistanceEvent);

    env->DeleteLocalRef(jDistanceEvent);
    jDistanceEvent = nullptr;

    env->DeleteLocalRef(jDistanceUnit);
    jDistanceUnit = nullptr;

}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_NavigationStatusEvent_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/projection/NavigationStatusEvent");
    JNavigationStatusEvent::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
        Java_it_smg_libs_aasdk_projection_NavigationStatusEvent_nativeSetup(JNIEnv *env, jobject thiz) {
    JNavigationStatusEvent::Pointer jNavigationStatusEvent = new JNavigationStatusEvent(env, thiz);
    return (jlong)((size_t)jNavigationStatusEvent);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_NavigationStatusEvent_nativeDelete(JNIEnv *env, jobject thiz) {
    JNavigationStatusEvent::Pointer jNavigationStatusEvent = JNavigationStatusEvent::getJNavigationStatusEvent(env, thiz);
    delete jNavigationStatusEvent;
}