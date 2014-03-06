/*
 * DrSniffer.h
 *
 *  Created on: 2014年3月3日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef DRSNIFFER_H_
#define DRSNIFFER_H_

#include "../common/DrSocket.h"
#include "../common/DrThread.h"

class DrSniffer : public DrSocket{
public:
	DrSniffer();
	virtual ~DrSniffer();

	// 函数
	bool StartSniffer(string deviceName = "");
	void StopSniffer();
	bool IsRunning();
	void ResetParam();

	int SinfferData();
	void AnalyseData(unsigned char* pBuffer, int iLen);
	void AnalyseIPData(unsigned char* pBuffer, int iLen);
	void AnalyseTcpData(unsigned char* pBuffer, int iLen);
	void AnalyseUdpData(unsigned char* pBuffer, int iLen);
private:
	DrThread *m_pDrSnifferThread;
	bool m_bRunning;
};

#endif /* DRSNIFFER_H_ */
