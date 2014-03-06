/*
 * DrAuthStatus.h
 *
 *  Created on: 2014年2月22日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef DRAUTHSTATUS_H_
#define DRAUTHSTATUS_H_

#include <sys/sysinfo.h>

#include "../common/DrUDPSocket.h"
#include "../common/IPAddress.h"

#include "DrStepDef.h"
#include "DrCode.h"

#include "DRCOMAUTHSVR_VERSION.H"

#define AUTO_FIND_SERVER_IP "1.1.1.1"
#define AUTO_FIND_SERVER_IP_202 "202.1.1.1"

#define DRCOM_DIAL_EXT_PROTO_CRC_CONST 19680126
#define DRCOM_DIAL_EXT_PROTO_CRC_INIT  20000711

#define MAX_MSG_COUNTER_NUM 5
#define DISPLAY_COUNTER 5
#define COUNTER_INDEX_NOT_ENOUGH     0

class DrAuthStatus {
public:
	DrAuthStatus();
	virtual ~DrAuthStatus();

public:
	bool InitDrAuth();
	bool StartDrAuth();
	void InitPPPoE() ;


	struct _tagAuthStatus m_AuthStatus;
	struct _tagDrSvrConfig m_drcfg;
	struct _tagUserStatus m_us;

public:
	// 发送认证命令
	int SendAuthCmd(char* cmd, int len);

	// Dr.COM and PPPoE认证发送过程
	// 服务器数据返回处理
	int OnDrcomAuthLoginingHandle(struct sockaddr_in* pAuthServerAddr, unsigned char* AuthData, int len);
	// 认证挑战值
	int DrcomAuthSendLoginChallenge();
	// 认证挑战值响应
	int OnDrcomLoginChallenge(struct sockaddr_in* pAuthServerAddr, unsigned char* ChallengeData, int len);
	// 认证账户密码
	int DrcomAuthSendNameAndPassword();
	int OnLoginSuccess(unsigned char* LoginData, int len);
	int OnDrcomLoginSuccess(unsigned char* LoginData, int len);
	int OnLoginError(unsigned char* LoginData, int len);
	int OnDrcomLoginError(unsigned char* LoginData, int len);
	// 客户端发起心跳
	int NewDrcomAuthSendClientPing();
	// 服务器响应心跳
	int DrcomAuthOnliningMsgCodeHandle(struct sockaddr_in* pAuthServerAddr, unsigned char* AuthData, int len);
	// 客户端回应服务器心跳
	int DrcomAuthRepalyServerOnlineMsg();




	int DrcomAuthSendLogoutChallenge();
	int DrcomAuthSendClientPing();
	int DrcomAuthSendChallenge();

	// 不明觉厉,照抄就是
	int CheckIPConfig(unsigned int nHostIP);
	int CheckEnvBeforeSendNameAuth();
	void ClearLastRealTimeOnlineStatus();
	int CheckEnvBeforeSendLoginChallenge();
	int check_ip(unsigned long dwIPAddress);
	int CheckAuthConfig(const char* szConfig);
	void ChangeAuthServerIP();
//	unsigned long get_sysuptime();
	char* GetCheckEnvBeforeAuthErrInfo();
	int GetHostInfo(struct  _tagHostInfo* pHostInfo, int bShow);
	unsigned int GetUserStatus(unsigned int status);


	void SetUserStatus(unsigned int status);
	void SetServerIP(unsigned long ip);
	void SaveAuthServerIP(unsigned int svrip);
	void TransChallenge(unsigned long* pChallenge);

	bool GetCrcCheckField(unsigned char* pChallenge, \
			unsigned int nChallengeSize, \
			unsigned int type, \
			unsigned long* pCrcBuff, \
			unsigned int* pnEncryptionType\
			);

	void Unix_GetHostInfo(struct _tagHostInfo* pHostInfo, int bShow, unsigned int* uiAuthHostIPMatched);

	int DrCOMLogout(EXECUTOR_TYPE excutor);

	void HandleNotLoginMsg(struct sockaddr_in* pAuthServerAddr, unsigned char* AuthData, int len);

	int CheckMsg(unsigned short* msg, int msg_data_len);
	int ChkMsgDisplayCounter(int index);

	int IsDrcom21330or2033();
	int IsDrcom2166();

	char* OnChkWWWAddress(unsigned char* AuthData, int len);
	char* OnChkIPAddress(unsigned char* AuthData, int len);
	char* OnChkSeedPort(unsigned char* AuthData, int len);
	char* OnRunProgramMsg(unsigned char* AuthData, int len);

	unsigned short PlusClientDigest(unsigned short* data);

	unsigned long DrcomCRC32(unsigned int nInitValue, unsigned char* ptrData, int nByteDataLen);
	int is_dhcp_169_ip_addr(unsigned int ip_addr);
	unsigned int string_to_ip(const char* string_ip);
	char* ip_to_string(unsigned int ip_addr);
	unsigned int GetDrcomCRC(unsigned int org_crc);

	unsigned char m_cDRCOM_CLIENT_KERNEL_VER;

private:
	DrUdpSocket m_Socket;
	int m_iServerPort;
	int m_iServerIP;
	int m_nClientPingNotResponseNum;
	int nSendDrcomPingInterval;

	dr_tick_count_t m_LastNotLoginMsgTime;
	dr_tick_count_t m_last_relay_cmd_send_time;

	int m_nSendChallengeRetryTimes;
	char m_szCheckEnvBeforeAuthErr[2048];
	unsigned int m_MsgDispalyCounter[MAX_MSG_COUNTER_NUM];
};

#endif /* DRAUTHSTATUS_H_ */
