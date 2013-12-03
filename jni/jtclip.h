#include <jni.h>

#ifndef __JTCLIP__
#define __JTCLIP__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_opencv_TClip
 * Method:    crop
 * Signature: (Ljava/lang/String;Landroid/graphics/Bitmap;II)Landroid/graphics/Bitmap;
 */
JNIEXPORT static jobject JNICALL crop(JNIEnv * env, jclass cls,
                    jobject config, jobject bitmap_src, int width, int height);

#ifdef __cplusplus
}
#endif

#endif
