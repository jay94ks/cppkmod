/*
 * purevirtual.cpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#include <modcpp.h>
#include <modcpp/kthread.hpp>

KMOD_EXTERN void __cxa_pure_virtual() {
	printk(KERN_ERR
		"cppkmod, %s, __cxa_pure_virtual(): the abstract method executed.\n"
		" * this can cause \"kernel panic\" if this perform core actions.\n",
		MODULE_NAME);
}
