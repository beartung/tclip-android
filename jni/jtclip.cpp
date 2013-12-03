#include "common.h"  
#include "jtclip.h"
#include "tclip.h"

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
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

using namespace cv;
using namespace std;

char * jstring_to_str(JNIEnv * env, jobject jstr){
    char * rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jobject strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte * ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0){
        rtn = new char[alen + 1];
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

void bitmap_to_mat(JNIEnv * env, jobject bitmap, Mat & dst, bool needUnPremultiplyAlpha){
    AndroidBitmapInfo  info;
    void*              pixels = 0;

    try {
            CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
            CV_Assert(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                       info.format == ANDROID_BITMAP_FORMAT_RGB_565);
            CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);
            CV_Assert(pixels);
            dst.create(info.height, info.width, CV_8UC4);
            if(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888){
                Mat tmp(info.height, info.width, CV_8UC4, pixels);
                if(needUnPremultiplyAlpha){
                    cvtColor(tmp, dst, COLOR_mRGBA2RGBA);
                }else{
                    tmp.copyTo(dst);
                }
            }else{
                // info.format == ANDROID_BITMAP_FORMAT_RGB_565
                Mat tmp(info.height, info.width, CV_8UC2, pixels);
                cvtColor(tmp, dst, CV_BGR5652RGBA);
            }
            AndroidBitmap_unlockPixels(env, bitmap);
            return;
        }catch(cv::Exception e){
            AndroidBitmap_unlockPixels(env, bitmap);
            LOGE("nBitmapToMat catched cv::Exception: %s", e.what());
            jclass je = env->FindClass("org/opencv/core/CvException");
            if(!je) je = env->FindClass("java/lang/Exception");
            env->ThrowNew(je, e.what());
            return;
        }catch (...){
            AndroidBitmap_unlockPixels(env, bitmap);
            LOGE("nBitmapToMat catched unknown exception (...)");
            jclass je = env->FindClass("java/lang/Exception");
            env->ThrowNew(je, "Unknown exception in JNI code {nBitmapToMat}");
            return;
        }
}

jobject mat_to_bitmap(JNIEnv * env, Mat & src, bool needPremultiplyAlpha, jobject bitmap_config){

    jclass java_bitmap_class = (jclass)env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetStaticMethodID(java_bitmap_class,
            "createBitmap", "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");

    jobject bitmap = env->CallStaticObjectMethod(java_bitmap_class,
                        mid, src.size().width, src.size().height, bitmap_config);

    AndroidBitmapInfo  info;
    void*              pixels = 0;

    try {
            CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
            CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4);
            CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);
            CV_Assert(pixels);
            if(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888){
                Mat tmp(info.height, info.width, CV_8UC4, pixels);
                if(src.type() == CV_8UC1){
                    cvtColor(src, tmp, CV_GRAY2RGBA);
                }else if(src.type() == CV_8UC3){
                    cvtColor(src, tmp, CV_RGB2RGBA);
                }else if(src.type() == CV_8UC4){
                    if(needPremultiplyAlpha){
                        cvtColor(src, tmp, COLOR_RGBA2mRGBA);
                    }else{
                        src.copyTo(tmp);
                    }
                }
            }else{
                // info.format == ANDROID_BITMAP_FORMAT_RGB_565
                Mat tmp(info.height, info.width, CV_8UC2, pixels);
                if(src.type() == CV_8UC1){
                    cvtColor(src, tmp, CV_GRAY2BGR565);
                }else if(src.type() == CV_8UC3){
                    cvtColor(src, tmp, CV_RGB2BGR565);
                }else if(src.type() == CV_8UC4){
                    cvtColor(src, tmp, CV_RGBA2BGR565);
                }
            }
            AndroidBitmap_unlockPixels(env, bitmap);
            return bitmap;
        }catch(cv::Exception e){
            AndroidBitmap_unlockPixels(env, bitmap);
            LOGE("nMatToBitmap catched cv::Exception: %s", e.what());
            jclass je = env->FindClass("org/opencv/core/CvException");
            if(!je) je = env->FindClass("java/lang/Exception");
            env->ThrowNew(je, e.what());
            return bitmap;
        }catch (...){
            AndroidBitmap_unlockPixels(env, bitmap);
            LOGE("nMatToBitmap catched unknown exception (...)");
            jclass je = env->FindClass("java/lang/Exception");
            env->ThrowNew(je, "Unknown exception in JNI code {nMatToBitmap}");
            return bitmap;
        }

}

/*
 * Class:     com_opencv_TClip
 * Method:    crop
 * Signature: (Ljava/lang/String;Landroid/graphics/Bitmap;II)Landroid/graphics/Bitmap;
 */
JNIEXPORT static jobject JNICALL crop(JNIEnv * env, jclass cls,
                    jobject config, jobject bitmap_src, int width, int height){

    //first convert Bitmap to Mat
    Mat img;
    bitmap_to_mat(env, bitmap_src, img, false);
    //LOGD("clip start");

    //do clipping & get cropped Mat
    Mat dst;
    char * config_str = jstring_to_str(env, config);
    int ret = clip(img, dst, config_str, width, height);
    //LOGD("clip mat width=%d, height=%d", dst.size().width, dst.size().height);

    //get source bitmap's config
    jclass java_bitmap_class = (jclass)env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetMethodID(java_bitmap_class, "getConfig", "()Landroid/graphics/Bitmap$Config;");
    jobject bitmap_config = env->CallObjectMethod(bitmap_src, mid);

    //create new Bitmap with cropped Mat
    jobject bitmap_dst = mat_to_bitmap(env, dst, false, bitmap_config);
    
    //clean up
    if (config_str){
        delete [] config_str;
    }
    return bitmap_dst;
}

//Java Class
#define JNIREG_CLASS "com/opencv/TClip"

/**
* Table of methods associated with a single class.
*/
static JNINativeMethod gMethods[] = {
    {"crop", "(Ljava/lang/String;Landroid/graphics/Bitmap;II)Landroid/graphics/Bitmap;", (void*)crop},
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
