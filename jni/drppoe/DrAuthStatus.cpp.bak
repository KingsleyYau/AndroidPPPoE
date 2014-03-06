/*
 * DrAuthStatus.cpp
 *
 *  Created on: 2014年2月22日
 *      Author: kingsley
 */

#include "DrAuthStatus.h"

DrAuthStatus::DrAuthStatus() {
	// TODO Auto-generated constructor stub
	m_drcfg.dwConfigVer = 1;

	m_nClientPingNotResponseNum = 0;
	nSendDrcomPingInterval = DRCOM_AUTH_PING_INTERVAL;
	m_LastNotLoginMsgTime = 0;
	m_last_relay_cmd_send_time = 0;
	m_cDRCOM_CLIENT_KERNEL_VER = atoi(DRCOMAUTHSVR_MINOR_VERSION);

	InitDrAuth();
}

DrAuthStatus::~DrAuthStatus() {
	// TODO Auto-generated destructor stub
}
bool DrAuthStatus::StartDrAuth() {
	return InitDrAuth();
}
bool DrAuthStatus::InitDrAuth() {
	m_AuthStatus.DrcomAddr.sin_family = AF_INET;
	m_AuthStatus.DrcomAddr.sin_port = htons(61440);
	inet_aton("1.1.1.1", &m_AuthStatus.DrcomAddr.sin_addr);

	return true;
}
void DrAuthStatus::InitPPPoE() {
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

		showLog("jni.DrAuthStatus::InitPPPoE", "%x %x", (iSrc & iMsk), (iDst & iMsk));

		if((iSrc & iMsk) == (iDst & iMsk)) {
			broadAddress = device.broad;
			showLog("jni.DrAuthStatus::InitPPPoE", "found broadAddress:%s device:%s", broadAddress.c_str(), device.name.c_str());
			break;
		}
	}


	if(0 < broadAddress.length()) {
		inet_aton(broadAddress.c_str(), &m_AuthStatus.DrcomAddr.sin_addr);
	}

	m_AuthStatus.DrcomDialExtProtoActiveCRC = 0;
	m_AuthStatus.DrcomDialExtProtoAuthLastCRC = 0;
	m_AuthStatus.DrcomDialExtProtoURLFirstMark = 0x01;
	m_AuthStatus.IsDrcomDialConnectionFirstActive = 0x01;
	m_AuthStatus.DrcomAuthRetryTimes = 0;
	m_AuthStatus.IsDrcomDialExtProtoProxyCheckActive = DRCOM_DIAL_EXT_PROTO_PROXYACTV_INIT;
	m_AuthStatus.DrcomAuthTimeout = DRCOM_AUTH_LOGIN_TIMEOUT;
	m_nSendChallengeRetryTimes = 0;
	m_AuthStatus.AuthPPPoEPluginStep = DRCOM_AUTH_STEP_PPPOE_SEND_CHALLENFE;
	m_AuthStatus.DrcomPPPoEAuthRetry966Kern = 0;
	m_AuthStatus.DrcomAuthLastTime = 0;
	SetUserStatus(ST_DRCOM_PPPOE_LOGOUT);

}
// #################################################################################
/*
 * Dr.COM and PPPoE认证发送过程
 */
/*
 * 发送认证命令
 */
int DrAuthStatus::SendAuthCmd(char* cmd, int len) {
    int res = 1;
    int addrlen = sizeof(struct sockaddr_in);
    //DebugMessage("======>向%s发送命令......\n", ip_to_string(as.DrcomAddr.sin_addr.s_addr));
    //show_data(cmd, len, "Drcom auth cmd:");

    DrUdpSocket udpSocket;
    int iRet = udpSocket.SendData((struct sockaddr_in *)&m_AuthStatus.DrcomAddr, cmd, len);
    if(iRet < 0) {
    	res = DRCOM_ERR;
    	// 网络连接中断，发送认证数据错误！
    }

    //DebugMessage("test auto find ip 202\n");
    if (string_to_ip(AUTO_FIND_SERVER_IP) == m_AuthStatus.DrcomAddr.sin_addr.s_addr) {
        struct sockaddr_in AutoFindAddr;
        memcpy(&AutoFindAddr, &m_AuthStatus.DrcomAddr, sizeof(struct sockaddr_in));
        AutoFindAddr.sin_addr.s_addr = string_to_ip(AUTO_FIND_SERVER_IP_202);
        showLog("jni.DrAuthStatus::SendAuthCmd", "搜索认证服务器:202.1.1.1");
        iRet = udpSocket.SendData((struct sockaddr_in *)&AutoFindAddr, cmd, len);
        if(iRet < 0) {
        	res = DRCOM_ERR;
        	// 网络连接中断，发送认证数据错误！
        }
    }

    if (check_ip(m_AuthStatus.AuthServerIntIP)
            && (m_AuthStatus.AuthServerIntIP != m_AuthStatus.DrcomAddr.sin_addr.s_addr))
    {
        struct sockaddr_in AutoFindAddr;
        memcpy(&AutoFindAddr, &m_AuthStatus.DrcomAddr, sizeof(struct sockaddr_in));
        addrlen = sizeof(struct sockaddr_in);
        AutoFindAddr.sin_addr.s_addr = m_AuthStatus.AuthServerIntIP;
        // 搜索认证服务器[%s]......
        showLog("jni.DrAuthStatus::SendAuthCmd", "搜索认证服务器AuthServerIntIP");

        memcpy(&AutoFindAddr, &m_AuthStatus.DrcomAddr, sizeof(struct sockaddr_in));
        iRet = udpSocket.SendData((struct sockaddr_in *)&AutoFindAddr, cmd, len);
        if(iRet < 0) {
        	res = DRCOM_ERR;
        	// 网络连接中断，发送认证数据错误！
        }
    }

    // fgx 2012-08-31 如果 as.AuthServerIntIP==as.DrcomAddr.sin_addr.s_addr，代表已经获取IP成功，不用重复获取
    //if (CheckAuthConfig(";multi_gw_find_tg;") && as.AuthStep < DRCOM_AUTH_STEP_LOGIN_RECV_CHALLENGE)
    if ((m_AuthStatus.AuthServerIntIP != m_AuthStatus.DrcomAddr.sin_addr.s_addr)
        && CheckAuthConfig(";multi_gw_find_tg;") && m_AuthStatus.AuthStep < DRCOM_AUTH_STEP_LOGIN_RECV_CHALLENGE)
    {
        int index = 0;

        for (index = 0; index < MAX_AUTH_SERVER_NUM; index++)
        {
            if (check_ip(m_drcfg.DrcomAuthServersIP[index]))
            {
                if (m_drcfg.DrcomAuthServersIP[index] != m_AuthStatus.DrcomAddr.sin_addr.s_addr)
                {
                    struct sockaddr_in MultiFindAddr;
                    memcpy(&MultiFindAddr, &m_AuthStatus.DrcomAddr, sizeof(struct sockaddr_in));
                    addrlen = sizeof(struct sockaddr_in);
                    MultiFindAddr.sin_addr.s_addr = m_drcfg.DrcomAuthServersIP[index];
                    // 搜索认证服务器[%s]......
                    showLog("jni.DrAuthStatus::SendAuthCmd", "搜索认证服务器m_drcfg.DrcomAuthServersIP[index]");
                    iRet = udpSocket.SendData((struct sockaddr_in *)&MultiFindAddr, cmd, len);
                    if(iRet < 0) {
                    	res = DRCOM_ERR;
                    	// 网络连接中断，发送认证数据错误！
                    }
                }
            }
            else
            {
                break;
            }
        }
    }
    return res;
}

// 认证挑战值
int DrAuthStatus::DrcomAuthSendLoginChallenge() {
    int res = DRCOM_SUC;
    struct  _tagChallenge login_cmd;
    time_t seconds = time(NULL);

    if (DRCOM_ERR == CheckEnvBeforeSendLoginChallenge())
    {
//        SendLoginError(0, GetCheckEnvBeforeAuthErrInfo());
//        ResetAuthStatus2None();
        return DRCOM_ERR;
    }

    bzero(&login_cmd, sizeof(struct _tagChallenge));
    m_AuthStatus.ChallengeID++;
    m_AuthStatus.DrcomLastRecvTime = dr_tick_count();
    memcpy(&m_AuthStatus.ChallengeRandomID, &seconds, sizeof(unsigned short));
    login_cmd.code = AUTH_CODE_CHALLENGE;
    login_cmd.ChallengeID = m_AuthStatus.ChallengeID;
    login_cmd.ChallengeRandomID = m_AuthStatus.ChallengeRandomID;
    login_cmd.ClientVerno = m_cDRCOM_CLIENT_KERNEL_VER;

    #ifdef DRCOM_ENCRYPT_PROTO
    DrcomGetRandomBuff(login_cmd.unused,sizeof(login_cmd.unused));
    DrcomGetRandomBuff(login_cmd.encrypt_info,sizeof(login_cmd.encrypt_info));
    #endif

    m_AuthStatus.AuthStep = DRCOM_AUTH_STEP_LOGIN_SEND_CHALLENGE;
    SetUserStatus(ST_DRCOM_LOGINING);
    res = SendAuthCmd((char *)&login_cmd, sizeof(struct _tagChallenge));
    return res;
}

/*
 * 服务器返回数据处理
 */
int DrAuthStatus::OnDrcomAuthLoginingHandle(struct sockaddr_in* pAuthServerAddr, unsigned char* AuthData, int len) {
    int res = DRCOM_SUC;
    struct  _tagDrCOMHeader* pHeader = (struct  _tagDrCOMHeader*)AuthData;
//    DebugMessage("DrcomAuthLoginingHandle called.\n");

    if (m_AuthStatus.AuthStep > DRCOM_AUTH_STEP_LOGIN_SEND_NAME_AND_PASS
            && m_AuthStatus.AuthServerIntIP != m_AuthStatus.AuthServerAddr.sin_addr.s_addr)
    {
//        DebugMessage("DrcomAuthOnliningHandle Recv data not from auth server.\n");
        return DRCOM_ERR;
    }

    switch (pHeader->AuthCode){
    case AUTH_CODE_RETURN_CHALLENGE:{
    	// 认证挑战值响应
    	res = OnDrcomLoginChallenge(pAuthServerAddr, AuthData, len);
    }break;
    case AUTH_CODE_LOGIN_SUC:{
    	// 认证账户密码成功
    	SaveAuthServerIP(pAuthServerAddr->sin_addr.s_addr);
        res = OnLoginSuccess(AuthData, len);
    } break;
    case AUTH_CODE_LOGIN_ERR:{
    	// 认证账户密码失败
    	res = OnLoginError(AuthData, len);
    }break;
    default:{
//        DebugMessage("DrcomAuthLoginingHandle:Drcom Auth data not handled(0x%02X)!!!", pHeader->AuthCode);
//        res = DrcomAuthOtherHandle(pAuthServerAddr, AuthData, len);
    }break;
    }
    return res;
}

/*
 * 获取挑战值响应处理
 */
int DrAuthStatus::OnDrcomLoginChallenge(struct sockaddr_in* pAuthServerAddr, unsigned char* ChallengeData, int len) {
	int res = DRCOM_SUC;
	struct  _tagReturnChallenge* pResChall = (struct  _tagReturnChallenge*)ChallengeData;

	if (pResChall->ChallengeID == m_AuthStatus.ChallengeID
			&& pResChall->ChallengeRandomID == m_AuthStatus.ChallengeRandomID)
	{
		if (m_AuthStatus.AuthStep >= DRCOM_AUTH_STEP_LOGIN_RECV_CHALLENGE)
		{
			return DRCOM_SUC;
		}

		SaveAuthServerIP(pAuthServerAddr->sin_addr.s_addr);
		ChangeAuthServerIP();
		// 发现认证服务器
		m_AuthStatus.AuthStep = DRCOM_AUTH_STEP_LOGIN_RECV_CHALLENGE;
		memcpy(m_AuthStatus.ChallengeSeed, pResChall->ChallengeSeed, SEED_LEN);
		m_AuthStatus.AuthChallengeDhcpsServerIP = pResChall->DhcpsServerIP;
		m_AuthStatus.AuthHostIP = pResChall->HostIP;
		m_AuthStatus.AuthTypeSele = pResChall->AuthTypeSele;
		//as.AuthDhcpOption = pResChall->DhcpOption;
		m_AuthStatus.DeviceKernelVer = pResChall->DeviceKernelVer;
		m_AuthStatus.DeviceVer = pResChall->DeviceVer;
		/// fgx 2013-05-10 判断内核支持DrCOM新的4步骤心跳
		m_AuthStatus.EncryptionMode = 0x8000 & pResChall->SystemAuthOption;
//                DebugMessage("DrcomAuthLoginingHandle() pResChall->SystemAuthOption: %x", pResChall->SystemAuthOption);
#if !defined(CONSOLE_CLIENT)
//                LogMessage("Dr.COM Auth Proto kVer:%d.%d Auth Host IP:%s", as.DeviceKernelVer, as.DeviceVer, ip_to_string(as.AuthHostIP));
#endif
		GetHostInfo(&m_AuthStatus.HostInfo, 1);

		if (DRCOM_ERR == CheckEnvBeforeSendNameAuth())
		{
//                    SendLoginError(0, GetCheckEnvBeforeAuthErrInfo());
//                    ResetAuthStatus2None();
			return DRCOM_SUC;
		}

		res = DrcomAuthSendNameAndPassword();
//                SendMessageToUI(SCMD_STATUS_MSG, _DRLANG("发送帐号密码......"));
	}
	return res;
}
/*
 * 认证账户密码
 */
int DrAuthStatus::DrcomAuthSendNameAndPassword() {
	int res = DRCOM_SUC;
	return res;
}
/*
 * 认证成功处理
 */
int DrAuthStatus::OnLoginSuccess(unsigned char* LoginData, int len) {
	int res = DRCOM_SUC;
	m_us.LastOnlineSec = m_us.OnlineSec;
	m_us.OnlineSec = 0;
	m_us.LoginTime = dr_tick_count();
	m_AuthStatus.AuthStep = DRCOM_AUTH_STEP_LOGIN_RECV_RESUALT;

	res = OnDrcomLoginSuccess(LoginData, len);
	//OnAntiProxyModuleLoginSuccess must call after OnDrcomLoginSuccess called
//	res = OnAntiProxyModuleLoginSuccess(LoginData, len);
	SetUserStatus(ST_DRCOM_LOGIN);
	m_AuthStatus.AuthStep = AUTH_STEP_ALL_DONE;

	DrcomAuthSendClientPing();

//	THREAD_RUN_FUNCTION(CheckTcpConnect, (void*)ip_to_string(as.AuthServerIntIP));
	return res;
}
/*
 * 认证成功处理
 */
int DrAuthStatus::OnDrcomLoginSuccess(unsigned char* LoginData, int len) {
    int res = DRCOM_SUC;
    nSendDrcomPingInterval = m_AuthStatus.nServerRequireSendDrcomPingInterval;

    if (LoginData && len)
    {
        struct _tagReturnLoginResult* login_res = NULL;
        struct _tagDrcomDogControlData* pDogControlData = NULL;
        struct _tagNewVersionInfo* pNewVerInfo = NULL;
        int offset = 0;
        login_res = (struct _tagReturnLoginResult*) LoginData;
        m_us.UsedFlux = login_res->UsedFlux;
        m_us.UsedTime = login_res->UsedMinutes;

        if (login_res->Balance != 0xFFFFFFFF)
        {
        	m_us.Balance = login_res->Balance & 0x7FFFFFFF;
        }
        else
        {
        	m_us.Balance = 0;
        }

        if (len >= (sizeof(struct _tagReturnLoginResult) + sizeof(struct _tagDrcomDogData)))
        {
            offset = sizeof(struct _tagReturnLoginResult);
            pDogControlData = (struct _tagDrcomDogControlData*)(LoginData + offset);
            offset = sizeof(struct _tagReturnLoginResult)
                     + sizeof(struct _tagDrcomDogControlData)
                     + sizeof(struct _tagDogControlTableItem) * (pDogControlData->DogControlTblLen - 1);
            //不管是否是封装防代理，都需要记录 AuthDogData，评需要
            memcpy(&m_AuthStatus.AuthDogData, &pDogControlData->DogData, sizeof(struct _tagDrcomDogData));
        }

        //offset = sizeof(struct _tagReturnLoginResult)+sizeof(struct _tagDrcomDogControlData)-sizeof(struct _tagDogControlTable)*;
        if (len >= (offset + (int) sizeof(struct _tagNewVersionInfo)))
        {
            pNewVerInfo = (struct _tagNewVersionInfo*)(LoginData + offset);
            memcpy(&m_AuthStatus.NewVerInfo, pNewVerInfo, sizeof(struct _tagNewVersionInfo));

            //2166 2133 需要配置，默认是2133，由于2133默认只有启了drcomdll就全部防代理，
            //所以2133默认需要加NEW_VER_OPTION_2166_ANTIP_PROXY标记
            //if(!IS_CONFIG_PROTO_NEW_VER_OPTION_IS_2166_KERNEL)
            if (IsDrcom21330or2033())
            {
            	m_AuthStatus.NewVerInfo.NewVerOption  |= NEW_VER_OPTION_2166_ANTIP_PROXY;
            }

            if (m_AuthStatus.NewVerInfo.NewVerOption != 0xFF
                    && (m_AuthStatus.NewVerInfo.NewVerOption & NEW_VER_OPTION_2166_ANTIP_PROXY) == 0x00)
            {
                if (IsDrcom2166()) {
                }
            }

            if ((m_AuthStatus.NewVerInfo.NewVerMark & 0x01) == 0)
            {
            	m_AuthStatus.ClientDigest = PlusClientDigest((unsigned short*) m_AuthStatus.DigestMD5);
            }
            else
            {
            	m_AuthStatus.ClientDigest = * ((unsigned short*) m_AuthStatus.KeepAliveMD5);
            }

            if (EXECUTOR_8021X_LOCAL_NET == m_AuthStatus.LoginExecutor)
            {
                //登录内网不防代理
                //#pragma TAG_MSG ("添加 802.1x 内网登录不防代理")
                //#pragma NOT_IMPLEMENT_YET
                //as.NewVerInfo.NewVerOption  &= ~NEW_VER_OPTION_2166_ANTIP_PROXY;
            }

            offset += (int) sizeof(struct _tagNewVersionInfo);
        }
        else
        {
//            LogMessage("Auth Data has not new info.");
        }

        //DebugMessage("len=%d,offset=%d",len,offset);
        if (len >= (offset + USER_NAME_MIN_LEN))
        {
            char* ptrName = NULL;
            ptrName = (char*)(LoginData + offset);
            //DebugMessage("name=%s",ptrName);
            //DebugMessage("name+1=%s",ptrName+1);
            strncpy(m_us.Name, ptrName, ACCOUNT_MAX_LEN - 1);
        }
    }

    if (strlen(m_us.Name) == 0)
    {
        strncpy(m_us.Name, "Null", sizeof(m_us.Name) - 1);
    }

    m_AuthStatus.DrcomAuthTimeout = m_AuthStatus.nServerRequireSendDrcomPingTimeout;
//    SendLoginStatus(0);
//#if defined(LOCALE_CODE_CHECK_PROXY)
//
//    if (GetConfigValueFromConfigRule("ping_guard") > 0)
//    {
//        THREAD_RUN_FUNCTION(DrcomClientPingGuard, NULL);
//    }
//
//#endif
//#ifdef ENABLE_COPYDATA_INTERFACE
//    CopydataIntfSendAuthRetToIntf(CDI_OPT_LOGIN, AE_DRCOM_AUTH_SUCCESS);
//#endif
    return res;
}
/*
 * 认证失败处理
 */
int DrAuthStatus::OnLoginError(unsigned char* LoginData, int len) {
	int res = DRCOM_SUC;
	return res;
}
/*
 * 客户端发起心跳
 */
int DrAuthStatus::NewDrcomAuthSendClientPing()
{
    int res = DRCOM_SUC;
    struct _tagHostPingServer DrcomPing;
    time_t seconds = time(NULL);
    unsigned int nEncryptionType;
    unsigned char aChallenge[SEED_LEN];
    bzero(&DrcomPing, sizeof(struct _tagHostPingServer));
    DrcomPing.Code = 0xFF;
    memcpy(&DrcomPing.DogData, &m_AuthStatus.AuthDogData, sizeof(struct _tagDrcomDogData));
    DrcomPing.Milliseconds = (unsigned short)seconds;

    // fgx 2013-03-29 修改第三步的校验方式
    memcpy(DrcomPing.checkcode.newcheck.ChallengeSeed, m_AuthStatus.ChallengeSeed, SEED_LEN);
    TransChallenge((unsigned long*)aChallenge);
    GetCrcCheckField(aChallenge, SEED_LEN, m_AuthStatus.EncryptionMode, DrcomPing.checkcode.newcheck.crc, &nEncryptionType);
//    DebugMessage("NewDrcomAuthSendClientPing %x %d %d, %d %x.\n", *((unsigned int*)aChallenge), m_AuthStatus.EncryptionMode, nEncryptionType, DrcomPing.checkcode.newcheck.ChallengeSeed, DrcomPing.checkcode.newcheck.crc);
    if (ENCRYPTION_MODE_NONE == nEncryptionType) {
        // 使用原来的加密方式
        memcpy(DrcomPing.checkcode.oldcheck.KeepAliveMD5, m_AuthStatus.KeepAliveMD5, MD5_LEN);
    }

    DrcomPing.InternetAccessControl = m_AuthStatus.InternetAccessControl;
//    DebugMessage("Sending Client ping...... as.InternetAccessControl=%d", as.InternetAccessControl);
    m_nClientPingNotResponseNum++;

    if (DRCOM_AUTH_LOCALNET_INERNET == m_AuthStatus.LocalNetAuthMode)
    {
        return SendAuthCmd((char *)&DrcomPing, sizeof(struct _tagHostPingServer));
    }
    else
    {
        return SendAuthCmd((char *)&DrcomPing, sizeof(struct _tagHostPingServer) - sizeof(DrcomPing.InternetAccessControl));
    }

    return res;
}
/*
 * 服务器响应心跳
 */
int DrAuthStatus::DrcomAuthOnliningMsgCodeHandle(struct sockaddr_in* pAuthServerAddr, unsigned char* AuthData, int len)
{
    int res = DRCOM_SUC;
    struct  _tagMessagePacket* pDrcomMsg = (struct  _tagMessagePacket*)AuthData;
    //DebugMessage("DrcomAuthOnliningMsgCodeHandle called.\n");

    if (pDrcomMsg->MsgType != AUTH_MSG_SYSTEM_MSG &&  len > MAX_DRCOM_MESSAGE_LEN)
    {
//        release_show_data((char*)AuthData, len, "Error Length Dr.COM Message:");
        return DRCOM_ERR;
    }
    else if (len > MAX_DRCOM_MESSAGE_DATA_LEN)
    {
//        release_show_data((char*)AuthData, len, "Error Length Dr.COM Message:");
        return DRCOM_ERR;
    }

    switch (pDrcomMsg->MsgType)
    {
    case AUTH_MSG_CODE_NONE:
        break;

    case AUTH_MSG_CHECK_WWW_ADDR:
        {
            char* szChkMsg = NULL;
            static dr_tick_count_t last_check_www_mesg_send_time = 0;
            static char szLastWWWPChkMesg[1024] = {'\0'};
            szChkMsg = OnChkWWWAddress(AuthData, len);

            if (szChkMsg)
            {
                if (strlen(szChkMsg))
                {
                    if (strcasecmp(szChkMsg, szLastWWWPChkMesg) == 0)
                    {
                        dr_tick_count_t now = dr_tick_count();

                        if (DRCOM_AUTH_SHOW_MESG_INTERVAL > (now - last_check_www_mesg_send_time) / DR_CLOCKS_PER_SEC)
                        {
                            free(szChkMsg);
                            break;
                        }
                    }

//                    res = SendMessageToUI(SCMD_SYS_MSG MSG_RYPE_WARNING, szChkMsg);
                    last_check_www_mesg_send_time = dr_tick_count();
                    strncpy(szLastWWWPChkMesg, szChkMsg, sizeof(szLastWWWPChkMesg) / sizeof(szLastWWWPChkMesg[0]));
                }

                free(szChkMsg);
            }
        }
        break;

    case AUTH_MSG_CHECK_IP_ADDR:
        {
            char* szChkMsg = NULL;
            static dr_tick_count_t last_check_ip_mesg_send_time = 0;
            static char szLastIPChkMesg[1024] = {'\0'};
            szChkMsg = OnChkIPAddress(AuthData, len);
            if (szChkMsg)
            {
                if (strlen(szChkMsg))
                {
                    if (strcasecmp(szChkMsg, szLastIPChkMesg) == 0)
                    {
                        dr_tick_count_t now = dr_tick_count();

                        if (DRCOM_AUTH_SHOW_MESG_INTERVAL > (now - last_check_ip_mesg_send_time) / DR_CLOCKS_PER_SEC)
                        {
                            free(szChkMsg);
                            break;
                        }
                    }
//                    res = SendMessageToUI(SCMD_SYS_MSG MSG_RYPE_WARNING, szChkMsg);
                    last_check_ip_mesg_send_time = dr_tick_count();
                    strncpy(szLastIPChkMesg, szChkMsg, sizeof(szLastIPChkMesg) / sizeof(szLastIPChkMesg[0]));
                }

                free(szChkMsg);
            }
        }
        break;
    case AUTH_MSG_CHECK_RESET_IP:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("IP设置错误，不能向本机提供Internet服务，请重新设置本机IP地址。"));
        break;
    case AUTH_MSG_CHECK_ACCOUNT_DEL4:
    case AUTH_MSG_CHECK_ACCOUNT_DEL5:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("你使用的帐号已经被注销。"));
        break;
    case AUTH_MSG_TODAY_NOT_USE:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("今天禁止上网。"));
        break;
    case AUTH_MSG_THIS_TIME_NOT_USE:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("本时间段禁止上网。"));
        break;
    case AUTH_MSG_CHECK_SEED_PORT:
        {
            char* szChkMsg = NULL;
            szChkMsg = OnChkSeedPort(AuthData, len);

            if (szChkMsg)
            {
                if (strlen(szChkMsg))
                {
//                    res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, szChkMsg);
                }

                free(szChkMsg);
            }
        }
        break;

    case AUTH_MSG_NOT_ENOUGH:
        if (ChkMsgDisplayCounter(COUNTER_INDEX_NOT_ENOUGH))
        {
//            res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("费用已经超支！！！"));
        }
        break;
    case AUTH_MSG_8021X_LOGOUT:
        break;

    case AUTH_MSG_USER_GROUP_NOT_EXSIT:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("本帐号所属的用户组不存在。"));
        break;
    case AUTH_MSG_NOT_LOGIN:
        {
            dr_tick_count_t now = dr_tick_count();

            if (now < m_LastNotLoginMsgTime)
            {
            	m_LastNotLoginMsgTime = now;
            }

            if (((now - m_LastNotLoginMsgTime) / DR_CLOCKS_PER_SEC) > NOT_LOGIN_MSG_MIN_INTERVAL)
            {
            	m_LastNotLoginMsgTime = now;
                //res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR,"本机尚未登录服务器，请登录后再上网！！！");
            }

            m_AuthStatus.onlineing_logout_flag++;
            HandleNotLoginMsg(pAuthServerAddr, AuthData, len);
//#ifdef ENABLE_COPYDATA_INTERFACE
//            CopydataIntfSendAuthRetToIntf(CDI_OPT_OFFLINE, AE_DRCOM_NoOnline);
//#endif
        }
        //logout
        break;
    case AUTH_MSG_PASS_CHANGED:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("你的帐号密码已经被修改。"));
    	m_AuthStatus.onlineing_logout_flag++;
        //logout
        break;
    case AUTH_MSG_CODE_23:
        //break;
    case AUTH_MSG_CODE_24:
        //break;
    case AUTH_MSG_CODE_25:
        //break;
    case AUTH_MSG_CODE_26:
        //break;
    case AUTH_MSG_CODE_27:
        //break;
    case AUTH_MSG_CODE_28:
        //break;
    case AUTH_MSG_CODE_X88: //-120
        break;
    case AUTH_MSG_CODE_GW_UTP2_ERR:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("UTP2连接失败，请检查线路和参数设置。"));
        break;
    case AUTH_MSG_GROUP_TIME_OVER_CAN_NOT_USE:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("本帐号的累计时间已经超过组限制！！！不能使用外网资源。"));
        m_AuthStatus.onlineing_logout_flag++;
        //logout
        break;

    case AUTH_MSG_TIME_OVER_CAN_NOT_USE:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("本帐号的累计时间已经超过限制！！！不能使用外网资源。"));
        m_AuthStatus.onlineing_logout_flag++;
        //logout
        break;
    case AUTH_MSG_GROUP_FLUX_OVER_CAN_NOT_USE:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("本帐号的累计流量已经超过组限制！！！不能使用外网资源。"));
        m_AuthStatus.onlineing_logout_flag++;
        //logout
        break;

    case AUTH_MSG_FLUX_OVER_CAN_NOT_USE:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("本帐号的累计流量已经超过限制！！！不能使用外网资源。"));
        // fgx 1X拨号时不断开
//        if (!NEW_IS_CONFIG_PROTO_DRCOM_8021X) {
//            m_AuthStatus.onlineing_logout_flag++;
//        }
        //logout
        break;

    case AUTH_MSG_GROUP_TIME_OVER:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("本帐号的累计时间已经超过组限制！！！"));
        break;

    case AUTH_MSG_TIME_OVER:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("本帐号的累计时间已经超过限制！！！"));
        break;

    case AUTH_MSG_GROUP_FLUX_OVER:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("本帐号的累计流量已经超过组限制！！！"));
        break;

    case AUTH_MSG_FLUX_OVER:
//        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_ERR, _DRLANG("本帐号的累计流量已经超过限制！！！"));
        break;

    case AUTH_MSG_SERVER_ALIVE: //服务器心跳
        {
            unsigned short plusword = 0;
            struct _tagServerKeepAlive* pServerAlive = NULL;
            pServerAlive = (struct _tagServerKeepAlive*)AuthData;
            plusword = pServerAlive->PlusWord;
            m_AuthStatus.RecvClientDigest = plusword;
            plusword = plusword ^ m_AuthStatus.ClientDigest;
            res = DrcomAuthRepalyServerOnlineMsg();

            if (len >= sizeof(struct _tagServerKeepAlive))
            {
                if (m_AuthStatus.IPInfo.num > pServerAlive->AllowMaxIPNum)
                {
                    //ip地址太多，下线
//                    ErrorMessage(_DRLANG("你配置的IP地址太多！！！"));
                }
            }
        }
        break;

    case AUTH_MSG_NONE_CHAR_1:
        break;

    case AUTH_MSG_NONE_CHAR_2:
        break;

    case AUTH_MSG_SYSTEM_MSG:
        {
            char* strSysMsg = NULL;
            strSysMsg = (char*)(pDrcomMsg->MsgData);

            if (!CheckMsg((unsigned short*)&pDrcomMsg->MsgData1, len - 2))
            {
//                ErrorMessage(_DRLANG("收到非法的攻击信息！！！"));
                break;
            }

//            if (DRCOM_SUC != (res = DrcomTxtCmdHandler(strSysMsg)))
//            {
//                if (strstr(strSysMsg, "DRCOMWEBMSG:") == strSysMsg)
//                {
////                    res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_URL, &strSysMsg[strlen("DRCOMWEBMSG:")]);
//                }
//                else if (strstr(strSysMsg, "#POPUP#") == strSysMsg)
//                {
////                    res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_POPUP, &strSysMsg[strlen("#POPUP#")]);
//                }
//                else
//                {
////                    res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_TXT, strSysMsg);
//                }
//            }
        }
        break;

    case AUTH_MSG_SYSTEM_RUN_MSG: //'9'+1
        {
            char* szChkMsg = NULL;
            szChkMsg = OnRunProgramMsg(AuthData, len);

            if (szChkMsg)
            {
                //DebugMessage("as.DrcomDialExtProtoURLFirstMark=%d\n",as.DrcomDialExtProtoURLFirstMark);
                //DebugMessage("IS_CONFIG_PROTO_DRCOM_PPPOE_PLUGIN=%d\n",IS_CONFIG_PROTO_DRCOM_PPPOE_PLUGIN);
                if (strlen(szChkMsg))
                {
                    if (/*NEW_IS_CONFIG_PROTO_DRCOM_PPPOE_PLUGIN*/1)
                    {
                        if (m_AuthStatus.DrcomDialExtProtoURLFirstMark != 0)
                        {
//                            res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_URL, szChkMsg);
                            m_AuthStatus.DrcomDialExtProtoURLFirstMark = 0x00;
                        }
                    }
                    else
                    {
//                        res = SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_URL, szChkMsg);
                    }
                }

                free(szChkMsg);
            }
        }
        break;
    default:
//        DebugMessage("DrcomAuthOnliningMsgCodeHandle not handled:0x%02X\n", pDrcomMsg->MsgType);
        break;
    }

    if (m_AuthStatus.LoginExecutor == EXECUTOR_8021X_LOCAL_NET
            || m_AuthStatus.LoginExecutor == EXECUTOR_8021X_INTERNET_2_LOCAL_NET)
    {
    	m_AuthStatus.onlineing_logout_flag = 0;
    }

    //res = OnliningMsgLogout(as.onlineing_logout_flag);
    //as.onlineing_logout_flag = 0;
//    DebugMessage("DrcomAuthOnliningMsgCodeHandle handled:0x%02X return 0x%08X\n",
//                 pDrcomMsg->MsgType, res);
    return res;
}

int DrAuthStatus::DrcomAuthRepalyServerOnlineMsg()
{
    int res = DRCOM_SUC;
    struct _tagHostReplayServerAlive replay;
    unsigned char* pMD5Res = NULL;
    dr_tick_count_t now = dr_tick_count();

    if ((unsigned long)((now - m_last_relay_cmd_send_time) / DR_CLOCKS_PER_SEC) < 5)
    {
//        DebugMessage("DrcomAuthRepalyServerOnlineMsg before 5 second(%u,%u,%u,%u) .\n",
//                     (unsigned int)((now - last_relay_cmd_send_time) / DR_CLOCKS_PER_SEC),
//                     (unsigned int)now,
//                     (unsigned int)last_relay_cmd_send_time,
//                     DR_CLOCKS_PER_SEC);
        return DRCOM_SUC;
    }

    bzero(&replay, sizeof(struct _tagHostReplayServerAlive));
    replay.Code = 0xFE;
    memcpy(&replay.Digest, &m_AuthStatus.ClientDigest, sizeof(unsigned short));
    replay.Digest = (unsigned char)(replay.Digest + replay.HostIpNum);
    replay.HostIpNum = m_AuthStatus.IPInfo.num;
    replay.KeepReplayMd5[0] = 0x14;
    replay.KeepReplayMd5[1] = 0x00;
    replay.KeepReplayMd5[2] = 0x07;
    replay.KeepReplayMd5[3] = 0x0B;
    memcpy(&replay.KeepReplayMd5[4], &m_AuthStatus.ClientDigest, sizeof(unsigned short));
    // mark by kk
    //pMD5Res = MD5Encode((unsigned char*)&replay, 9);
    memcpy(replay.KeepReplayMd5, pMD5Res, MD5_LEN);
    memcpy(&replay.DogData, &m_AuthStatus.AuthDogData, sizeof(struct _tagDrcomDogData));
    replay.unknow3 = 0xFF;
    m_nClientPingNotResponseNum--;
    res = SendAuthCmd((char *)&replay, sizeof(struct _tagHostReplayServerAlive));
    //DebugMessage("Send Drcom Auth Repaly Server Online Msg.\n");
    m_last_relay_cmd_send_time = dr_tick_count();
    return res;
};


// #############################################################################################
// #############################################################################################
/*
 * 不明觉厉
 */

unsigned long DrAuthStatus::DrcomCRC32(unsigned int nInitValue, unsigned char* ptrData, int nByteDataLen)
{
    unsigned long* ptrCRCData = NULL;
    int i = 0;
    unsigned long nResCRC = 0;
    nResCRC = nInitValue;
    ptrCRCData = (unsigned long*)ptrData;

    for (i = 0; i < nByteDataLen / 4; i++)
    {
        nResCRC = nResCRC ^ ptrCRCData[i];
    }

    return nResCRC;
}

int DrAuthStatus::is_dhcp_169_ip_addr(unsigned int ip_addr) {
    return ((ip_addr & string_to_ip("255.255.0.0")) == string_to_ip("169.254.0.0"));
}

#define ACTIVE_CRC_RBIT 2
#define ACTIVE_CRC_LBIT (32-ACTIVE_CRC_RBIT)
unsigned int DrAuthStatus::GetDrcomCRC(unsigned int org_crc)
{
    return ((org_crc >> ACTIVE_CRC_RBIT) + (org_crc << ACTIVE_CRC_LBIT));
}

char* DrAuthStatus::ip_to_string(unsigned int ip_addr) {
    struct in_addr  in_ip;
    char* stringip = NULL;
    in_ip.s_addr = ip_addr;
    stringip =  inet_ntoa(in_ip);
    return stringip;
}
unsigned int DrAuthStatus::string_to_ip(const char* string_ip) {
    unsigned int dwIPAddress = inet_addr(string_ip);

    if (dwIPAddress != INADDR_NONE &&
            dwIPAddress != INADDR_ANY)
    {
        return dwIPAddress;
    }

    return dwIPAddress;
}
void DrAuthStatus::HandleNotLoginMsg(struct sockaddr_in* pAuthServerAddr, unsigned char* AuthData, int len)
{
    char* ptrNotLoginMsg = (char*)(AuthData + 4);
    char* ptrAdminRest = strstr(ptrNotLoginMsg, DR_ADMIN_RESET_CMD);

    if (ptrAdminRest == ptrNotLoginMsg)
    {
        char* ptrAdminRestReson = strstr(ptrAdminRest + strlen(DR_ADMIN_RESET_CMD), DR_ADMIN_RESET_CMD_RESONE_SPLIT);
//        LogMessage(ptrNotLoginMsg);

        if (ptrAdminRestReson && strlen(ptrAdminRestReson + 1))
        {
            *ptrAdminRestReson = '\0';
//            ErrorMessageToUI(_DRLANG(ptrAdminRest),
//                             ptrAdminRestReson + 1,
//                             _DRLANG("\n请修改密码!")); //吉林大学 //到 https://ip.jlu.edu.cn/pay
        }
        else
        {
            if (m_AuthStatus.onlineing_logout_flag == 1)
            {
//                ErrorMessageToUI(_DRLANG(ptrAdminRest));
            }
        }

//        OnliningMsgLogout(1);
        m_AuthStatus.onlineing_logout_flag = 0;
    }
    else
    {
//        SendMessageToUI(SCMD_SYS_MSG MSG_TYPE_COMMAND, SCMD_LOGIN);
    }
}

char* DrAuthStatus::OnRunProgramMsg(unsigned char* AuthData, int len)
{
    char run_msg[512] = {'\0'};
    int msglen = len - 2;
    char* msg = (char*)(AuthData + 2);
    char* msg_start = (char*)(AuthData + 2);

    if (len > 200)
    {
        return NULL;
    }

    if (!CheckMsg((unsigned short*)msg, msglen))
    {
        return NULL;
    }

    memcpy(run_msg, msg_start + 2, msglen);
    run_msg[msglen] = 0x00;
    return strdup(run_msg);
}


unsigned short DrAuthStatus::PlusClientDigest(unsigned short* data) {
	return 1;
}
int DrAuthStatus::CheckEnvBeforeSendLoginChallenge() {
    int index = 0;

    for (index = 0; index < m_AuthStatus.IPInfo.num; index++)
    {
//        if (CheckIPConfig(m_AuthStatus.IPInfo.ip_list[index]) == DRCOM_ERR)
//        {
//            return DRCOM_ERR;
//        }
    }

    return DRCOM_SUC;
}

void DrAuthStatus::ChangeAuthServerIP()
{
//    DebugMessage("ChangeAuthServerIP Status=%08X AuthServerIntIP=%s\n", GetUserStatus(0), ip_to_string(as.AuthServerIntIP));

    if (m_AuthStatus.DrcomAddr.sin_addr.s_addr != m_AuthStatus.AuthServerIntIP
            && check_ip(m_AuthStatus.AuthServerIntIP))
    {
        //if(/*bForce || */(string_to_ip(AUTO_FIND_SERVER_IP) == as.DrcomAddr.sin_addr.s_addr))
        {
            if (GetUserStatus(ST_DRCOM_LOGINING))
            {
                SetServerIP(m_AuthStatus.AuthServerIntIP);
            }
            else if (GetUserStatus(ST_DRCOM_MODIFY_PWDING))
            {
                SetServerIP(m_AuthStatus.AuthServerIntIP);
            }
        }
    }
}

char* DrAuthStatus::GetCheckEnvBeforeAuthErrInfo() {
    return m_szCheckEnvBeforeAuthErr;
}

int DrAuthStatus::CheckEnvBeforeSendNameAuth()
{
    int res = 0;
    memset(m_szCheckEnvBeforeAuthErr, 0, 2048);

//    if (DRCOM_ERR == drcom_check_router())
//    {
//        bzero(szCheckEnvBeforeAuthErr, sizeof(szCheckEnvBeforeAuthErr));
//        strncpy(szCheckEnvBeforeAuthErr,
//                _DRLANG("发现您在使用路由器登录，请勿使用路由器或代理软件上网，谢谢合作！"),
//                sizeof(szCheckEnvBeforeAuthErr) - 1);
//        ResetAuthStatus2None();
//        return DRCOM_ERR;
//    }
//
//    if (NEW_IS_CONFIG_ANTI_PROXY_VER_CHECK_PROXY_NO_DOG)
//    {
//        SendMessageToUI(SCMD_STATUS_MSG, _DRLANG("检测认证协议更新......"));
//        res =  AntiProxyModule_Call(DRCOMDLL_HANDLE_RECV_CODE2);
//
//        if (DRCOM_SUC == res)
//        {
//            bzero(szCheckEnvBeforeAuthErr, sizeof(szCheckEnvBeforeAuthErr));
//            strncpy(szCheckEnvBeforeAuthErr,
//                    _DRLANG("客户端认证协议核心模块正在升级，请稍后重新登录！"),
//                    sizeof(szCheckEnvBeforeAuthErr) - 1);
//            return DRCOM_ERR;
//        }
//    }

    if (CheckIPConfig(m_AuthStatus.AuthHostIP) == DRCOM_ERR)
    {
        return DRCOM_ERR;
    }

    if ((m_AuthStatus.HostInfo.DHCPServerIP == INADDR_ANY || m_AuthStatus.HostInfo.DHCPServerIP == INADDR_NONE)
            && m_AuthStatus.IPInfo.num < 1)
    {
    	m_AuthStatus.IPInfo.num = 1;
    	m_AuthStatus.IPInfo.ip_list[0] = m_AuthStatus.AuthHostIP;
    }

    return DRCOM_SUC;
}

int DrAuthStatus::CheckIPConfig(unsigned int nHostIP)
{
    return DRCOM_SUC;
}

// fgx 2013-04-01 转换Challenge
void DrAuthStatus::TransChallenge(unsigned long* pChallenge)
{
	// 旧版本，用旧方式生成校验码及Challenge
	memcpy(pChallenge, m_AuthStatus.ChallengeSeed, SEED_LEN);
}

int DrAuthStatus::IsDrcom21330or2033() {
	return false;
}
int DrAuthStatus::IsDrcom2166() {
	return true;
}

char* DrAuthStatus::OnChkWWWAddress(unsigned char* AuthData, int len)
{
    char* wwwaddr = NULL;
    int sublen = 0;
    char szwwwaddr[256] = {'\0'};
    char msg[512] = {'\0'};
    int index = 0;

    if (len <= 3 || len > MAX_DRCOM_WWW_MESSAGE_LEN)
    {
        return NULL;
    }

    wwwaddr = (char*)&AuthData[2];
    sublen = wwwaddr[0];
    index = 1;

    while (wwwaddr[index] != 0 && sublen)
    {
        strncat(szwwwaddr, &wwwaddr[index], sublen);
        index = index + sublen;
        sublen = wwwaddr[index];
        index++;
        strcat(szwwwaddr, ".");
    }

    sublen = strlen(szwwwaddr);

    if (sublen && szwwwaddr[sublen - 1] == '.')
    {
        szwwwaddr[sublen - 1] = '\0';
    }

    snprintf(msg, sizeof(msg) - 1, "网址%s禁止访问", szwwwaddr);
    return strdup(msg);
}

char* DrAuthStatus::OnChkIPAddress(unsigned char* AuthData, int len)
{
    unsigned long ip = 0;
    char msg[512] = {'\0'};

    if (len != 6)
    {
        return NULL;
    }

    memcpy(&ip, &AuthData[2], sizeof(unsigned long));
    snprintf(msg, sizeof(msg) - 1, "网址%s禁止访问", ip_to_string(ip));
    return strdup(msg);
}

char* DrAuthStatus::OnChkSeedPort(unsigned char* AuthData, int len)
{
    unsigned short port = 0;
    char msg[512] = {'\0'};

    if (len < 4)
    {
        return NULL;
    }

    memcpy(&port, &AuthData[2], sizeof(unsigned short));
    snprintf(msg, sizeof(msg) - 1, "禁止使用端口[%d]", port);
    return strdup(msg);
}

int DrAuthStatus::CheckMsg(unsigned short* msg, int msg_data_len)
{
    unsigned short msgcrc = 0;
    int index = 0;
    int text_msg_body_len = msg_data_len - 2;

    if (text_msg_body_len < 2
            || msg_data_len > 400
            || strlen((char*)(&msg[1])) < 2
       )
    {
        return 0;
    }

//    DebugMessage("msg[0]=%04X,msg[1]=%04X", msg[0], msg[1]);

    //消息是两个空格不显示
    if ((msg[1] == 0x2020 && text_msg_body_len == 2)
            || (msg[1] == 0x2020 && text_msg_body_len == 3)
            || (msg[1] == 0x0020 && text_msg_body_len == 2)
            || (msg[1] == 0x2000 && text_msg_body_len == 2)
       )
    {
        return 0;
    }

    for (msgcrc = 0, index = 1; index < (msg_data_len + 1) / 2; index++)
    {
        msgcrc = (unsigned short)(msgcrc + * (msg + index));
    }

    if (*msg != (unsigned short)(msgcrc * m_AuthStatus.ClientDigest))
    {
//        DebugMessage("MessageCheck msg=%04X ClientDigest=%04X msgcrc=%04X msgcrcres=%04X",
//                     *msg, as.ClientDigest, msgcrc, (unsigned short)(msgcrc * as.ClientDigest));
        return 0;
    }

    return 1;
}

int DrAuthStatus::ChkMsgDisplayCounter(int index)
{
    if (index > MAX_MSG_COUNTER_NUM)
    {
        return 1;
    }

    m_MsgDispalyCounter[index]++;

    if (m_MsgDispalyCounter[index] < DISPLAY_COUNTER)
    {
        return 0;
    }

    m_MsgDispalyCounter[index] = 0;
    return 1;
}

// fgx 2013-03-29 生成校验位
// pChallenge：从第二个包中收到的 challenge
// nChallengeSize：challenge的大小，目前默认为sizeof(unsigned long)，即4个字节
// type：ENCRYPTION_MODE_TYPE定义，用于指定采用何种算法生成
// pCrcBuff：生成校验位的空间，默认为8个字节
bool DrAuthStatus::GetCrcCheckField(unsigned char* pChallenge, unsigned int nChallengeSize, unsigned int type, unsigned long* pCrcBuff, unsigned int* pnEncryptionType)
{
    bool bResult = false;
//    unsigned int* pnChallenge = NULL;
//    unsigned int nEncryptionType = 0;
//	if (nChallengeSize < sizeof(unsigned long))
//	{
////	    ErrorMessage("GetCrcCheckField Error nChallengeSize:%d.\n", nChallengeSize);
//		return false;
//	}
//
//    pnChallenge = (unsigned int*)pChallenge;
//    nEncryptionType = *pnChallenge % 4;
//    if (NULL != pnEncryptionType) {
//        *pnEncryptionType = nEncryptionType;
//    }
//
//    if (type > 0) {
//        // 新版本默认可以处理
//        bResult = true;
//        if (ENCRYPTION_MODE_NONE == nEncryptionType) {
//            pCrcBuff[0] = DRCOM_DIAL_EXT_PROTO_CRC_INIT;
//            pCrcBuff[1] = 126;
//        }
//        else if (ENCRYPTION_MODE_MD5 == nEncryptionType) {
//            // 用MD5生成校验码
//            unsigned char* pCheckBuff = (unsigned char*)pCrcBuff;
//            // MD5 Challenge
//            unsigned char ucEnBuf[16] = {0};
//            MD5_CTX md5;
//            MD5_Init(&md5);
//            MD5_Update(&md5, pChallenge, nChallengeSize);
//            MD5_Final(ucEnBuf, &md5);
//            // 取第2,3,8,9,5,6,13,14位（0为第一位）
//            pCheckBuff[0] = ucEnBuf[2];
//            pCheckBuff[1] = ucEnBuf[3];
//            pCheckBuff[2] = ucEnBuf[8];
//            pCheckBuff[3] = ucEnBuf[9];
//            pCheckBuff[4] = ucEnBuf[5];
//            pCheckBuff[5] = ucEnBuf[6];
//            pCheckBuff[6] = ucEnBuf[13];
//            pCheckBuff[7] = ucEnBuf[14];
//        }
//        else if (ENCRYPTION_MODE_MD4 == nEncryptionType) {
//            // 用MD4生成校验码
//            unsigned char* pCheckBuff = (unsigned char*)pCrcBuff;
//            // MD5 Challenge
//            unsigned char ucEnBuf[16] = {0};
//            MD4_CTX md4;
//            MD4Init(&md4);
//            MD4Update(&md4, pChallenge, nChallengeSize);
//            MD4Final(ucEnBuf, &md4);
//            // 取第1,2,8,9,4,5,11,12位（0为第一位）
//            pCheckBuff[0] = ucEnBuf[1];
//            pCheckBuff[1] = ucEnBuf[2];
//            pCheckBuff[2] = ucEnBuf[8];
//            pCheckBuff[3] = ucEnBuf[9];
//            pCheckBuff[4] = ucEnBuf[4];
//            pCheckBuff[5] = ucEnBuf[5];
//            pCheckBuff[6] = ucEnBuf[11];
//            pCheckBuff[7] = ucEnBuf[12];
//        }
//        else if (ENCRYPTION_MODE_SHA1 == nEncryptionType) {
//            // 用SHA1生成校验码
//            unsigned char* pCheckBuff = (unsigned char*)pCrcBuff;
//            // MD5 Challenge
//            unsigned char ucEnBuf[20] = {0};
//            SHA1Context sha1;
//            SHA1Reset(&sha1);
//            SHA1Input(&sha1, pChallenge, nChallengeSize);
//            SHA1Result(&sha1, ucEnBuf);
//            // 取第2,3,9,10,5,6,15,16位（0为第一位）
//            pCheckBuff[0] = ucEnBuf[2];
//            pCheckBuff[1] = ucEnBuf[3];
//            pCheckBuff[2] = ucEnBuf[9];
//            pCheckBuff[3] = ucEnBuf[10];
//            pCheckBuff[4] = ucEnBuf[5];
//            pCheckBuff[5] = ucEnBuf[6];
//            pCheckBuff[6] = ucEnBuf[15];
//            pCheckBuff[7] = ucEnBuf[16];
//        }
//		else {
//		    // ERROR 未知类型不能处理
//			ErrorMessage("GetCrcCheckField type error:%d.\n", type);
//			bResult = FALSE;
//		}
//    }
//    else {
//        // 旧版本，不用处理
//        pCrcBuff[0] = DRCOM_DIAL_EXT_PROTO_CRC_INIT;
//        pCrcBuff[1] = 126;
//        bResult = FALSE;
//    }
    return bResult;
}

int DrAuthStatus::check_ip(unsigned long dwIPAddress) {
    if (dwIPAddress == INADDR_NONE
            || dwIPAddress == INADDR_ANY
            || dwIPAddress == INADDR_LOOPBACK) {
        return 0;
    }
    return 1;
}
int DrAuthStatus::CheckAuthConfig(const char* szConfig)
{
    int res = 0;
    res =  (NULL != strstr(m_AuthStatus.StringParams, szConfig));
    //DebugMessage("%s  check %s return %d", as.StringParams, szConfig, res);
#if defined(LOCALE_CODE_CHECK_PROXY)
    if (0 == res)
    {
        res = StatusFromConfigRule(szConfig);
    }
#endif
    return res;
}
//unsigned long DrAuthStatus::get_sysuptime()
//{
////    struct sysinfo s_info;
////    sysinfo(&s_info);
////    return s_info.uptime;
//	return 1;
//}

void DrAuthStatus::SetServerIP(unsigned long ip) {
    if (check_ip(ip)) {
    	m_AuthStatus.DrcomAddr.sin_addr.s_addr = ip;
    }
    else {
    	// 错误的IP地址
    }
}
int DrAuthStatus::GetHostInfo(struct  _tagHostInfo* pHostInfo, int bShow) {
    int res = DRCOM_SUC;
    unsigned int uiAuthHostIPMatched = 0;
    static bool bFrist = true;

    if (bFrist) {
        m_AuthStatus.AuthHostIPMatched = 0;
        bFrist = false;
    }

    Unix_GetHostInfo(pHostInfo, bShow, &uiAuthHostIPMatched);

    m_AuthStatus.AuthHostIPMatched = uiAuthHostIPMatched;

#ifdef STRESS_TEST_CLIENT
    {
        int vmac = string_to_ip(g_szTestClientIP);
        memcpy(&as.AuthHostMacAddress[2], (char*) &vmac, 4);
    }
#endif
    strncpy(pHostInfo->OSVersion.Hash, m_AuthStatus.AuthModuleFileHash, sizeof(pHostInfo->OSVersion.Hash) - 1);
    return res;
}

void DrAuthStatus::SaveAuthServerIP(unsigned int svrip) {
	m_AuthStatus.AuthServerIntIP = svrip;
	m_drcfg.DrcomAuthServersIP[0] = m_AuthStatus.AuthServerIntIP;

	if (m_AuthStatus.AuthServerIntIP) {
		SetServerIP(m_AuthStatus.AuthServerIntIP);
	}
	return;
}

void DrAuthStatus::Unix_GetHostInfo(struct _tagHostInfo* pHostInfo, int bShow, unsigned int* uiAuthHostIPMatched) {
	// it is complicated
}

unsigned int DrAuthStatus::GetUserStatus(unsigned int status) {
    unsigned int res_status = 0;

    if (status == 0)
    {
        res_status = m_us.Status;
    }

    if (status && (m_us.Status & status))
    {
        res_status = m_us.Status&status;
    }

    //DebugMessage("GetUserStatus %08X us.Status %08X return  %08X",status, us.Status, res_status);
    return res_status;
}

void DrAuthStatus::SetUserStatus(unsigned int status)
{
    if (ST_ALL_LOGOUT == status || ST_ALL_LOGIN == status)
    {
    	m_us.Status = status;
        return;
    }

    //if (GetStatusMask(status) == status)
    //{
    //    us.Status = us.Status & status;
    //    return;
    //}
    {
        int tmpstatus = status;
        int oldstatus = m_us.Status;
        unsigned char* ptrStatus = (unsigned char*)&m_us.Status;
        unsigned char* ptrSetStatus = (unsigned char*)&tmpstatus;
        int i = 0;

        for (i = 0; i < sizeof(unsigned int); i++)
        {
            if (ptrSetStatus[i] && ptrSetStatus[i] != 0xFF)
            {
                if (ptrSetStatus[i] == 0x0F || ptrSetStatus[i] == 0xF0)
                {
                    ptrStatus[i] = ptrStatus[i] & ptrSetStatus[i];
                }
                else if (ptrSetStatus[i] == 0x08)
                {
                    ptrStatus[i] = (ptrStatus[i] & 0xF0) | 0x08;
                }
                else if (ptrSetStatus[i] == 0x80)
                {
                    ptrStatus[i] = (ptrStatus[i] & 0x0F) | 0x80;
                }
                else if (ptrSetStatus[i] & 0x70)
                {
                    ptrStatus[i] = (ptrStatus[i] & 0x0F) | ptrSetStatus[i];
                }
                else if (ptrSetStatus[i] & 0x07)
                {
                    ptrStatus[i] = (ptrStatus[i] & 0xF0) | ptrSetStatus[i];
                }
            }
        }

//        DebugMessage("SetUserStatus us.Status %08X add %08X return %08X", oldstatus, status,us.Status);
    }
}
void DrAuthStatus::ClearLastRealTimeOnlineStatus() {
    memset(&m_AuthStatus.onlineInfo, 0, sizeof(struct ExtClientPingInfo));
    m_us.UsedFlux = 0;
    m_us.UsedTime = 0;
}
unsigned int string_to_ip(const char* string_ip)
{
    unsigned int dwIPAddress = inet_addr(string_ip);

    if (dwIPAddress != INADDR_NONE &&
            dwIPAddress != INADDR_ANY)
    {
        return dwIPAddress;
    }

    return dwIPAddress;
}
int DrAuthStatus::DrCOMLogout(EXECUTOR_TYPE excutor)
{
    int res = DRCOM_SUC;
//    DebugMessage("NEW_IS_CONFIG_PROTO_DRCOM_PPPOE_PLUGIN=%d GetUserStatus(ST_DRCOM_PPPOE_NOT_LOGOUT)=%d excutor=%d",
//                 NEW_IS_CONFIG_PROTO_DRCOM_PPPOE_PLUGIN , GetUserStatus(ST_DRCOM_PPPOE_NOT_LOGOUT), excutor);

    if (GetUserStatus(ST_DRCOM_LOGIN))
    {
    	m_AuthStatus.DrcomAuthTimeout = DRCOM_AUTH_LOGIN_TIMEOUT;
    	m_AuthStatus.DrcomLastRecvTime = dr_tick_count();
    	m_AuthStatus.LogoutExecutor = excutor;
    	m_AuthStatus.DrcomAuthRetryTimes = 0;

        {
#ifdef WEB_IPV6_AUTH

            if (NEW_IS_CONFIG_PROTO_DRCOM_IPV6)
            {
                DrWebIPv6Logout();
            }

#endif
            res = DrcomAuthSendLogoutChallenge();
        }
    }

#ifdef DRCOM_DIAL_PLUGIN
    else if ((IsUseDrcomDial() == DRCOM_SUC && GetUserStatus(ST_DRCOM_DIAL_NOT_DISCONNECTED))
             || (NEW_IS_CONFIG_PROTO_DRCOM_PPPOE_PLUGIN
                 && (GetUserStatus(ST_DRCOM_PPPOE_NOT_LOGOUT) || GetUserStatus(ST_DRCOM_DIAL_NOT_DISCONNECTED))
                )
            )
    {
    	m_AuthStatus.DrcomAuthTimeout = DRCOM_AUTH_LOGIN_TIMEOUT * 9999;
    	m_AuthStatus.DrcomLastRecvTime = dr_tick_count();
    	m_AuthStatus.LogoutExecutor = excutor;
    	m_AuthStatus.DrcomAuthRetryTimes = 0;
        return DisconnectDrDialConnection(0);
    }

#endif
    else if (m_AuthStatus.AuthStep > AUTH_STEP_NONE)
    {
//        DebugMessage("ERR:Can not logout, User Status:%08X   Auth Step:%d\n", GetUserStatus(0), as.AuthStep);
#ifdef DRCOM_DIAL_PLUGIN

        if (IsUseDrcomDial() == DRCOM_SUC
                && GetUserStatus(ST_DRCOM_DIAL_NOT_DISCONNECTED))
        {
        	m_AuthStatus.DrcomAuthTimeout = DRCOM_AUTH_LOGIN_TIMEOUT * 9999;
        	m_AuthStatus.DrcomLastRecvTime = dr_tick_count();
        	m_AuthStatus.LogoutExecutor = excutor;
        	m_AuthStatus.DrcomAuthRetryTimes = 0;
            return DisconnectDrDialConnection(0);
        }

#endif
    }
    else
    {
//        DebugMessage("User Status:%08X   Auth Step:%d\n", GetUserStatus(0), as.AuthStep);
#ifdef DRCOM_DIAL_PLUGIN
        DebugMessage("GetUserStatus(0)=%08X as.AuthPPPoEStep=%d ......\n", GetUserStatus(0), as.AuthPPPoEStep);

        if (IsUseDrcomDial() == DRCOM_SUC
                && GetUserStatus(ST_DRCOM_DIAL_NOT_DISCONNECTED))
        {
        	m_AuthStatus.DrcomAuthTimeout = DRCOM_AUTH_LOGIN_TIMEOUT * 9999;
        	m_AuthStatus.DrcomLastRecvTime = dr_tick_count();
        	m_AuthStatus.LogoutExecutor = excutor;
        	m_AuthStatus.DrcomAuthRetryTimes = 0;
            return DisconnectDrDialConnection(0);
        }

#endif
//        SendLogoutStatus(0);
    }

    return res;
}

// #################################################################################
/*
 * 认证过程
 */


int DrAuthStatus::DrcomAuthSendLogoutChallenge()
{
    int res = DRCOM_SUC;
    struct  _tagChallenge logout_cmd;
    time_t seconds = time(NULL);
    unsigned char ClientCurrVer = m_cDRCOM_CLIENT_KERNEL_VER;
#ifdef SPC_DRCOM_CLIENT_VER
    ClientCurrVer = SPC_DRCOM_CLIENT_VER_BASE + ClientCurrVer % (255 - SPC_DRCOM_CLIENT_VER_BASE);
#endif
    DrcomAuthSendClientPing();
//    sp_msleep(500);
    bzero(&logout_cmd, sizeof(struct _tagChallenge));
    m_AuthStatus.ChallengeID++;
    m_AuthStatus.DrcomLastRecvTime = dr_tick_count();
    memcpy(&m_AuthStatus.ChallengeRandomID, &seconds, sizeof(unsigned short));
    logout_cmd.code = AUTH_CODE_CHALLENGE;
    logout_cmd.ChallengeID = m_AuthStatus.ChallengeID;
    logout_cmd.ChallengeRandomID = m_AuthStatus.ChallengeRandomID;
    logout_cmd.ClientVerno = ClientCurrVer;
    m_AuthStatus.AuthStep = DRCOM_AUTH_STEP_LOGOUT_SEND_CHALLENGE;
    SetUserStatus(ST_DRCOM_LOGOUTING);
    res = SendAuthCmd((char *)&logout_cmd, sizeof(struct _tagChallenge));
    return res;
}
int DrAuthStatus::DrcomAuthSendClientPing()
{
//    DebugMessage("DrcomAuthSendClientPing() as.EncryptionMode: %d", as.EncryptionMode);
    if (m_AuthStatus.EncryptionMode == 0) {
        // 2步心跳（旧版本）
        int res = DRCOM_SUC;
        struct _tagHostPingServer DrcomPing;
        time_t seconds = time(NULL);
        bzero(&DrcomPing, sizeof(struct _tagHostPingServer));
        DrcomPing.Code = 0xFF;
        memcpy(&DrcomPing.DogData, &m_AuthStatus.AuthDogData, sizeof(struct _tagDrcomDogData));
        DrcomPing.Milliseconds = (unsigned short)seconds;
        memcpy(DrcomPing.checkcode.oldcheck.KeepAliveMD5, m_AuthStatus.KeepAliveMD5, MD5_LEN);
        DrcomPing.InternetAccessControl = m_AuthStatus.InternetAccessControl;
        /*
        AntiProxyModuleDllSendPacket.serverip=as.DrcomAddr.sin_addr.s_addr;
        AntiProxyModuleDllSendPacket.nDataLen=sizeof(struct _tagHostPingServer);
        memcpy(AntiProxyModuleDllSendPacket.SendDatabuff,&DrcomPing,sizeof(struct _tagHostPingServer));
        AntiProxyModule_Call(DRCOMDLL_SEND_PACKET);
        //*/
//        DebugMessage("Sending Client ping...... as.InternetAccessControl=%d", as.InternetAccessControl);
        m_nClientPingNotResponseNum++;

        if (DRCOM_AUTH_LOCALNET_INERNET == m_AuthStatus.LocalNetAuthMode)
        {
            return SendAuthCmd((char *)&DrcomPing, sizeof(struct _tagHostPingServer));
        }
        else
        {
            return SendAuthCmd((char *)&DrcomPing, sizeof(struct _tagHostPingServer) - sizeof(DrcomPing.InternetAccessControl));
        }

        return res;
    }
    else {
        // 4步心跳（新版本）
        return DrcomAuthSendChallenge();
    }
}
/// fgx 2013-05-10
// 增加发送挑战值到2166，心跳1
int DrAuthStatus::DrcomAuthSendChallenge()
{
    if (m_AuthStatus.AuthStep >= DRCOM_AUTH_STEP_LOGIN_RECV_RESUALT)
    {
        struct _tagDrcom2133Challenge drcom_2133_challenge;
        bzero(&drcom_2133_challenge, sizeof(struct _tagDrcom2133Challenge));
        drcom_2133_challenge.code = AUTH_CODE_SVR_RET;
        drcom_2133_challenge.id = m_AuthStatus.ChallengeID++;
        drcom_2133_challenge.length = sizeof(struct _tagDrcom2133Challenge);
        drcom_2133_challenge.type = DRCOM_2133_CHALLENGE_REQ;
    //    as.DrcomAuthTimeout = DRCOM_AUTH_PPPOE_SEND_TIMEOUT;
        m_AuthStatus.AuthStep = DRCOM_AUTH_STEP_ONLINE_SEND_CHALLENGE ;
//        DebugMessage("Send Find Drcom Server CMD.\n");
        return SendAuthCmd((char *)&drcom_2133_challenge, sizeof(struct _tagDrcom2133Challenge));
    }
    return DRCOM_ERR;
}
