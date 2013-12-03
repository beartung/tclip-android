package com.opencv;

import android.content.Context;
import android.content.res.AssetManager;

import android.graphics.Bitmap;

import android.util.Log;

public class TClip {

    private static final String TAG = "TClipJAVA";

    private static final String CONFIG = "/sdcard/haarcascade_frontalface_alt.xml";

    //RGB_565 || ARGB_8888
    public static native Bitmap crop(String config, Bitmap src, int width, int height);

    static {
        try {
            System.loadLibrary("jtclip");
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

}
