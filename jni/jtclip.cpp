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

void nBitmapToMat2 (JNIEnv * env, jobject bitmap, Mat & dst, bool needUnPremultiplyAlpha)
{
    AndroidBitmapInfo  info;
    void*              pixels = 0;

    try {
            LOGD("nBitmapToMat");
            CV_Assert( AndroidBitmap_getInfo(env, bitmap, &info) >= 0 );
            CV_Assert( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                       info.format == ANDROID_BITMAP_FORMAT_RGB_565 );
            CV_Assert( AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0 );
            CV_Assert( pixels );
            dst.create(info.height, info.width, CV_8UC4);
            if( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 )
            {
                LOGD("nBitmapToMat: RGBA_8888 -> CV_8UC4");
                Mat tmp(info.height, info.width, CV_8UC4, pixels);
                if(needUnPremultiplyAlpha){
                    cvtColor(tmp, dst, COLOR_mRGBA2RGBA);
                }else{
                    tmp.copyTo(dst);
                }
            } else {
                // info.format == ANDROID_BITMAP_FORMAT_RGB_565
                LOGD("nBitmapToMat: RGB_565 -> CV_8UC4");
                Mat tmp(info.height, info.width, CV_8UC2, pixels);
                cvtColor(tmp, dst, CV_BGR5652RGBA);
            }
            AndroidBitmap_unlockPixels(env, bitmap);
            return;
        } catch(cv::Exception e) {
            AndroidBitmap_unlockPixels(env, bitmap);
            LOGE("nBitmapToMat catched cv::Exception: %s", e.what());
            jclass je = env->FindClass("org/opencv/core/CvException");
            if(!je) je = env->FindClass("java/lang/Exception");
            env->ThrowNew(je, e.what());
            return;
        } catch (...) {
            AndroidBitmap_unlockPixels(env, bitmap);
            LOGE("nBitmapToMat catched unknown exception (...)");
            jclass je = env->FindClass("java/lang/Exception");
            env->ThrowNew(je, "Unknown exception in JNI code {nBitmapToMat}");
            return;
        }
}

void nMatToBitmap2 (JNIEnv * env, Mat & src, jobject bitmap, bool needPremultiplyAlpha)
{
    AndroidBitmapInfo  info;
    void*              pixels = 0;

    try {
            LOGD("nMatToBitmap");
            CV_Assert( AndroidBitmap_getInfo(env, bitmap, &info) >= 0 );
            CV_Assert( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                       info.format == ANDROID_BITMAP_FORMAT_RGB_565 );
            //CV_Assert( src.dims == 2 && info.height == (uint32_t)src.rows && info.width == (uint32_t)src.cols );
            CV_Assert( src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4 );
            CV_Assert( AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0 );
            CV_Assert( pixels );
            if( info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 )
            {
                Mat tmp(info.height, info.width, CV_8UC4, pixels);
                if(src.type() == CV_8UC1)
                {
                    LOGD("nMatToBitmap: CV_8UC1 -> RGBA_8888");
                    cvtColor(src, tmp, CV_GRAY2RGBA);
                } else if(src.type() == CV_8UC3){
                    LOGD("nMatToBitmap: CV_8UC3 -> RGBA_8888");
                    cvtColor(src, tmp, CV_RGB2RGBA);
                } else if(src.type() == CV_8UC4){
                    LOGD("nMatToBitmap: CV_8UC4 -> RGBA_8888");
                    if(needPremultiplyAlpha){
                        cvtColor(src, tmp, COLOR_RGBA2mRGBA);
                    }else{
                        src.copyTo(tmp);
                    }
                }
            } else {
                // info.format == ANDROID_BITMAP_FORMAT_RGB_565
                Mat tmp(info.height, info.width, CV_8UC2, pixels);
                if(src.type() == CV_8UC1)
                {
                    LOGD("nMatToBitmap: CV_8UC1 -> RGB_565");
                    cvtColor(src, tmp, CV_GRAY2BGR565);
                } else if(src.type() == CV_8UC3){
                    LOGD("nMatToBitmap: CV_8UC3 -> RGB_565");
                    cvtColor(src, tmp, CV_RGB2BGR565);
                } else if(src.type() == CV_8UC4){
                    LOGD("nMatToBitmap: CV_8UC4 -> RGB_565");
                    cvtColor(src, tmp, CV_RGBA2BGR565);
                }
            }
            AndroidBitmap_unlockPixels(env, bitmap);
            return;
        } catch(cv::Exception e) {
            AndroidBitmap_unlockPixels(env, bitmap);
            LOGE("nMatToBitmap catched cv::Exception: %s", e.what());
            jclass je = env->FindClass("org/opencv/core/CvException");
            if(!je) je = env->FindClass("java/lang/Exception");
            env->ThrowNew(je, e.what());
            return;
        } catch (...) {
            AndroidBitmap_unlockPixels(env, bitmap);
            LOGE("nMatToBitmap catched unknown exception (...)");
            jclass je = env->FindClass("java/lang/Exception");
            env->ThrowNew(je, "Unknown exception in JNI code {nMatToBitmap}");
            return;
        }
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
    nBitmapToMat2(env, bitmap_src, img, false);
    LOGD("clip start");
    Mat dst;
    int ret = clip(img, dst, width, height);
    LOGD("clip done r=%d", ret);
    nMatToBitmap2(env, dst, bitmap_dst, false);
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
