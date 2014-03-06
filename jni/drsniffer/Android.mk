# Copyright (C) 2014 The PPOE Project
# DrSinffer Project Main Makefile
#
# Created on: 2014年2月14日
# Author: Kingsley Yau
# Email: Kingsleyyau@gmail.com
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := drsinffer

LOCAL_MODULE_FILENAME := drsinffer

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog -lssl -lcrypto

LOCAL_STATIC_LIBRARIES := drcommon

#LOCAL_CERTIFICATE := platform

REAL_PATH := $(realpath $(LOCAL_PATH))
LOCAL_SRC_FILES := $(call all-cpp-files-under, $(REAL_PATH))

LOCAL_FORCE_STATIC_EXECUTABLE := true
include $(BUILD_EXECUTABLE)
