#Bitmap Smart Clipping using OpenCV

### based on [http://code.taobao.org/p/tclip/](http://code.taobao.org/p/tclip/)

#### recommend iOS version: [https://github.com/croath/UIImageView-BetterFace](https://github.com/croath/UIImageView-BetterFace)

#Demo Screenshots

![demo screenshots](https://raw.github.com/beartung/tclip-android/master/screenshots/s1.png "Demo Screenshots")


#Features

* using OpenCV to detect faces firstly, if have faces, won't cut faces off
* using OpenCV to detect other characters secondly, if found significant zone, won't cut it off
* using FAST feature detector instead of SURF, thanks for [@yanunon](https://github.com/yanunon)

#Usage

* copy config file to app dir

    ```
    String configPath = TClip.copyConfig(context, TClip.CONFIG, R.raw.haarcascade_frontalface_alt);
    ```

* get cropped bitmap

    ```
    //config: /data/data/com.demo.tclip.debug/haarcascade_frontalface_alt.xml
    Bitmap ret = TClip.crop(configPath, sourceBitmap, width, height);
    ```

#Build

* download OpenCV & unzip to /home/user/opencv-android-sdk
* export OPENCV_PACKAGE_DIR="/home/user/opencv-android-sdk"
* ~~libnonfree.so & libopencv_java.so from [beartung/nonfree-opencv-android](https://github.com/beartung/nonfree-opencv-android)~~
* using FAST feature detector without libnonfree
* cd jni && ndk-build
