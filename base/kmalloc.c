/*
 * kmalloc.c
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <linux/kthread.h>

typedef unsigned __bitwise__ gfp_t;

/*
 * the kmalloc(...) function defined as inline.
 * so, it cannot be used by 'extern "C"'
 *
 * NOTE: this can be software overhead during encapsulation.
 * 	we must find other way to use kmalloc instead of ___kmalloc in below.
 * */
void* ___kmalloc(size_t sz, gfp_t type) {
	return kmalloc(sz, type);
}
