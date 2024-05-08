package it.smg.hu.ui.settings;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import it.smg.hu.R;
import it.smg.hu.config.Settings;
import it.smg.hu.projection.AudioInput;
import it.smg.libs.aasdk.projection.IAudioInput;
import it.smg.libs.aasdk.projection.IAudioOutput;
import it.smg.hu.projection.MediaAudioOutput;
import it.smg.hu.projection.SpeechAudioOutput;
import it.smg.hu.projection.SystemAudioOutput;

public class AudioFragment extends BaseSettingsFragment {

    private static final String TAG = "AudioFragment";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.audio_fragment, container, false);

        settings = Settings.instance();

        initMediaAudio(view);
        initSpeechAudio(view);
        initSystemAudio(view);
        initMic(view);

        return view;

    }

    private void initMediaAudio(View view){

        CheckBox enableMediaAudio = view.findViewById(R.id.enable_media_channel);
        initCheckBox(enableMediaAudio, settings.audio, Settings.Audio.MEDIA_ENABLE_CHANNEL, Settings.Audio.MEDIA_ENABLE_CHANNEL_DEFAULT_VALUE);

        EditText sampleRate = view.findViewById(R.id.media_samplerate);
        initEditText(sampleRate, settings.audio, Settings.Audio.MEDIA_SAMPLERATE, Settings.Audio.MEDIA_SAMPLERATE_DEFAULT_VALUE);

        Spinner sampleSize = view.findViewById(R.id.media_samplesize);
        initSpinner(sampleSize, R.array.audio_samplesize_labels, R.array.audio_samplesize_values, settings.audio, Settings.Audio.MEDIA_SAMPLESIZE, Settings.Audio.MEDIA_SAMPLESIZE_DEFAULT_VALUE);

        Spinner channelCount = view.findViewById(R.id.media_channel_count);
        initSpinner(channelCount, R.array.audio_channels_labels, R.array.audio_channels_values, settings.audio, Settings.Audio.MEDIA_CHANNELCOUNT, Settings.Audio.MEDIA_CHANNELCOUNT_DEFAULT_VALUE);

        Button check = view.findViewById(R.id.media_check_button);
        check.setOnClickListener(v -> {
            checkMediaAudioOutput();
        });
    }

    private void initSpeechAudio(View view){

        CheckBox enableSpeechAudio = view.findViewById(R.id.enable_speech_channel);
        initCheckBox(enableSpeechAudio, settings.audio, Settings.Audio.SPEECH_ENABLE_CHANNEL, Settings.Audio.SPEECH_ENABLE_CHANNEL_DEFAULT_VALUE);

        EditText sampleRate = view.findViewById(R.id.speech_samplerate);
        initEditText(sampleRate, settings.audio, Settings.Audio.SPEECH_SAMPLERATE, Settings.Audio.SPEECH_SAMPLERATE_DEFAULT_VALUE);

        Spinner sampleSize = view.findViewById(R.id.speech_samplesize);
        initSpinner(sampleSize, R.array.audio_samplesize_labels, R.array.audio_samplesize_values, settings.audio, Settings.Audio.SPEECH_SAMPLESIZE, Settings.Audio.SPEECH_SAMPLESIZE_DEFAULT_VALUE);

        Spinner channelCount = view.findViewById(R.id.speech_channel_count);
        initSpinner(channelCount, R.array.audio_channels_labels, R.array.audio_channels_values, settings.audio, Settings.Audio.SPEECH_CHANNELCOUNT, Settings.Audio.SPEECH_CHANNELCOUNT_DEFAULT_VALUE);

        Button check = view.findViewById(R.id.speech_check_button);
        check.setOnClickListener(v -> {
            checkSpeechAudioOutput();
        });

    }

    private void initSystemAudio(View view){
        EditText sampleRate = view.findViewById(R.id.system_samplerate);
        initEditText(sampleRate, settings.audio, Settings.Audio.SYSTEM_SAMPLERATE, Settings.Audio.SYSTEM_SAMPLERATE_DEFAULT_VALUE);

        Spinner sampleSize = view.findViewById(R.id.system_samplesize);
        initSpinner(sampleSize, R.array.audio_samplesize_labels, R.array.audio_samplesize_values, settings.audio, Settings.Audio.SYSTEM_SAMPLESIZE, Settings.Audio.SYSTEM_SAMPLESIZE_DEFAULT_VALUE);

        Spinner channelCount = view.findViewById(R.id.system_channel_count);
        initSpinner(channelCount, R.array.audio_channels_labels, R.array.audio_channels_values, settings.audio, Settings.Audio.SYSTEM_CHANNELCOUNT, Settings.Audio.SYSTEM_CHANNELCOUNT_DEFAULT_VALUE);

        Button check = view.findViewById(R.id.system_check_button);
        check.setOnClickListener(v -> {
            checkSystemAudioOutput();
        });

    }

    private boolean checkMediaAudioOutput(){
        return checkAudioSettings(new MediaAudioOutput(), "Wrong Media Audio Settings");
    }

    private boolean checkSpeechAudioOutput(){
        return checkAudioSettings(new SpeechAudioOutput(), "Wrong Speech Audio Settings");
    }

    private boolean checkSystemAudioOutput(){
        return checkAudioSettings(new SystemAudioOutput(), "Wrong System Audio Settings");
    }

    private boolean checkAudioSettings(IAudioOutput audioOutput, String error){
        try {
            if (!audioOutput.open()){
                throw new Exception();
            };
            audioOutput.stop();
            Toast.makeText(getContext(), "Configuration OK", Toast.LENGTH_SHORT).show();
        } catch (Exception e){
            Toast.makeText(getContext(), error + ": " + e.getLocalizedMessage(), Toast.LENGTH_SHORT).show();
            return false;
        }
        return true;
    }

    private void initMic(View view){
        CheckBox enableAudioInput = view.findViewById(R.id.enable_audioinput_channel);
        initCheckBox(enableAudioInput, settings.audio, Settings.Audio.MIC_ENABLE_CHANNEL, Settings.Audio.MIC_ENABLE_CHANNEL_DEFAULT_VALUE);

        EditText sampleRate = view.findViewById(R.id.mic_samplerate);
        initEditText(sampleRate, settings.audio, Settings.Audio.MIC_SAMPLERATE, Settings.Audio.MIC_SAMPLERATE_DEFAULT_VALUE);

        Spinner channelCount = view.findViewById(R.id.mic_channel_count);
        initSpinner(channelCount, R.array.mic_channels_labels, R.array.mic_channels_values, settings.audio, Settings.Audio.MIC_CHANNELCOUNT, Settings.Audio.MIC_CHANNELCOUNT_DEFAULT_VALUE);

        Spinner sampleSize = view.findViewById(R.id.mic_samplesize);
        initSpinner(sampleSize, R.array.audio_samplesize_labels, R.array.audio_samplesize_values, settings.audio, Settings.Audio.MIC_SAMPLESIZE, Settings.Audio.MIC_SAMPLESIZE_DEFAULT_VALUE);

        Spinner source = view.findViewById(R.id.mic_source);
        initSpinner(source, R.array.mic_source_labels, R.array.mic_source_values, settings.audio, Settings.Audio.MIC_SOURCE, Settings.Audio.MIC_SOURCE_DEFAULT_VALUE);

        Button check = view.findViewById(R.id.mic_check_button);
        check.setOnClickListener(v -> {
            checkMicSettings("Wrong Microphone Settings");
        });

    }

    private boolean checkMicSettings(String error){
        try {
            if (ContextCompat.checkSelfPermission(getActivity(), Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
                Toast.makeText(getContext(), "Mic permission KO, request it", Toast.LENGTH_SHORT).show();
                ActivityCompat.requestPermissions(getActivity(), new String[]{Manifest.permission.RECORD_AUDIO}, 111);
            } else {
                Toast.makeText(getContext(), "Mic permission OK", Toast.LENGTH_SHORT).show();
            }

            IAudioInput audioInput = new AudioInput();
            if (!audioInput.open()){
                throw new Exception();
            }
            Toast.makeText(getContext(), "Configuration OK", Toast.LENGTH_SHORT).show();
        } catch (Exception e){
            Toast.makeText(getContext(), error + ": " + e.getLocalizedMessage(), Toast.LENGTH_SHORT).show();
            return false;
        }
        return true;
    }

    @Override
    protected String tag() {
        return TAG;
    }
}
