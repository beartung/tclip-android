package com.opencv;

import android.content.Context;

import android.graphics.Bitmap;

import android.util.Log;

import java.lang.UnsatisfiedLinkError;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfKeyPoint;
import org.opencv.core.MatOfRect;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.features2d.FeatureDetector;
import org.opencv.features2d.KeyPoint;
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

    private static int detectCharacter(Mat mat){
        int startX = 0; //特征点X坐标开始位置 
        int endX = 0; //特征点X坐标结束位置
        int sectionIndex = 0; //Y坐标段数字索引
        HashMap<Integer, Integer> sectionNum; //每个Y坐标段中特征点的数量
        int total = 0; //总共特征点数量
        int avg = 0; //每个Y坐标段的平均特征点数量
        int conNum = 4; //需要连续的阀值 
        int flag = 0;
        int counter = 0;
        int Y = 0;

        //cv::initModule_nonfree();//使用SIFT/SURF create之前，必须先initModule_<modulename>(); 

        FeatureDetector detector = FeatureDetector.create(FeatureDetector.SURF);

        if(detector.empty()){
            Log.d(TAG, "Can not create detector or descriptor exstractor or descriptor matcher of given types");
            return -1;
        }

        //startX = mat.size().width / 5;
        //endX = startX * 4;
        startX = 0;
        endX = (int)mat.size().width;

        MatOfKeyPoint matKeypoints = new MatOfKeyPoint();
        detector.detect(mat, matKeypoints);
        KeyPoint[] keypoints = matKeypoints.toArray();
        sectionNum = new HashMap();
        for (int i = 0; i < keypoints.length; i++){
            if (keypoints[i].pt.x > startX && keypoints[i].pt.x < endX){
                sectionIndex = (int)Math.ceil(keypoints[i].pt.y / 10);
                if (sectionNum.containsKey(sectionIndex)){
                    sectionNum.put(sectionIndex, sectionNum.get(sectionIndex) + 1);
                }else{
                    sectionNum.put(sectionIndex, 1); 
                }
                total = total + 1;
            }
        }
        avg = total / sectionNum.size();

        Iterator iter = sectionNum.entrySet().iterator(); 
        while (iter.hasNext()) { 
            Map.Entry entry = (Map.Entry) iter.next(); 
            Integer key = (Integer)entry.getKey(); 
            Integer val = (Integer)entry.getValue(); 
            Log.d(TAG, key + " " +  val);
        }

        Log.d(TAG, "detectCharacter:avg is " + avg);

        //检测特征点分布是否均匀
        int sliceTotal = 10 ; 
        int sliceNum = sectionNum.size() / sliceTotal;
        int sliceCounter = 0;
        for (int m = 0; m < sliceTotal; m++){
            for (int n = m * sliceNum; n < (m + 1) * sliceNum; n++){
                if ( sectionNum.get(n) >= avg ){
                    sliceCounter++;
                    break;
                }
            }
        }
        if (sliceCounter >= sliceTotal){
            return -1;
        }

        //检测特征点主要分布区域[找最开始连续大于avg的Y]
        iter = sectionNum.entrySet().iterator(); 
        while (iter.hasNext()) { 
            Map.Entry entry = (Map.Entry) iter.next(); 
            Integer key = (Integer)entry.getKey(); 
            Integer val = (Integer)entry.getValue(); 
            if (val.intValue() >= avg && flag == 0){
                counter++;
            } else {
                counter = 0;
            }
            if (counter >= conNum && flag == 0){
                Y = key;
                flag = 1;
            }
        }

        if (Y > conNum && Y < mat.size().height / 4){
            return (Y - conNum - 11) * sliceTotal < 0 ? 0 : (Y - conNum - 11) * sliceTotal ;//fix
        }else if(Y > conNum){
            return (Y - conNum) * sliceTotal;
        }
        return Y * 10;
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

            int y;
            //y = detectFace(mat);
            //Log.d(TAG, "detectFace y " + y);
            y = detectCharacter(mat);
            Log.d(TAG, "detectCharacter y " + y);

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
