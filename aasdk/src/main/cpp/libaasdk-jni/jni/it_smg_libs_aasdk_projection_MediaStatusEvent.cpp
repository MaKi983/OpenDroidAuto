#include "it_smg_libs_aasdk_projection_MediaStatusEvent.h"

using namespace projection;

jfieldID JMediaStatusEvent::handleId = nullptr;

JMediaStatusEvent::JMediaStatusEvent(JNIEnv *env, jobject jmediastatusevent) :
        JNIBase(env, jmediastatusevent, "JMediaStatusEvent"){

    initJavaMethods();
}

JMediaStatusEvent::~JMediaStatusEvent(){
    JNIEnv* env = getJniEnv();
    env->DeleteGlobalRef(playbackStateClass_);
    env->DeleteGlobalRef(playbackUpdateEventClass_);
    env->DeleteGlobalRef(metadataUpdateEventClass_);
}

JMediaStatusEvent::Pointer JMediaStatusEvent::getJMediaStatusEvent(JNIEnv *env, jobject jmediastatusevent) {
    return (JMediaStatusEvent::Pointer)env->GetLongField(jmediastatusevent, JMediaStatusEvent::handleId);;
}

void JMediaStatusEvent::initJavaMethods() {
    JNIEnv* env = getJniEnv();
    jclass cls = env->GetObjectClass(androidClass_);

    mediaPlaybackUpdateMethodId_ = env->GetMethodID(cls, "mediaPlaybackUpdate", "(Lit/smg/libs/aasdk/projection/IMediaStatusEvent$PlaybackUpdateEvent;)V");
    mediaMetadataUpdateMethodId_ = env->GetMethodID(cls, "mediaMetadataUpdate", "(Lit/smg/libs/aasdk/projection/IMediaStatusEvent$MetadataUpdateEvent;)V");

    jclass playbackStateClass = env->FindClass("it/smg/libs/aasdk/projection/IMediaStatusEvent$PlaybackState");
    playbackStateClass_ = (jclass)env->NewGlobalRef(playbackStateClass);

    jclass playbackUpdateEventClass = env->FindClass("it/smg/libs/aasdk/projection/IMediaStatusEvent$PlaybackUpdateEvent");
    playbackUpdateEventClass_ = (jclass)env->NewGlobalRef(playbackUpdateEventClass);

    jclass metadataUpdateEventClass = env->FindClass("it/smg/libs/aasdk/projection/IMediaStatusEvent$MetadataUpdateEvent");
    metadataUpdateEventClass_ = (jclass)env->NewGlobalRef(metadataUpdateEventClass);

    env->DeleteLocalRef(cls);
}

void JMediaStatusEvent::mediaPlaybackUpdate(const aasdk::proto::messages::MediaInfoChannelPlaybackData& playback){
    JNIEnv* env = getJniEnv();

    // create playback state enum
    jfieldID playbackStateID = NULL;
    if (playback.has_playback_state()){
        std::string playbackState = aasdk::proto::messages::MediaInfoChannelPlaybackData_PlaybackState_Name(playback.playback_state());
        playbackStateID = env->GetStaticFieldID(playbackStateClass_, playbackState.c_str(), "Lit/smg/libs/aasdk/projection/IMediaStatusEvent$PlaybackState;");
    }
    if (playbackStateID == NULL){
        playbackStateID = env->GetStaticFieldID(playbackStateClass_, "NONE", "Lit/smg/libs/aasdk/projection/IMediaStatusEvent$PlaybackState;");
    }
    jobject jPlaybackState = env->GetStaticObjectField(playbackStateClass_, playbackStateID);

    jstring jmediaSource = env->NewStringUTF(playback.has_media_source() ? playback.media_source().c_str() : "");

    jint jtrackProgress = playback.has_track_progress() ? playback.track_progress() : -1;

    // create playbackUpdateEvent obj
    jmethodID playbackUpdateEventConst = env->GetMethodID(playbackUpdateEventClass_, "<init>","(Lit/smg/libs/aasdk/projection/IMediaStatusEvent$PlaybackState;Ljava/lang/String;I)V");
    jobject jPlaybackUpdateEvent = env->NewObject(playbackUpdateEventClass_, playbackUpdateEventConst, jPlaybackState, jmediaSource, jtrackProgress);

    env->CallVoidMethod(androidClass_, mediaPlaybackUpdateMethodId_, jPlaybackUpdateEvent);

    env->DeleteLocalRef(jmediaSource);

    env->DeleteLocalRef(jPlaybackUpdateEvent);
    jPlaybackUpdateEvent = nullptr;

    env->DeleteLocalRef(jPlaybackState);
    jPlaybackState = nullptr;
}

void JMediaStatusEvent::mediaMetadataUpdate(const aasdk::proto::messages::MediaInfoChannelMetadataData& metadata){
    JNIEnv* env = getJniEnv();

    jstring jtrackName = env->NewStringUTF(metadata.has_track_name() ? metadata.track_name().c_str() : "");

    jstring jartistName = env->NewStringUTF(metadata.has_artist_name() ? metadata.artist_name().c_str() : "");

    jstring jalbumName = env->NewStringUTF(metadata.has_album_name() ? metadata.album_name().c_str() : "");

    // create bytebuffer obj for image
    jobject imageByteBuffer = NULL;
    jclass byteBufferClass = NULL;
    jclass byteOrderClass = NULL;
    jobject nativeByteOrderObj = NULL;
    if (metadata.has_album_art()) {
        byteBufferClass = env->FindClass("java/nio/ByteBuffer");
        jmethodID orderID = env->GetMethodID(byteBufferClass, "order",
                                             "(Ljava/nio/ByteOrder;)Ljava/nio/ByteBuffer;");
        byteOrderClass = env->FindClass("java/nio/ByteOrder");
        jmethodID nativeOrderID = env->GetStaticMethodID(byteOrderClass, "nativeOrder",
                                                         "()Ljava/nio/ByteOrder;");
        nativeByteOrderObj = env->CallStaticObjectMethod(byteOrderClass, nativeOrderID);
        imageByteBuffer = env->NewDirectByteBuffer((uint8_t *) metadata.album_art().data(),
                                                           metadata.album_art().size());
        jobject me = env->CallObjectMethod(imageByteBuffer, orderID, nativeByteOrderObj);
        env->DeleteLocalRef(me);
        me = nullptr;
    }

    jint jtrackLength = metadata.has_track_length() ? metadata.track_length() : 0;

    // create metadataUpdateEvent obj
    jmethodID metadataUpdateEventConst = env->GetMethodID(metadataUpdateEventClass_, "<init>","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/nio/ByteBuffer;I)V");
    jobject jMetadataUpdateEvent = env->NewObject(metadataUpdateEventClass_, metadataUpdateEventConst, jtrackName, jartistName, jalbumName, imageByteBuffer, jtrackLength);

    env->CallVoidMethod(androidClass_, mediaMetadataUpdateMethodId_, jMetadataUpdateEvent);

    env->DeleteLocalRef(jMetadataUpdateEvent);
    jMetadataUpdateEvent = nullptr;

    env->DeleteLocalRef(jtrackName);
    env->DeleteLocalRef(jartistName);
    env->DeleteLocalRef(jalbumName);

    if (imageByteBuffer != NULL) {
        env->DeleteLocalRef(imageByteBuffer);
        imageByteBuffer = nullptr;

        env->DeleteLocalRef(nativeByteOrderObj);
        nativeByteOrderObj = nullptr;

        env->DeleteLocalRef(byteBufferClass);
        byteBufferClass = nullptr;

        env->DeleteLocalRef(byteOrderClass);
        byteOrderClass = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_MediaStatusEvent_nativeInit(JNIEnv *env, jclass clazz) {
    jclass _class = env->FindClass("it/smg/libs/aasdk/projection/MediaStatusEvent");
    JMediaStatusEvent::handleId = env->GetFieldID(_class, "handle_", "J");

    env->DeleteLocalRef(_class);
    _class = nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_it_smg_libs_aasdk_projection_MediaStatusEvent_nativeSetup(JNIEnv *env, jobject thiz) {
    auto jMediaStatusEvent = new JMediaStatusEvent(env, thiz);
    return (jlong)((size_t)jMediaStatusEvent);
}

extern "C"
JNIEXPORT void JNICALL
Java_it_smg_libs_aasdk_projection_MediaStatusEvent_nativeDelete(JNIEnv *env, jobject thiz) {
    JMediaStatusEvent::Pointer jMediaStatusEvent = JMediaStatusEvent::getJMediaStatusEvent(env, thiz);
    delete jMediaStatusEvent;
}