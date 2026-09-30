package it.smg.hu.ui.settings;

import android.content.Context;
import android.content.res.ColorStateList;
import android.view.Gravity;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.view.ViewGroup;

import androidx.fragment.app.Fragment;
import androidx.core.widget.CompoundButtonCompat;

import java.util.concurrent.Callable;

import it.smg.hu.R;
import it.smg.hu.config.Settings;
import it.smg.libs.common.Log;

public abstract class BaseSettingsFragment extends Fragment {
    protected Settings settings;

    protected abstract String tag();

    @Override
    public void onViewCreated(View view, android.os.Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        boolean dark = Settings.instance().appearance.darkTheme();
        view.setBackgroundColor(getResources().getColor(dark ? R.color.oda_surface : R.color.settings_surface));
        styleControls(view, dark);
    }

    private void styleControls(View view, boolean dark) {
        if (view instanceof ViewGroup && !(view instanceof Spinner)) {
            ViewGroup group = (ViewGroup) view;
            for (int i = 0; i < group.getChildCount(); i++) {
                styleControls(group.getChildAt(i), dark);
            }
            return;
        }
        int primaryColor = dark ? R.color.oda_text_primary : R.color.settings_text_primary;
        int secondaryColor = dark ? R.color.oda_text_secondary : R.color.settings_text_secondary;
        if (view instanceof EditText) {
            EditText field = (EditText) view;
            field.setTextColor(getResources().getColor(primaryColor));
            field.setHintTextColor(getResources().getColor(secondaryColor));
            field.setBackgroundResource(dark ? R.drawable.settings_input_dark : R.drawable.settings_input_light);
            field.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
            field.setMinHeight(dp(48));
        } else if (view instanceof CheckBox) {
            CheckBox checkBox = (CheckBox) view;
            checkBox.setTextColor(getResources().getColor(primaryColor));
            checkBox.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
            int[][] states = new int[][]{
                    new int[]{android.R.attr.state_checked}, new int[]{}
            };
            int[] colors = new int[]{
                    getResources().getColor(R.color.oda_accent),
                    getResources().getColor(dark ? R.color.oda_text_primary : R.color.settings_text_secondary)
            };
            CompoundButtonCompat.setButtonTintList(checkBox, new ColorStateList(states, colors));
            alignToStart(checkBox, true);
            view.setMinimumHeight(dp(48));
        } else if (view instanceof Spinner) {
            view.setBackgroundResource(dark ? R.drawable.settings_spinner_dark : R.drawable.settings_spinner_light);
            view.setMinimumHeight(dp(48));
        } else if (view instanceof TextView) {
            TextView text = (TextView) view;
            text.setTextColor(getResources().getColor(primaryColor));
            text.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        }
    }

    private void alignToStart(View view, boolean fillWidth) {
        ViewGroup.LayoutParams params = view.getLayoutParams();
        if (fillWidth) {
            params.width = ViewGroup.LayoutParams.MATCH_PARENT;
        }
        if (params instanceof LinearLayout.LayoutParams) {
            ((LinearLayout.LayoutParams) params).gravity = Gravity.LEFT;
        }
        view.setLayoutParams(params);
    }

    private int dp(int value) {
        return Math.round(value * getResources().getDisplayMetrics().density);
    }

    protected void initEditText(EditText editText, Settings.Base base, String settingsKey, String defaultValue){
        initEditText(editText, base, settingsKey, defaultValue, null);
    }

    protected void initEditText(EditText editText, Settings.Base base, String settingsKey, int defaultValue){
        initEditText(editText, base, settingsKey, defaultValue, null);
    }

    protected void initEditText(EditText editText, Settings.Base base, String settingsKey, int defaultValue, Callable<Void> custonCheck){
        editText.setText(String.valueOf(base.get(settingsKey, defaultValue)));

        editText.setOnEditorActionListener((v, actionId, event) -> {
            if (actionId == EditorInfo.IME_ACTION_DONE){
                String rawValue = v.getText().toString().trim();
                try {
                    base.set(settingsKey, Integer.parseInt(rawValue));
                    v.setError(null);
                } catch (NumberFormatException error) {
                    v.setError(getString(R.string.settings_invalid_number));
                    return true;
                }

                InputMethodManager imm = (InputMethodManager) v.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.hideSoftInputFromWindow(v.getWindowToken(), 0);
                if (custonCheck != null){
                    try {
                        custonCheck.call();
                    } catch (Exception e) {
                        Log.e(tag(), "errore in custom check", e);
                    }
                }

                return true;
            }
            return false;
        });
    }

    protected void initEditText(EditText editText, Settings.Base base, String settingsKey, String defaultValue, Callable<Void> custonCheck){
        editText.setText(base.get(settingsKey, defaultValue));
        editText.setOnEditorActionListener((v, actionId, event) -> {
            if (actionId == EditorInfo.IME_ACTION_DONE){
                base.set(settingsKey, v.getText().toString());

                InputMethodManager imm = (InputMethodManager) v.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.hideSoftInputFromWindow(v.getWindowToken(), 0);
                if (custonCheck != null){
                    try {
                        custonCheck.call();
                    } catch (Exception e) {
                        Log.e(tag(), "errore in custom check", e);
                    }
                }

                return true;
            }
            return false;
        });
    }

    protected void initCheckBox(CheckBox checkBox, Settings.Base base, String settingsKey, boolean defaultValue){
        initCheckBox(checkBox, base, settingsKey, defaultValue, null);
    }

    protected void initCheckBox(CheckBox checkBox, Settings.Base base, String settingsKey, boolean defaultValue, Callable<Void> custonCheck){
        checkBox.setChecked(base.get(settingsKey, defaultValue));
        checkBox.setOnCheckedChangeListener((buttonView, isChecked) -> {
            base.set(settingsKey, isChecked);
            if (custonCheck != null){
                try {
                    custonCheck.call();
                } catch (Exception e) {
                    Log.e(tag(), "errore in custom check", e);
                }
            }
        });
    }

    protected void initSpinner(Spinner spinner, int data, int dataValue, Settings.Base base, String settingsKey, String defaultValue){
        initSpinner(spinner, data, dataValue, base, settingsKey, defaultValue, null);
    }

    protected void initSpinner(Spinner spinner, int data, int dataValue, Settings.Base base, String settingsKey, int defaultValue){
        initSpinner(spinner, data, dataValue, base, settingsKey, defaultValue, null);
    }

    protected void initSpinner(Spinner spinner, int data, int dataValue, Settings.Base base, String settingsKey, int defaultValue, Callable<Void> custonCheck){
        ArrayAdapter<CharSequence> adapter = createSpinnerAdapter(data);
        spinner.setAdapter(adapter);

        int value = base.get(settingsKey, defaultValue);
        int[] elements = getContext().getResources().getIntArray(dataValue);

        for (int i = 0; i < elements.length; i++){
            int e = elements[i];
            if (value == e){
                spinner.setSelection(i);
                break;
            }
        }

        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                base.set(settingsKey, elements[position]);
                if (custonCheck != null){
                    try {
                        custonCheck.call();
                    } catch (Exception e) {
                        Log.e(tag(), "errore in custom check", e);
                    }
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
                base.set(settingsKey, defaultValue);
                if (custonCheck != null){
                    try {
                        custonCheck.call();
                    } catch (Exception e) {
                        Log.e(tag(), "errore in custom check", e);
                    }
                }
            }
        });
    }

    protected void initSpinner(Spinner spinner, int data, int dataValue, Settings.Base base, String settingsKey, String defaultValue, Callable<Void> custonCheck){
        ArrayAdapter<CharSequence> adapter = createSpinnerAdapter(data);
        spinner.setAdapter(adapter);

        String value = base.get(settingsKey, defaultValue);
        String[] elements = getContext().getResources().getStringArray(dataValue);

        for (int i = 0; i < elements.length; i++){
            String e = elements[i];
            if (value.equalsIgnoreCase(e)){
                spinner.setSelection(i);
                break;
            }
        }

        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                base.set(settingsKey, elements[position]);
                if (custonCheck != null){
                    try {
                        custonCheck.call();
                    } catch (Exception e) {
                        Log.e(tag(), "errore in custom check", e);
                    }
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {
                base.set(settingsKey, defaultValue);
                if (custonCheck != null){
                    try {
                        custonCheck.call();
                    } catch (Exception e) {
                        Log.e(tag(), "errore in custom check", e);
                    }
                }
            }
        });
    }

    private ArrayAdapter<CharSequence> createSpinnerAdapter(int data) {
        boolean dark = Settings.instance().appearance.darkTheme();
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(getContext(), data,
                dark ? R.layout.spinner_item_dark : R.layout.spinner_item);
        adapter.setDropDownViewResource(dark
                ? R.layout.spinner_dropdown_item_dark : R.layout.spinner_dropdown_item);
        return adapter;
    }
}
