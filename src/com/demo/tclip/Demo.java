package com.demo.tclip;

import android.app.Activity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import android.os.Bundle;

import android.util.Log;

import android.widget.ImageView;

import com.opencv.TClip;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;

public class Demo extends Activity {

    private static final String TAG = "Demo";
    private ImageView img;

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        img = (ImageView)findViewById(R.id.img);
    }

    private BaseLoaderCallback callback = new BaseLoaderCallback(this){

        public void onManagerConnected(int status) {

            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                    Log.i(TAG, "OpenCV loaded successfully");
                    Bitmap src = getPic();
                    Bitmap gray = TClip.gray(src);
                    img.setImageBitmap(gray);
                    src.recycle();
                    break;
                default:
                    super.onManagerConnected(status);
                    break;

            }
        }

    };

    public void onResume(){
        super.onResume(); 
        TClip.crop_test();

        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_7, this, callback);
    }

    private Bitmap getPic(){
        File f = new File("/sdcard/a1.jpg"); 
        Bitmap ret = null;
        try {
            InputStream stream = new FileInputStream(f);
            ret = BitmapFactory.decodeStream(stream);
            if (stream != null) stream.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return ret;
    }

}
