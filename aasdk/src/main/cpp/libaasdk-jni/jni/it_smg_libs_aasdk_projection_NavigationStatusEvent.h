#pragma once

#include <JNIBase.h>
#include <projection/INavigationStatusEvent.h>

namespace projection {

    class JNavigationStatusEvent : JNIBase, public INavigationStatusEvent {
    public:
        typedef JNavigationStatusEvent* Pointer;

        JNavigationStatusEvent(JNIEnv *env, jobject jnavigationstatusevent);
        ~JNavigationStatusEvent();

        static jfieldID handleId;
        static JNavigationStatusEvent::Pointer getJNavigationStatusEvent(JNIEnv* env, jobject jnavigationstatusevent);

        void initJavaMethods() override;

        void navigationStatusUpdate(const aasdk::proto::messages::NavigationStatus& navStatus) override;
        void navigationTurnEvent(const aasdk::proto::messages::NavigationTurnEvent& turnEvent) override;
        void navigationDistanceEvent(const aasdk::proto::messages::NavigationDistanceEvent& distanceEvent) override;
    private:
        jmethodID navigationTurnEventMethodId_;
        jmethodID navigationStatusMethodId_;
        jmethodID navigationDistanceEventMethodId_;

        jclass navigationStatusClass_;
        jclass maneuverTypeClass_;
        jclass maneuverDirectionClass_;
        jclass turnEventClass_;
        jclass distanceUnitClass_;
        jclass distanceEventClass_;
    };

}
