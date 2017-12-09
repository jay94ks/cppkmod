/*
 * forwards.hpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_FORWARDS_HPP_
#define HEADERS_MODCPP_FORWARDS_HPP_

/*
 * for serving the macros for message levels.
 * */
#include <linux/kern_levels.h>

/*
 * Forwards the printk into C++ definitions.
 * */
KMOD_FORWARD int printk(const char* fmt, ...);

/*
 * Forwards the kmalloc and kfree into C++ definitions.
 * */
#include <modcpp/base/forwards/kmalloc.hpp>

/*
 * Forwards the vmalloc, vfree and its series into C++.
 * */
#include <modcpp/base/forwards/vmalloc.hpp>

/*
 * Forwards the yield method into C++.
 * */
KMOD_FORWARD void yield(void);

#endif /* HEADERS_MODCPP_FORWARDS_HPP_ */
