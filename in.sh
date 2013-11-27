#!/bin/bash
gradle build; adb install -r build/apk/tclip-android-debug-unaligned.apk; adb shell am start -n com.demo.tclip.debug/com.demo.tclip.Demo
