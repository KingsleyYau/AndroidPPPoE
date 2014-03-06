# Copyright (C) 2014 The PPOE Project
# DrAuth Project Main Makefile
#
# Created on: 2014年2月14日
# Author: Kingsley Yau
# Email: Kingsleyyau@gmail.com
#

MAIN_LOCAL_PATH := $(call my-dir)
# 包含自定义公共宏
include $(MAIN_LOCAL_PATH)/Function.mk
include $(call all-subdir-makefiles)
include $(CLEAR_VARS)

LOCAL_PATH := $(MAIN_LOCAL_PATH)

LOCAL_MODULE := drauth

LOCAL_MODULE_FILENAME := libdrauth

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -lssl -lcrypto

LOCAL_STATIC_LIBRARIES := drcommon drppoe

#LOCAL_CERTIFICATE := platform

REAL_PATH := $(realpath $(LOCAL_PATH))
LOCAL_SRC_FILES := $(call all-cpp-files-under, $(REAL_PATH))

include $(BUILD_SHARED_LIBRARY)