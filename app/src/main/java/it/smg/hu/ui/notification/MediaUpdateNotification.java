package it.smg.hu.ui.notification;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.View;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.nio.ByteBuffer;

import it.smg.hu.R;
import it.smg.hu.ui.PlayerActivity;
import it.smg.libs.aasdk.projection.IMediaStatusEvent.MetadataUpdateEvent;
import it.smg.libs.aasdk.projection.IMediaStatusEvent.PlaybackUpdateEvent;
import it.smg.libs.common.Log;

public class MediaUpdateNotification extends Notification {

    private static final String TAG = "MediaUpdateNotification";

    private static MediaUpdateNotification instance_;

    public static MediaUpdateNotification instance(Context ctx){
        if (instance_ == null){
            instance_ = new MediaUpdateNotification(ctx);
        }

        return instance_;
    }

    private MediaUpdateNotification(Context ctx){
        super(ctx);

        layoutResId_ = R.layout.media_update_notification;
        contentResId_ = R.id.media_update_layout;
        viewResId_ = R.id.media_update_content;
        toggleDismissBtnResId_ = R.id.media_update_toggle_dismiss;
        dismissBtnResId_ = R.id.media_update_dismiss;
        dismissLayoutResId_ = R.id.media_update_dismiss_layout;

        setOnClickListener(view -> {
            Intent i = new Intent(ctx_.getApplicationContext(), PlayerActivity.class);
            i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
            ctx_.getApplicationContext().startActivity(i);
            dialog_.dismiss();
        });
    }

    @Override
    protected void createUi(INotificationEvent event){
        MediaUpdateEvent mediaUpdateEvent = (MediaUpdateEvent) event;
        PlaybackUpdateEvent playbackEvent = mediaUpdateEvent.playbackUpdateEvent;
        MetadataUpdateEvent metadataUpdateEvent = mediaUpdateEvent.metadataUpdateEvent;

        // set source
        TextView sourceTxt = dialog_.findViewById(R.id.media_update_source);
        sourceTxt.setText(playbackEvent.mediaSource);

        // set album art image
        ImageView imageView = dialog_.findViewById(R.id.media_update_albumart);
        if (metadataUpdateEvent.albumArt != null){
            ByteBuffer buffer = metadataUpdateEvent.albumArt.asReadOnlyBuffer();
            if (Log.isDebug()) Log.d(TAG, "image buffer " + buffer);
            byte[] image = new byte[buffer.limit()];
            buffer.get(image);
            BitmapFactory.Options opts = new BitmapFactory.Options();
            opts.inPreferredConfig = Bitmap.Config.ARGB_8888;
            Bitmap bmp = BitmapFactory.decodeByteArray(image, 0, image.length, opts);
            if (bmp != null) {
//                imageView.setImageBitmap(Bitmap.createScaledBitmap(bmp, 256, 256, false));
                imageView.setImageBitmap(bmp);
            }
        }

        // set title
        TextView titleTxt = dialog_.findViewById(R.id.media_update_title);
        titleTxt.setText(metadataUpdateEvent.trackName);

        // if both artist and album empty hidden the line
        boolean hasArtistName = metadataUpdateEvent.artistName != null && !"".equals(metadataUpdateEvent.artistName.trim());
        boolean hasAlbumName = metadataUpdateEvent.albumName != null && !"".equals(metadataUpdateEvent.albumName.trim());
        TextView artistTxt = dialog_.findViewById(R.id.media_update_artist_album);
        if (!hasArtistName && !hasAlbumName){
            artistTxt.setVisibility(View.GONE);
        } else {
            String artist_album = "";
            if (hasArtistName) {
                artist_album += metadataUpdateEvent.artistName;
            }

            if (hasArtistName && hasAlbumName) {
                artist_album += "  -  ";
            }

            if (hasAlbumName) {
                artist_album += metadataUpdateEvent.albumName;
            }

            artistTxt.setText(artist_album);
        }

        ProgressBar progressBar =  dialog_.findViewById(R.id.media_update_progress);
        progressBar.setProgress(playbackEvent.trackProgress);
        progressBar.setIndeterminate(false);
        progressBar.setMax(metadataUpdateEvent.trackLength);
    }

    @Override
    protected String tag() {
        return TAG;
    }
}
