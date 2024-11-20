package it.smg.libs.aasdk.projection;

import androidx.annotation.Keep;

import java.nio.ByteBuffer;

public interface INavigationStatusEvent {
    void navigationStatusUpdate(NavigationStatus navigationStatus);

    void navigationTurnEvent(TurnEvent turnEvent);

    void navigationDistanceEvent(DistanceEvent distanceEvent);

    void delete();

    @Keep
    enum DistanceUnit {
        UNKNOWN,
        METERS,
        KILOMETERS,
        KILOMETERS_PARTIAL,
        MILES,
        MILES_PARTIAL,
        FEET,
        YARDS
    }

    @Keep
    enum NavigationStatus {
        UNAVAILABLE,
        ACTIVE,
        INACTIVE,
        REROUTING
    }

    @Keep
    enum ManeuverDirection {
        UNKNOWN,
        LEFT,
        RIGHT,
        UNSPECIFIED
    }

    @Keep
    enum ManeuverType {
        UNKNOWN,
        DEPART,
        NAME_CHANGE,
        SLIGHT_TURN,
        TURN,
        SHARP_TURN,
        U_TURN,
        ON_RAMP,
        OFF_RAMP,
        FORK,
        MERGE,
        ROUNDABOUT_ENTER,
        ROUNDABOUT_EXIT,
        ROUNDABOUT_ENTER_AND_EXIT,
        STRAIGHT,
        FERRY_BOAT,
        FERRY_TRAIN,
        DESTINATION
    }

    @Keep
    class TurnEvent {
        public final String street;
        public final ByteBuffer image;
        public final ManeuverType maneuverType;
        public final ManeuverDirection maneuverDirection;
        public final long roundAboutExitAngle;
        public final long roundAboutExitNumber;

        TurnEvent(String street, ByteBuffer image, ManeuverType maneuverType, ManeuverDirection maneuverDirection, long roundAboutExitAngle, long roundAboutExitNumber){
            this.street = street;
            this.image = clone(image);
            this.maneuverType = maneuverType;
            this.maneuverDirection = maneuverDirection;
            this.roundAboutExitAngle = roundAboutExitAngle;
            this.roundAboutExitNumber = roundAboutExitNumber;
        }

        private ByteBuffer clone(ByteBuffer b){
            if (b == null){
                return null;
            }

            final ByteBuffer clone = ByteBuffer.allocate(b.capacity());
            clone.put(b);
            clone.flip();
            return clone;
        }
    }

    @Keep
    class DistanceEvent {
        public final long meters;
        public final long timeToStepSeconds;
        public final long distanceToStepMillis;
        public final DistanceUnit distanceUnit;

        DistanceEvent(long meters, long timeToStepSeconds, long distanceToStepMillis, DistanceUnit distanceUnit) {
            this.meters = meters;
            this.timeToStepSeconds = timeToStepSeconds;
            this.distanceToStepMillis = distanceToStepMillis;
            this.distanceUnit = distanceUnit;
        }
    }
}
