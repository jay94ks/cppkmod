/*
 * forwards.hpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_KTHREAD_FORWARDS_HPP_
#define HEADERS_MODCPP_KTHREAD_FORWARDS_HPP_

#include <modcpp/base/alternatives/error.hpp>
#include <modcpp/base/kmod_forward.hpp>

/*
 * forwarded from /usr/src/kernel-header.../build/include/linux/kthread.h
 * Forward targets are minimum requirements. if you want more, you can issue that through github.
 * */
KMOD_FORWARD int wake_up_process(struct task_struct *tsk);
KMOD_FORWARD struct task_struct *kthread_create_on_node(
		int (*threadfn)(void *data), void *data, int node, const char namefmt[], ...);

#define kthread_create(threadfn, data, namefmt, arg...) \
	kthread_create_on_node(threadfn, data, -1, namefmt, ##arg)

#define kthread_run(threadfn, data, namefmt, ...)			   \
({									   \
	struct task_struct *__k						   \
		= kthread_create(threadfn, data, namefmt, ## __VA_ARGS__); \
	if (!IS_ERR(__k))						   \
		wake_up_process(__k);					   \
	__k;								   \
})

KMOD_FORWARD void free_kthread_struct(struct task_struct *k);
KMOD_FORWARD void kthread_bind(struct task_struct *k, unsigned int cpu);
KMOD_FORWARD void kthread_bind_mask(struct task_struct *k, const struct cpumask *mask);
KMOD_FORWARD int kthread_stop(struct task_struct *k);
KMOD_FORWARD bool kthread_should_stop(void);
KMOD_FORWARD bool kthread_should_park(void);
KMOD_FORWARD bool kthread_freezable_should_stop(bool *was_frozen);
KMOD_FORWARD void *kthread_data(struct task_struct *k);
KMOD_FORWARD void *kthread_probe_data(struct task_struct *k);
KMOD_FORWARD int kthread_park(struct task_struct *k);
KMOD_FORWARD void kthread_unpark(struct task_struct *k);
KMOD_FORWARD void kthread_parkme(void);

KMOD_FORWARD int kthreadd(void *unused);

/// MUTEX

KMOD_FORWARD int ___mutex_size();

KMOD_FORWARD void __mutex_init(struct mutex *lock,
		const char *name, struct lock_class_key *key);

KMOD_FORWARD int ___mutex_init(struct mutex* lock);

#define mutex_init(lock) ___mutex_init(lock);

KMOD_FORWARD void mutex_lock(struct mutex *lock);
KMOD_FORWARD int mutex_lock_interruptible(struct mutex *lock);
KMOD_FORWARD int mutex_lock_killable(struct mutex *lock);
KMOD_FORWARD void mutex_lock_io(struct mutex *lock);
KMOD_FORWARD int mutex_trylock(struct mutex *lock);
KMOD_FORWARD void mutex_unlock(struct mutex *lock);

#define mutex_lock_nested(lock, subclass) mutex_lock(lock)
#define mutex_lock_interruptible_nested(lock, subclass) mutex_lock_interruptible(lock)
#define mutex_lock_killable_nested(lock, subclass) mutex_lock_killable(lock)
#define mutex_lock_nest_lock(lock, nest_lock) mutex_lock(lock)
#define mutex_lock_io_nested(lock, subclass) mutex_lock(lock)

static inline void mutex_destroy(struct mutex *lock) {}
KMOD_FORWARD struct task_struct* ___mutex_owner(struct mutex *lock);

static inline int mutex_is_locked(struct mutex *lock)
{
	/*
	 * XXX think about spin_is_locked
	 */
	return ___mutex_owner(lock) != 0;
}


enum {
	MUTEX_TRYLOCK_FAILED    = 0,
	MUTEX_TRYLOCK_SUCCESS   = 1,
	MUTEX_TRYLOCK_RECURSIVE,
};

#endif /* HEADERS_MODCPP_KTHREAD_FORWARDS_HPP_ */
