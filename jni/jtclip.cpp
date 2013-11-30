#include "jtclip.h"
#include "tclip.h"

#include <android/log.h>

#include <assert.h>
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cv.h"  
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

#include <android/bitmap.h>
#include "cv.h"  

#define  LOG_TAG    "TClip" 

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__) 
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__) 
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__) 

using namespace cv;
using namespace std;

/*
 * Class:     com_opencv_TClip
 * Method:    crop_test
 * Signature: ()V
 */
JNIEXPORT static void JNICALL crop_test(JNIEnv * env, jclass cls){
    LOGI("C: crop_test");
}

/*
 * Class:     com_opencv_TClip
 * Method:    crop
 * Signature: ()V;
 */
JNIEXPORT static void JNICALL crop(JNIEnv * env, jclass cls,
        jobject bitmap_src, jobject bitmap_dst, int width, int height){
    LOGI("C: crop");
    Mat img;
    void*              pixels = 0;
    AndroidBitmapInfo  info;
    try {
        LOGD("nBitmapToMat");
        CV_Assert( AndroidBitmap_getInfo(env, bitmap_src, &info) >= 0 );
        CV_Assert( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                info.format == ANDROID_BITMAP_FORMAT_RGB_565 );
        CV_Assert( AndroidBitmap_lockPixels(env, bitmap_src, &pixels) >= 0 );
        CV_Assert( pixels );
        img.create(info.height, info.width, CV_8UC4);
        if( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 )
        {
            LOGD("nBitmapToMat: RGBA_8888 -> CV_8UC4");
            Mat tmp(info.height, info.width, CV_8UC4, pixels);
            /*
               if(needUnPremultiplyAlpha) cvtColor(tmp, dst, COLOR_mRGBA2RGBA);
               else tmp.copyTo(dst);
               */
            tmp.copyTo(img);
        } else {
            // info.format == ANDROID_BITMAP_FORMAT_RGB_565
            LOGD("nBitmapToMat: RGB_565 -> CV_8UC4");
            Mat tmp(info.height, info.width, CV_8UC2, pixels);
            cvtColor(tmp, img, CV_BGR5652RGBA);
        }
        
        AndroidBitmap_unlockPixels(env, bitmap_src);

        detectFace(img, "/sdcard/haarcascade_frontalface_alt.xml");

        return;
    } catch(cv::Exception e) {
        AndroidBitmap_unlockPixels(env, bitmap_src);
        LOGE("nBitmapToMat catched cv::Exception: %s", e.what());
        jclass je = env->FindClass("org/opencv/core/CvException");
        if(!je) je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, bitmap_src);
        LOGE("nBitmapToMat catched unknown exception (...)");
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Unknown exception in JNI code {nBitmapToMat}");
        return;
    }
}

#define JNIREG_CLASS "com/opencv/TClip"

/**
* Table of methods associated with a single class.
*/
static JNINativeMethod gMethods[] = {
    { "crop_test", "()V", (void*)crop_test},
    { "crop", "(Landroid/graphics/Bitmap;Landroid/graphics/Bitmap;II)V", (void*)crop},
};

/*
* Register several native methods for one class.
*/
static int registerNativeMethods(JNIEnv* env, const char* className,
        JNINativeMethod* gMethods, int numMethods){

    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
* Register native methods for all classes we know about.
*/
static int registerNatives(JNIEnv* env){

    if (!registerNativeMethods(env, JNIREG_CLASS, gMethods,
                                 sizeof(gMethods) / sizeof(gMethods[0])))
        return JNI_FALSE;

    return JNI_TRUE;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved){
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    assert(env != NULL);

    if (!registerNatives(env)) {
        return -1;
    }

    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

    return result;
}
