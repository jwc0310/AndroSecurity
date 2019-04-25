package com.andy.androsecurity.ui;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;

import com.andy.androsecurity.R;
import com.andy.androsecurity.jni.jni;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import butterknife.Unbinder;

public class MainActivity extends AppCompatActivity {

    @BindView(R.id.sample_text)
    TextView sampleText;
    @BindView(R.id.sample_encrypt)
    Button sampleEncrypt;
    private Unbinder unbinder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        unbinder = ButterKnife.bind(this);
        sampleText.setText(jni.getStringFromJNI());
    }

    @Override
    public void onDestroy() {
        if (null != unbinder)
            unbinder.unbind();
        super.onDestroy();
    }

    @OnClick(R.id.sample_encrypt)
    public void onViewClicked() {
        encryptSo();
    }

    private void encryptSo() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                Log.e("Andy", "encry start");
                int res = jni.encryptSo();
                if (0 == res)
                    Log.e("Andy", "encry complete");
                else
                    Log.e("Andy", "encry failed");
            }
        }).start();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
}
