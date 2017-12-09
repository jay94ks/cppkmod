/*
 * allocation.hpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_ALLOCATION_HPP_
#define HEADERS_MODCPP_ALLOCATION_HPP_

/*
 * the build-system will enforce it to be following C++11 rather than C++14 or highers.
 *
 * C++11 defines "new" operators:
 * throwing		(1) void* operator new (std::size_t size);
 * nothrow		(2) void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) noexcept;
 * placement	(3) void* operator new (std::size_t size, void* ptr) noexcept;
 *
 * "delete" operators:
 * ordinary		(1) void operator delete (void* ptr) noexcept;
 * nothrow		(2) void operator delete (void* ptr, const std::nothrow_t& nothrow_constant) noexcept;
 * placement	(3) void operator delete (void* ptr, void* voidptr2) noexcept;
 *
 * # the array-tone operators use same form for definitions and
 * 	this framework will not define "nothrow" versions.
 * */

/*
 * placement operators:
 * */
inline void* operator new (size_t, void* p) { return p; }
inline void* operator new [](size_t, void* p) { return p; }

inline void operator delete(void*, void*) { }
inline void operator delete [](void*, void*) { }

/*
 * normal operators:
 * */
void* operator new (size_t);
void* operator new[] (size_t);

void operator delete (void*);
void operator delete[] (void*);

#endif /* HEADERS_MODCPP_ALLOCATION_HPP_ */
