/*
 * DrRunnable.h
 *
 *  Created on: 2014年2月22日
 *      Author: kingsley
 */

#ifndef DRRUNNABLE_H_
#define DRRUNNABLE_H_

class DrRunnable {
public:
	virtual void onRun() = 0;
	virtual ~DrRunnable(){};
};


#endif /* DRRUNNABLE_H_ */
