/*
 * DrCommandHandler.cpp
 *
 *  Created on: 2014年3月6日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#include "DrCommandHandler.h"

DrCommandHandler::DrCommandHandler() {
	// TODO Auto-generated constructor stub
	m_sServerAddress = "127.0.0.1";
	m_iPort = 61441;
}

DrCommandHandler::~DrCommandHandler() {
	// TODO Auto-generated destructor stub
}
bool DrCommandHandler::BindLocal() {
	if(!m_UdpSocket.Bind(m_iPort, m_sServerAddress, true)) {
		showLog("jni.DrCommandHandler.BindLocal", "绑定端口(%d)失败", m_iPort);
		return false;
	}
	else {
		showLog("jni.DrCommandHandler.BindLocal", "绑定端口(%d)成功", m_iPort);
	}
	return true;
}
bool DrCommandHandler::SendCommand(SnifferCommand scmd) {
	showLog("jni.DrCommandHandler::SendCommand", "发送命令...");
	DrUdpSocket udpSocket;
    int iRet = udpSocket.SendData(m_sServerAddress, m_iPort, (char *)&scmd, sizeof(SnifferCommand));
    if(iRet < 0) {
    	showLog("jni.DrCommandHandler::SendCommand", "网络连接中断，发送命令错误!");
    	return false;
    }
    return true;
}
SnifferCommand DrCommandHandler::RecvCommand() {

	SnifferCommand scmd;
	bzero(&scmd, sizeof(SnifferCommand));

	struct sockaddr_in remoteAddr;
	bzero(&remoteAddr, sizeof(remoteAddr));

	showLog("jni.DrCommandHandler::RecvCommand", "等待命令...");
	int iRet = m_UdpSocket.RecvData((char *)&scmd, sizeof(SnifferCommand), 0,  &remoteAddr);
	if (iRet > 0) {
		showLog("jni.DrCommandHandler::RecvCommand", "接收到命令类型:%d", scmd.scmdt);
	}

	return scmd;
}
