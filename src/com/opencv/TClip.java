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
import org.opencv.core.MatOfRect;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;
import org.opencv.objdetect.CascadeClassifier;
import org.opencv.objdetect.Objdetect;

public class TClip {

    private static final String TAG = "TClipJAVA";

    private static final String CONFIG = "/sdcard/haarcascade_frontalface_alt.xml";

    public static int detectFace(Mat mat){

	    CascadeClassifier faceCascade = new CascadeClassifier(CONFIG);
	    
        MatOfRect faceRects = new MatOfRect();
        Imgproc.cvtColor(mat, mat, Imgproc.COLOR_RGB2GRAY);
        Imgproc.equalizeHist(mat, mat);
        faceCascade.detectMultiScale(mat, faceRects, 1.1, 2, 0|Objdetect.CASCADE_SCALE_IMAGE,
                new Size(30, 30), new Size(100, 100));

        Rect [] faces = faceRects.toArray();
        int faceSize = faces.length;
	    int Y;

        if (faceSize > 0){
            Log.d(TAG, "detectFace:faceSize is " + faceSize);
            Log.d(TAG, "detectFace:faces[0].y is " + faces[0].y);
            Log.d(TAG, "detectFace:faces[0].height is " + faces[0].height);
            Log.d(TAG, "detectFace:faces[0].width is " + faces[0].width);
            Log.d(TAG, "detectFace:faces[face_size - 1].y is " + faces[faceSize - 1].y);
            Log.d(TAG, "detectFace:faces[face_size - 1].height is " + faces[faceSize - 1].height);
            Log.d(TAG, "detectFace:faces[face_size - 1].width is " + faces[faceSize - 1].height);
            Y = faces[faceSize - 1].y - faces[faceSize - 1].height / 2;
            //fix
            if ( Y > mat.size().height / 2 ){
                return -1;
            }else{
                return Y < 0 ? 0 : Y;
            }
        }else{
            return -1;
        }
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

            //Imgproc.cvtColor(mat, mat, Imgproc.COLOR_RGB2GRAY, 4);

            int y = detectFace(mat);
            Log.d(TAG, "detectFace y " + y);

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
