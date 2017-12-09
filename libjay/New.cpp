/*
 * New.cpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#include <modcpp.h>
#include <modcpp/base/policies.hpp>

#ifdef CPP_NEW_USE_VMALLOC
#define _TARGET_ALLOC_(sz, gfp)		vmalloc(sz)
#define _TARGET_RELEASE_(ptr)			vfree(ptr)
#else
#define _TARGET_ALLOC_(sz, gfp)		kmalloc(sz, gfp)
#define _TARGET_RELEASE_(ptr)			kfree(ptr)
#endif

#ifdef CPP_NEW_USE_TRYCOUNTER
#define TRYCOUNTER_PERFORM(x)			x
#else
#define TRYCOUNTER_PERFORM(x)
#endif

#ifdef CPP_NEW_USEWARN_NULLPTR
#define NULLPTRWARN_PERFORM(x)			x
#else
#define NULLPTRWARN_PERFORM(x)
#endif

void* operator_new_stub(size_t sz) {
	void* newptr = 0;
	TRYCOUNTER_PERFORM( int tryCounts = 0 );
	/*
	 * block the module with YIELD if no memory available.
	 * */
	while((newptr = _TARGET_ALLOC_(sz, CPP_NEW_ALLOCATION_GFP)) == 0) {
		TRYCOUNTER_PERFORM( tryCounts++ );
		::yield();
	}

	TRYCOUNTER_PERFORM(
		if(tryCounts) {
			printk(KERN_WARNING "cppkmod, %s: ::operator new...(%d): allocation took "
					"too much times. - %d times.\n", MODULE_NAME, sz, tryCounts);
		}
	);

	return newptr;
}

void operator_delete_stub(void* ptr) {
	if(ptr)
		_TARGET_RELEASE_(ptr);

	NULLPTRWARN_PERFORM(
		else printk(KERN_WARNING "cppmod, %s: ::operator delete(ptr): "
			"tried to release NULL pointer.\n", MODULE_NAME);
	);
}

void* operator new (size_t sz) {
	return operator_new_stub(sz);
}

void* operator new[] (size_t sz) {
	return operator_new_stub(sz);
}

void operator delete (void* ptr) {
	operator_delete_stub(ptr);
}

void operator delete[] (void* ptr) {
	operator_delete_stub(ptr);
}
