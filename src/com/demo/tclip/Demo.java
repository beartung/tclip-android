package com.demo.tclip;

import android.app.Activity;

import android.os.Bundle;

import com.opencv.TClip;

public class Demo extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
    }

    public void onResume(){
        super.onResume(); 
        TClip.crop_test();
    }
}
