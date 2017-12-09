/*
 * kmod_forward.hpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_BASE_KMOD_FORWARD_HPP_
#define HEADERS_MODCPP_BASE_KMOD_FORWARD_HPP_

#ifdef __cplusplus
#define KMOD_FORWARD		extern "C"
#else
#define KMOD_FORWARD		extern
#endif

#define KMOD_EXTERN		KMOD_FORWARD

#endif /* HEADERS_MODCPP_BASE_KMOD_FORWARD_HPP_ */
