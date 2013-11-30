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
 * Method:    crop
 * Signature: ()V;
 */
JNIEXPORT static void JNICALL crop(JNIEnv * env, jclass cls,
        jobject bitmap_src, jobject bitmap_dst, int width, int height);

#ifdef __cplusplus
}
#endif

#endif
