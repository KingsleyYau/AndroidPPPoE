/*
 * DrStepDef.h
 *
 *  Created on: 2014年2月22日
 *      Author: kingsley
 */

#ifndef DRAUTHDEF_H_
#define DRAUTHDEF_H_

#include "DRCOMAUTHSVR_VERSION.H"
#include "drsvr_cfg_struct.h"
#include "DrppoeProtoDef.h"

#include "../common/DrSocket.h"

//使用GetTickCount函数，修改时间对计时不影响
#define get_sysuptime() 1

#ifdef _WIN32
#define dr_tick_count_t DWORD
#define dr_tick_count   GetTickCount
#define dr_time_diff(start, end)  (end>=start ? end-start : (dr_tick_count_t)(-1)-start+end)
#endif

#ifndef _WIN32
#define DR_CLOCKS_PER_SEC 1
#ifdef MACOS
#define dr_tick_count_t time_t
#define dr_tick_count() (time(NULL)*DR_CLOCKS_PER_SEC)
#define dr_time_diff(start, end)  (end - start)
#else
#define dr_tick_count_t time_t
#define dr_tick_count() (get_sysuptime()*DR_CLOCKS_PER_SEC)
#define dr_time_diff(start, end)  (end - start)
#endif

#else
#define DR_CLOCKS_PER_SEC CLOCKS_PER_SEC
#endif

#define MAX_WAIT_TIMEOUT_VALUE (60*60*24*3)

/// fgx 2013-10-16
typedef struct _tagClientVerification
{
    long    client_version;     // 客户端告知拨号模块调用的版本号(10000 <= client_version存在该参数)
    char    rulefile_md5[64];   // 规则文件密钥，若为空则表示使用默认密钥(20000 <= client_version存在该参数)
}CLIENT_VERIFICATION;

typedef enum eEXECUTOR_TYPE
{
    EXECUTOR_UI_PROG = 1,
    EXECUTOR_AUTHSVR = 2,
    EXECUTOR_SYSTEM_SVR_MGR = 3,
    EXECUTOR_AUTHSVR_FORCE_LOGOUT = 4,

    EXECUTOR_8021X_LOCAL_NET = 20,
    EXECUTOR_8021X_INTERNET = 21,
    EXECUTOR_8021X_LOCAL_NET_2_INTERNET = 22,
    EXECUTOR_8021X_INTERNET_2_LOCAL_NET = 23,
    EXECUTOR_AUTHSVR_ERR = 100,
    EXECUTOR_AUTHSVR_GWERR_CODE = EXECUTOR_AUTHSVR_ERR + 1,
    EXECUTOR_AUTHSVR_DRCOMDLL_ERR_CODE = EXECUTOR_AUTHSVR_ERR + 1,
} EXECUTOR_TYPE;

typedef enum eAUTH_STEP
{
    AUTH_STEP_NONE = 0,
    AUTH_8021X_BASE_STEP = 100,
    AUTH_8021X_STATUS_LOGOUT = AUTH_8021X_BASE_STEP,
    AUTH_8021X_STEP_LOGIN_SEND_CHALLENGE = AUTH_8021X_BASE_STEP + 1,
    AUTH_8021X_STEP_LOGIN_SENDING_NAME = AUTH_8021X_BASE_STEP + 2,
    AUTH_8021X_STEP_LOGIN_SENDED_NAME = AUTH_8021X_BASE_STEP + 3,
    AUTH_8021X_STEP_LOGIN_SENDING_PASS = AUTH_8021X_BASE_STEP + 4,
    AUTH_8021X_STEP_LOGIN_SENDED_PASS = AUTH_8021X_BASE_STEP + 5,
    AUTH_8021X_STEP_LOGIN_SUCCESS,
    AUTH_8021X_STEP_LOGOUT_SEND_CHALLENGE = AUTH_8021X_BASE_STEP + 11,
    AUTH_8021X_STEP_LOGOUT_SEND_NAME = AUTH_8021X_BASE_STEP + 12,
    AUTH_8021X_STEP_LOGOUT_SUCCESS,


    DRCOM_AUTH_BASE_STEP = 200,
    DRCOM_AUTH_STATUS_LOGOUT = DRCOM_AUTH_BASE_STEP,
    DRCOM_AUTH_STEP_LOGIN_SEND_CHALLENGE = DRCOM_AUTH_BASE_STEP + 1,
    DRCOM_AUTH_STEP_LOGIN_RECV_CHALLENGE = DRCOM_AUTH_BASE_STEP + 2,
    DRCOM_AUTH_STEP_LOGIN_SEND_NAME_AND_PASS = DRCOM_AUTH_BASE_STEP + 3,
    DRCOM_AUTH_STEP_LOGIN_RECV_RESUALT = DRCOM_AUTH_BASE_STEP + 4,
    DRCOM_AUTH_STEP_LOGIN_SUCCESS = DRCOM_AUTH_BASE_STEP + 20,
    DRCOM_AUTH_STEP_LOGIN_ERROR = DRCOM_AUTH_BASE_STEP + 25,

    /// fgx 2013-05-13 增加心跳检测步骤
    DRCOM_AUTH_STEP_ONLINE_SEND_CHALLENGE = DRCOM_AUTH_BASE_STEP + 10,      // 发送获取挑战值
    DRCOM_AUTH_STEP_ONLINE_RECV_CHALLENGE = DRCOM_AUTH_BASE_STEP + 11,      // 接收挑战值
    DRCOM_AUTH_STEP_ONLINE_SEND_CHECKCODE = DRCOM_AUTH_BASE_STEP + 12,      // 发送验证包
    DRCOM_AUTH_STEP_ONLINE_RECV_CHECKRESULT = DRCOM_AUTH_BASE_STEP + 13,    // 接收验证结果

    DRCOM_AUTH_STEP_LOGOUT_SEND_CHALLENGE = DRCOM_AUTH_BASE_STEP + 31,
    DRCOM_AUTH_STEP_LOGOUT_RECV_CHALLENGE = DRCOM_AUTH_BASE_STEP + 32,
    DRCOM_AUTH_STEP_LOGOUT_SEND_NAME_AND_PASS = DRCOM_AUTH_BASE_STEP + 33,
    DRCOM_AUTH_STEP_LOGOUT_RECV_RESUALT = DRCOM_AUTH_BASE_STEP + 34,
    DRCOM_AUTH_STEP_LOGOUT_SUCCESS = DRCOM_AUTH_BASE_STEP + 50,

    DRCOM_MODIFY_PWD_BASE = DRCOM_AUTH_BASE_STEP + 100,
    DRCOM_AUTH_STEP_MODIFY_PWD_SEND_CHALLENGE = DRCOM_MODIFY_PWD_BASE + 1,
    DRCOM_AUTH_STEP_MODIFY_PWD_RECV_CHALLENGE = DRCOM_MODIFY_PWD_BASE + 2,
    DRCOM_AUTH_STEP_MODIFY_PWD_SEND_NAME_AND_PASS = DRCOM_MODIFY_PWD_BASE + 3,
    DRCOM_AUTH_STEP_MODIFY_PWD_RECV_RESUALT = DRCOM_MODIFY_PWD_BASE + 4,
    DRCOM_AUTH_STEP_MODIFY_PWD_SUCCESS = DRCOM_MODIFY_PWD_BASE + 5,
    DRCOM_AUTH_STEP_MODIFY_PWD_ERROR = DRCOM_MODIFY_PWD_BASE + 6,

    DRCOM_DIAL_PLUGIN_BASE  = DRCOM_AUTH_BASE_STEP + 200,
    DRCOM_AUTH_STEP_DIAL_START_CONNECT = DRCOM_DIAL_PLUGIN_BASE + 1,
    DRCOM_AUTH_STEP_DIAL_DIALING = DRCOM_DIAL_PLUGIN_BASE + 2,
    DRCOM_AUTH_STEP_DIAL_CONNECTED = DRCOM_DIAL_PLUGIN_BASE + 3,

    DRCOM_AUTH_STEP_DIAL_START_HUNGUP = DRCOM_DIAL_PLUGIN_BASE + 21,
    DRCOM_AUTH_STEP_DIAL_HUNGUPING = DRCOM_DIAL_PLUGIN_BASE + 22,
    DRCOM_AUTH_STEP_DIAL_DISCONNECTED = DRCOM_DIAL_PLUGIN_BASE + 23,


    DRCOM_PPPOE_PLUGIN_BASE = DRCOM_AUTH_BASE_STEP + 300,
    DRCOM_AUTH_STEP_PPPOE_NONE = DRCOM_PPPOE_PLUGIN_BASE,
    DRCOM_AUTH_STEP_PPPOE_SEND_CHALLENFE = DRCOM_PPPOE_PLUGIN_BASE + 1,
    DRCOM_AUTH_STEP_PPPOE_RECV_CHALLENFE = DRCOM_PPPOE_PLUGIN_BASE + 2,
    DRCOM_AUTH_STEP_PPPOE_SEND_LOGIN_PACKET = DRCOM_PPPOE_PLUGIN_BASE + 3,
    DRCOM_AUTH_STEP_PPPOE_RECV_LOGIN_RESULT = DRCOM_PPPOE_PLUGIN_BASE + 4,
    DRCOM_AUTH_STEP_PPPOE_LOGIN_SUCCESS = DRCOM_PPPOE_PLUGIN_BASE + 5,
    DRCOM_AUTH_STEP_PPPOE_LOGIN_ERROR = DRCOM_PPPOE_PLUGIN_BASE + 6,

    DRCOM_WEB_IPV6_BASE = DRCOM_AUTH_BASE_STEP + 400,
    DRCOM_WEB_IPV6_STEP_NONE = DRCOM_WEB_IPV6_BASE,
    DRCOM_WEB_IPV6_STEP_CONNECT_LOGIN_PAGE = DRCOM_WEB_IPV6_BASE + 1,
    DRCOM_WEB_IPV6_STEP_REQUEST_LOGIN_PAGE = DRCOM_WEB_IPV6_BASE + 2,
    DRCOM_WEB_IPV6_STEP_RECV_LOGIN_PAGE = DRCOM_WEB_IPV6_BASE + 3,
    DRCOM_WEB_IPV6_STEP_HANDLE_LOGIN_PAGE = DRCOM_WEB_IPV6_BASE + 4,
    DRCOM_WEB_IPV6_STEP_LOGIN_SUCCESS = DRCOM_WEB_IPV6_BASE + 5,
    DRCOM_WEB_IPV6_STEP_LOGIN_ERROR = DRCOM_WEB_IPV6_BASE + 6,


    DRCOM_WEB_IPV6_STEP_CONNECT_LOGOUT_PAGE = DRCOM_WEB_IPV6_BASE + 21,
    DRCOM_WEB_IPV6_STEP_REQUEST_LOGOUT_PAGE = DRCOM_WEB_IPV6_BASE + 22,
    DRCOM_WEB_IPV6_STEP_RECV_LOGOUT_PAGE = DRCOM_WEB_IPV6_BASE + 23,
    DRCOM_WEB_IPV6_STEP_HANDLE_LOGOUT_PAGE = DRCOM_WEB_IPV6_BASE + 24,
    DRCOM_WEB_IPV6_STEP_LOGOUT_SUCCESS = DRCOM_WEB_IPV6_BASE + 25,
    DRCOM_WEB_IPV6_STEP_LOGOUT_ERROR = DRCOM_WEB_IPV6_BASE + 26,


    DRCOM_8021X_PLUGIN_BASE = DRCOM_AUTH_BASE_STEP + 500,
    DRCOM_AUTH_STEP_8021X_NONE = DRCOM_8021X_PLUGIN_BASE,
    DRCOM_AUTH_STEP_8021X_SEND_CHALLENFE = DRCOM_8021X_PLUGIN_BASE + 1,
    DRCOM_AUTH_STEP_8021X_RECV_CHALLENFE = DRCOM_8021X_PLUGIN_BASE + 2,
    DRCOM_AUTH_STEP_8021X_SEND_LOGIN_PACKET = DRCOM_8021X_PLUGIN_BASE + 3,
    DRCOM_AUTH_STEP_8021X_RECV_LOGIN_RESULT = DRCOM_8021X_PLUGIN_BASE + 4,
    DRCOM_AUTH_STEP_8021X_LOGIN_SUCCESS = DRCOM_8021X_PLUGIN_BASE + 5,
    DRCOM_AUTH_STEP_8021X_LOGIN_ERROR = DRCOM_8021X_PLUGIN_BASE + 6,


    DRCOM_WEB_AUTH_BASE = DRCOM_AUTH_BASE_STEP + 450,
    DRCOM_WEB_AUTH_STEP_NONE = DRCOM_WEB_AUTH_BASE,
    DRCOM_WEB_AUTH_STEP_CONNECT_LOGIN_PAGE = DRCOM_WEB_AUTH_BASE + 1,
    DRCOM_WEB_AUTH_STEP_REQUEST_LOGIN_PAGE = DRCOM_WEB_AUTH_BASE + 2,
    DRCOM_WEB_AUTH_STEP_RECV_LOGIN_PAGE = DRCOM_WEB_AUTH_BASE + 3,
    DRCOM_WEB_AUTH_STEP_HANDLE_LOGIN_PAGE = DRCOM_WEB_AUTH_BASE + 4,
    DRCOM_WEB_AUTH_STEP_LOGIN_SUCCESS = DRCOM_WEB_AUTH_BASE + 5,
    DRCOM_WEB_AUTH_STEP_LOGIN_ERROR = DRCOM_WEB_AUTH_BASE + 6,

    DRCOM_WEB_AUTH_STEP_CONNECT_LOGOUT_PAGE = DRCOM_WEB_AUTH_BASE + 21,
    DRCOM_WEB_AUTH_STEP_REQUEST_LOGOUT_PAGE = DRCOM_WEB_AUTH_BASE + 22,
    DRCOM_WEB_AUTH_STEP_RECV_LOGOUT_PAGE = DRCOM_WEB_AUTH_BASE + 23,
    DRCOM_WEB_AUTH_STEP_HANDLE_LOGOUT_PAGE = DRCOM_WEB_AUTH_BASE + 24,
    DRCOM_WEB_AUTH_STEP_LOGOUT_SUCCESS = DRCOM_WEB_AUTH_BASE + 25,
    DRCOM_WEB_AUTH_STEP_LOGOUT_ERROR = DRCOM_WEB_AUTH_BASE + 26,

    DRCOM_AUTH_STATUS_LOGIN = DRCOM_AUTH_STEP_LOGIN_SUCCESS + 1,
    AUTH_STEP_ALL_DONE = 0x0000FFFF,

    CANCLE_DO_AUTH = 0xFFFFFFFF,
} AUTH_STEP;

typedef enum eUSER_STATUS_TYPE
{
    ST_ALL_LOGOUT =         0x00000000,
    ST_ALL_LOGIN =          0x88888888,
    ST_SET_ALL_LOGOUT =     0xFFFFFFFF,


    ST_8021X_LOGIN =        0x00000008,
    ST_8021X_NOT_LOGOUT =   0x0000000F,
    ST_8021X_LOGOUT =      ~ST_8021X_NOT_LOGOUT,
    ST_8021X_LOGINING =     0x00000001,
    ST_8021X_LOGOUTING =    0x00000002,

    ST_DRCOM_LOGIN =        0x00000080,
    ST_DRCOM_NOT_LOGOUT =   0x000000F0,
    ST_DRCOM_LOGOUT =      ~ST_DRCOM_NOT_LOGOUT,
    ST_DRCOM_LOGINING =     0x00000010,
    ST_DRCOM_LOGOUTING =    0x00000020,
    ST_DRCOM_MODIFY_PWDING = 0x00000040,

    ST_DRCOM_PPPOE_LOGIN =          0x00000800,
    ST_DRCOM_PPPOE_NOT_LOGOUT =     0x00000F00,
    ST_DRCOM_PPPOE_LOGOUT =        ~ST_DRCOM_PPPOE_NOT_LOGOUT,
    ST_DRCOM_PPPOE_LOGINING =       0x00000100,
    ST_DRCOM_PPPOE_ONLINENING =      0x00000200,


    ST_DRCOM_DIAL_CONNECTED =           0x00008000,
    ST_DRCOM_DIAL_NOT_DISCONNECTED =    0x0000F000,
    ST_DRCOM_DIAL_DISCONNECTED =       ~ST_DRCOM_DIAL_NOT_DISCONNECTED,
    ST_DRCOM_DIAL_DIALING =             0x00001000,
    ST_DRCOM_DIAL_HUNGUPING =           0x00002000,
    ST_DRCOM_DIAL_CONNECTED_HTTP =      0x00004000,

    ST_WEB_IPV6_LOGIN =        0x00080000,
    ST_WEB_IPV6_NOT_LOGOUT =   0x000F0000,
    ST_WEB_IPV6_LOGOUT =      ~ST_WEB_IPV6_NOT_LOGOUT,
    ST_WEB_IPV6_LOGINING =     0x00010000,
    ST_WEB_IPV6_LOGOUTING =    0x00020000,
    ST_WEB_IPV6_NOT_NEED_LOGIN = 0x00040000,


    ST_DRCOM_8021X_LOGIN =          0x00800000,
    ST_DRCOM_8021X_NOT_LOGOUT =     0x00F00000,
    ST_DRCOM_8021X_LOGOUT =        ~ST_DRCOM_8021X_NOT_LOGOUT,
    ST_DRCOM_8021X_LOGINING =       0x00100000,
    ST_DRCOM_8021X_ONLINENING =      0x00200000,

    ST_WEB_AUTH_LOGIN =        0x08000000,
    ST_WEB_AUTH_NOT_LOGOUT =   0x0F000000,
    ST_WEB_AUTH_LOGOUT =      ~ST_WEB_AUTH_NOT_LOGOUT,
    ST_WEB_AUTH_LOGINING =     0x01000000,
    ST_WEB_AUTH_LOGOUTING =    0x02000000,
    ST_WEB_AUTH_NOT_NEED_LOGIN = 0x04000000,

} USER_STATUS_TYPE;

enum
{
    DRCOM_AUTH_LOGIN_TIMEOUT = 10,
    DRCOM_AUTH_PING_TIMEOUT = 60,
    DRCOM_AUTH_PING_INTERVAL = 20,
    DRCOM_AUTH_PING_MIN_INTERVAL = 5,
    DRCOM_AUTH_DEFAULT_RETRY_TIMES = 3,
    DRCOM_AUTH_PPPOE_SEND_TIMEOUT = 4,
    DRCOM_AUTH_PPPOE_ONLINE_INTERVAL = 15,
    DRCOM_AUTH_8021X_SEND_TIMEOUT = 5,
    DRCOM_AUTH_8021X_CHECK_AUTH_TYPE = 3,
    DRCOM_AUTH_DIRECT_INTERNET = 0,
    DRCOM_AUTH_LOCALNET_INERNET = 1,
    DRCOM_ONLINE_TIMEOUT_PING_COUNT = 5,
    DRCOM_AUTH_8021X_ONLINE_INTERVAL = DRCOM_AUTH_PING_INTERVAL,
    DRCOM_ANTI_PROXY_START_TIME = 10,
    DRCOM_ANTI_PROXY_CHECK_INTERVAL = 3*60,
    DRCOM_AUTH_SHOW_MESG_INTERVAL = 10,
    DRCOM_AUTH_RENEWIPADDR_RETRYTIMES = 10,

    DRCOM_MODULE_FILE_NAME_LEN = 512,
    DRCOM_MODULE_FILE_HASH_LEN = 64,
};

// PPPoE第二步返回的标志，决定第三步的加密方式
enum
{
    ENCRYPTION_MODE_NONE = 0,   // 旧版本，用旧方式生成校验码
    ENCRYPTION_MODE_MD5 = 1,    // 用MD5生成校验码
    ENCRYPTION_MODE_MD4 = 2,    // 用MD4生成校验码
    ENCRYPTION_MODE_SHA1 = 3,   // 用SHA1生成校验码
};

struct _tagUserStatus
{
    char Account[ACCOUNT_MAX_LEN];
    char Password[ACCOUNT_MAX_LEN];
    char Name[ACCOUNT_MAX_LEN];
    unsigned int Status;
    unsigned int UsedTime;
    unsigned int UsedFlux;
    unsigned int Balance;
    unsigned int OnlineSec;
    unsigned int LastOnlineSec;
    unsigned int PPPoEOnlineSec;
    unsigned int Local8021XOnlineSec;
    dr_tick_count_t LoginTime;
    char NewPassword[ACCOUNT_MAX_LEN];
    char AccountDot1X[ACCOUNT_MAX_LEN];
    char PasswordDot1X[ACCOUNT_MAX_LEN];

    char InputAccount[ACCOUNT_MAX_LEN];
    char InputPassword[ACCOUNT_MAX_LEN];
};

struct _tagHostIPList
{
    int num;
    unsigned int ip_list[HOST_MAX_IP_NUM];
};


struct _tagHostMacList
{
    int num;
    unsigned char mac_list[HOST_MAX_MAC_NUM][8];
};

struct _tagAuthStatus
{
    EXECUTOR_TYPE   LoginExecutor;
    EXECUTOR_TYPE   LogoutExecutor;
    struct _tagUserStatus*  pUserStatus;
    AUTH_STEP   AuthStep;
    AUTH_STEP   AuthIPv6Step;
    AUTH_STEP   AuthPPPoEStep;
    AUTH_STEP   AuthPPPoEPluginStep;
    AUTH_STEP   Auth8021XStep;
    AUTH_STEP   Auth8021XPluginStep;

    int DrcomSock;
    int DrcomOnlyRecvSock;
    struct sockaddr_in DrcomAddr;
    struct sockaddr_in AuthClientBindAddr;
    struct sockaddr_in AuthClientOnlyRecvBindAddr;
    struct sockaddr_in AuthServerAddr;

    int ServerIndex;
    unsigned int* DrcomAuthServerList;
    unsigned int AuthServerIntIP;
    unsigned int AuthServerExtIP;
    unsigned short  ClientDigest;
    unsigned short  RecvClientDigest;
    unsigned char   KeepAliveMD5[16];
    unsigned char   DigestMD5[16];
    unsigned char ChallengeID;
    unsigned char HostMacNum;
    unsigned short ChallengeRandomID;
    unsigned int ChallengeSeed[SEED_LEN];
    unsigned int EncryptionMode;        // fgx 2013-03-29 决定PPPoE第三步的加密方式，及DrCOM增加心跳步骤
    unsigned int PwdChallengeSeed[SEED_LEN];
    unsigned int NetSettingDhcpsServerIP;
    unsigned int AuthChallengeDhcpsServerIP;
    unsigned int AuthHostIP;
    unsigned int AuthOnlineHostIP;
    unsigned int LocalCodeGetBySocketAuthHostIP_First;
    unsigned int LocalCodeGetBySocketAuthHostIP;
    unsigned int AuthHostPPPIP;
    unsigned short AuthHostPort;
    unsigned int AuthHostGWIP;
    unsigned int AuthHostMask;
    unsigned char AuthHostMacAddress[MAX_MAC_LEN];
    unsigned char AuthServerIntMacAddress[MAX_MAC_LEN];
    unsigned int AuthHostAdapterIndex;
    unsigned short AuthTypeSele;
    unsigned short AuthDhcpOption;
    unsigned short DeviceVer;
    unsigned short DeviceKernelVer;
    unsigned int ActiveAdaptersNum;
    unsigned int LinuxForceDhcpsServerIP;

    struct _tagHostInfo HostInfo;
    struct _tagHostIPList IPInfo;
    struct _tagHostMacList MacInfo;
    unsigned int AuthHostIPMatched;
    struct _tagDrcomDogData AuthDogData;
    dr_tick_count_t DrcomLastRecvTime;
    unsigned int DrcomAuthTimeout;
    unsigned int DrcomAuthRetryTimes;
    dr_tick_count_t DrcomAuthLastTime;
    unsigned int Dr8021XAuthTimeout;
    unsigned int Dr8021XAuthRetryTimes;
    struct _tagNewVersionInfo NewVerInfo;
    unsigned char ControlCheckStatus;
    unsigned char ControlCheckStatusBak;
    unsigned char unused001;
    unsigned short TryLoginMinInterval;
    unsigned int HandledDrcomDialExtProtoNetInfoIndex;
    struct _tagDrcomDialExtProtoNetWorkInfo DrcomDialExtProtoNetInfo[MAX_DRCOM_DIAL_EXT_PROTO_NET_NUM];
#define Drcom8021XExtProtoNetInfo DrcomDialExtProtoNetInfo
    unsigned int DrcomDialExtProtoNetInfoIFIndex[MAX_DRCOM_DIAL_EXT_PROTO_NET_NUM];
    unsigned int DrcomDialExtProtoAuthLastCRC;
    unsigned int DrcomDialExtProtoActiveCRC;
    unsigned int IsDrcomDialExtProtoProxyCheckActive;
    unsigned int IsDrcomDialConnectionFirstActive;
    unsigned int DrcomDialExtProtoURLFirstMark;
    unsigned int AntiProxyResault;
    unsigned int DrcomPPPoEAuthRetry966Kern;


    // 802.1x
    char AdapterDescrBy8021X[MAX_ADAPTER_NAME_LEN];
    dr_tick_count_t Last8021XRecvTime;
    unsigned int nLoginType;
    unsigned int nInputLoginType;
    char szLoginType[8];

    unsigned int CancleAuthFlag;
    unsigned int InternetAccessControl;
    int SystemInfoLoged;
    unsigned int DialServerIP;
    unsigned int DrcomDialEntryCreated;
    int DialAuthInfoCapThreadInitOver;
    struct ExtClientPingInfo onlineInfo;
    unsigned int MustUse8021xAuth;
    unsigned int nServerRequireSendDrcomPingInterval;
    unsigned int nServerRequireSendDrcomPingTimeout;
    unsigned int NotBindAuthPort;
    int onlineing_logout_flag;
    char StringParams[MAX_STRING_PARAMS_LEN];
    unsigned int LocalNetAuthMode;
    unsigned int Drcom8021XExtProtoActiveCRC;
    unsigned int IsDrcom8021XExtProtoProxyCheckActive;
    unsigned int VpnDialFound;
    unsigned int OtherDhcpServerIP;
    unsigned int DisableAutoFindAuthSvr;
    unsigned int ping_interval_weight;
    char AuthHostIPv6Address[64];
    char AuthSvrIPv6Address[64];
    unsigned int WebIPv6AuthRetryTimes;
    unsigned int WebAuthRetryTimes;
    char AuthModuleFile[DRCOM_MODULE_FILE_NAME_LEN];
    char AuthModuleFileHash[DRCOM_MODULE_FILE_HASH_LEN];
};



struct _tagUISocketStatus
{
    int sock_fd;
    struct sockaddr_in ui_client_addr;
    struct sockaddr_in bindaddr;
    struct sockaddr_in localhost_client_addr;
};


typedef struct _tagAuthConfig
{
    //auth type,tcpipdog ,drcomdll,802.1x,pppoe, hostname auth
    unsigned int AntiProxyTcpipDog: 1;
    unsigned int AntiProxyDriverDog: 1;
    unsigned int AntiProxyDrcomdllModule: 1;
    unsigned int AntiProxyPPPoE: 1;
    unsigned int AntiProxyNotUsed: 12;
    unsigned int Auth8021xMode: 1;
    unsigned int AuthPPPoEMode: 1;
    unsigned int AuthHostNameMode: 1;
    unsigned int AuthNotUsed: 1;
    unsigned int ConfigNotUsed: 12;
    // unsigned int AntiProxyTcpipDog:1;

} DRCOMAUTHCONFIG, *PDRCOMAUTHCONFIG;


#endif /* DRAUTHDEF_H_ */
