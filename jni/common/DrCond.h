/*
 * DrCond.h
 *
 *  Created on: 2014年2月25日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef DRCOND_H_
#define DRCOND_H_
#include <pthread.h>

#include "DrMutex.h"
#include "DrLog.h"

class DrCond {
public:
	DrCond() {
		initcond();
	}
	virtual ~DrCond() {
		desrtoycond();
		showLog("jni.DrCond", "~DrCond");
	}
	int signal() {
		showLog("jni.DrCond", "signal");
		int ret;
		m_DrMutex.lock();
		ret = pthread_cond_signal(&m_Cond);
		m_DrMutex.unlock();
		return ret;
	}
	int timedwait(int second) {
		int ret;
		m_DrMutex.lock();

		struct timeval now;
		gettimeofday(&now, NULL);

		struct timespec outtime;
		outtime.tv_sec = now.tv_sec + second;
		outtime.tv_nsec = now.tv_usec * 1000;

		ret = pthread_cond_timedwait(&m_Cond, (pthread_mutex_t *)m_DrMutex.getMutex(), &outtime);
		m_DrMutex.unlock();
		return ret;
	}
	int wait() {
		showLog("jni.DrCond", "wait");
		int ret;
		m_DrMutex.lock();
		ret = pthread_cond_wait(&m_Cond, (pthread_mutex_t *)m_DrMutex.getMutex());
		m_DrMutex.unlock();
		return ret;
	}
	int broadcast() {
		int ret;
		m_DrMutex.lock();
		ret = pthread_cond_broadcast(&m_Cond);
		m_DrMutex.unlock();
		return ret;
	}

	int lock(){
		return m_DrMutex.lock();
	}
	int unlock(){
		return m_DrMutex.unlock();
	}

	pthread_cond_t getCond() {
		return m_Cond;
	}
protected:
	int initcond() {
		return pthread_cond_init(&m_Cond, NULL);
	}
	int desrtoycond() {
		return pthread_cond_destroy(&m_Cond);
	}
private:
	pthread_cond_t m_Cond;
	pthread_condattr_t m_Condattr;
	DrMutex m_DrMutex;
};

#endif /* DRCOND_H_ */
