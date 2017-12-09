/*
 * AtExit.cpp
 *
 *  Created on: 2017. 12. 10.
 *      Author: jaehoon
 */

#include "../../libjay/abi/AtExit.hpp"
using namespace jayabi;

/*
 * the list.
 * */
AtExit* AtExit::front = 0;
AtExit* AtExit::last = 0;

/*
 * mutex for ensuring mutual exclusion.
 * */
kmutex* AtExit::mutex = 0;

/*
 * dummy DSO handle...
 * */
KMOD_EXTERN { void* __dso_handle = 0; }

/*
 * this method will be exported for g++ generated codes.
 * */
KMOD_EXTERN int __cxa_atexit(AtExit::destructor_t dtor, void* obj, void* handle) {
	return AtExit::registerDtor(dtor, obj, handle);
}

AtExit::AtExit()
: next(0), prev(0), dtor(0), target(0), handle(0)
{
}

AtExit::~AtExit() {
	if(next)
		next->prev = prev;

	if(prev)
		prev->next = next;

	if(this == last)
		last = this->prev;

	if(this == front)
		front = this->next;
}

void AtExit::append(AtExit* newbie) {
	newbie->next = next;
	newbie->prev = this;

	if(next)
		next->prev = newbie;

	next = newbie;

	if(this == last)
		last = newbie;
}

int AtExit::registerDtor(destructor_t dtor, void* obj, void* handle) {
	AtExit* newbie = new AtExit();

	newbie->dtor = dtor;
	newbie->target = obj;
	newbie->handle = handle;

	if(!mutex)
		mutex = new kmutex();

	mutex->lock();
	if(last)
		last->append(newbie);

	if(!front)
		front = newbie;
	mutex->unlock();

	return 0;
}

int AtExit::cleanUp(void* handle) {
	AtExit* entity = 0;
	AtExit* tmp = 0;

	if(!handle) {
		printk(KERN_INFO "libjay, %s: calling global destructors...\n", MODULE_NAME);
	}

	if(front) {
		mutex->lock();

		entity = front;
		while(entity) {
			tmp = entity;
			entity = entity->next;

			if(tmp->dtor && (handle == 0 ||
					tmp->handle == handle))
			{
				(*tmp->dtor)(tmp->target);
				delete tmp;
			}
		}

		mutex->unlock();
	}

	if(!front) {
		delete mutex;
		mutex = 0;
	}

	if(!handle) {
		printk(KERN_INFO "libjay, %s: uninitializing completed.\n", MODULE_NAME);
	}

	return 0;
}
