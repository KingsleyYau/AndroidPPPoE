/*
 * Source for class com_drcom_ppoe_DrPPOEJni
 *
 *  Created on: 2014年2月14日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */
#include "com_drcom_drauth_DrAuthJni.h"

#include <unistd.h>
#include <string>
#include <stl/_string.h>

#include "common/IPAddress.h"
#include "common/command.h"
#include "common/DrCommandHandler.h"

#include "drppoe/DrppoePlugin.h"
#include "drsniffer/DrSinfferExecuteDef.h"


#include "ppoedef.h"

DrppoePlugin g_DrppoePlugin;
DrCommandHandler g_DrCommandHandler;

inline bool StatusPPPoE() {
	bool bFlag = false;
	list<string> deviceList = IPAddress::GetDeviceList();
	for(list<string>::iterator itr = deviceList.begin(); itr != deviceList.end(); itr++) {
		string device = *itr;
		if(string::npos != device.find("ppp", 0)) {
			// found device like 'ppp0'
			bFlag = true;
			break;
		}
	}
	return bFlag;
}
/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    IsRoot
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_drcom_drauth_DrAuthJni_IsRoot
	(JNIEnv *, jobject) {
	jboolean jbFlag = false;

	string version = GetDrSnifferVersion();
	if(version.length() > 0) {
		jbFlag = true;
	}

	return jbFlag;
}

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    ReleaseDrSniffer
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_drcom_drauth_DrAuthJni_ReleaseDrSniffer
	(JNIEnv *env, jobject thiz, jstring sdcardPath)  {
	jboolean jbFlag = false;

	const char *pPath = env->GetStringUTFChars(sdcardPath, 0);
	string filePath = pPath;
	env->ReleaseStringUTFChars(sdcardPath, pPath);

	// 安装DrSniffer
	string releaseFile = ReleaseDrSniffer(filePath);
	if(releaseFile.length() > 0) {
		jbFlag = RootExecutableFile(releaseFile.c_str());
		if(jbFlag) {
			showLog("jni.DrAuthJni.ReleaseDrSniffer", "安装DrSniffer成功!");
		}
	}

	return jbFlag;
}
/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    SystemComandExcute
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_drcom_drauth_DrAuthJni_SystemComandExcute
	(JNIEnv *env, jobject thiz, jstring command) {
	jstring valueString;

	const char *pCommand = env->GetStringUTFChars(command, 0);
	string result = SystemComandExecuteWithResult(pCommand);
	env->ReleaseStringUTFChars(command, pCommand);

	valueString = env->NewStringUTF(result.c_str());
	return valueString;
}

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    SystemComandExcuteWithRoot
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_drcom_drauth_DrAuthJni_SystemComandExcuteWithRoot
	(JNIEnv *env, jobject thiz, jstring command) {
	jstring valueString;

	const char *pCommand = env->GetStringUTFChars(command, 0);
	string result = SystemComandExecuteWithRootWithResult(pCommand);
	env->ReleaseStringUTFChars(command, pCommand);

	valueString = env->NewStringUTF(result.c_str());
	return valueString;
}

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    GetDevice
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_drcom_drauth_DrAuthJni_GetDevice
	(JNIEnv *env, jobject thiz) {
	jobjectArray array = 0;

	list<string> maclist = IPAddress::GetDeviceList();
	array = env->NewObjectArray(maclist.size(), env->FindClass("java/lang/String"), 0);

	int i = 0;
	for(list<string>::iterator itr = maclist.begin(); itr != maclist.end(); itr++) {
		jstring jstr = env->NewStringUTF((*itr).c_str());
		env->SetObjectArrayElement(array, i++, jstr);
	}
	return array;
}

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    GetMacAddress
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_drcom_drauth_DrAuthJni_GetMacAddress
	(JNIEnv *env, jobject thiz) {
	jobjectArray array = 0;

	list<string> maclist = IPAddress::GetMacAddressList();
	array = env->NewObjectArray(maclist.size(), env->FindClass("java/lang/String"), 0);

	int i = 0;
	for(list<string>::iterator itr = maclist.begin(); itr != maclist.end(); itr++) {
		jstring jstr = env->NewStringUTF((*itr).c_str());
		env->SetObjectArrayElement(array, i++, jstr);
	}
	return array;
}

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    GetIPAddress
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_com_drcom_drauth_DrAuthJni_GetIPAddress
	(JNIEnv *env, jobject thiz) {
	jobjectArray array = 0;

	list<string> maclist = IPAddress::GetIPAddress();
	array = env->NewObjectArray(maclist.size(), env->FindClass("java/lang/String"), 0);

	int i = 0;
	for(list<string>::iterator itr = maclist.begin(); itr != maclist.end(); itr++) {
		jstring jstr = env->NewStringUTF((*itr).c_str());
		env->SetObjectArrayElement(array, i++, jstr);
	}
	return array;
}

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    LoginPPOE
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_drcom_drauth_DrAuthJni_LoginPPOE
	(JNIEnv *env, jobject thiz, jstring username, jstring password) {

	SnifferCommand scmd;
	scmd.scmdt = StartSinfferType;
	g_DrCommandHandler.SendCommand(scmd);

	jint iCode = 0;

	const char *pUsername = env->GetStringUTFChars(username, 0);
	const char *pPassword = env->GetStringUTFChars(password, 0);

	showLog("jni.LoginPPOE", "开始PPPoE拨号...");

	string sInterfaceName = "";
	string loginCmd = DrPPPD;
	loginCmd += " pty '";
	loginCmd += DrPPPOE;
	loginCmd += " -p ";
	loginCmd += DrPPPID;
	loginCmd += " -I ";

	list<string> deviceList = IPAddress::GetDeviceList();
	for(list<string>::iterator itr = deviceList.begin(); itr != deviceList.end(); itr++) {
		string device = *itr;
		if(string::npos != device.find("wlan", 0)) {
			// found device like 'wlan0'
		}
		else if(string::npos != device.find("eth", 0)) {
			// found device like 'eth0'
		}
		else if(string::npos != device.find("en", 0)) {
			// found device like 'en0'
		}
		loginCmd += device;
		break;
	}

	loginCmd += " -T 30 -U -m 1412' noipdefault noauth default-asyncmap defaultroute hide-password nodetach usepeerdns mtu 1492 mru 1492 noaccomp nodeflate nopcomp novj novjccomp user ";
	loginCmd += pUsername;
	loginCmd += " password ";
	loginCmd += pPassword;
	loginCmd += " lcp-echo-interval 20 lcp-echo-failure 3 &";

	SystemComandExecuteWithRoot(loginCmd);

	loginCmd = "chmod 777 ";
	loginCmd += DrPPPID;
	SystemComandExecuteWithRoot(loginCmd);

	sleep(3);
	// 检测是否在线
	if(StatusPPPoE()) {
		Java_com_drcom_drauth_DrAuthJni_StatusPPOE(env, thiz);
		showLog("jni.LoginPPOE", "PPPoE拨号成功!");
		iCode = true;
	}
	else {
		iCode = false;
	}

	env->ReleaseStringUTFChars(username, pUsername);
	env->ReleaseStringUTFChars(password, pPassword);
	return iCode;
}

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    LogoutPPOE
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_drcom_drauth_DrAuthJni_LogoutPPOE
	(JNIEnv *env, jobject thiz) {

	SnifferCommand scmd;
	scmd.scmdt = StopSnifferType;
	g_DrCommandHandler.SendCommand(scmd);

	jint iCode = 0;

	char cmd[1024] = {'\0'};
	sprintf(cmd, "setprop net.pppoe.ppp-exit %s", DrPPPID);
	SystemComandExecuteWithRoot(cmd);
	SystemComandExecuteWithRoot("setprop net.pppoe.reason gone");
	SystemComandExecuteWithRoot("setprop net.pppoe.interface");

	FILE* file = fopen(DrPPPID, "r");
	if(NULL != file) {
		showLog("jni.LogoutPPOE", "file is not NULL");
		int pid = -1;
		fscanf(file, "%d", &pid);
		showLog("jni.LogoutPPOE", "pid:%d", pid);
		if(-1 != pid) {
			char cmd[1024] = {'\0'};
			sprintf(cmd, "kill -s HUP %d", pid);
			SystemComandExecuteWithRoot(cmd);

			sprintf(cmd, "rm %s", DrPPPID);
			SystemComandExecuteWithRoot(cmd);
		}
		fclose(file);
	}

	sleep(2);
//	if(!StatusPPPoE()) {
		// 注销成功, 停止DrPPPoE插件协议
		showLog("jni.LogoutPPOE", "PPPoE注销成功, 停止DrPPPoE插件协议");
		g_DrppoePlugin.StopDrPPPoEPlugin();
//	}

	return iCode;
}

/*
 * Class:     com_drcom_drauth_DrAuthJni
 * Method:    StatusPPOE
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_drcom_drauth_DrAuthJni_StatusPPOE
	(JNIEnv *env, jobject) {
	jboolean jbFlag = StatusPPPoE();
	if(jbFlag) {
		// 开始DrPPPoE插件协议
		showLog("jni.StatusPPOE", "检测到PPPoE已经拨号");
		if(!g_DrppoePlugin.IsRunning()) {
			g_DrppoePlugin.StartDrPPPoEPlugin();
		}
	}
	else {
		showLog("jni.StatusPPOE", "没有检测到PPPoE拨号!");
	}
	return jbFlag;
}
