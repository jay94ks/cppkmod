/*
 * modcpp.h
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_MODCPP_H_
#define HEADERS_MODCPP_MODCPP_H_

/*
 * definitions for forwards...
 * */
#include <modcpp/base/kmod_forward.hpp>

// these are for the framework.
KMOD_EXTERN int koInit(void);
KMOD_EXTERN int koCleanup(void);

// user implementations...
KMOD_EXTERN int koMain(void);
KMOD_EXTERN int koExit(void);

/*
 * The C++ source codes can access on the kernel's function with include preprocessors...
 * 	but, please DO NOT use directly without forwarding routines.
 * 	because it is VERY DANGEROUS for kernel-land. maybe it will damage your module's safety.
 * */

KMOD_FORWARD const char* __module_name;
#define MODULE_NAME		__module_name

#ifdef __cplusplus
#include <modcpp/base/typedefs.hpp>		// for strict type definitions...
#include <modcpp/base/forwards.hpp>		// function forwards...
#include <modcpp/base/allocation.hpp>		// allocation implements for c++ operators...
#endif

#endif /* HEADERS_MODCPP_MODCPP_H_ */
