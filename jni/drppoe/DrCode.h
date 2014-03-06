/*
 * DrCode.h
 *
 *  Created on: 2014年2月22日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef DRCODE_H_
#define DRCODE_H_

enum
{
    DRCOM_FLAG = 0x6F637244,
    DRCOM_ERR = 0x20525245,
    DRCOM_SUC = 0x20435553,

    PROXY_RULE_SIZE = 512,
    MAX_RULE_COUNT = 256,

    MAX_ADAPTER_NUM = 16,

    MAX_DRCOM_AUTH_DATA_LEN = 400,
    MAX_DRCOM_MESSAGE_LEN = 400,
    MAX_DRCOM_MESSAGE_DATA_LEN = 200,
    MAX_DRCOM_WWW_MESSAGE_LEN = 100,

    SPC_DRCOM_CLIENT_VER_BASE = 100,

    STD_BUF = 4*1024,

    DRCOM_LANG_ZH_CN = 1,
    DRCOM_LANG_EN = 2,
    CPU_LOAD_SLEEP_TIME = 2,
};

enum
{
    CONFIG_MAKE_CRC = 0,
    CONFIG_CHECK_CRC
};

enum
{
    CONFIG_FILE_READ_WRITE_OK = 0,
    OPEN_CFG_FILE_ERROR = -1,
    CFG_FILE_CRC_CHK_ERROR = -2,
    WRITE_CONFIG_FILE_ERROR = -3,
    READ_CONFIG_FILE_ERROR = -4,
};


#define  DR_ADMIN_RESET_CMD                 "AdminReset"
#define  DR_ADMIN_RESET_CMD_RESONE_SPLIT   ">"
#define DR_PPPoE_WELCOM "Welcome to Drcom System:"

#endif /* DRCODE_H_ */
