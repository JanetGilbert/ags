#!/bin/bash

unset DEVROOT SDKROOT CFLAGS CC LD CPP CXX AR AS NM CXXCPP RANLIB LDFLAGS CPPFLAGS CXXFLAGS

export IOS_BASE_SDK="8.1"
export IOS_DEPLOY_TGT="4.3"
export DEVROOT=/Applications/Xcode.app/Contents/Developer
export SDKROOT=$DEVROOT/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS$IOS_BASE_SDK.sdk
export IOS_ADDITIONAL_LIBRARY_PATH=$(pwd)/../../nativelibs/fat
export IOS_HOST_NAME=arm-apple-darwin10

export PATH="$PATH;$DEVROOT/usr/bin"

export CPP="$DEVROOT/usr/bin/cpp"
export CXX="$DEVROOT/usr/bin/g++"
export CXXCPP="$DEVROOT/usr/bin/cpp"
export CC="$DEVROOT/usr/bin/gcc"
export LD=$DEVROOT/usr/bin/ld
export AR=$DEVROOT/usr/bin/ar
export AS=$DEVROOT/usr/bin/as
export NM=$DEVROOT/usr/bin/nm
export RANLIB=$DEVROOT/usr/bin/ranlib
