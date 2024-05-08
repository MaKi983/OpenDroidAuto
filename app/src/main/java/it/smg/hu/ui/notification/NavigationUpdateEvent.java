package it.smg.hu.ui.notification;

import it.smg.libs.aasdk.projection.INavigationStatusEvent.*;

public class NavigationUpdateEvent implements INotificationEvent {
    final TurnEvent turnEvent;
    final DistanceEvent distanceEvent;

    NavigationUpdateEvent(TurnEvent turnEvent, DistanceEvent distanceEvent){
        this.turnEvent = turnEvent;
        this.distanceEvent = distanceEvent;
    }
}
