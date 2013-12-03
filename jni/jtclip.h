#include <jni.h>

#ifndef __JTCLIP__
#define __JTCLIP__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_opencv_TClip
 * Method:    crop_test
 * Signature: ()V;
 */
JNIEXPORT static void JNICALL crop_test(JNIEnv * env, jclass cls);

/*
 * Class:     com_opencv_TClip
 * Method:    init 
 * Signature: (Landroid/content/res/AssetManager;Ljava/lang/String;)V;
 */
JNIEXPORT static void JNICALL init(JNIEnv * env, jclass cls, jobject manager, jobject cascade_config);

/*
 * Class:     com_opencv_TClip
 * Method:    crop
 * Signature: (Landroid/graphics/Bitmap;II)Landroid/graphics/Bitmap;
 */
JNIEXPORT static jobject JNICALL crop(JNIEnv * env, jclass cls, jobject bitmap_src, int width, int height);

#ifdef __cplusplus
}
#endif

#endif
