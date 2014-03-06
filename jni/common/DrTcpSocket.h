/*
 * DrTcpSocket.h
 *
 *  Created on: 2014年2月24日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef DRTCPSOCKET_H_
#define DRTCPSOCKET_H_

#include "DrSocket.h"

#include <netdb.h>
#include <netinet/tcp.h>
#include <netinet/in.h>

class DrTcpSocket: public DrSocket {
public:
	DrTcpSocket();
	virtual ~DrTcpSocket();
	DrTcpSocket & operator=(const DrTcpSocket &ojb);

	virtual int Connect(string strAddress, unsigned int uiPort = 80);
	virtual int SendData(char* pBuffer, unsigned int uiSendLen, unsigned int uiTimeout = 3000);
	virtual int RecvData(char* pBuffer, unsigned int uiRecvLen, bool bRecvAll = true, unsigned int uiTimeout = 1000);
	virtual int Close();
	int getSocket();

protected:
	virtual int BaseConnect(string strAddress, unsigned int uiPort, bool bBlock);
	int m_iPort;
	string m_sAddress;

};

#endif /* DRTCPSOCKET_H_ */
