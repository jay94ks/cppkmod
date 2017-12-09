/*
 * vmalloc.hpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_BASE_FORWARDS_VMALLOC_HPP_
#define HEADERS_MODCPP_BASE_FORWARDS_VMALLOC_HPP_

/*
 * Forwards vmalloc series into C++.
 * */
KMOD_FORWARD void *vmalloc(unsigned long size);
KMOD_FORWARD void *vzalloc(unsigned long size);
KMOD_FORWARD void *vmalloc_user(unsigned long size);
KMOD_FORWARD void *vmalloc_node(unsigned long size, int node);
KMOD_FORWARD void *vzalloc_node(unsigned long size, int node);
KMOD_FORWARD void *vmalloc_exec(unsigned long size);
KMOD_FORWARD void *vmalloc_32(unsigned long size);
KMOD_FORWARD void *vmalloc_32_user(unsigned long size);
KMOD_FORWARD void vfree(const void *addr);
KMOD_FORWARD void vfree_atomic(const void *addr);

#endif /* HEADERS_MODCPP_BASE_FORWARDS_VMALLOC_HPP_ */
