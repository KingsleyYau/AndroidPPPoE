/*
 * DrSniffer.cpp
 *
 *  Created on: 2014年3月3日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

#include "DrSniffer.h"
#include "../common/IPAddress.h"
#include "../common/Arithmetic.h"

/*
 * 监听线程
 */
class DrSnifferRunnable : public DrRunnable {
public:
	DrSnifferRunnable(DrSniffer *pDrSniffer) {
		m_pDrSniffer = pDrSniffer;
	}
	virtual ~DrSnifferRunnable() {
		m_pDrSniffer = NULL;
	}
protected:
	void onRun() {
		while(1) {
			if(m_pDrSniffer && m_pDrSniffer->IsRunning()) {
				m_pDrSniffer->SinfferData();
			}
			else {
				break;
			}
		}

	}
private:
	DrSniffer *m_pDrSniffer;
};

DrSniffer::DrSniffer() {
	// TODO Auto-generated constructor stub
	m_pDrSnifferThread = new DrThread(new DrSnifferRunnable(this));
}

DrSniffer::~DrSniffer() {
	// TODO Auto-generated destructor stub
	if(m_pDrSnifferThread) {
		delete m_pDrSnifferThread;
		m_pDrSnifferThread = NULL;
	}
}

bool DrSniffer::StartSniffer(string deviceName) {
	if(IsRunning()) {
		showLog("jni.DrppoePlugin.StartDrPPPoEPlugin", "DrSniffer正在运行...");
		return true;
	}

	// 抓取数据链路层数据
	if((m_Socket = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0) {
//	 抓取传输层数据
//	if((m_Socket = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) < 0) {

		showLog("Jni.DrSniffer.StartSniffer", "create socket error!");
	    return false;
	}

	// set to nonblocking
	int iFlag = 1;
	if ((iFlag = fcntl(m_Socket, F_GETFL, 0)) == -1) {
		showLog("Jni.DrUDPSocket.Bind", "fcntl F_GETFL socket error");
		return false;
	}
	if (fcntl(m_Socket, F_SETFL, iFlag | O_NONBLOCK) == -1) {
		showLog("Jni.DrUDPSocket.Bind", "fcntl F_SETFL socket error");
		return false;
	}

	struct ifreq ifr_ip;
	if(deviceName.length() == 0) {
		// 默认监听全部
		list<IpAddressNetworkInfo> deviceList = IPAddress::GetNetworkInfoList();
		for(list<IpAddressNetworkInfo>::iterator itr = deviceList.begin(); itr != deviceList.end(); itr++) {
			IpAddressNetworkInfo device = *itr;

			// 设置监听为混杂模式
			strcpy(ifr_ip.ifr_name, device.name.c_str());
			ifr_ip.ifr_flags = IFF_UP | IFF_PROMISC | IFF_BROADCAST | IFF_RUNNING;
			if (!(ioctl(m_Socket, SIOCSIFFLAGS, (char *) &ifr_ip))) {
				showLog("Jni.DrSniffer.StartSniffer", "ioctl %s flag:0x%x success!", device.name.c_str(), ifr_ip.ifr_flags);
			}
			else {
				showLog("Jni.DrSniffer.StartSniffer", "ioctl SIOCGIFFLAGS error!");
				return false;
			}
		}
	}
	else {
		// 设置监听为混杂模式
		strcpy(ifr_ip.ifr_name, deviceName.c_str());
		ifr_ip.ifr_flags = IFF_UP | IFF_PROMISC | IFF_BROADCAST | IFF_RUNNING;
		if (!(ioctl(m_Socket, SIOCSIFFLAGS, (char *) &ifr_ip))) {
			showLog("Jni.DrSniffer.StartSniffer", "ioctl %s flag:0x%x success!", deviceName.c_str(), ifr_ip.ifr_flags);
		}
		else {
			showLog("Jni.DrSniffer.StartSniffer", "ioctl SIOCGIFFLAGS error!");
			return false;
		}
	}

	m_bRunning = true;
	if(m_pDrSnifferThread->start() > 0) {
		showLog("Jni.DrSniffer.StartSniffer", "启动监听线程成功");
	}

	showLog("jni.DrSniffer.StartSniffer", "开始监听网卡数据!");
	return true;
}
void DrSniffer::StopSniffer() {
	if(m_pDrSnifferThread) {
		m_pDrSnifferThread->stop();
	}
	m_bRunning = false;
	showLog("jni.DrSniffer.StartSniffer", "停止监听网卡数据!");
}
bool DrSniffer::IsRunning() {
	return m_bRunning;
}
void DrSniffer::ResetParam() {

}
int DrSniffer::SinfferData() {
	showLog("Jni.DrSniffer.SinfferData", "等待数据...");
	int iRet = 0;

//	struct sockaddr_in remoteAddr;
//	bzero(&remoteAddr, sizeof(remoteAddr));
//	int iRemoteAddrLen = sizeof(struct sockaddr_in);

	struct sockaddr_ll remoteAddr;
	bzero(&remoteAddr, sizeof(remoteAddr));
	int iRemoteAddrLen = sizeof(struct sockaddr_ll);

	unsigned char buff[2048];
	memset(buff, '\0', sizeof(buff));

	iRet = recvfrom(m_Socket, buff, sizeof(buff), 0, (struct sockaddr *)&remoteAddr, &iRemoteAddrLen);

//	if(iRet > 0) {
//		AnalyseData(buff, iRet);
////			string log = Arithmetic::AsciiToHexWithSep(buff, iRet);
////			showLog("Jni.DrSniffer.SinfferData", "Sinffer bytes:(%d) Hex编码:%s", iRet, log.c_str());
//	}

	timeval tout;
	tout.tv_sec = 3;
	tout.tv_usec = 0;

	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(m_Socket, &rset);
	int iRetS = select(m_Socket + 1, &rset, NULL, NULL, &tout);

	if(iRetS > 0) {
		iRet = recvfrom(m_Socket, buff, sizeof(buff), 0, (struct sockaddr *)&remoteAddr, &iRemoteAddrLen);

		if(iRet > 0) {
			AnalyseData(buff, iRet);
//			string log = Arithmetic::AsciiToHexWithSep(buff, iRet);
//			showLog("Jni.DrSniffer.SinfferData", "Sinffer bytes:(%d) Hex编码:%s", iRet, log.c_str());
		}
	}
	else if(iRetS == 0){
//		showLog("Jni.DrSniffer.SinfferData", "Sinffer no data for 3 second!");
	}
	else {
		showLog("Jni.DrSniffer.SinfferData", "Sinffer data error!");
	}

	return iRet;
}
void DrSniffer::AnalyseData(unsigned char* pBuffer, int iLen) {
	showLog("jni.DrSniffer.AnalyseData", "############################################################");
	struct ethhdr *ether;
	string result = "";
	// 打印数据链路层头部
	if(iLen > sizeof(struct ethhdr)) {
		showLog("jni.DrSniffer.AnalyseData", "数据链路层头部---------------------------------------------");
		ether = (struct ethhdr *)pBuffer;

		switch(ether->h_proto) {
		case ETH_P_PPP_DISC:{
			// PPPoE discovery messages
			result = "(PPPoE discovery)";
		}break;
		case ETH_P_PPP_SES:{
			// PPPoE session messages
			result = "(PPPoE session)";
		}break;
		default: {
			break;
		}
		}

		showLog("jni.DrSniffer.AnalyseData", "Protocol:0x%x%s", ether->h_proto, result.c_str());
		result = Arithmetic::AsciiToHexWithSep((const char*)ether->h_source, sizeof(ether->h_source));
		showLog("jni.DrSniffer.AnalyseData", "Source Max:%s ", result.c_str());
		result = Arithmetic::AsciiToHexWithSep((const char*)ether->h_dest, sizeof(ether->h_dest));
		showLog("jni.DrSniffer.AnalyseData", "Dest Max:%s ", result.c_str());

		AnalyseIPData(pBuffer + sizeof(struct ethhdr), iLen - sizeof(struct ethhdr));
	}
	showLog("jni.DrSniffer.AnalyseData", "############################################################");
//	AnalyseIPData(pBuffer, iLen);
}
void DrSniffer::AnalyseIPData(unsigned char* pBuffer, int iLen) {
	struct iphdr *ip;
	string result = "";
	// 打印传输层头部
	if(iLen > sizeof(struct iphdr)) {
		showLog("jni.DrSniffer.AnalyseData", "传输层头部---------------------------------------------");
		ip = (struct iphdr *)pBuffer;

		switch(ip->protocol) {
		case IPPROTO_TCP :{
			// Tcp
			result = "(Tcp)";
			AnalyseTcpData(pBuffer + sizeof(struct iphdr),  iLen - sizeof(struct iphdr));
		}break;
		case IPPROTO_UDP:{
			// Udp
			result = "(Udp)";
			AnalyseUdpData(pBuffer + sizeof(struct iphdr),  iLen - sizeof(struct iphdr));
		}break;
		default:{
//			showLog("jni.DrSniffer.AnalyseIPData", "其他类型的包不处理");
		}break;
		}

		showLog("jni.DrSniffer.AnalyseIPData", "Protocol:%d%s", ip->protocol,result.c_str());
		showLog("jni.DrSniffer.AnalyseIPData", "Source IP:%s ", inet_ntoa(*(struct in_addr *)&ip->saddr));
		showLog("jni.DrSniffer.AnalyseIPData", "Dest IP:%s ", inet_ntoa(*(struct in_addr *)&ip->daddr));

	}
}
void DrSniffer::AnalyseTcpData(unsigned char* pBuffer, int iLen) {
	// 打印Tcp头部
	struct tcphdr *tcp;
	if(iLen > sizeof(struct tcphdr)) {
		showLog("jni.DrSniffer.AnalyseData", "Tcp头部---------------------------------------------");
		tcp = (struct tcphdr *)pBuffer;
		showLog("jni.DrSniffer.AnalyseIPData", "Source Port:%d ", ntohs(tcp->source));
		showLog("jni.DrSniffer.AnalyseIPData", "Dest Port:%d ", ntohs(tcp->dest));
	}
}
void DrSniffer::AnalyseUdpData(unsigned char* pBuffer, int iLen) {
	// 打印Udp头部
	struct udphdr *udp;
	if(iLen > sizeof(struct udphdr)) {
		showLog("jni.DrSniffer.AnalyseData", "打印Udp头部---------------------------------------------");
		udp = (struct udphdr *)pBuffer;
		showLog("jni.DrSniffer.AnalyseUdpData", "Source Port:%d ", ntohs(udp->source));
		showLog("jni.DrSniffer.AnalyseUdpData", "Dest Port:%d ", ntohs(udp->dest));
		showLog("jni.DrSniffer.AnalyseUdpData", "Len :%d ", ntohs(udp->len));
		showLog("jni.DrSniffer.AnalyseUdpData", "Check :%d ", ntohs(udp->check));
	}
}
