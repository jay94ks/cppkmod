/*
 * kthread.hpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_KTHREAD_KTHREAD_HPP_
#define HEADERS_MODCPP_KTHREAD_KTHREAD_HPP_

#include "mutex.hpp"

/*
 * wrapper for kthread forwardings.
 * */
class kthread {
public:
	typedef int (*runnable_t)(kthread* thread, void* arg);

public:
	kthread(runnable_t worker = 0)
	: taskInfo(0), runnable(worker), runArgs(0)
	{
	}

	virtual ~kthread() {
		if(taskInfo)
			kthread_stop(taskInfo);

		taskInfo = 0;
	}

private:
	struct task_struct* taskInfo;
	runnable_t runnable;
	void* runArgs;

private:
	static int onProxy(void* args);

protected:
	virtual int onRun(void* args);

public:
	bool start(const char* name);
	bool stop();

public:
	bool isAlive() { return taskInfo != 0; }

public:
	static inline bool shouldStop() {
		return kthread_should_stop();
	}
};

#endif /* HEADERS_MODCPP_KTHREAD_KTHREAD_HPP_ */
