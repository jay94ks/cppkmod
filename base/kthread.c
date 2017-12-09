/*
 * kthread.c
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#include <linux/mutex.h>

struct task_struct* ___mutex_owner(struct mutex *lock) {
	return __mutex_owner(lock);
}

int ___mutex_size(void) {
	return sizeof(struct mutex);
}

int ___mutex_init(struct mutex* lock) {
	mutex_init(lock);
	return 0;
}
