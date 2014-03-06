/*
 * DrppoePlugin.cpp
 *
 *  Created on: 2014年2月22日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#include "DrppoePlugin.h"

#include <openssl/md5.h>
#include <openssl/md4.h>
#include <openssl/sha.h>

/*
 * 认证线程
 */
class DrSendChallengeRunnable : public DrRunnable {
public:
	DrSendChallengeRunnable(DrppoePlugin *pDrppoePlugin) {
		m_pDrppoePlugin = pDrppoePlugin;
	}
	virtual ~DrSendChallengeRunnable() {
		m_pDrppoePlugin = NULL;
	}
protected:
	void onRun() {
		if(m_pDrppoePlugin) {
			showLog("jni.DrSendChallengeRunnable.onRun", "开始发送线程");

			int iLen = MAX_AUTH_DATA_LEN;
			char pBuffer[MAX_AUTH_DATA_LEN];

			while(m_pDrppoePlugin->IsRunning()) {
				// 第一步, 发送认证请求
				if(DRCOM_ERR != m_pDrppoePlugin->DrcomDialExtProtoSendChallenge()) {
					// 第二步, 接收认证服务器认证请求的响应
					iLen = m_pDrppoePlugin->RecvAuthData(pBuffer, MAX_AUTH_DATA_LEN);
					if(DRCOM_ERR != m_pDrppoePlugin->DrcomDialExtProtoHandle_ChallengeRep(pBuffer, iLen)) {
			            // 第三步, 发送认证数据命令
						if(m_pDrppoePlugin->DrcomDialExtProtoSendLoginPacket()) {
							// 第4步,接收认证结果处理
							iLen = MAX_AUTH_DATA_LEN;
							iLen = m_pDrppoePlugin->RecvAuthData(pBuffer, MAX_AUTH_DATA_LEN);
							if(DRCOM_ERR != m_pDrppoePlugin->DrcomDialExtProtoHandle_LoginRes(pBuffer, iLen)) {
								// 开始防代理认证
								int iAntiTimes = 0;
								if(m_pDrppoePlugin->GetIsFirstTimeAnti()) {
									// 第一次防代理认证，需要发三个包
									iAntiTimes = 3;
								}
								else {
									// 其他只发2个包
									iAntiTimes = 2;
									m_pDrppoePlugin->SetFirstTimeAnti();
								}

								for(int i = 0; i < iAntiTimes; i++) {
									if(DRCOM_ERR != m_pDrppoePlugin->SendNextDownloadModuleFileCmd()) {
										iLen = m_pDrppoePlugin->RecvAuthData(pBuffer, MAX_AUTH_DATA_LEN);
										m_pDrppoePlugin->HandleClientRecvPacket(pBuffer, iLen);
									}
								}
							}
						}
					}
				}

				// 等待下次发送
				showLog("jni.DrSendChallengeRunnable::onRun", "等待16秒后发送!");
				sleep(16);
			}

		}
	}
private:
	DrppoePlugin *m_pDrppoePlugin;
};

/*
 * Dr.COM认证插件
 */
DrppoePlugin::DrppoePlugin() {
	// TODO Auto-generated constructor stub
	m_pAuthSendDrThread = new DrThread(new DrSendChallengeRunnable(this));
}

DrppoePlugin::~DrppoePlugin() {
	// TODO Auto-generated destructor stub
	StopDrPPPoEPlugin();
}

bool DrppoePlugin::StartDrPPPoEPlugin() {
	if(IsRunning()) {
		showLog("jni.DrppoePlugin.StartDrPPPoEPlugin", "Dr.COM插件协议正在运行");
		return true;
	}

	showLog("jni.DrppoePlugin.StartDrPPPoEPlugin", "开始Dr.COM插件协议");
	if(!ResetParam()) {
		showLog("jni.DrppoePlugin.StartDrPPPoEPlugin", "初始化参数失败");
		return false;
	}

	if(!m_UdpSocket.Bind(m_iLocalPort)) {
		showLog("jni.DrppoePlugin.StartDrPPPoEPlugin", "绑定端口(%d)失败", m_iLocalPort);
		return false;
	}
	else {
		showLog("jni.DrppoePlugin.StartDrPPPoEPlugin", "Dr.COM插件绑定端口(%d)成功", m_iLocalPort);
	}

	// 标记为已经运行
	m_bIsRunning = true;

	// 创建发送线程
	pthread_t pThreadSend = m_pAuthSendDrThread->start();
	if(pThreadSend <= 0) {
		showLog("jni.DrppoePlugin.StartDrPPPoEPlugin", "创建发送线程失败");
		m_bIsRunning = false;
		return false;
	}

	return true;
}
void DrppoePlugin::StopDrPPPoEPlugin() {
	showLog("jni.DrppoePlugin.StartDrPPPoEPlugin", "停止Dr.COM插件协议");
	m_bIsRunning = false;

	if(m_pAuthSendDrThread) {
		m_pAuthSendDrThread->stop();
//		showLog("jni.DrppoePlugin.StartDrPPPoEPlugin", "停止发送线程成功!");
	}

	m_UdpSocket.Close();
}
bool DrppoePlugin::ReStartDrPPPoEPlugin() {
	StopDrPPPoEPlugin();
	return StartDrPPPoEPlugin();
}
bool DrppoePlugin::GetOnlineStatus() {
	return true;
}
bool DrppoePlugin::IsRunning() {
	return m_bIsRunning;
}
DrUdpSocket DrppoePlugin::GetUdpSocket() {
	return m_UdpSocket;
}
//DrCond DrppoePlugin::GetDrCondSend() {
//	return m_DrCondSend;
//}
//DrCond DrppoePlugin::GetDrCondRecv() {
//	return m_DrCondRecv;
//}
string DrppoePlugin::GetServerAddress() {
	return m_sServerAddress;
}
void DrppoePlugin::SetServerAddress(string ip) {
	m_sServerAddress = ip;
}
bool DrppoePlugin::ResetParam() {
	showLog("jni.DrppoePlugin.ResetParam", "初始化参数...");
	m_SetpCurrent = SetpNone;
	m_bIsRunning = false;

	m_iServerPort = 61440;
	m_sServerAddress = "192.168.12.201";//"1.1.1.1";

    m_iLocalPort = 61440;

	m_iChallengeID = 0;
	m_cEncryptionMode = 0x00;
	m_bIsFirstTimeConnect = true;
	m_ulDrcomDialExtProtoAuthLastCRC = 0;
	m_iDrcomDialExtProtoActiveCRC = 0;
	m_iIsDrcomDialConnectionFirstActive = 0x01;
	m_sDRCOM_CLIENT_KERNEL_VER = atoi(DRCOMAUTHSVR_MINOR_VERSION);
	m_uiAntiProxyResault = 0x00;

	m_nClientPingNotResponseNum = 0;
	m_LocalNetAuthMode = DRCOM_AUTH_DIRECT_INTERNET;
	m_InternetAccessControl = PING_CONTROL_ENABLE_INTERNET;

    memset(m_KeepAliveMD5, 0, sizeof(m_KeepAliveMD5));
	memset(m_DrcomDialExtProtoNetInfo, 0, sizeof(m_DrcomDialExtProtoNetInfo));

	// 初始化网卡
	int index = 0;
	list<IpAddressNetworkInfo> broadList = IPAddress::GetNetworkInfoList();
	for(list<IpAddressNetworkInfo>::iterator itr = broadList.begin(); itr != broadList.end(); itr++) {
		IpAddressNetworkInfo device = *itr;
		if(string::npos != device.name.find("ppp", 0)) {
			// found device like 'ppp0'
			m_ClientPPPoEId = device.ip;

			m_DrcomDialExtProtoNetInfo[index].sip = DrSocket::StringToIp(device.ip.c_str());
			m_DrcomDialExtProtoNetInfo[index].smask = 0xFFFFFFFF;
			m_DrcomDialExtProtoNetInfo[index].netmark = 0;
			m_DrcomDialExtProtoNetInfo[index].type = 23;
//			memcpy(m_DrcomDialExtProtoNetInfo[index].mac, device.macByte, 6);
			index++;
		}
	}

	// 防代理标识
	m_RandomIndex = 0;
	m_DownLoadReqPacketIndex = 0;
	m_nStartUpdateCount = 0;
	m_nModuleOnlineCount = 0;
	m_NextUpdateModuleCmdWaitTime = 0;
	m_CHECK_MODULE_UPDATE_INTERVAL = 10;
	memset(m_TMPcode1recExtchalleng, 0, sizeof(m_TMPcode1recExtchalleng));
	m_CHECK_SVR_LINUX_MODULE_VER = 9999;
	m_PACKET_CRC_CONST = 711;
	m_DownloadModuleBuff = {0, 0, NULL, 0};
	m_AntiProxyModuleUpdateStatus = AntiProxyModuleUpdateStatus_NOT_SET;
	m_nServerRequireSendDrcomPingInterval = 0;
	m_ptrServerDllPacketHead = NULL;
	m_bHasNewVersionCheckMark = HAS_NEW_VERSION_NOT_CHECK;

	m_bIsFirstTimeAnti = true;
	return true;
}
string DrppoePlugin::GetPPPoEServerAddress() {
	string broadAddress = "";
	string ipAddress = "";
	list<IpAddressNetworkInfo> broadList = IPAddress::GetNetworkInfoList();
	for(list<IpAddressNetworkInfo>::iterator itr = broadList.begin(); itr != broadList.end(); itr++) {
		IpAddressNetworkInfo device = *itr;
		if(string::npos != device.name.find("ppp", 0)) {
			// found device like 'ppp0'
			ipAddress = device.ip;
			break;
		}
	}

//	if(0 == ipAddress.length()) {
//		showLog("jni.DrppoePlugin::GetPPPoEServerAddress", "没有找到已连接的PPPoE设备!");
//		return "";
//	}

	for(list<IpAddressNetworkInfo>::iterator itr = broadList.begin(); itr != broadList.end(); itr++) {
		IpAddressNetworkInfo device = *itr;

		if(0 == device.ip.compare(ipAddress)) {
			continue;
		}

		int iSrc, iDst, iMsk;
		struct in_addr addr;
		inet_aton(device.netmast.c_str(), &addr);
		iMsk = addr.s_addr;

		inet_aton(device.ip.c_str(), &addr);
		iSrc = addr.s_addr;

		inet_aton(ipAddress.c_str(), &addr);
		iDst = addr.s_addr;

		if((iSrc & iMsk) == (iDst & iMsk)) {
			broadAddress = device.broad;
			showLog("jni.DrppoePlugin::GetPPPoEServerAddress", "获取到PPPoE服务器:%s", broadAddress.c_str());
			break;
		}
	}
//	broadAddress = "192.168.12.119";
	return broadAddress;
}

// fgx 2013-03-29 生成校验位
// pChallenge：从第二个包中收到的 challenge
// nChallengeSize：challenge的大小，目前默认为sizeof(unsigned long)，即4个字节
// type：ENCRYPTION_MODE_TYPE定义，用于指定采用何种算法生成
// pCrcBuff：生成校验位的空间，默认为8个字节
bool DrppoePlugin::GetCrcCheckField(unsigned char* pChallenge, unsigned int nChallengeSize, unsigned int type, unsigned long* pCrcBuff, unsigned int* pnEncryptionType) {
//    bool bFlag = true;

    unsigned int* pnChallenge = NULL;
    unsigned int nEncryptionType = 0;
	if (nChallengeSize < sizeof(unsigned long))
	{
		showLog("jni.DrppoePlugin::GetCrcCheckField", "nChallengeSize < sizeof(unsigned long)");
		return false;
	}

    pnChallenge = (unsigned int*)pChallenge;
    nEncryptionType = *pnChallenge % 4;
    if (NULL != pnEncryptionType) {
        *pnEncryptionType = nEncryptionType;
    }

    if (type > 0) {
        // 新版本默认可以处理
    	//    	bFlag = true;
        if (ENCRYPTION_MODE_NONE == nEncryptionType) {
        	showLog("jni.DrppoePlugin::GetCrcCheckField", "加密方式为无:(%d)", nEncryptionType);
            pCrcBuff[0] = DRCOM_DIAL_EXT_PROTO_CRC_INIT;
            pCrcBuff[1] = 126;
        }
        else if (ENCRYPTION_MODE_MD5 == nEncryptionType) {
        	showLog("jni.DrppoePlugin::GetCrcCheckField", "加密方式为MD5:(%d)", nEncryptionType);
            // 用MD5生成校验码
            unsigned char* pCheckBuff = (unsigned char*)pCrcBuff;
            // MD5 Challenge
        	unsigned char ucEnBuf[16] = {0};
        	MD5((unsigned char*)pChallenge, nChallengeSize, ucEnBuf);
            // 取第2,3,8,9,5,6,13,14位（0为第一位）
            pCheckBuff[0] = ucEnBuf[2];
            pCheckBuff[1] = ucEnBuf[3];
            pCheckBuff[2] = ucEnBuf[8];
            pCheckBuff[3] = ucEnBuf[9];
            pCheckBuff[4] = ucEnBuf[5];
            pCheckBuff[5] = ucEnBuf[6];
            pCheckBuff[6] = ucEnBuf[13];
            pCheckBuff[7] = ucEnBuf[14];
        }
        else if (ENCRYPTION_MODE_MD4 == nEncryptionType) {
        	showLog("jni.DrppoePlugin::GetCrcCheckField", "加密方式为MD4:(%d)", nEncryptionType);
            // 用MD4生成校验码
            unsigned char* pCheckBuff = (unsigned char*)pCrcBuff;
            // MD5 Challenge
            unsigned char ucEnBuf[16] = {0};
            MD4((unsigned char*)pChallenge, nChallengeSize, ucEnBuf);
            // 取第1,2,8,9,4,5,11,12位（0为第一位）
            pCheckBuff[0] = ucEnBuf[1];
            pCheckBuff[1] = ucEnBuf[2];
            pCheckBuff[2] = ucEnBuf[8];
            pCheckBuff[3] = ucEnBuf[9];
            pCheckBuff[4] = ucEnBuf[4];
            pCheckBuff[5] = ucEnBuf[5];
            pCheckBuff[6] = ucEnBuf[11];
            pCheckBuff[7] = ucEnBuf[12];
        }
        else if (ENCRYPTION_MODE_SHA1 == nEncryptionType) {
        	showLog("jni.DrppoePlugin::GetCrcCheckField", "加密方式为SHA1:(%d)", nEncryptionType);
            // 用SHA1生成校验码
            unsigned char* pCheckBuff = (unsigned char*)pCrcBuff;
            // MD5 Challenge
            unsigned char ucEnBuf[20] = {0};
            SHA1(pChallenge, nChallengeSize, ucEnBuf);
            // 取第2,3,9,10,5,6,15,16位（0为第一位）
            pCheckBuff[0] = ucEnBuf[2];
            pCheckBuff[1] = ucEnBuf[3];
            pCheckBuff[2] = ucEnBuf[9];
            pCheckBuff[3] = ucEnBuf[10];
            pCheckBuff[4] = ucEnBuf[5];
            pCheckBuff[5] = ucEnBuf[6];
            pCheckBuff[6] = ucEnBuf[15];
            pCheckBuff[7] = ucEnBuf[16];
        }
		else {
		    // ERROR 未知类型不能处理
			showLog("jni.DrppoePlugin::GetCrcCheckField", "ERROR 未知类型不能处理");
			return false;
		}
        return true;
    }
    else {
        // 旧版本，不用处理
        pCrcBuff[0] = DRCOM_DIAL_EXT_PROTO_CRC_INIT;
        pCrcBuff[1] = 126;
        showLog("jni.DrppoePlugin::GetCrcCheckField", "旧版本，不用处理");
        return false;
    }

    return true;
}

unsigned long DrppoePlugin::DrcomCRC32(unsigned int nInitValue, unsigned char* ptrData, int nByteDataLen) {
    unsigned long* ptrCRCData = NULL;
    int i = 0;
    unsigned long nResCRC = 0;
    nResCRC = nInitValue;
    ptrCRCData = (unsigned long*)ptrData;

	string log = Arithmetic::AsciiToHexWithSep((const char*)ptrCRCData, nByteDataLen);
	showLog("jni.DrppoePlugin::DrcomCRC32", "DrcomCRC32 加密前 nByteDataLen %%d:(%d)", nByteDataLen);
	showLog("jni.DrppoePlugin::DrcomCRC32", "DrcomCRC32 加密前 nResCRC %%lu:(%lu)", nResCRC);
	showLog("jni.DrppoePlugin::DrcomCRC32", "DrcomCRC32 加密前 Hex:(%s)", log.c_str());

    for (i = 0; i < nByteDataLen / 4; i++)
    {
        log = Arithmetic::AsciiToHexWithSep((const char*)&nResCRC, sizeof(nResCRC));
        showLog("jni.DrppoePlugin::DrcomCRC32", "DrcomCRC32 加密前 nResCRC[%d]:(%s)", i, log.c_str());
        nResCRC = nResCRC ^ ptrCRCData[i];
        log = Arithmetic::AsciiToHexWithSep((const char*)&nResCRC, sizeof(nResCRC));
        showLog("jni.DrppoePlugin::DrcomCRC32", "DrcomCRC32 加密后 nResCRC[%d]:(%s)", i, log.c_str());
    }
	log = Arithmetic::AsciiToHexWithSep((const char*)&nResCRC, sizeof(nResCRC));
	showLog("jni.DrppoePlugin::DrcomCRC32", "DrcomCRC32 加密后 nResCRC %%lu:(%lu)", nResCRC);
	showLog("jni.DrppoePlugin::DrcomCRC32", "DrcomCRC32 加密后 nResCRC %%s:(%s) sizeof(unsigned long):(%d)", log.c_str(), sizeof(nResCRC));
    return nResCRC;
}

#define ACTIVE_CRC_RBIT 2
#define ACTIVE_CRC_LBIT (32-ACTIVE_CRC_RBIT)
unsigned int DrppoePlugin::GetDrcomCRC(unsigned int org_crc)
{
    return ((org_crc >> ACTIVE_CRC_RBIT) + (org_crc << ACTIVE_CRC_LBIT));
}


int DrppoePlugin::is_dhcp_169_ip_addr(unsigned int ip_addr)
{
    return ((ip_addr & DrSocket::StringToIp("255.255.0.0")) == DrSocket::StringToIp("169.254.0.0"));
}

/*
 * 发送认证命令
 */
int DrppoePlugin::SendAuthCmd(char* cmd, int iLen) {
    int addrlen = sizeof(struct sockaddr_in);

    showLog("jni.DrppoePlugin::SendAuthCmd", "向%s(%d)发送命令...", m_sServerAddress.c_str(), m_iServerPort);

    int iRet = m_UdpSocket.SendData(m_sServerAddress, m_iServerPort, cmd, iLen);
    if(iRet < 0) {
    	showLog("jni.DrppoePlugin::SendAuthCmd", "网络连接中断，发送认证数据错误!");
    	return DRCOM_ERR;
    }

    return DRCOM_SUC;
}

/*
 * 接收认证返回
 */
int DrppoePlugin::RecvAuthData(char* pBuffer, unsigned int iLen) {
    int iRet = -1;

	// 超时为3秒
	struct sockaddr_in remoteAddr;
	bzero(&remoteAddr, sizeof(remoteAddr));

	bzero(pBuffer, iLen);

	iRet = GetUdpSocket().RecvData(pBuffer, iLen, 3000, &remoteAddr);

	string ip = DrSocket::IpToString(remoteAddr.sin_addr.s_addr);
	if(m_sServerAddress == "1.1.1.1") {
		showLog("jni.DrppoePlugin::RecvAuthData", "发现PPPoE服务器地址:%s!", ip.c_str());
		m_sServerAddress = ip;
	}

    return iRet;
}

/*
 * 第1步,发起认证请求
 */
int DrppoePlugin::DrcomDialExtProtoSendChallenge() {
	int iRet = -1;
	m_SetpCurrent = SetpSendChallenge;
	showLog("jni.DrppoePlugin::DrcomDialExtProtoSendChallenge", "第1步,发起认证请求...");
	struct _tagDrcomDialExtProtoChallenge drcom_dial_ext_challenge;
	bzero(&drcom_dial_ext_challenge, sizeof(struct _tagDrcomDialExtProtoChallenge));
	drcom_dial_ext_challenge.hcode = AUTH_CODE_PPPOE;
	drcom_dial_ext_challenge.hid = m_iChallengeID++;
	drcom_dial_ext_challenge.hlength = sizeof(struct _tagDrcomDialExtProtoChallenge);
	drcom_dial_ext_challenge.htype = AUTH_CODE_TYPE_PPPOE_CHALLENGE;

	return SendAuthCmd((char *)&drcom_dial_ext_challenge, sizeof(struct _tagDrcomDialExtProtoChallenge));
}

/*
 * 接收服务器响应处理
 */
void DrppoePlugin::OnDrcomDialExtProtoHandler(char *pBuffer, int iLen) {
	switch(m_SetpCurrent) {
	case SetpSendChallenge:{
		// 第二步, 已经发送认证请求, 接收服务器返回
		DrcomDialExtProtoHandle_ChallengeRep(pBuffer, iLen);
	}break;
	case SetpSendLogin:{
		// 第四步, 已经发送认证数据命令, 接收服务器返回
		DrcomDialExtProtoHandle_LoginRes(pBuffer, iLen);
	}break;
	default:break;
	}
}

/*
 * 第2步,认证服务器认证请求的响应处理
 */
int DrppoePlugin::DrcomDialExtProtoHandle_ChallengeRep(char *pBuffer, int iLen) {
	showLog("jni.DrppoePlugin::DrcomDialExtProtoSendChallenge", "第2步,接收到服务器认证请求返回...");
	m_SetpCurrent = SetpChallengeRep;
	struct _tagDrcomDialExtProtoReturnChallengeAndIP* retchall = (struct _tagDrcomDialExtProtoReturnChallengeAndIP*)pBuffer;
	if (iLen >= sizeof(struct _tagDrcomDialExtProtoReturnChallengeAndIP)) {
		if (retchall->hcode == AUTH_CODE_PPPOE && retchall->htype == AUTH_CODE_TYPE_PPPOE_RET_CHALL) {

			m_AuthHostIP = retchall->ClientSouIP;

            memcpy(m_sChallengeSeed, retchall->ChallengeSeed, SEED_LEN);

            // fgx 2013-12-26 决定第三步的加密方式
            m_cEncryptionMode = retchall->other[0];
            return DRCOM_SUC;
		}
	}
	showLog("jni.DrppoePlugin::DrcomDialExtProtoHandle_ChallengeRep", "第2步,认证服务器认证请求的响应处理处理失败");
	// 回调错误
	return DRCOM_ERR;
}

/*
 * 第3步,发送认证数据
 */
int DrppoePlugin::DrcomDialExtProtoSendLoginPacket() {
	showLog("jni.DrppoePlugin::DrcomDialExtProtoSendChallenge", "第3步,发送认证数据命令...");
	m_SetpCurrent = SetpSendLogin;
    char drcom_dial_ext_login_buff[512];
    unsigned char aChallenge[SEED_LEN];
    bool bIsCrcCheckHandle = false;

    struct _tagDrcomDialExtProtoLoginPacket* drcom_dial_ext_login
        = (struct _tagDrcomDialExtProtoLoginPacket *)drcom_dial_ext_login_buff;

    unsigned char* ptrExtInfoStart = (unsigned char*)(drcom_dial_ext_login_buff + sizeof(struct _tagDrcomDialExtProtoLoginPacket));
    unsigned char ClientCurrVer = m_sDRCOM_CLIENT_KERNEL_VER;

    bzero(drcom_dial_ext_login_buff, sizeof(drcom_dial_ext_login_buff));

    drcom_dial_ext_login->hcode = AUTH_CODE_PPPOE;
    drcom_dial_ext_login->hid =  m_iChallengeID++;
    drcom_dial_ext_login->htype = AUTH_CODE_TYPE_PPPOE_LOGIN;
    memset(drcom_dial_ext_login->mac, 0, 6);
    drcom_dial_ext_login->sip = m_AuthHostIP;
    drcom_dial_ext_login->option = DRCOM_DIAL_PROTO_OPT_DEFAULT;     //2006-9-1      //0;

    // fgx 2013-03-29 修改第三步的校验方式
    memcpy(drcom_dial_ext_login->ChallengeSeed, m_sChallengeSeed, SEED_LEN);
    bIsCrcCheckHandle = GetCrcCheckField(m_sChallengeSeed, SEED_LEN, m_cEncryptionMode, drcom_dial_ext_login->crc, NULL);

    //us.Accout[0] = 0x00;
    drcom_dial_ext_login->uidlength = 0;//strlen(us.Account);
    drcom_dial_ext_login->hlength = sizeof(struct _tagDrcomDialExtProtoLoginPacket)
                                    + drcom_dial_ext_login->uidlength
                                    + sizeof(struct _tagDrcom8021XExtProtoNetWorkInfo) * MAX_DRCOM_8021X_EXT_PROTO_NET_NUM;

    // add by fgx 2012-07-20
    // 修复上传了 169.254 IP段的问题
    void *temp = NULL;
    {

        struct _tagDrcom8021XExtProtoNetWorkInfo* pNewWorkInfo
            = (struct _tagDrcom8021XExtProtoNetWorkInfo*)ptrExtInfoStart + drcom_dial_ext_login->uidlength;
        showLog("qqqq", "pNewWorkInfo1:%p", pNewWorkInfo);
        for (int index = 0; index < MAX_DRCOM_DIAL_EXT_PROTO_NET_NUM; index++)
        {
            if (!is_dhcp_169_ip_addr(m_DrcomDialExtProtoNetInfo[index].sip))
            {
                memcpy(pNewWorkInfo, m_DrcomDialExtProtoNetInfo + index, sizeof(struct _tagDrcomDialExtProtoNetWorkInfo));
                pNewWorkInfo++;
            }
            if(index == 0) {
            	temp = (void *)pNewWorkInfo;
            	temp = temp + 6;
            	showLog("qqqq", "pNewWorkInfo2:%p temp:%p", pNewWorkInfo, temp);
            }
        }

    }

    if(1) {
    	drcom_dial_ext_login->option = drcom_dial_ext_login->option | DRCOM_DIAL_PROTO_OPT_DRCOMDLL_CHECK;
    	if(1) {
    		// 大部分内核版本需要上传版本号，只有少数不需要
    		drcom_dial_ext_login->option = drcom_dial_ext_login->option
    	                                           + (DRCOM_DIAL_PROTO_OPT_ANTI_CLIENT_VER * ClientCurrVer);
    	}
    }
    else {
        drcom_dial_ext_login->option = drcom_dial_ext_login->option
                                       + (DRCOM_DIAL_PROTO_OPT_ANTI_CLIENT_VER * ClientCurrVer);
    }

    if (m_uiAntiProxyResault != 0x00) //2007-11-22
    {
        // pppoe发现代理网关将其踢下线
        drcom_dial_ext_login->option = drcom_dial_ext_login->option | DRCOM_DIAL_PROTO_OPT_ANTI_PROXY_OFFLINE;
    }

    if (m_uiAntiProxyResault > 5)
    {
    	// 请停止使用路由器或代理软件上网,并重新登录，谢谢合作!!!
        return DRCOM_ERR;
    }

    if (m_bIsFirstTimeConnect) {
    	drcom_dial_ext_login->option = drcom_dial_ext_login->option | DRCOM_DIAL_PROTO_OPT_CONNECTION_START;
    	drcom_dial_ext_login->option = 0x1d006200;
    }
    else {
    	drcom_dial_ext_login->option = drcom_dial_ext_login->option | DRCOM_DIAL_PROTO_OPT_CONNECTION_ONLINEING;
    	drcom_dial_ext_login->option = 0x1d006300;
    }

    if (!bIsCrcCheckHandle){
        // 旧版本
        drcom_dial_ext_login->hlength = ((drcom_dial_ext_login->hlength + 3) / 4) * 4;
        drcom_dial_ext_login->crc[0] = DRCOM_DIAL_EXT_PROTO_CRC_CONST *
                                       DrcomCRC32(0, (unsigned char*)drcom_dial_ext_login, drcom_dial_ext_login->hlength);
        drcom_dial_ext_login->crc[1] = 0;
    }

    m_ulDrcomDialExtProtoAuthLastCRC = drcom_dial_ext_login->crc[0];

    if (m_iIsDrcomDialConnectionFirstActive == 0x01) {
    	m_iDrcomDialExtProtoActiveCRC = drcom_dial_ext_login->crc[0];
    }

	int index = 0;
	unsigned char* pcrcstart = (unsigned char*)(ptrExtInfoStart + drcom_dial_ext_login->uidlength); //&drcom_dial_ext_login->netinfo;

	int pcrcLen = (sizeof(struct _tagDrcomDialExtProtoNetWorkInfo) * MAX_DRCOM_DIAL_EXT_PROTO_NET_NUM);
	string log = Arithmetic::AsciiToHexWithSep((const char*)pcrcstart, pcrcLen);
	showLog("jni.DrppoePlugin::DrcomDialExtProtoSendChallenge", "pcrcstart:(%d)加密前 Hex编码:\n%s", pcrcLen, log.c_str());


	for (index = 0; index < pcrcLen; index++)
	{
		pcrcstart[index] =
			(unsigned char)((pcrcstart[index] << (index & 0x07))
							+ (pcrcstart[index] >> (8 - (index & 0x07))));
	}

	log = Arithmetic::AsciiToHexWithSep((const char*)pcrcstart, pcrcLen);
	showLog("jni.DrppoePlugin::DrcomDialExtProtoSendChallenge", "pcrcstart:(%d)加密后 Hex编码:\n%s", pcrcLen, log.c_str());

	if(NULL !=  temp) {
	    char a = 0x40;
	    memcpy((void *)temp, &a, sizeof(a));
	}

	// 正在向服务器确认IP地址,请稍后.....
    return SendAuthCmd((char *)drcom_dial_ext_login, drcom_dial_ext_login->hlength);
}
/*
 * 第4步,接收认证结果处理
 */
int DrppoePlugin::DrcomDialExtProtoHandle_LoginRes(char *pBuffer, int iLen) {
	//	DrcomDialExtProtoHandle_ping(pBuffer, iLen);
	// 因为每次收到包为3秒，所以发送第三步后3秒都没有数据包返回就认为是认证失败
	showLog("jni.DrppoePlugin::DrcomDialExtProtoHandle_LoginRes", "第4步,接收到服务器认证数据返回...");
	m_SetpCurrent = SendLoginRes;
	struct _tagDrcomDialExtProtoLoginResult* loginRes = (struct _tagDrcomDialExtProtoLoginResult*)pBuffer;
    if (iLen >= sizeof(struct _tagDrcomDialExtProtoLoginResult)
            && loginRes->infolength <= sizeof(struct _tagDrcomDogControlHeader) + 64 * sizeof(struct _tagDogControlTableItem)
            && loginRes->hcode == AUTH_CODE_PPPOE
       ) {
        if ((loginRes->htype & 0x40) != 0)
        {
            char* pLoginMsg = (char*)&pBuffer[sizeof(struct _tagDrcomDialExtProtoLoginResult)];
            char* pLoginURLMsg = pLoginMsg + strlen((char*)pLoginMsg) + 1;

            if (strlen(pLoginMsg))
            {
            	// 系统消息
            }
            else
            {
            	// 客户端需要升级，请下载最新的安装包重新安装客户端!
            }

            //可能需要保持连接状态，在内网下载最新版本客户端，暂时还是断开连接
            //m_DrAuthStatus.DrCOMLogout(EXECUTOR_AUTHSVR_DRCOMDLL_ERR_CODE);

            if (strlen(pLoginURLMsg))
            {
            	// url 系统链接
            }
            return DRCOM_SUC;
        }

        if ((loginRes->htype & 0x80) != 0)
        {
            // start release ip thread
            DrcomDialCheckStaticIPAddress();
            return DRCOM_SUC;
        }

        if ((loginRes->htype & 0x0F) == AUTH_CODE_TYPE_PPPOE_VPN_TBL)
        {
            OnDrcomDialExtProtoLoginResualt();
            return DRCOM_SUC;
        }
    }

    showLog("jni.DrppoePlugin::DrcomDialExtProtoHandle_ChallengeRep", "第4步,接收认证结果处理失败");
	return DRCOM_ERR;
}

/*
 * 发送心跳?
 */
int DrppoePlugin::DrcomAuthSendClientPing() {
    if (m_cEncryptionMode == 0) {
        // 2步心跳（旧版本）
        struct _tagHostPingServer DrcomPing;
        time_t seconds = time(NULL);
        bzero(&DrcomPing, sizeof(struct _tagHostPingServer));
        DrcomPing.Code = 0xFF;
        memcpy(&DrcomPing.DogData, &m_AuthDogData, sizeof(struct _tagDrcomDogData));
        DrcomPing.Milliseconds = (unsigned short)seconds;
        memcpy(DrcomPing.checkcode.oldcheck.KeepAliveMD5, m_KeepAliveMD5, MD5_LEN);
        DrcomPing.InternetAccessControl = m_InternetAccessControl;

        m_nClientPingNotResponseNum++;

        if (DRCOM_AUTH_LOCALNET_INERNET == m_LocalNetAuthMode)
        {
            return SendAuthCmd((char *)&DrcomPing, sizeof(struct _tagHostPingServer));
        }
        else
        {
            return SendAuthCmd((char *)&DrcomPing, sizeof(struct _tagHostPingServer) - sizeof(DrcomPing.InternetAccessControl));
        }

        return DRCOM_SUC;
    }
    else {
        // 4步心跳（新版本）
        return DrcomAuthSendChallenge();
    }
}
// 增加发送挑战值到2166，心跳1
int DrppoePlugin::DrcomAuthSendChallenge() {

	struct _tagDrcom2133Challenge drcom_2133_challenge;
	bzero(&drcom_2133_challenge, sizeof(struct _tagDrcom2133Challenge));
	drcom_2133_challenge.code = AUTH_CODE_SVR_RET;
	drcom_2133_challenge.id = m_iChallengeID++;
	drcom_2133_challenge.length = sizeof(struct _tagDrcom2133Challenge);
	drcom_2133_challenge.type = DRCOM_2133_CHALLENGE_REQ;

	return SendAuthCmd((char *)&drcom_2133_challenge, sizeof(struct _tagDrcom2133Challenge));
}
/*
 * 接收心跳?
 */
int DrppoePlugin::DrcomDialExtProtoHandle_ping(char* AuthData, int len) {
	showLog("jni.DrppoePlugin::DrcomDialExtProtoSendChallenge", "发送心跳检测...");
    struct _tagDrcomDialExtProtoLoginResult* loginRes
        = (struct _tagDrcomDialExtProtoLoginResult*)AuthData;

    if (len >= sizeof(struct _tagDrcomDialExtProtoLoginResult))
    {
        if (loginRes->hcode == AUTH_CODE_PPPOE
                && ((loginRes->htype & 0x0F) == AUTH_CODE_TYPE_PPPOE_PING
                    || (loginRes->htype & 0x0F) == AUTH_CODE_TYPE_PPPOE_VPN_TBL
                   )
           )
        {
            if (GetDrcomCRC(m_iDrcomDialExtProtoActiveCRC) != loginRes->crc[0])
            {
                if (GetDrcomCRC(m_ulDrcomDialExtProtoAuthLastCRC) == loginRes->crc[0])
                {
                	m_iDrcomDialExtProtoActiveCRC = m_ulDrcomDialExtProtoAuthLastCRC;
                }
                else
                {
                    static int nDrcomDialExtProtoActiveCRC_NEQ = 0;
                    nDrcomDialExtProtoActiveCRC_NEQ++;
                    //if(nDrcomDialExtProtoActiveCRC_NEQ>0)
                    {
                    	m_iDrcomDialExtProtoActiveCRC = loginRes->crc[0];
                        nDrcomDialExtProtoActiveCRC_NEQ = 0;
                    }
                }
            }

            if (loginRes->crc[1] & 0x00000001)
            {
//            	m_DrAuthStatus.m_AuthStatus.NewVerInfo.NewVerOption  &= ~NEW_VER_OPTION_2166_ANTIP_PROXY;
//                DebugMessage(_DRLANG("服务器配置拨号插件停止防代理功能"));
            }
            else
            {
//                LogMessage(_DRLANG("配置拨号插件默认防代理功能"));
            }
        }
    }

    return DRCOM_SUC;
}

int DrppoePlugin::OnDrcomDialExtProtoLoginResualt() {
	if(m_bIsFirstTimeConnect) {
		m_bIsFirstTimeConnect = false;
		// PPPoE拨号Dr.COM扩展数据请求成功.
	}
	else {
		// PPPoE拨号插件Dr.COM扩展数据在线.
	}
    return 1;
}

int DrppoePlugin::DrcomDialCheckStaticIPAddress() {
	// 提示不能为否静态ip
    return 1;
}
// ################################################################################################
/*
 * 防代理函数
 */
// ################################################################################################
/*
 * 发送防代理心跳
 */
int DrppoePlugin::SendNextDownloadModuleFileCmd() {
	return SendNextDownloadModuleFileCmd(m_AntiProxyModuleUpdateStatus);
}
int DrppoePlugin::SendNextDownloadModuleFileCmd(int nNowAntiProxyModuleUpdateStatus)
{
    unsigned char cmdBuffer[512];
    DR2133REQINFO* ptrDownloadModuleCmd = NULL;
    //DebugMessage("SendNextDownloadModuleFileCmd call\n");
    srand((unsigned int)time(NULL));
    m_RandomIndex = rand();
    memset(cmdBuffer, 0x00, sizeof(DR2133REQINFO));
    ptrDownloadModuleCmd = (DR2133REQINFO*)cmdBuffer;
    ptrDownloadModuleCmd->code = 7;
    ptrDownloadModuleCmd->id = m_DownLoadReqPacketIndex;
    //应为检测是否是检测模块的包要判定包的ID==m_DownLoadReqPacketIndex，只能在判定成功之后递增g_DownLoadReqPacketIndex
    ptrDownloadModuleCmd->randomID = m_RandomIndex;
    ptrDownloadModuleCmd->length = sizeof(DR2133REQINFO);
    ptrDownloadModuleCmd->type = 11;
    memcpy(ptrDownloadModuleCmd->Extchalleng, m_TMPcode1recExtchalleng, sizeof(m_TMPcode1recExtchalleng));
    ptrDownloadModuleCmd->ActiveVerNo = m_CHECK_SVR_LINUX_MODULE_VER;

    if (m_DownloadModuleBuff.DownloadingModuleVer != 0)
    {
        ptrDownloadModuleCmd->ActiveVerNo = m_DownloadModuleBuff.DownloadingModuleVer;
    }

    if (nNowAntiProxyModuleUpdateStatus == AntiProxyModuleUpdateStatus_NOT_SET)
    {
        m_nStartUpdateCount = 0;
        ptrDownloadModuleCmd->code1 = CMD_CHECK_UPDATE;
    }

    if (nNowAntiProxyModuleUpdateStatus == AntiProxyModuleUpdateStatus_BEGIN_UPDATE)
    {
        ptrDownloadModuleCmd->mysouip = m_AuthHostIP;
        ptrDownloadModuleCmd->code1 = CMD_START_UPDATE;
    }

    if (nNowAntiProxyModuleUpdateStatus == AntiProxyModuleUpdateStatus_UPDATING_MODULE)
    {
        ptrDownloadModuleCmd->code1 = CMD_UPDATEING;
    }

    ptrDownloadModuleCmd->dlloffset = m_DownloadModuleBuff.DownloadedLen;

    if (nNowAntiProxyModuleUpdateStatus == AntiProxyModuleUpdateStatus_UPDATING_MODULE)
    {
        ptrDownloadModuleCmd->ActiveVerNo = m_DownloadModuleBuff.DownloadingModuleVer;
    }

    if (m_DownloadModuleBuff.DownloadingModuleVer != 0)
    {
        ptrDownloadModuleCmd->ActiveVerNo = m_DownloadModuleBuff.DownloadingModuleVer;
    }

    if (ptrDownloadModuleCmd->code1 == CMD_START_UPDATE)// 2008-11-11
    {
        ptrDownloadModuleCmd->crcvalue1
            = MadeCmdPacketCRCSum(cmdBuffer, ptrDownloadModuleCmd->length);
        m_nStartUpdateCount++;
    }

    if (ptrDownloadModuleCmd->code1 == CMD_CHECK_UPDATE);

    {
        if (m_nModuleOnlineCount == m_CHECK_MODULE_UPDATE_INTERVAL)
        {
            ptrDownloadModuleCmd->ActiveVerNo = m_DownloadModuleBuff.DownloadingModuleVer - 1;
            m_nModuleOnlineCount = 0;
        }
    }
    m_NextUpdateModuleCmdWaitTime = ANTI_CMD_SEND_TIMEOUT;
    return SendAuthCmd((char*)cmdBuffer, ptrDownloadModuleCmd->length);
}

unsigned long DrppoePlugin::MadeCmdPacketCRCSum(unsigned char* ptrData, int nDataLen)
{
    int i, i1 = (nDataLen + 1) / 2;
    unsigned long CRCResult = 0;
    unsigned short* ptrWord = (unsigned short*)ptrData;
    * (unsigned short*)&ptrData[nDataLen] = 0;

    for (i = 0; i < i1; i++)
    {
        CRCResult = CRCResult ^ *(ptrWord + i);
    }

    CRCResult = CRCResult & 0XFFFF;
    CRCResult = CRCResult * m_PACKET_CRC_CONST;
    return CRCResult;
}

void DrppoePlugin::HandleClientRecvPacket(char* RecvData, int nRecvLen)
{
    DR2133REQINFO* ptrRecvReqInfo = (DR2133REQINFO*)RecvData;
    ANTIPROXYMODULECHECKSUMHEAD* ptrModuleCheckSumHeader =
        (ANTIPROXYMODULECHECKSUMHEAD*)&RecvData[sizeof(DR2133REQHEAD)];
    static int ver_info_show = 0;
    //DebugMessage("AntiProxyModuleUpdateStatus = %d\n", AntiProxyModuleUpdateStatus);

    if (!IsDrcomAntiProxyModulePacket(nRecvLen, ptrRecvReqInfo))
    {
        return;
    }

    if (nRecvLen < sizeof(DR2133REQINFO))
    {
        return;
    }

    m_NextUpdateModuleCmdWaitTime = m_nServerRequireSendDrcomPingInterval;

    if (m_AntiProxyModuleUpdateStatus == AntiProxyModuleUpdateStatus_NOT_SET)
    {
        if (ptrRecvReqInfo->code1 == CMD_CHECK_UPDATE_RES)
        {
            HandleClientRecvPacket_code1eq2(nRecvLen, ptrRecvReqInfo);
            return;
        }

        if (IsModuleUpdateFirstPacket(ptrRecvReqInfo, nRecvLen, ptrModuleCheckSumHeader))
        {
            if (m_DownloadModuleBuff.Buffer != NULL)
            {
                free(m_DownloadModuleBuff.Buffer);
            }

            m_DownloadModuleBuff.dwBuffLen = sizeof(ANTIPROXYMODULECHECKSUMHEAD)
                                           + (((m_ptrServerDllPacketHead->mydll_length + 3) / 4) * 4);
            m_DownloadModuleBuff.Buffer = (char*)malloc(m_DownloadModuleBuff.dwBuffLen);
            m_DownloadModuleBuff.DownloadedLen = 0;

            if (m_DownloadModuleBuff.Buffer)
            {
                // send code5, update client
            	m_AntiProxyModuleUpdateStatus = AntiProxyModuleUpdateStatus_UPDATING_MODULE;
                m_DownloadModuleBuff.DownloadedLen += nRecvLen;
                m_DownloadModuleBuff.DownloadingModuleVer = ptrRecvReqInfo->ActiveVerNo;

                if (m_DownloadModuleBuff.DownloadingModuleVer > (6 * 100 + 6 * 10 + 8))
                {
                    if (0 == ver_info_show)
                    {
//                        LogMessage("Dr.COM Auth Kernel Driver(U%d)\n", DownloadModuleBuff.DownloadingModuleVer - 6 * 100 - 60 - 8);
                        ver_info_show = 1;
                    }
                }
                else
                {
                    if (0 == ver_info_show)
                    {
//                        LogMessage("Dr.COM Auth Kernel Driver(U%d)\n", DownloadModuleBuff.DownloadingModuleVer);
                        ver_info_show = 1;
                    }
                }

                //HandleModuleFileDownloadedData(RecvData, nRecvLen);
            }

            m_NextUpdateModuleCmdWaitTime = NEXT_CMD_SEND_AFTER_1_SCE;
            m_AntiProxyModuleUpdateStatus = AntiProxyModuleUpdateStatus_NOT_SET;
        }

        return;
    }

    if (m_AntiProxyModuleUpdateStatus == AntiProxyModuleUpdateStatus_BEGIN_UPDATE)
    {
        if (ptrRecvReqInfo->code1 == CMD_START_UPDATE_RES)
        {
            m_nModuleOnlineCount++;
            m_nStartUpdateCount = 0;
#ifndef TEST_ERR
            m_AntiProxyModuleUpdateStatus = AntiProxyModuleUpdateStatus_NOT_SET;
#endif
            return;
        }

        return;
    }

    if (m_AntiProxyModuleUpdateStatus == AntiProxyModuleUpdateStatus_UPDATING_MODULE)
    {
        if (ptrRecvReqInfo->code1 == CMD_PROXY_MODULE_HEADER)
        {
            if ((ptrRecvReqInfo->dlloffset == m_DownloadModuleBuff.DownloadedLen)
                    && (ptrRecvReqInfo->ActiveVerNo == m_DownloadModuleBuff.DownloadingModuleVer))
            {
                //HandleModuleFileDownloadedData(RecvData, nRecvLen);
                return;
            }
        }

        return;
    }

    return;
}

void DrppoePlugin::HandleClientRecvPacket_code1eq2(int nUdpRecvLen, DR2133REQINFO* ptrRecvReqInfo)
{
    //DebugMessage("HandleClientRecvPacket_code1eq2 called\n");
    m_bHasNewVersionCheckMark = HAS_NEW_VERSION_CHECKED;

    // send code3
    //g_DownLoadReqPacketIndex++;
    if (ptrRecvReqInfo->ActiveVerNo == 0)// 2133 server not mydll packet
    {
//        ErrorMessage(_DRLANG("接入服务器可能没有加载客户端认证核心模块！"));
        m_NextUpdateModuleCmdWaitTime = m_nServerRequireSendDrcomPingInterval;//ModuleUpdateCheckInterval;
        m_AntiProxyModuleUpdateStatus = AntiProxyModuleUpdateStatus_NOT_SET;
        return;
    }

    memcpy(m_TMPcode1recExtchalleng, ptrRecvReqInfo->Extchalleng, sizeof(m_TMPcode1recExtchalleng));
    m_AntiProxyModuleUpdateStatus = AntiProxyModuleUpdateStatus_BEGIN_UPDATE;
    SendNextDownloadModuleFileCmd(AntiProxyModuleUpdateStatus_BEGIN_UPDATE);
    return;
}

int DrppoePlugin::IsModuleUpdateFirstPacket(DR2133REQINFO* ptrRecvReqInfo,
                              int nUdpRecvLen, ANTIPROXYMODULECHECKSUMHEAD* ptrModuleCheckSumHeader)
{
    //DebugMessage("IsModuleUpdateFirstPacket called\n");
    return (ptrRecvReqInfo->code1 == CMD_PROXY_MODULE_HEADER)
           && (nUdpRecvLen >= (sizeof(DR2133REQHEAD) + sizeof(ANTIPROXYMODULECHECKSUMHEAD)))
           && (ptrModuleCheckSumHeader->mydll_length > MIN_MODULE_FILE_SIZE)
           && (ptrModuleCheckSumHeader->mydll_length <= MAX_MODULE_FILE_SIZE);
}

int DrppoePlugin::IsDrcomAntiProxyModulePacket(int nUdpRecvLen, DR2133REQINFO* ptrRecvReqInfo)
{
    if ((nUdpRecvLen < sizeof(DR2133REQHEAD))
            || (nUdpRecvLen != ptrRecvReqInfo->length))
    {
        //DebugMessage("IsDrcomAntiProxyModulePacket return false\n");
        return 0;
    }

    if ((m_DownLoadReqPacketIndex != ptrRecvReqInfo->id)
            || (m_RandomIndex != ptrRecvReqInfo->randomID))
    {
        //可能需要重置状态，待测试观察日志再确定
        m_nStartUpdateCount = 0;
        m_AntiProxyModuleUpdateStatus = AntiProxyModuleUpdateStatus_NOT_SET;
        m_DownLoadReqPacketIndex = 0;
        return 0;
    }

    m_DownLoadReqPacketIndex++;
    return 1;
}
void DrppoePlugin::SetFirstTimeAnti() {
	m_bIsFirstTimeAnti = true;
}
bool DrppoePlugin::GetIsFirstTimeAnti() {
	return m_bIsFirstTimeAnti;
}
// ################################################################################################
