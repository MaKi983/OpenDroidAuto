package it.smg.hu.widget;

import android.app.PendingIntent;
import android.appwidget.AppWidgetManager;
import android.appwidget.AppWidgetProvider;
import android.content.Context;
import android.content.Intent;
import android.widget.RemoteViews;

import it.smg.hu.R;
import it.smg.hu.service.ODAService;
import it.smg.libs.common.Log;

public class ExitWidget extends AppWidgetProvider {

    private static final String ACTION_WIDGET_EXIT = "it.smg.hu.widget.ACTION_WIDGET_EXIT";

    @Override
    public void onUpdate(Context context, AppWidgetManager manager, int[] appWidgetIds) {
        if (Log.isDebug()) Log.d("ExitWidget", "onUpdate");

        RemoteViews views = new RemoteViews(context.getPackageName(), R.layout.widget_exit);

        Intent intent = new Intent(context, ExitWidget.class);
        intent.setAction(ACTION_WIDGET_EXIT);
        PendingIntent pendingIntent = PendingIntent.getBroadcast(
                context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT
        );

        views.setOnClickPendingIntent(R.id.widget_exit, pendingIntent);

        for (int id : appWidgetIds) {
            manager.updateAppWidget(id, views);
        }
    }

    @Override
    public void onReceive(Context context, Intent intent) {
        if (Log.isDebug()) Log.d("ExitWidget", "onReceive");

        super.onReceive(context, intent);

        if (ACTION_WIDGET_EXIT.equals(intent.getAction())) {
            Intent stopService = new Intent(context, ODAService.class);
            context.stopService(stopService);

            android.os.Process.killProcess(android.os.Process.myPid());
            System.exit(0);
        }
    }
}
