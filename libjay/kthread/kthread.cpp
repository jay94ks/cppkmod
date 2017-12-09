/*
 * kthread.cpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#include <modcpp/kthread.hpp>
#include <modcpp.h>

bool kthread::start(const char* name) {
	if(taskInfo)
		return false;

	printk(KERN_INFO "libjay, %s: kthread execution: 0x%x\n",
			MODULE_NAME, this);
	taskInfo = kthread_run(onProxy, this, name);
	return true;
}

bool kthread::stop() {
	if(taskInfo) {
		kthread_stop(taskInfo);
		return true;
	}

	return false;
}

int kthread::onProxy(void* args) {
	kthread* thread = (kthread*) args;
	int retVal = thread->onRun(thread->runArgs);

	printk(KERN_INFO "libjay, %s: kthread stopped: 0x%x\n",
			MODULE_NAME, thread);
	thread->taskInfo = 0;
	return retVal;
}

int kthread::onRun(void* args) {
	if(runnable) {
		return runnable(this, args);
	}

	return -1;
}
