/*
 * DrppoeProtoDef.h
 *
 *  Created on: 2014年2月22日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef DRPPOEPROTODEF_H_
#define DRPPOEPROTODEF_H_

#pragma  pack(1) // 指定结构体对齐为1个byte

// ################################################################################################# //
enum ENUM_DEFINE
{
    ACCOUNT_MAX_LEN = 36,
    PASSWD_MAX_LEN = 16,
    USER_NAME_MIN_LEN = 2,
    HOST_MAX_IP_NUM = 4,
    HOST_MAX_MAC_NUM = 4,
    HOST_NAME_MAX_LEN = 32,
    MD5_LEN = 16,
    MAC_LEN = 6,
    MAX_MAC_LEN = 8,
    MAX_AUTH_DATA_LEN = 1600, //must great than mtu 1500
    SEED_LEN = 4,
#ifndef _WIN32
    LINUX_THREAD_TIME_EXT = 0,
    LINUX_THREAD_TIME_INC_EXT = 0,
#else
    LINUX_THREAD_TIME_EXT = 100,
    LINUX_THREAD_TIME_INC_EXT = 5,
#endif
    MAX_THREAD_EVENT_WAIT_TIME_MS = 100 + LINUX_THREAD_TIME_EXT,
    THREAD_EVENT_WAIT_INC_MS = 5 + LINUX_THREAD_TIME_INC_EXT, // ns

    MAX_THREAD_EVENT_WAIT_TIME_NS = MAX_THREAD_EVENT_WAIT_TIME_MS * (1000 * 1000), //ns
    THREAD_EVENT_WAIT_INC_NS = THREAD_EVENT_WAIT_INC_MS * (1000 * 1000), // ns

    MAX_ADAPTER_NAME_LEN = 256,
    MAX_STRING_PARAMS_LEN = 2048,
};
// ################################################################################################# //

// ################################################################################################# //
#define AUTH_CODE_CHALLENGE             0x01
#define AUTH_CODE_RETURN_CHALLENGE      0x02
#define AUTH_CODE_LOGIN                 0x03
#define AUTH_CODE_LOGIN_SUC             0x04
#define AUTH_CODE_LOGOUT_SUC            AUTH_CODE_LOGIN_SUC
#define AUTH_CODE_LOGIN_ERR             0x05
#define AUTH_CODE_LOGOUT                0x06
#define AUTH_CODE_SVR_RET               0x07
#define AUTH_CODE_SEND_NEW_PWD          0x09
#define AUTH_CODE_MSG                   0x4D
#define AUTH_CODE_PPPOE                 0x07
#define AUTH_CODE_8021X                 0x07


#define AUTH_TYPE_LOGIN                 0x00
#define AUTH_TYPE_LOGOUT                0x06

#define AUTH_CODE_TYPE_PPPOE_CHALLENGE  0x01
#define AUTH_CODE_TYPE_PPPOE_RET_CHALL  0x02
#define AUTH_CODE_TYPE_PPPOE_LOGIN      0x03
#define AUTH_CODE_TYPE_PPPOE_PING       0x06
#define AUTH_CODE_TYPE_PPPOE_VPN_TBL    0x04

#define AUTH_CODE_TYPE_8021X_CHALLENGE  0x01
#define AUTH_CODE_TYPE_8021X_RET_CHALL  0x02
#define AUTH_CODE_TYPE_8021X_LOGIN      0x03
#define AUTH_CODE_TYPE_8021X_PING       0x06
#define AUTH_CODE_TYPE_8021X_VPN_TBL    0x04


#define AUTH_ERR_CODE_CHECK_MAC         0x01
#define AUTH_ERR_CODE_SERVER_BUSY       0x02
#define AUTH_ERR_CODE_WRONG_PASS        0x03
#define AUTH_ERR_CODE_NOT_ENOUGH        0x04
#define AUTH_ERR_CODE_FREEZE_UP         0x05
#define AUTH_ERR_CODE_NOT_ON_THIS_IP    0x07
#define AUTH_ERR_CODE_NOT_ON_THIS_MAC   0x0B
#define AUTH_ERR_CODE_TOO_MUCH_IP       0x14
#define AUTH_ERR_CODE_UPDATE_CLIENT     0x15
#define AUTH_ERR_CODE_NOT_ON_THIS_IP_MAC    0x16
#define AUTH_ERR_CODE_MUST_USE_DHCP     0x17
#define AUTH_ERR_CODE_24                0x18
#define AUTH_ERR_CODE_25                0x19
#define AUTH_ERR_CODE_26                0x1A
#define AUTH_ERR_CODE_27                0x1B
#define AUTH_ERR_CODE_28                0x1C

#define MODIFY_PWD_ERROR_ACCOUNT_IN_USE 0x01
#define MODIFY_PWD_ERROR_ANTI_MODIFY    0x02


#define AUTH_MSG_CODE_NONE              0x00
#define AUTH_MSG_CHECK_WWW_ADDR         0x01
#define AUTH_MSG_CHECK_IP_ADDR          0x02
#define AUTH_MSG_CHECK_RESET_IP         0x03
#define AUTH_MSG_CHECK_ACCOUNT_DEL4     0x04
#define AUTH_MSG_CHECK_ACCOUNT_DEL5     0x05
#define AUTH_MSG_TODAY_NOT_USE          0x06
#define AUTH_MSG_THIS_TIME_NOT_USE      0x07
#define AUTH_MSG_CHECK_SEED_PORT        0x08
#define AUTH_MSG_NOT_ENOUGH             0x09
#define AUTH_MSG_8021X_LOGOUT           0x0A
#define AUTH_MSG_USER_GROUP_NOT_EXSIT   0x14
#define AUTH_MSG_NOT_LOGIN              0x15
#define AUTH_MSG_PASS_CHANGED           0x16

#define AUTH_MSG_CODE_23                0x17
#define AUTH_MSG_CODE_24                0x18
#define AUTH_MSG_CODE_25                0x19
#define AUTH_MSG_CODE_26                0x1A
#define AUTH_MSG_CODE_27                0x1B
#define AUTH_MSG_CODE_28                0x1C
#define AUTH_MSG_CODE_X88               0x88

#define AUTH_MSG_CODE_GW_UTP2_ERR       0x1D


#define AUTH_MSG_GROUP_TIME_OVER_CAN_NOT_USE    0x1F
#define AUTH_MSG_TIME_OVER_CAN_NOT_USE          0x21
#define AUTH_MSG_GROUP_FLUX_OVER_CAN_NOT_USE    0x23
#define AUTH_MSG_FLUX_OVER_CAN_NOT_USE          0x25

#define AUTH_MSG_GROUP_TIME_OVER        0x1E
#define AUTH_MSG_TIME_OVER              0x20
#define AUTH_MSG_GROUP_FLUX_OVER        0x22
#define AUTH_MSG_FLUX_OVER              0x24
#define AUTH_MSG_SERVER_ALIVE           0x26


#define AUTH_MSG_NONE_CHAR_1            0x31
#define AUTH_MSG_NONE_CHAR_2            0x32
#define AUTH_MSG_SYSTEM_MSG             0x38
#define AUTH_MSG_SYSTEM_RUN_MSG         0x3A


#define AUTH_CODE_LOGIN_EXT             0x02
#define INIT_CRC_VALUE                  0x11072601


#define NOT_LOGIN_MSG_MIN_INTERVAL 2*60

#define FLAG_HAS_DOG 0x01
#define FLAG_HAS_NOT_DOG 0x00

#define MAX_DATA_BUFF_SIZE 1024

#define DRCOM_DIAL_EXT_PROTO_PROXYACTV_INIT 0x80
#define DRCOM_8021X_EXT_PROTO_PROXYACTV_INIT 0x00

#define CLIENT_UI_CLASS_AD "drcom_2133_adclient"
#define CLIENT_UI_CLASS_V01 "DrSupplicant_2033_v01"
// ################################################################################################# //


struct  _tagOSVERSIONINFO
{
    unsigned int OSVersionInfoSize;
    unsigned int MajorVersion;
    unsigned int MinorVersion;
    unsigned int BuildNumber;
    unsigned int PlatformID;
    char ServicePack[128];
};

struct  _tagDrCOM_OSVERSIONINFO
{
    unsigned int dwOSVersionInfoSize;
    unsigned int dwMajorVersion;
    unsigned int dwMinorVersion;
    unsigned int dwBuildNumber;
    unsigned int dwPlatformID;
    char ServicePack[64];
    char Hash[64];
};

struct  _tagHostInfo
{
    char HostName[HOST_NAME_MAX_LEN];
    unsigned int DNSIP1;
    unsigned int DHCPServerIP;
    unsigned int DNSIP2;
    unsigned int WINSIP1;
    unsigned int WINSIP2;
//#ifndef _WIN32
    struct _tagDrCOM_OSVERSIONINFO OSVersion;
//#else
    //OSVERSIONINFO OSVersion;
//#endif
};

struct  _tagDrCOMHeader
{
    unsigned char AuthCode;
    unsigned char AuthType;
    unsigned char SubCode;
    unsigned char CmdLen;
};

struct _tagDrcomDogData
{
    unsigned int DrcomFlag;
    unsigned int DrcomServerIP;
    unsigned short DrcomServerPort;
    unsigned int HostIP;
    unsigned short HostPort;
};

struct _tagDogControlTableItem
{
    unsigned short function;
    unsigned short DestPort;
    unsigned int DestIP;
    unsigned int DestMask;
};

#define MAX_CONTROL_TABLE_NUM 80
#ifndef _WIN32
typedef unsigned long HWND;
#endif
typedef struct _tagDrcomDogParams
{
    char stbword[16];
    HWND hClientWnd;    // ishare_user.exe 窗口句柄
    int DebugMark;
    int ActiveControlTableNum;
    int MaxUdpLength;
    char HeadBuf[16];
    struct _tagDogControlTableItem  ControlTable[MAX_CONTROL_TABLE_NUM];
} DRCOMDOGPARAMS;


/* ####################################################################################
 * 认证过程
 */

struct  _tagChallenge
{
    unsigned char code;
    unsigned char ChallengeID;      // ID 每发送一次，累加
    unsigned short ChallengeRandomID;   // 发送时的毫秒数，其实就是随机数
    unsigned char ClientVerno;
    unsigned char unused[15];
#ifdef DRCOM_ENCRYPT_PROTO
    unsigned char encrypt_info[32];
#endif
};

struct  _tagReturnChallenge
{
    unsigned char code;
    unsigned char ChallengeID;      // 返回Challenge 发送的ID
    unsigned short ChallengeRandomID;   // 返回Challenge 发送的ID
    unsigned char ChallengeSeed[SEED_LEN];
    unsigned short AuthTypeSele; //WORD seleA;//authtypesele
    unsigned short DeviceVer;
    unsigned short DeviceKernelVer;
    unsigned short AdminTcpPort;
    unsigned int DhcpsServerIP;
    unsigned int HostIP;
    unsigned short HostPort;

    unsigned char MyDllHeader[16];
    unsigned short SystemAuthOption;//b(0-3)=systemInitPara.mainv6mode, b4-14 unuse, b(15)==1为4步心跳
    unsigned long MainServerV6IP[4];
    unsigned long HostV6IP[4];

#ifdef DRCOM_ENCRYPT_PROTO
    unsigned char unused[4];
    unsigned char encrypt_info[32];
#endif
};

#define INTERNET_MODE_DIRECT_INTERNET       0x00
#define INTERNET_MODE_INTERNET_OR_CHINANET  0x01

#define CONTROL_CHECK_STATUS_ALLNET     (0x20|0x40)
#define CONTROL_CHECK_STATUS_NET1       0x20
#define CONTROL_CHECK_STATUS_NET2       0x40

#define CONTROL_CHECK_STATUS_NET_INTERNET   CONTROL_CHECK_STATUS_NET1
#define CONTROL_CHECK_STATUS_NET_CHINANET   CONTROL_CHECK_STATUS_NET2
#define CONTROL_CHECK_STATUS_NET_LOCALNET   CONTROL_CHECK_STATUS_NET2

#define CONTROL_CHECK_STATUS_USE_INTERNET   CONTROL_CHECK_STATUS_NET1
#define CONTROL_CHECK_STATUS_USE_LOCALE_NET CONTROL_CHECK_STATUS_NET2

//  ===>
struct  _tagLoginPacket
{
    struct _tagDrCOMHeader Header;
    unsigned char PasswordMd5[MD5_LEN];
    char Account[ACCOUNT_MAX_LEN];
    unsigned char ControlCheckStatus;
    unsigned char AdapterNum;
    unsigned char MacAddrXORPasswordMD5[MAC_LEN];
    unsigned char PasswordMd5_2[MD5_LEN];
    unsigned char HostIpNum;
    unsigned int HostIPList[HOST_MAX_IP_NUM];
    unsigned char HalfMD5[8];
    unsigned char DogFlag;
    unsigned int unkown2;
    struct _tagHostInfo HostInfo;
    unsigned char ClientVerInfoAndInternetMode;
    unsigned char DogVersion;
};

//LDAP认证数据附加在认证结果后面，没有LDAp认证时，没有这些数据
struct  _tagLDAPAuth
{
    unsigned char Code;
    unsigned char PasswordLen;
    unsigned char Password[MD5_LEN];
};


//认证扩展数据，在LDAP认证数据后面
struct  _tagDrcomAuthExtData
{
    unsigned char Code;
    unsigned char Len;
    unsigned long CRC;
    unsigned short Option;
    unsigned char AdapterAddress[MAC_LEN];
};

//  <===
struct  _tagReturnErrorCode
{
    struct _tagDrCOMHeader Header;
    unsigned char ErrCode;
    unsigned char ErrData[15];
    unsigned char ErrMessage[2];
};

//  <===
struct _tagReturnLoginResult
{
    struct _tagDrCOMHeader Header;
    unsigned char SubCode;
    unsigned int UsedMinutes;   //min
    unsigned int UsedFlux;      //Kb
    unsigned int Balance;
    unsigned char unknow1;
    unsigned char VPNFlag;
    unsigned int VPNxChangeIP;
};

//封装防代理数据,不是防代理帐号认证结果没有这些数据的,附加在认证结果_tagReturnLoginResult后面

struct _tagDrcomDogControlHeader
{
    struct _tagDrcomDogData DogData;
    unsigned short MaxLen;
    unsigned char DogControlTblLen;
    unsigned char Other;
};
struct _tagDrcomDogControlData
{
    struct _tagDrcomDogData DogData;
    unsigned short MaxLen;
    unsigned char DogControlTblLen;
    unsigned char Other;
    struct _tagDogControlTableItem DogControlTbl[1];
};

#define NEW_VER_OPTION_SHOW_NONE_INFO       0x01
#define NEW_VER_OPTION_SHOW_TIME_INFO       0x02
#define NEW_VER_OPTION_SHOW_FLOW_INFO       0x04
#define NEW_VER_OPTION_SHOW_CHARGE_INFO     0x08
#define NEW_VER_OPTION_2166_ANTIP_PROXY     0x10


//在认证结果的放代理数据_tagDrcomDogControlData后面
struct  _tagNewVersionInfo
{
    unsigned char NewVerMark;
    unsigned char NewVerOption;
};



#define PING_CONTROL_ENABLE_INTERNET    0x00000000
#define PING_CONTROL_DISABLE_INTERNET   0x00000001
//  ===>
struct  _tagHostPingServer
{
    unsigned char Code;
    union {
        struct{
            unsigned char unused[7];
            unsigned char ChallengeSeed[SEED_LEN];
            unsigned long crc[2];
        }newcheck;
        struct {
            unsigned char KeepAliveMD5[MD5_LEN];
            unsigned char unused[3];
        }oldcheck;
    }checkcode;
    struct _tagDrcomDogData DogData;
    unsigned short Milliseconds;
    unsigned int InternetAccessControl;

#ifdef DRCOM_ENCRYPT_PROTO
    unsigned char unused2[6];
    unsigned char encrypt_ping[48];
#endif
};

//  <===
struct  _tagServerKeepAlive
{
    unsigned char Code;
    unsigned char SubCode;
    unsigned short PlusWord;
    unsigned char AllowMaxIPNum;
    unsigned char Data[13];
};

//  ===>
struct  _tagHostReplayServerAlive
{
    unsigned char Code;
    unsigned char Digest;
    unsigned char HostIpNum;
    unsigned char KeepReplayMd5[MD5_LEN];
    unsigned char unknow3;
    struct _tagDrcomDogData DogData;
};

//  <===
struct _tagMessagePacket
{
    unsigned char Code; // 'M'
    unsigned char MsgType;
    unsigned char MsgData1;
    unsigned char MsgData2;
    unsigned char MsgData[1];
};

struct _tagLogoutPacket
{
    struct _tagDrCOMHeader Header;
    unsigned char PasswordMD5[MD5_LEN];
    char Account[ACCOUNT_MAX_LEN];
    unsigned char ControlCheckStatus;
    unsigned char AdapterNum;
    unsigned char MacAddrXORPasswordMD5[MAC_LEN];
    struct _tagDrcomDogData DogData;
};


struct _tagLogoutResult
{
    struct _tagDrCOMHeader Header;
    unsigned char SubCode;
    unsigned int UsedMintues;
    unsigned int UsedFlux;
    unsigned int Balance;

    unsigned int OLinuseupflow;        //本次登录实时上行流量up kb
    unsigned int OLinusedownflow;      //本次登录实时下行流量down kb
    //unsigned char unkonw1;
    //unsigned char unkonw2[5];
    //struct _tagDrcomDogData DogData;
    //unsigned int unkonw3;
    //unsigned short unkonw4;
};

#define DRCOM_2133_RET_VPN_TABLE 4
#define DRCOM_2133_RET_PING_INTERVAL 6
/// fgx 2013-05-10
#define DRCOM_2133_CHALLENGE_REQ 0x01       // 登录成功后，向服务器获取挑战值
#define DRCOM_2133_CHALLENGE_REP 0x02       // 服务器返回挑战值结果

typedef struct ExtClientPingInfo
{
    unsigned long OLinusesec;           //在线时长sec
    unsigned long OLinuseupflow;        //本次登录实时上行流量up kb
    unsigned long OLinusedownflow;      //本次登录实时下行流量down kb
    unsigned long OLlocation_sum_t;     //已使用时长used min
    unsigned long OLlocation_sum_kb;    //已使用流量used kb
    unsigned long OLlocation_umoney;    //余额 yuan/10000
    unsigned long OL_ISessionTimeOut;   //本次登录可用时间 sec
    unsigned long OL_ulimiteflow;       //本次登录可用流量KByte
} EXTCLIENTPINGINFO;

struct _tagDrcom2133ReturnHead
{
    unsigned char code;     //=7
    unsigned char id;
    unsigned short length;
    unsigned char type;     //4=dvpntable, 6=ping return
    unsigned char pingsec;
    unsigned short infolength;
    unsigned long crc[2];
};

struct _tagDrcom2133ReturnWithDogData
{
    unsigned char code;     //=7
    unsigned char id;
    unsigned short length;
    unsigned char type;     //4=dvpntable, 6=ping return
    unsigned char pingsec;
    unsigned short infolength;
    unsigned long crc[2];
    struct _tagDrcomDogData DogData;
};

// 登录完成后，心跳--获取挑战值包
struct _tagDrcom2133Challenge   // ------>2133
{
    unsigned char code;     //=7
    unsigned char id;
    unsigned short length;
    unsigned char type;     //=1

    unsigned char other[3];

#ifdef DRCOM_ENCRYPT_PROTO
    unsigned char unused[12];
    unsigned char encrypt_info[32];
#endif
};

// 登录完成后，心跳--返回挑战值包
struct _tagDrcom2133ReturnChallenge         // <------2133
{
    unsigned char code;     //=7
    unsigned char id;
    unsigned short length;
    unsigned char type;     //=2

    unsigned char other[3];
    unsigned char ChallengeSeed[SEED_LEN];
    unsigned long ClientSouIP;

#ifdef DRCOM_ENCRYPT_PROTO
    unsigned char unused[20+48];
	unsigned char encrypt_info[32];
#endif
};

struct _tagDrcom2133ReturnWithOnlineInfo
{
    unsigned char code;     //=7
    unsigned char id;
    unsigned short length;
    unsigned char type;     //4=dvpntable, 6=ping return
    unsigned char pingsec;
    unsigned short infolength;
    unsigned long crc[2];
    struct _tagDrcomDogData DogData;
    EXTCLIENTPINGINFO OnlineInfo;
};


typedef struct tagNewPasswdPacket
{
    struct _tagDrCOMHeader header;
    unsigned char pwd_and_seed_md5[MD5_LEN];
    unsigned char account_left_16[MD5_LEN];
    unsigned char account_and_pwd_md5[MD5_LEN];
    unsigned char account_right[ACCOUNT_MAX_LEN - MD5_LEN];
} PASSWDPACKET, *PPASSWDPACKET;



#define DRCOM_DIAL_PROTO_OPT_ANTI_PROXY_OFFLINE         0x8000  //b15=find proxy cut line
#define DRCOM_DIAL_PROTO_OPT_ANTI_CLIENT_VER            0x01000000

#define DRCOM_DIAL_PROTO_OPT_CONNECTION_START           0x00
#define DRCOM_DIAL_PROTO_OPT_CONNECTION_ONLINEING       0x100  //b8=(no first)
#define DRCOM_DIAL_PROTO_OPT_DEFAULT                    0x200  //b9=有tcpipdog.dll 应该是 ANNTI_PROXY

#define DRCOM_DIAL_PROTO_OPT_DRCOMDLL_CHECK             (0x200+0x2000+0x4000)  //0x2000 b13(mydll mark2007-2-28)  0x4000 b14(mydll mark


#define DRCOM_8021X_PROTO_OPT_DEFAULT                   0x00
#define DRCOM_8021X_PROTO_OPT_DRCOMDLL_CHECK            (0x02+0x200+0x2000)
#define DRCOM_8021X_PROTO_OPT_DHCP                      0x01
#define DRCOM_8021X_PROTO_OPT_DOG_AND_VPN               0x02
#define DRCOM_8021X_PROTO_OPT_ANNTI_PROXY               0x200
#define DRCOM_8021X_PROTO_OPT_ANTI_CLIENT_VER           0x01000000

/* ########################################################################################
 * PPOE Puglin
 */
typedef struct _tagDrcomDialExtProtoHeader
{
    unsigned char code;
    unsigned char id;
    unsigned short length;
    unsigned char type;
} PPPOEHEADER;

/*
 * 发起服务器认证请求
 * 2133
 */
typedef struct _tagDrcomDialExtProtoChallenge {
    PPPOEHEADER header;
    unsigned char other[3];

#define hcode   header.code     //=7
#define hid     header.id
#define hlength header.length
#define htype   header.type     //=1

#ifdef DRCOM_ENCRYPT_PROTO
    unsigned char unused[12];
    unsigned char encrypt_info[32];
#endif
} PPPOECHALLENGE;
#define _tagDrcom8021XExtProtoChallenge _tagDrcomDialExtProtoChallenge

/*
 * 认证服务器对认证请求的响应
 * 2133
 */
struct _tagDrcomDialExtProtoReturnChallenge {
    PPPOEHEADER header;

#define hcode   header.code     //=7
#define hid     header.id
#define hlength header.length
#define htype   header.type     //=2
    unsigned char other[3];
    unsigned char challenge[SEED_LEN];

#ifdef DRCOM_ENCRYPT_PROTO
unsigned char unused[20+48];
unsigned char encrypt_info[32];
#endif
};

#define  _tagDrcom8021XExtProtoReturnChallenge _tagDrcomDialExtProtoReturnChallenge
#define MAX_DRCOM_DIAL_EXT_PROTO_NET_NUM 4
#define MAX_DRCOM_8021X_EXT_PROTO_NET_NUM MAX_DRCOM_DIAL_EXT_PROTO_NET_NUM
#define USE_LIMIT_IP (unsigned long)(169+254*256)

struct _tagDrcomDialExtProtoNetWorkInfo
{
    unsigned char mac[6];
    unsigned char netmark;      //dhcp mark
    unsigned char type;     //interface type(ether,ppp)
    unsigned long sip;
    unsigned long smask;
    //ULONG gateway1;
};

#define _tagDrcom8021XExtProtoNetWorkInfo _tagDrcomDialExtProtoNetWorkInfo

/*
 * 认证数据命令
 * 2133
 */
struct _tagDrcomDialExtProtoLoginPacket {
    PPPOEHEADER header; // 5 byte

#define hcode   header.code     // PPPoE ext = 0x07
#define hid     header.id		// 自定义，每次不同就可以
#define hlength header.length	// 发送数据包的长度
#define htype   header.type     // PPPoE = 0x03

    unsigned char uidlength;
    unsigned char mac[6];
    unsigned long sip;
    unsigned long option;   //bit0=dhcp, bit1=请求封装, bit2-7(verno)
    //b8=(no first),b9=有tcpipdog.dll, b10-12 选择出口线路 2006-12-18
    //b13(mydll mark2007-2-28), bit14-31 unuse
    //b14(mydll mark b15=find proxy cut line (pppoe模式2007-11-22)
    //bit16-31 unuse

    unsigned char ChallengeSeed[SEED_LEN];
    unsigned long crc[2];

    //account ,len 0
    //struct _tagDrcomDialExtProtoNetWorkInfo netinfo[MAX_DRCOM_DIAL_EXT_PROTO_NET_NUM];
    //unsigned char unused[4];
};

#define _tagDrcom8021XExtProtoLoginPacket _tagDrcomDialExtProtoLoginPacket

struct _tagDrcomDialExtProtoReturnChallengeAndIP        // <------2133
{
    PPPOEHEADER header;
#define hcode   header.code     //=7
#define hid     header.id
#define hlength header.length
#define htype   header.type     //=2
    unsigned char other[3];
    unsigned char ChallengeSeed[SEED_LEN];
    unsigned long ClientSouIP;

#ifdef DRCOM_ENCRYPT_PROTO
    unsigned char unused[20+44];
	unsigned char encrypt_info[32];
#endif
};

#define _tagDrcom8021XExtProtoReturnChallengeAndIP _tagDrcomDialExtProtoReturnChallengeAndIP

/*
 * 认证结果
 *
 */
struct _tagDrcomDialExtProtoLoginResult {
    PPPOEHEADER header;

#define hcode   header.code     //=7
#define hid     header.id
#define hlength header.length
#define htype   header.type     //4=dvpntable, 6=ping return

    unsigned char   pingsec;
    unsigned short  infolength;
    unsigned long   crc[2];  //type==4 crc[1]=0x01,允许代理 ，kernel
};

// #################
/*
 * 防代理用的结构体
 */
typedef struct _tagAntiProxyDllGetVerInfoRet
{
    unsigned short code;        //1=init, 2=exit
    unsigned short dllverno;
    unsigned char* AuthDataBuff;
    int AuthDataLen;
    int AuthSocket;
    HWND hMainWnd;
    unsigned int serverip;
    char dllretcode; //bool
    unsigned int other[32];
} ANTIPROXYDLLGETVERINFORET;

enum
{
    AntiProxyModuleUpdateStatus_NOT_SET = 0,
    AntiProxyModuleUpdateStatus_BEGIN_UPDATE = 1,
    AntiProxyModuleUpdateStatus_UPDATING_MODULE = 2,
    AntiProxyModuleUpdateStatus_UPDATE_MODULE_OVER = 3,


    HAS_NEW_VERSION_NOT_CHECK = 0,
    HAS_NEW_VERSION_CHECKED = 1,

    CMD_CHECK_UPDATE = 1,
    CMD_CHECK_UPDATE_RES = 2,
    CMD_START_UPDATE = 3,
    CMD_START_UPDATE_RES = 4,
    CMD_UPDATEING = 5,
    CMD_PROXY_MODULE_HEADER = 6,

    NEXT_CMD_WAIT_TIME_INVALID = -1,
    NEXT_CMD_SEND_NOW = 0,
    NEXT_CMD_SEND_AFTER_1_SCE = 1,
    ANTI_CMD_SEND_TIMEOUT = 2,
    UPDATE_MODULE_TRY_AGAIN_INTERVAL = 5,
};



typedef struct _tagDr2133ReqHead
{
    unsigned char code;
    unsigned char id;
    unsigned short length;
    unsigned char type;
    unsigned char code1;
    unsigned short ActiveVerNo;
    unsigned int hrandomID;
    unsigned int hdlloffset;
} DR2133REQHEAD;

typedef struct _tagDr2133ReqInfo
{
    unsigned char code;
    unsigned char id;
    unsigned short length;
    unsigned char type;
    unsigned char code1;
    unsigned short ActiveVerNo;
    unsigned int randomID;
    unsigned int dlloffset;

    unsigned char Extchalleng[8];
    unsigned int crcvalue1;
    unsigned int mysouip;
    unsigned char mysoumac[6];
    unsigned char other[2];

} DR2133REQINFO, *PDR2133REQINFO;


typedef struct _tagDrcomAntiModuleDownloadBuff
{
    unsigned int dwBuffLen;
    unsigned int DownloadedLen;
    char* Buffer;
    unsigned short DownloadingModuleVer;
} DrcomAntiModuleDownloadBuff;


enum
{
    MAX_XXTEA_KEY_LEN = 4,
    MAX_SERVERIP_NUM = 4,
    FILE_CHECK_SUM_LEN = 32,
    MAX_CHECK_SUM_FILE_NUM = 8,
    MAX_RULE_STRING_LEN = 160,
    MAX_RESULT_HANDLE_RULE_NUM = 50,
    MAX_PACKET_CHECK_RULE_NUM = 20,
    RULE_TYPE_BASE = 0x00000000,
    RULE_TYPE_RES_CHK_DEFAULT = RULE_TYPE_BASE,
    RULE_TYPE_RES_CHK_ALL = 0xFFFFFFFF,
    RULE_TYPE_RES_CHK_PROCESS_CHECK = RULE_TYPE_BASE + 1,
    RULE_TYPE_RES_CHK_SERVICE_CHECK = RULE_TYPE_BASE + 2,
    RULE_TYPE_RES_CHK_DRIVER_CHECK = RULE_TYPE_BASE + 3,
    RULE_TYPE_RES_CHK_FILE_CHECK = RULE_TYPE_BASE + 4,
    RULE_TYPE_RES_CHK_ROUTER_CHECK = RULE_TYPE_BASE + 5,
    RULE_TYPE_RES_CHK_PACKET_CHECK = RULE_TYPE_BASE + 6,

    MAX_RULE_TYPE_NUM = 32,
    RULE_DEFAULT_VALUE = 10,
};

enum
{
    PROCESS_BASE_INFO = 0x00000001,
    PROCESS_FILES_INFO = 0x00000002,
    PROCESS_DLL_MODULES_INFO = 0x00000004,
    PROCESS_HANDLES_INFO = 0x00000008,
    PROCESS_WINDOWS_INFO = 0x00000010,
    PROCESS_THREADS_INFO = 0x00000020,
    PROCESS_MEMORY_MAPS_INFO = 0x00000040,
    PROCESS_SOCKETS_INFO = 0x00000080,



    PROCESS_CHK_ALLINFO_TYPE =  0xFFFFFFFF,
    ANTI_PROXY_PROCESS_CHK_TYPE = PROCESS_BASE_INFO
                                  | PROCESS_FILES_INFO
                                  | PROCESS_DLL_MODULES_INFO
                                  | PROCESS_HANDLES_INFO
                                  | PROCESS_WINDOWS_INFO
                                  | PROCESS_SOCKETS_INFO,
};

enum
{
    DRCOM_BUF_LEN = 1024 * 256,
};



typedef struct _tagRegexRule
{
    unsigned long RuleType;
    long RuleValue; //值小于0不匹配时加分，大于0匹配时加分
    unsigned long Priority;
    char RuleString[MAX_RULE_STRING_LEN];
} REGEXRULE, *PREGEXRULE;


typedef struct _tagFileCheckSum
{
    char FileName[32];
    unsigned char CheckSum[FILE_CHECK_SUM_LEN];
} FILECHECKSUM;

//size 必须是4的倍数，需要加密
typedef struct _tagDrcomConfig
{
    unsigned long serverip[MAX_SERVERIP_NUM];
    unsigned long anti_proxy_svrip[MAX_SERVERIP_NUM];
    unsigned long default_control[MAX_SERVERIP_NUM];
    unsigned short anti_proxy_svr_port;
    unsigned short CheckSumHashType;
    unsigned short nResultRuleNum;
    unsigned short nPacketRuleNum;
    REGEXRULE ResultHandleRule[MAX_RESULT_HANDLE_RULE_NUM];
    REGEXRULE PacketCheckRule[MAX_PACKET_CHECK_RULE_NUM];
    FILECHECKSUM DrcomdllCheckSum;
    FILECHECKSUM ClientCheckSum[MAX_CHECK_SUM_FILE_NUM];
    unsigned long AdapterPacketSumInterval; //sec
    long xxtea_key[MAX_XXTEA_KEY_LEN];
    unsigned int Len;
} DRCOMCONFIG, *PDRCOMCONFIG;

#define DRCOM_CONFIG_DECRYPT_ENCRYPT_LEN ((sizeof(DRCOMCONFIG)/sizeof(long))-1-MAX_XXTEA_KEY_LEN)


typedef struct _tagAntiProxyModuleCheckSumHead
{
    unsigned long mydll_length;
    unsigned long mydll_crc;
    unsigned long AllFileCRC;
    unsigned short mydllver;
    unsigned char other[2];
} ANTIPROXYMODULECHECKSUMHEAD, *PANTIPROXYMODULECHECKSUMHEAD;


// ##############
#define _tagDrcom8021XExtProtoLoginResult _tagDrcomDialExtProtoLoginResult
#endif
