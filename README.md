#Bitmap Smart Clipping using OpenCV
=================

### based on [http://code.taobao.org/p/tclip/](http://code.taobao.org/p/tclip/)

#Demo Screenshots
=================

![demo screenshots](https://raw.github.com/beartung/tclip-android/master/screenshots/s1.png "Demo Screenshots")

#Features
=================

* using OpenCV to detect faces firstly, if have faces, won't cut faces off
* using OpenCV to detect other characters secondly, if found significant zone, won't cut it off

#Usage
=================

* copy config file to app dir

    ```
    TClip.copyConfig(context, TClip.CONFIG, R.raw.haarcascade_frontalface_alt);
    ```

* get cropped bitmap

    ```
    //config: /data/data/com.demo.tclip.debug/haarcascade_frontalface_alt.xml
    Bitmap ret = TClip.crop(configPath, sourceBitmap, width, height);
    ```
