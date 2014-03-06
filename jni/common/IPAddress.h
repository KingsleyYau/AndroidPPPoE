/*
 * IPAddress.h
 *
 *  Created on: 2014年2月14日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef IPADDRESS_H_
#define IPADDRESS_H_
#include <string>

#include <list>
#include <stl/_list.h> // just for ide error

using namespace std;

typedef struct _ipAddressNetworkInfo {
	string name;
	string ip;
	string broad;
	string netmast;
	string mac;
	char macByte[8];
	bool bUp;
	bool bPPP;

	_ipAddressNetworkInfo() {
		bUp = false;
		bPPP = false;
		bzero(macByte, sizeof(macByte));
	}

}IpAddressNetworkInfo;

class IPAddress {
public:
	IPAddress();
	virtual ~IPAddress();

	static list<IpAddressNetworkInfo> GetNetworkInfoList();

	static list<string> GetDeviceList();
	static list<string> GetMacAddressList();
	static list<string> GetIPAddress();
	static list<string> GetBroadAddress();
};

#endif /* IPADDRESS_H_ */
