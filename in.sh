#!/bin/bash
gradle build; adb install -r build/apk/xpaper-debug-unaligned.apk; adb shell am start -n com.bear.xpaper.debug/com.bear.xpaper.app.Welcome
