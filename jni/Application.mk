# Copyright (C) 2014 The PPOE Project
# PPOE Project Application Makefile
#
# Created on: 2014年2月14日
# Author: Kingsley Yau
# Email: Kingsleyyau@gmail.com
#

APP_PLATFORM := android-4
APP_STL := stlport_static #使用STLport作为静态库
#APP_STL := stlport_shared #使用STLport作为动态库，这个可能产生兼容性和部分低版本的Android固件
#APP_STL := gnustl_static #使用 GNU libstdc++ 作为静态库
#APP_STL := system #使用默认最小的C++运行库，这样生成的应用体积小，内存占用小，但部分功能将无法支持

#APP_ABI := all #NDK-rv7之后可以使用这样方式编译支持多种芯片 [armeabi] [armeabi-v7a] [x86] [mips]
APP_ABI	:= armeabi

#STLPORT_FORCE_REBUILD := true 可以强制重新编译STLPort源码
APP_CFLAGS := -DPRINT_JNI_LOG -g -std=gnu++0x#-std=c++0x