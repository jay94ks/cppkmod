/*
 * koface.c
 *
 *  Created on: 2017. 12. 8.
 *      Author: jaehoon
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <modcpp.h>

static int koInitSucceed = 0;

static int koBaseInit(void) {
	int retVal = -1;

	//kthread_create();
	printk(KERN_INFO "cppkmod: The framework for developing the Kernel Module in C++\n"
			"Copyright (C) 2017 Jay K, jay94ks@gmail.com (jaehoon joe).\n"
			"-----\n"
			"This framework is under GPL, but the USER-SIDE source codes will not be enforced as GPL.\n"
			"So, DO NOT claim about the license of the framework and its problems to the Module's Owner.\n"
			"-----\n");

	if((retVal = koInit()) != 0)
		printk(KERN_ERR "cppkmod, %s: the module couldn't be loaded "
				"because of interlocking-part.\n", MODULE_NAME);
	else {
		printk(KERN_INFO "cppkmod, %s: the module has initialized successfully.\n", MODULE_NAME);
		koInitSucceed = 1;
		retVal = koMain();
	}

    return retVal;
}

static void koBaseCleanup(void) {
	if(koInitSucceed)
		koExit();

	if(koCleanup())
		printk(KERN_ERR "cppkmod, %s: the module couldn't be uninitialized "
				"because of interlocking-part.\n", MODULE_NAME);
	else printk(KERN_INFO "cppkmod, %s: the module has uninitialized successfully.\n", MODULE_NAME);
}

module_init(koBaseInit);
module_exit(koBaseCleanup);
