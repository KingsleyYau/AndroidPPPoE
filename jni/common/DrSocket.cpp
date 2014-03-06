/*
 * DrSocket.cpp
 *
 *  Created on: 2014年2月22日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */
#include "DrSocket.h"

DrSocket::DrSocket() {
	// TODO Auto-generated constructor stub
	m_Socket = -1;
}

DrSocket::~DrSocket() {
	// TODO Auto-generated destructor stub
}

unsigned int DrSocket::StringToIp(const char* string_ip) {
    unsigned int dwIPAddress = inet_addr(string_ip);

    if (dwIPAddress != INADDR_NONE &&
            dwIPAddress != INADDR_ANY)
    {
        return dwIPAddress;
    }

    return dwIPAddress;
}
string DrSocket::IpToString(unsigned int ip_addr) {
    struct in_addr in_ip;
    string stringip = "";
    in_ip.s_addr = ip_addr;
    stringip =  inet_ntoa(in_ip);
    return stringip;
}
unsigned int DrSocket::GetTick()
{
	timeval tNow;
	gettimeofday(&tNow, NULL);
	if (tNow.tv_usec != 0){
		return (tNow.tv_sec * 1000 + (unsigned int)(tNow.tv_usec / 1000));
	}else{
		return (tNow.tv_sec * 1000);
	}
}

bool DrSocket::IsTimeout(unsigned int uiStart, unsigned int uiTimeout)
{
	showLog("Jni.DrSocket.IsTimeout", "uiStart:%d uiTimeout:%d", uiStart, uiTimeout);
	unsigned int uiCurr = GetTick();
	unsigned int uiDiff;

	if (uiTimeout == 0)
		return false;
	if (uiCurr >= uiStart) {
		uiDiff = uiCurr - uiStart;
	}else{
		uiDiff = (0xFFFFFFFF - uiStart) + uiCurr;
	}
	if(uiDiff >= uiTimeout){
		return true;
	}
	return false;
}
