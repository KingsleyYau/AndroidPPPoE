/*
 * DrMutex.h
 *
 *  Created on: 2014年2月14日
 *      Author: Kingsley Yau
 *      Email: Kingsleyyau@gmail.com
 */

#ifndef _INC_DRMUTEX_
#define _INC_DRMUTEX_

#include <pthread.h>
using namespace std;

class DrMutex
{
public:
	DrMutex(){
		initlock();
	}
	~DrMutex(){
		desrtoylock();
	}
	int trylock(){
		return pthread_mutex_trylock(&m_Mutex);
	}
	int lock(){
		return pthread_mutex_lock(&m_Mutex);
	}
	int unlock(){
		return pthread_mutex_unlock(&m_Mutex);
	}
	pthread_mutex_t const* getMutex() {
		return &m_Mutex;
	}
protected:
	int initlock(){
		return pthread_mutex_init(&m_Mutex, NULL);
	}
	int desrtoylock(){
		return pthread_mutex_destroy(&m_Mutex);
	}
private:
	pthread_mutex_t m_Mutex;
	pthread_mutexattr_t m_Mutexattr;
};
#endif
