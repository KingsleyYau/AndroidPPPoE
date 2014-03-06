/*
 * DrTcpSocket.cpp
 *
 *  Created on: 2014年2月24日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#include "DrTcpSocket.h"

DrTcpSocket::DrTcpSocket() {
	// TODO Auto-generated constructor stub
	m_sAddress = "";
	m_iPort = 0;
}

DrTcpSocket::~DrTcpSocket() {
	// TODO Auto-generated destructor stub
}

int DrTcpSocket::Connect(string strAddress, unsigned int uiPort) {
	return BaseConnect(strAddress, uiPort, false);
}
int DrTcpSocket::getSocket(){
	return m_Socket;
}
int DrTcpSocket::SendData(char* pBuffer, unsigned int uiSendLen, unsigned int uiTimeout) {
	unsigned int uiBegin = GetTick();
	int iOrgLen = uiSendLen;
	int iRet = -1, iRetS = -1;
	int iSendedLen = 0;
	fd_set wset;

	while (true) {
		struct timeval tout;
		tout.tv_sec = uiTimeout / 1000;
		tout.tv_usec = (uiTimeout % 1000) * 1000;
		FD_ZERO(&wset);
		FD_SET(m_Socket, &wset);
		iRetS = select(m_Socket + 1, NULL, &wset, NULL, &tout);
		if (iRetS == -1) {
			return -1;
		} else if (iRetS == 0) {
			return iSendedLen;
		}
		iRet = send(m_Socket, pBuffer, uiSendLen, 0);
		if (iRet == -1 || (iRetS == 1 && iRet == 0)) {
			usleep(100);
			if (EWOULDBLOCK == errno) {
				if (IsTimeout(uiBegin, uiTimeout)) {
					return iSendedLen;
				}
				continue;
			} else {
				if (iSendedLen == 0){
					return -1;
				}
				return iSendedLen;
			}
		} else if (iRet == 0) {
			return iSendedLen;
		}
		pBuffer += iRet;
		iSendedLen += iRet;
		uiSendLen -= iRet;
		if (iSendedLen >= iOrgLen) {
			return iSendedLen;
		}
		if (IsTimeout(uiBegin, uiTimeout)) {
			return iSendedLen;
		}
	}
	return 0;
}

int DrTcpSocket::RecvData(char* pBuffer, unsigned int uiRecvLen, bool bRecvAll, unsigned int uiTimeout) {
	unsigned int uiBegin = GetTick();
	int iOrgLen = uiRecvLen;
	int iRet = -1, iRetS = -1;
	int iRecvedLen = 0;
	fd_set rset;

	while (true) {
		timeval tout;
		tout.tv_sec = uiTimeout / 1000;
		tout.tv_usec = (uiTimeout % 1000) * 1000;
		FD_ZERO(&rset);
		FD_SET(m_Socket, &rset);
		iRetS = select(m_Socket + 1, &rset, NULL, NULL, &tout);

		if (iRetS == -1) {
			showLog("Jni.tcpSocket.RecvData", "iRetS == -1 return -1");
			return -1;
		} else if (iRetS == 0) {
			showLog("Jni.tcpSocket.RecvData", "iRetS == 0 return iRecvedLen:%d", iRecvedLen);
			return iRecvedLen;
		}
		//showLog("Jni.tcpSocket.RecvData", "pBuffer(%d):%s", pBuffer, uiRecvLen);
		iRet = recv(m_Socket, pBuffer, uiRecvLen, 0);
		showLog("Jni.tcpSocket.RecvData", "m_iSocket:%d, iRetS:%x, iRet:%d", m_Socket, iRetS, iRet);
		if (iRet==-1 || (iRetS == 1 && iRet == 0)) {
			usleep(1000);
			if (EWOULDBLOCK == errno){
				if (IsTimeout(uiBegin, uiTimeout)){
					return iRecvedLen;
				}
				continue;
			}else{
				if (iRecvedLen == 0){
					showLog("Jni.tcpSocket.RecvData", "EWOULDBLOCK != errno.");
					return -1;
				}
				return iRecvedLen;
			}
		} else if (iRet == 0) {
			return iRecvedLen;
		}
		pBuffer += iRet;
		iRecvedLen += iRet;
		uiRecvLen -= iRet;
		if (iRecvedLen >= iOrgLen) {
			return iRecvedLen;
		}
		if (!bRecvAll) {
			return iRecvedLen;
		}
		if (IsTimeout(uiBegin, uiTimeout)){
			return iRecvedLen;
		}
	}
	return 0;
}

int DrTcpSocket::Close() {
	if (m_Socket != -1) {
		shutdown(m_Socket, SHUT_RDWR);
		close(m_Socket);
		m_Socket = -1;
	}
	return 1;
}

int DrTcpSocket::BaseConnect(string strAddress, unsigned int uiPort, bool bBlock) {
	m_sAddress = strAddress;
	m_iPort = uiPort;

	int iRet = -1, iFlag = 1;
	struct sockaddr_in dest;
	hostent* hent = NULL;
	if ((m_Socket = socket(AF_INET, SOCK_STREAM, 0)) >= 0) {
		bzero(&dest, sizeof(dest));
		dest.sin_family = AF_INET;
		dest.sin_port = htons(uiPort);

		dest.sin_addr.s_addr = inet_addr((const char*)strAddress.c_str());
		if (dest.sin_addr.s_addr == -1L) {
			if ((hent = gethostbyname((const char*)strAddress.c_str())) != NULL) {
				dest.sin_family = hent->h_addrtype;
				memcpy((char*)&dest.sin_addr, hent->h_addr, hent->h_length);
			} else {
				showLog("DrServiceJni:tcpSocket", "BaseConnect_GETHOSTBYNAME_ERROR");
				showLog("DrServiceJni:tcpSocket", "BaseConnect_GETHOSTBYNAME_ERROR host=%s:%s\n",
						(const char*)strAddress.c_str(), hstrerror(h_errno));
				goto EXIT_ERROR_TCP;
			}
		}

		setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&iFlag, sizeof(iFlag));
		iFlag = 1500;
		setsockopt(m_Socket, SOL_SOCKET, SO_RCVBUF, (const char *)&iFlag, sizeof(iFlag));
		iFlag = 1;
		setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&iFlag, sizeof(iFlag));

		if (connect(m_Socket, (struct sockaddr *)&dest, sizeof(dest)) != -1) {
			// 设置非阻塞
			if (!bBlock) {
				if ((iFlag = fcntl(m_Socket, F_GETFL, 0)) != -1) {
					if (fcntl(m_Socket, F_SETFL, iFlag | O_NONBLOCK) != -1) {
						iRet = 1;
					}
					else {
						showLog("DrServiceJni:tcpSocket", "BaseConnect_SET_NONBLOCKING_ERROR");
					}
				}
				else {
					showLog("DrServiceJni:tcpSocket", "BaseConnect_GET_NONBLOCKING_ERROR");
				}
			} else {
				iRet = 1;
			}
			// 设置心跳检测
		    /*deal with the tcp keepalive
		      iKeepAlive = 1 ( check keepalive)
		      iKeepIdle = 600 (active keepalive after socket has idled for 10 minutes)
		      KeepInt = 60 (send keepalive every 1 minute after keepalive was actived)
		      iKeepCount = 3 (send keepalive 3 times before disconnect from peer)
		     */
		    int iKeepAlive = 1, iKeepIdle = 30, KeepInt = 30, iKeepCount = 2;
		    if (setsockopt(m_Socket, SOL_SOCKET, SO_KEEPALIVE, (void*)&iKeepAlive, sizeof(iKeepAlive)) < 0) {
		    	iRet = -1;
		    	showLog("DrServiceJni:tcpSocket", "BaseConnect_SO_KEEPALIVE_ERROR");
		    	goto EXIT_ERROR_TCP;
		    }
		    if (setsockopt(m_Socket, SOL_TCP, TCP_KEEPIDLE, (void*)&iKeepIdle, sizeof(iKeepIdle)) < 0) {
		    	iRet = -1;
		    	showLog("DrServiceJni:tcpSocket", "BaseConnect_TCP_KEEPIDLE_ERROR");
		    	goto EXIT_ERROR_TCP;
		    }
		    if (setsockopt(m_Socket, SOL_TCP, TCP_KEEPINTVL, (void *)&KeepInt, sizeof(KeepInt)) < 0) {
		    	iRet = -1;
		    	showLog("DrServiceJni:tcpSocket", "BaseConnect_TCP_KEEPINTVL_ERROR");
		    	goto EXIT_ERROR_TCP;
		    }
		    if (setsockopt(m_Socket, SOL_TCP, TCP_KEEPCNT, (void *)&iKeepCount, sizeof(iKeepCount)) < 0) {
		    	iRet = -1;
		    	showLog("DrServiceJni:tcpSocket", "BaseConnect_TCP_KEEPCNT_ERROR");
		    	goto EXIT_ERROR_TCP;
		    }
		}
		//else
			//showLog("DrServiceJni:tcpSocket", "BaseConnect_connect:error");

	}

EXIT_ERROR_TCP:
	if (iRet != 1) {
		Close();
	}
	return iRet;
}
