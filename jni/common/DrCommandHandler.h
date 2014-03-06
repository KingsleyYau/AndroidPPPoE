/*
 * DrCommandHandler.h
 *
 *  Created on: 2014年3月6日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef DRCOMMANDHANDLER_H_
#define DRCOMMANDHANDLER_H_

/*
 * 命令类型
 */
typedef enum SnifferCommandType{
	StartSinfferTypeNone = 0,
	StartSinfferType = 1,
	StopSnifferType = 2,
}SCMDT;

/*
 * 命令
 */
typedef struct SnifferCommand {
	SCMDT scmdt;	// 类型
}SNIFFERCOMMAND;

#include "../common/DrUdpSocket.h"

class DrCommandHandler {
public:
	DrCommandHandler();
	virtual ~DrCommandHandler();

	bool BindLocal();
	bool SendCommand(SnifferCommand scmd);
	SnifferCommand RecvCommand();

protected:
	DrUdpSocket m_UdpSocket;

	int m_iPort;					// 绑定本地接收端口
	string m_sServerAddress;		// 远程Ip
	int m_iServerPort;				// 服务器端口
};

#endif /* DRCOMMANDHANDLER_H_ */
