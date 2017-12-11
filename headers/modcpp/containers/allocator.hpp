/*
 * allocator.hpp
 *
 *  Created on: 2017. 12. 10.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_CONTAINERS_ALLOCATOR_HPP_
#define HEADERS_MODCPP_CONTAINERS_ALLOCATOR_HPP_

#include <modcpp/base/typedefs.hpp>
#include <modcpp/base/forwards.hpp>

enum EAllocStrategy {
	E_ALLOC_DEFAULT = 2,
	E_ALLOC_NORMAL = 1,
	E_ALLOC_DOUBLE = 2,
	E_ALLOC_QUADRA = 4,
	E_ALLOC_OCTA = 8
};

template<typename T>
class basic_allocator {
public:
	T* alloc (size_t count) {
		return (T*) new uint8_t[count * sizeof(T)];
	}

	void free (T* ptr) {
		delete[] ((uint8_t*) ptr);
	}
};

template<typename T, gfp_t type>
class kmalloc_allocator {
public:
	T* alloc(size_t count) {
		return (T*) kmalloc(count * sizeof(T), type);
	}

	void free(T* ptr) {
		kfree(ptr);
	}
};

template<typename T>
class vmalloc_allocator {
public:
	T* alloc(size_t count) {
		return (T*) vmalloc(count * sizeof(T));
	}

	void free(T* ptr) {
		vfree(ptr);
	}
};

#endif /* HEADERS_MODCPP_CONTAINERS_ALLOCATOR_HPP_ */
