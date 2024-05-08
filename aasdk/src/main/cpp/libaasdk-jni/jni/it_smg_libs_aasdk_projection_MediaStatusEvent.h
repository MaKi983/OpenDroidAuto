#pragma once

#include <JNIBase.h>
#include <projection/IMediaStatusEvent.h>

namespace projection {

    class JMediaStatusEvent : JNIBase, public IMediaStatusEvent {
    public:
        typedef JMediaStatusEvent* Pointer;

        JMediaStatusEvent(JNIEnv *env, jobject jmediastatusevent);
        ~JMediaStatusEvent();

        static jfieldID handleId;
        static JMediaStatusEvent::Pointer getJMediaStatusEvent(JNIEnv* env, jobject jmediastatusevent);

        void initJavaMethods() override;

        void mediaPlaybackUpdate(const aasdk::proto::messages::MediaInfoChannelPlaybackData& playback) override;
        void mediaMetadataUpdate(const aasdk::proto::messages::MediaInfoChannelMetadataData& metadata) override;
    private:
        jmethodID mediaPlaybackUpdateMethodId_;
        jmethodID mediaMetadataUpdateMethodId_;

        jclass playbackStateClass_;
        jclass playbackUpdateEventClass_;
        jclass metadataUpdateEventClass_;
    };

}
