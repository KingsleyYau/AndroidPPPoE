/*
 * DrThread.cpp
 *
 *  Created on: 2014年2月14日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#include "DrThread.h"
#include <time.h>
#include "DrLog.h"
DrThread::DrThread() {
	m_pthread_t = 0;
	m_pthread_t_id = 0;
	m_isRunning = false;
	m_pDrRunnable = NULL;
}
DrThread::DrThread(DrRunnable *runnable) {
	m_pthread_t = 0;
	m_pthread_t_id = 0;
	m_isRunning = false;
	this->m_pDrRunnable = runnable;
}
DrThread::~DrThread() {
	stop();
}
void DrThread::setRunning(bool isRunning){
	m_isRunning = isRunning;
}
void* DrThread::thread_proc_func(void *args){
	long threadId = 0;
	DrThread *pDrThread = (DrThread*)args;
	if(pDrThread){
		showLog("Jni.DrThread.thread_proc_func", "pDrThread:(%ld)->onRun", pDrThread->getThreadId());
		threadId = pDrThread->getThreadId();
		pDrThread->onRun();
		pDrThread->m_DrMutex.lock();
		pDrThread->setRunning(false);
		pDrThread->m_DrMutex.unlock();
	}
	showLog("Jni.DrThread.thread_proc_func", "线程:(%ld)已经运行完毕!", threadId);
	return (void*)0;
}
pthread_t DrThread::start(){
	m_DrMutex.lock();
	int ret = pthread_create(&m_pthread_t, NULL, &thread_proc_func, (void*)this);
	if(0 != ret) {
		showLog("Jni.DrThread.start", "启动线程失败,因为:(%s)!", strerror(ret));
		m_pthread_t = 0;
		m_pthread_t_id = 0;
	}
	else {
		setRunning(true);
		showLog("Jni.DrThread.start", "启动线程:(%ld)成功!", m_pthread_t);
		m_pthread_t_id = m_pthread_t;
	}
	m_DrMutex.unlock();
	return m_pthread_t;
}
void DrThread::stop(){
	m_DrMutex.lock();
	if(0 != m_pthread_t){
		if(isRunning()) {
			if(m_pthread_t > 0) {
				showLog("Jni.DrThread.stop", "等待线程:(%ld)退出...", m_pthread_t);
			}
			else if(m_pthread_t_id != 0){
				showLog("Jni.DrThread.stop", "线程:(%ld)已经退出!", m_pthread_t_id);
			}

//			pthread_kill(m_pthread_t, SIGALRM);
//			pthread_cancel(&m_pthread_t);
//			if(0 != pthread_join(m_pthread_t, NULL)){
//				showLog("Jni.DrThread.stop", "等待退出线程:(%ld)错误!", m_pthread_t);
//			}
//			else{
//				showLog("Jni.DrThread.stop", "退出线程:(%ld)成功!", m_pthread_t);
//			}
		}
		m_pthread_t = 0;
		m_pthread_t_id = 0;
		setRunning(false);
	}
	m_DrMutex.unlock();
}
void DrThread::sleep(uint32_t msec){
	//usleep(100);
//	struct timespect st;
//	st.tv_sec = msec / 1000;
//	st.tv_nsec = (msec % 1000) * 1000 * 1000;
//	if(-1 == nanosleep(&st, NULL)){
//		showLog("Jni.DrThread.sleep", "thread sleep error!");
//	}
}
pthread_t DrThread::getThreadId(){
	return m_pthread_t;
}
pthread_t DrThread::getSelfThreadId() {
	return pthread_self();
}
bool DrThread::isRunning(){
	return m_isRunning;
}
void DrThread::onRun() {
	if(NULL != m_pDrRunnable ) {
		m_pDrRunnable->onRun();
	}
}
