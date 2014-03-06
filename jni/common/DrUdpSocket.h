/*
 * DrUDPSocket.h
 *
 *  Created on: 2014年2月22日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef DRUDPSOCKET_H_
#define DRUDPSOCKET_H_

#include "DrSocket.h"

class DrUdpSocket : public DrSocket {
public:
	DrUdpSocket();
	virtual ~DrUdpSocket();

	int Bind(unsigned int iPort, string ip = "", bool bBlocking = false);
	int SendData(string ip, unsigned int iPort, char* pBuffer, unsigned int uiSendLen);
	int RecvData(char* pBuffer, unsigned int uiRecvLen, unsigned int uiTimeout = 1000, struct sockaddr_in* remoteAddr = NULL);
	int Close();

private:
	int m_iPort;
	bool m_bBlocking;
};

#endif /* DRUDPSOCKET_H_ */
