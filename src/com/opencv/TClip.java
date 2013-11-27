package com.opencv;

import java.lang.UnsatisfiedLinkError;

public class TClip {

    public static native void crop_test();

    static {
        try {
            System.loadLibrary("jtclip");
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

}
