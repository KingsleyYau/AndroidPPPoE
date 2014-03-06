/*
 * DrSocket.h
 *
 *  Created on: 2014年2月22日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef DRSOCKET_H_
#define DRSOCKET_H_

#include <string>
using namespace std;

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <resolv.h>
#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <resolv.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>

#include "DrLog.h"

typedef int socket_type;

class DrSocket {
public:
	DrSocket();
	virtual ~DrSocket();

	static unsigned int StringToIp(const char* string_ip);
	static string IpToString(unsigned int ip_addr);

protected:
	unsigned int GetTick();
	bool IsTimeout(unsigned int uiStart, unsigned int uiTimeout);

	socket_type m_Socket;
};

#endif /* DRSOCKET_H_ */
