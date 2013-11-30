#include "jtclip.h"

#include <android/log.h>

#include <assert.h>
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define  LOG_TAG    "TClip" 

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__) 

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__) 

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
    //bitmap_dst = bitmap_src;
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
    clazz = (*env)->FindClass(env, className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
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

    if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
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
