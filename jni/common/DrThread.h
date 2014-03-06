/*
 * DrThread.h
 *
 *  Created on: 2014年2月14日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef _INC_DRTHREAD_
#define _INC_DRTHREAD_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "DrMutex.h"
#include "DrCond.h"
#include "DrRunnable.h"

class DrThread
{
public:
	DrThread();
	DrThread(DrRunnable *runnable);
	virtual ~DrThread();
	pthread_t start();
	void stop();
	void sleep(uint32_t msec);
	bool isRunning();
	pthread_t getThreadId();
	pthread_t getSelfThreadId();
	DrMutex m_DrMutex;
protected:
	virtual void onRun();
private:
	DrRunnable *m_pDrRunnable;
	void setRunning(bool isRunning);
	bool m_isRunning;
	pthread_t m_pthread_t;
	pthread_t m_pthread_t_id; // 只在退出线程时候输出log用
	static void *thread_proc_func(void *args);
};
#endif
