/*
 * policies.hpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_BASE_POLICIES_HPP_
#define HEADERS_MODCPP_BASE_POLICIES_HPP_

/*
 * this macro controls the monitor variable for counting tries.
 * and it will warn the count if it took one or more times.
 * */
#define CPP_NEW_USE_TRYCOUNTER		1

/*
 * this macro controls the NULL pointer warning message into 'dmesg'.
 * if you want to turn it off, just comment it.
 * */
#define CPP_NEW_USEWARN_NULLPTR	1

/*
 * this macro controls the allocation method.
 * if you make this line to be uncommented,
 * 		c++ new operator will use vmalloc() instead of kmalloc() and
 * 		the CPP_NEW_ALLOCATION_GFP will be ignored.
 *
 * but, you must consider the memory corruption when you use kmalloc().
 * it will drive the kernel into critical situation which can stop entire of the system.
 * */
#define CPP_NEW_USE_VMALLOC		1

/*
 * this macro controls the allocation source in C++ new operators.
 * be careful if you want to modify this macro.
 *
 * WARN: the operator 'new' will try allocation until it success.
 *		 GFP_ATOMIC can cause the module blocked.
 *
 * NOTE: when CPP_NEW_USE_VMALLOC macro defined,
 * 		 this macro will be ignored and it will enforce the policy to be GFP_KERNEL.
 * */
#define CPP_NEW_ALLOCATION_GFP			GFP_KERNEL

//#define CPP_NEW_ALLOCATION_GFP		GFP_ATOMIC
//#define CPP_NEW_ALLOCATION_GFP		GFP_USER

#endif /* HEADERS_MODCPP_BASE_POLICIES_HPP_ */
