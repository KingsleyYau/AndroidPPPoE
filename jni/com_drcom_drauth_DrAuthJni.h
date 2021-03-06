/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_drcom_drauth_DrAuthJni */

#ifndef _Included_com_drcom_drauth_DrAuthJni
#define _Included_com_drcom_drauth_DrAuthJni
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    IsRoot
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_drcom_drauth_DrAuthJni_IsRoot
  (JNIEnv *, jobject);

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    ReleaseDrSniffer
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_drcom_drauth_DrAuthJni_ReleaseDrSniffer
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    SystemComandExcute
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_drcom_drauth_DrAuthJni_SystemComandExcute
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    SystemComandExcuteWithRoot
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_drcom_drauth_DrAuthJni_SystemComandExcuteWithRoot
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    GetDevice
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_drcom_drauth_DrAuthJni_GetDevice
  (JNIEnv *, jobject);

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    GetMacAddress
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_drcom_drauth_DrAuthJni_GetMacAddress
  (JNIEnv *, jobject);

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    GetIPAddress
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_drcom_drauth_DrAuthJni_GetIPAddress
  (JNIEnv *, jobject);

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    LoginPPOE
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_drcom_drauth_DrAuthJni_LoginPPOE
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    LogoutPPOE
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_drcom_drauth_DrAuthJni_LogoutPPOE
  (JNIEnv *, jobject);

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    StatusPPOE
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_drcom_drauth_DrAuthJni_StatusPPOE
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
