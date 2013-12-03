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

public class Demo extends Activity {

    private static final String TAG = "Demo";
    private ImageView img;

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        img = (ImageView)findViewById(R.id.img);
    }

    public void onResume(){
        super.onResume(); 

        TClip.init(getResources().getAssets(), "haarcascade_frontalface_alt.xml");

        TClip.crop_test();

        Bitmap pic = getPic();
        Bitmap cropped = TClip.crop(pic, 200, 100);
        img.setImageBitmap(cropped);

    }

    private Bitmap getPic(){
        File f = new File("/sdcard/a1.jpg"); 
        //File f = new File("/sdcard/b1.jpg"); 
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
