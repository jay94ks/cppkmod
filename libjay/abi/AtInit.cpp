/*
 * AtInit.cpp
 *
 *  Created on: 2017. 12. 10.
 *      Author: jaehoon
 */

#include "../../libjay/abi/AtInit.hpp"

#include <modcpp.h>
using namespace jayabi;

/*
 * .init_array section's exported symbols.
 * */
KMOD_EXTERN AtInit::constructor_t	__jayabi_init_array_start,
									__jayabi_init_array_end;

int AtInit::doInit() {
	constructor_t *ctor = &__jayabi_init_array_start;

	printk(KERN_INFO "libjay, %s: calling global constructors...\n", MODULE_NAME);

	for ( ; ctor < &__jayabi_init_array_end; ++ctor ) {
		(*ctor)();
	}

	printk(KERN_INFO "libjay, %s: initializing completed.\n", MODULE_NAME);
	return 0;
}
