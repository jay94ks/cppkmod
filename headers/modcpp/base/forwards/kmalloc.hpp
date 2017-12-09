/*
 * kmalloc.hpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_BASE_FORWARDS_KMALLOC_HPP_
#define HEADERS_MODCPP_BASE_FORWARDS_KMALLOC_HPP_

/*
 * I've tried to include <linux/gfp.hpp> and exclude headers which are caused errors,...
 * but it couldn't be compiled well with C++ compilers.
 *
 * So, I created the alternative header for supporting GFP flags into kmalloc method.
 * it includes the preprocessors rather than special things, and it will rely on kernel definitions.
 * if the kernel declares that "Now, we will change the flag's meaning!",
 * 		alternative header must be modified for supporting correctly.
 * */
#include <modcpp/base/alternatives/gfp.hpp>

/*
 * free functions for the memory which allocated by kmalloc.
 * */
KMOD_FORWARD void kfree(const void* ptr);
KMOD_FORWARD void kzfree(const void* ptr);
KMOD_FORWARD void* krealloc(const void *, size_t, gfp_t);

/*
 * below function, ___kmalloc, call the kmalloc function.
 * it can have software overhead. see /base/kmalloc.c file for detail.
 * */
KMOD_FORWARD void* ___kmalloc(size_t sz, gfp_t type);

/*
 * forward above alternative into C++.
 * */
#define kmalloc(sz, type)		___kmalloc((sz), (type))

#endif /* HEADERS_MODCPP_BASE_FORWARDS_KMALLOC_HPP_ */
