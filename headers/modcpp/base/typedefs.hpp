/*
 * typedefs.hpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_TYPEDEFS_HPP_
#define HEADERS_MODCPP_TYPEDEFS_HPP_

typedef char					int8_t;
typedef short					int16_t;
typedef int					int32_t;
typedef long int				int64_t;

typedef unsigned char			uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int			uint32_t;
typedef unsigned long int	uint64_t;

typedef float					float32_t;
typedef double					float64_t;

typedef __SIZE_TYPE__			size_t;
#if __SIZEOF_SIZE_T__ == 8
typedef int64_t				ssize_t, offset_t;
#else
typedef int32_t				ssize_t, offset_t;
#endif

#ifdef __CHECKER__
#define __bitwise__ __attribute__((bitwise))
#else
#define __bitwise__
#endif

typedef unsigned __bitwise__ gfp_t;

#endif /* HEADERS_MODCPP_TYPEDEFS_HPP_ */
