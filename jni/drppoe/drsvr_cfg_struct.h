#ifndef _DRCOM_CONFIG_STRUCT_H_
#define _DRCOM_CONFIG_STRUCT_H_

enum
{
    //MAX_AUTH_SERVER_NUM = 24, ///由于Linux及MacOS客户端配置工具不能重新编译，所以只能回溯代理到绑定8个IP
    MAX_AUTH_SERVER_NUM = 8,
    MAX_CONFIG_FILE_CRC_LEN = 64,
    MAX_CUSTOMER_NAME_LEN = 64,
    MAX_AUTH_SERVER_NAME_LEN = 32,
};

typedef struct _tagDrSvrConfig
{
    unsigned int dwConfigVer;
    unsigned char ConfigCRC[MAX_CONFIG_FILE_CRC_LEN];
    char szCustomerName[MAX_CUSTOMER_NAME_LEN];
    unsigned int DrcomAuthServersIP[MAX_AUTH_SERVER_NUM];
    char szDrcomAuthServerName[MAX_AUTH_SERVER_NAME_LEN];
    unsigned int dwAntiProxyVer;
    unsigned int dwAuthProtoVer;
    unsigned int dwSetupConfig;
    char szSupplicantName[64];
    char szWebUrl[32];
    unsigned int dwDrcomDialConfig;
    unsigned int dwAuthProtoConfig;
    unsigned int dwAntiProxyConfig;
    unsigned int unused[485]; //注意类型是 unsigned long 是4字节，1代表4字节
} DRSVRCONFIG, *PDRSVRCONFIG;


#endif //#ifndef _DRCOM_CONFIG_STRUCT_H_
