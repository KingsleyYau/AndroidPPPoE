/*
 * DrSnifferMain.cpp
 *
 *  Created on: 2014年3月3日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#include "DrSniffer.h"
#include "DrSinfferExecuteDef.h"

#include "../common/DrCommandHandler.h"

DrSniffer g_DrSniffer;
DrCommandHandler g_DrCommandHandler;
int main(int argc, char** argv) {
	char stra[16] = {'\0'};
	sprintf(stra, "%d", argc);

	if(2 == argc) {
		if(0 == strcmp(argv[1], "-v")) {
			printf("%s%s\n", DrSinfferVersoinString, DrSnifferVersion);
			return 0;
		}
	}

	showLog("jni.DrSniffer.main", "drsniffer启动!");
	int uid = 0, gid = 0;
	if(setgid(gid) || setuid(uid)) {
		showLog("jni.DrSniffer.main", "权限不够,请提升至管理员权限再运行程序!");
	}

	if(!g_DrCommandHandler.BindLocal()) {
		// 绑定失败,直接开始监听
		g_DrSniffer.StartSniffer();
	}

	while(1) {
		SnifferCommand scmd = g_DrCommandHandler.RecvCommand();
		SnifferCommandType scmdt = scmd.scmdt;
		switch(scmdt){
		case StartSinfferType:{
			// 开始监听
			g_DrSniffer.StartSniffer();
		}break;
		case StopSnifferType:{
			// 停止监听
			g_DrSniffer.StopSniffer();
		}break;
		default: {

		}break;
		}
	}

	showLog("jni.DrSniffer.main", "drsniffer退出!");
	return 1;
}
