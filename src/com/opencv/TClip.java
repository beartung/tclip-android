package com.opencv;

import android.content.Context;

import android.graphics.Bitmap;

import android.util.Log;

public class TClip {

    private static final String TAG = "TClipJAVA";

    private static final String CONFIG = "/sdcard/haarcascade_frontalface_alt.xml";

    public static native void crop_test();

    //RGB_565 || ARGB_8888
    public static native Bitmap crop(Bitmap src, int width, int height);

    static {
        try {
            System.loadLibrary("jtclip");
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

}
