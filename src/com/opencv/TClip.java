package com.opencv;

import android.content.Context;

import android.graphics.Bitmap;

import android.util.Log;

import java.lang.UnsatisfiedLinkError;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Scalar;
import org.opencv.imgproc.Imgproc;

public class TClip {

    private static final String TAG = "TClipJAVA";

    public static void init(Context context){
        OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_7, context, new BaseLoaderCallback(context){

            public void onManagerConnected(int status) {

                switch (status) {
                    case LoaderCallbackInterface.SUCCESS:
                        Log.i(TAG, "OpenCV loaded successfully");
                        break;
                    default:
                        super.onManagerConnected(status);
                        break;

                }
            }

        });
    }

    public static Bitmap gray(Bitmap src){
        Bitmap ret = null;

        if (src == null){
            return ret;
        }

        try{
            Mat mat = new Mat(src.getHeight(), src.getWidth(), CvType.CV_8U, new Scalar(4));
            Utils.bitmapToMat(src, mat);

            //Bitmap copy = src.copy(Bitmap.Config.ARGB_8888, true);
            //Utils.bitmapToMat(copy, mat);

            Imgproc.cvtColor(mat, mat, Imgproc.COLOR_RGB2GRAY, 4);

            ret = Bitmap.createBitmap(mat.cols(), mat.rows(), Bitmap.Config.ARGB_8888);
            Utils.matToBitmap(mat, ret);
        }catch (Exception e){
            e.printStackTrace();
        }

        return ret;
    }

    public static native void crop_test();

    static {
        try {
            System.loadLibrary("jtclip");
        } catch (UnsatisfiedLinkError ule) {
            ule.printStackTrace();
        }
    }

}
