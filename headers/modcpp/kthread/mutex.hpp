/*
 * mutex.hpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_KTHREAD_MUTEX_HPP_
#define HEADERS_MODCPP_KTHREAD_MUTEX_HPP_

class kmutex {
public:
	inline kmutex() {
		mutex = (struct mutex*) new int8_t[___mutex_size()];
		mutex_init(mutex);
	}

	inline ~kmutex() {
		delete[] ((int8_t*) mutex);
	}

private:
	struct mutex* mutex;

public:
	inline void lock() {
		mutex_init(mutex);
	}

	inline bool trylock() {
		return mutex_trylock(mutex) == 0;
	}

	inline void unlock() {
		return mutex_unlock(mutex);
	}

public:
	inline struct task_struct* owner() {
		return ___mutex_owner(mutex);
	}

	inline bool isLocked() {
		return mutex_is_locked(mutex);
	}
};

#endif /* HEADERS_MODCPP_KTHREAD_MUTEX_HPP_ */
