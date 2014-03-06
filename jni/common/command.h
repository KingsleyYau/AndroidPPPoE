/*
 * command.h
 *
 *  Created on: 2014年2月18日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */


#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
using namespace std;

#include "DrLog.h"

/*
 * 运行命令
 */
static inline void SystemComandExecute(string command) {
	string sCommand = command;
	system(sCommand.c_str());
	showLog("jni.command.SystemComandExecute", "command:%s", sCommand.c_str());
}

/*
 * 运行带返回命令
 */
static inline string SystemComandExecuteWithResult(string command) {
	string result = "";
	string sCommand = command;
	sCommand += " 2>&1";

	FILE *ptr = popen(sCommand.c_str(), "r");
	if(ptr != NULL) {
		char buffer[2048] = {'\0'};
		while(fgets(buffer, 2048, ptr) != NULL) {
			result += buffer;
		}
		pclose(ptr);
		ptr = NULL;
	}
	showLog("jni.command.SystemComandExecuteWithResult", "command:%s \ncommand result:%s", sCommand.c_str(), result.c_str());
	return result;
}

/*
 * 以Root权限运行命令
 */
static inline void SystemComandExecuteWithRoot(string command) {
	string sCommand = command;
	sCommand = "su -c \"";
	sCommand += command;
	sCommand += "\"";
	SystemComandExecute(sCommand);
}

/*
 * 以Root权限运行带返回命令
 */
static inline string SystemComandExecuteWithRootWithResult(string command) {
	string sCommand = command;
	sCommand = "su -c \"";
	sCommand += command;
	sCommand += "\"";
	return SystemComandExecuteWithResult(sCommand);
}

/*
 * 重新挂载/system为可读写模式
 */
static inline bool MountSystem() {
	bool bFlag = false;

	char pBuffer[2048] = {'\0'};
	string result = "";

	// 获取原/system挂载
	FILE *ptr = popen("mount", "r");
	if(ptr != NULL) {

		while(fgets(pBuffer, 2048, ptr) != NULL) {
			result = pBuffer;

			if(string::npos != result.find("/system")) {
				// 找到/system挂载路径
				char* dev = strtok(pBuffer, " ");
				result = dev;
				showLog("jni.command.MountSystem", "找到/system挂载路径:%s", result.c_str());
				break;
			}
		}
		pclose(ptr);
		ptr = NULL;
	}

	// 更改挂载为rw
	sprintf(pBuffer, "mount -o remount rw,%s /system", result.c_str());
	result = SystemComandExecuteWithRootWithResult(pBuffer);
	if(result.length() == 0) {
		showLog("jni.command.MountSystem", "挂载/system为可读写成功!");
		bFlag = true;
	}

	return bFlag;
}

/*
 * 安装可执行文件到/system/bin/目录下
 */
static inline bool RootExecutableFile(string filePath) {
	bool bFlag = false;

	char pBuffer[2048] = {'\0'};
	string result = "";

	if(MountSystem()) {
		// 开始拷贝
		string fileName = filePath;
		string::size_type pos = string::npos;
		pos = filePath.find_last_of('/');
		if(string::npos != pos) {
			pos ++;
			fileName = filePath.substr(pos, filePath.length() - pos);
		}
		fileName = "/system/bin/" + fileName;

		sprintf(pBuffer, "cat %s > %s", filePath.c_str(), fileName.c_str());
		result = SystemComandExecuteWithRootWithResult(pBuffer);
		if(result.length() == 0) {
			// 拷贝成功
			showLog("jni.command.RootExecutableFile", "拷贝成功%s到%s成功!", filePath.c_str(), fileName.c_str());
			sprintf(pBuffer, "chmod 4777 %s", fileName.c_str());
			result = SystemComandExecuteWithRootWithResult(pBuffer);
			if(result.length() == 0) {
				// 更改权限成功
				showLog("jni.command.RootExecutableFile", "提升%s权限为4777成功!", fileName.c_str());
				bFlag = true;
			}
		}
	}

	return bFlag;
}
#endif /* COMMAND_H_ */
