/*
 * error.hpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_BASE_ALTERNATIVES_ERROR_HPP_
#define HEADERS_MODCPP_BASE_ALTERNATIVES_ERROR_HPP_
//#include <linux/err.h>

#define IS_ERR_VALUE(x) ((x) >= (unsigned long)-4095)

static inline void * ERR_PTR(long error)
{
	return (void *) error;
}

static inline long PTR_ERR(const void *ptr)
{
	return (long) ptr;
}

static inline long IS_ERR(const void *ptr)
{
	return IS_ERR_VALUE((unsigned long)ptr);
}

static inline long IS_ERR_OR_NULL(const void *ptr)
{
	return !ptr || IS_ERR_VALUE((unsigned long)ptr);
}

#endif /* HEADERS_MODCPP_BASE_ALTERNATIVES_ERROR_HPP_ */
