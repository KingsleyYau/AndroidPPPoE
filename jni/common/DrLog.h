/*
 * DrLog.h
 *
 *  Created on: 2014年2月14日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef _INC_DRLOG_
#define _INC_DRLOG_

#include <string.h>

#ifdef PRINT_JNI_LOG
#include <android/log.h>
#define showLog(tag, format, ...) __android_log_print(ANDROID_LOG_INFO, tag, format, ## __VA_ARGS__);printf(format, ## __VA_ARGS__);printf("\r\n");
#else
#define showLog(tag, format, ...)
#endif

#endif
