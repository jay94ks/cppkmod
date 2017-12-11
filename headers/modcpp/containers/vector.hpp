/*
 * vector.hpp
 *
 *  Created on: 2017. 12. 10.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_CONTAINERS_VECTOR_HPP_
#define HEADERS_MODCPP_CONTAINERS_VECTOR_HPP_

#include "allocator.hpp"

template < typename T, typename ALLOC = basic_allocator<T> >
class vector {
public:
	vector()
	: memory(0), allocated(0), used(0),
	  strategy(E_ALLOC_DEFAULT)
	{
	}

	vector(T* ptr, size_t count)
	: memory(0), allocated(0), used(0),
	  strategy(E_ALLOC_DEFAULT)
	{
		for(size_t i = 0; i < count; i++)
			this->add(*(ptr + i));
	}

	vector(const vector<T, ALLOC>& other)
	: memory(0), allocated(0), used(0),
	  strategy(E_ALLOC_DEFAULT)
	{
		for(size_t i = 0; i < other.size(); i++)
			this->add(other[i]);
	}

	template<typename ALLOC2>
	vector(const vector<T, ALLOC2>& other)
	: memory(0), allocated(0), used(0),
	  strategy(E_ALLOC_DEFAULT)
	{
		for(size_t i = 0; i < other.size(); i++)
			this->add(other[i]);
	}

	~vector() {
		this->clear();
		this->collapse();
	}

private:
	T*		memory;
	size_t	allocated;
	size_t	used;
	EAllocStrategy strategy;
	ALLOC	alloc;

public:
	inline vector<T, ALLOC>& setStrategy(EAllocStrategy strategy) {
		this->strategy = strategy;
		return *this;
	}

	inline EAllocStrategy getStrategy() { return strategy; }

public:
	void swap(vector<T, ALLOC>& other) {
		size_t a = allocated, u = used;
		EAllocStrategy s = strategy;
		T* p = memory;

		memory = other.memory;
		strategy = other.strategy;
		allocated = other.allocated;
		used = other.used;

		other.memory = p;
		other.strategy = s;
		other.allocated = a;
		other.used = u;
	}

public:
	T& operator [](size_t index) {
		return *(memory + index);
	}

	size_t size() const {
		return used;
	}

	size_t available() const {
		return allocated;
	}

	vector<T, ALLOC>& clear() {
		for(size_t i = 0; i < used; i++)
			(memory + i)->~T();

		used = 0;
		return *this;
	}

public:
	inline bool add(const T& value) {
		if(allocated < used + 1 && !expand(1))
			return false;

		new (memory + used) T (value);

		used++;
		return true;
	}

	inline bool remove(size_t index) {
		if(index >= used) return false;

		(memory + index)->~T();

		for(size_t i = index; i < used - 1; i++) {
			new (memory + i) T(*(memory + i + 1));
			(memory + i + 1)->~T();
		}

		used--;
		return true;
	}

	inline bool insert(size_t index, const T& value) {
		if(index >= used) index = used;
		if(allocated < used + 1 && !expand(1))
			return false;

		for(size_t i = used; i > index; i--) {
			new (memory + i) T(*(memory + i - 1));
			(memory + i - 1)->~T();
		}

		new (memory + index) T (value);
		used++;

		return true;
	}

public:
	inline bool expand(size_t count) {
		if(!memory) {
			memory = alloc.alloc(count);
			allocated = count;
			used = 0;
		}

		else if(allocated >= used + count)
			return true;

		T* newbie = alloc.alloc(used + count * strategy);

		if(newbie) {
			for(size_t i = 0; i < used; i++) {
				new (newbie + i) T(*(memory + i));
				(memory + i)->~T();
			}

			alloc.free(memory);

			memory = newbie;
			allocated = used + count * strategy;
			return true;
		}

		return false;
	}

	inline bool collapse() {
		if(!memory) {
			return false;
		}

		else if(used == 0) {
			alloc.free(memory);
			memory = 0;
			allocated = 0;
		}
		else if(allocated > used) {
			T* newbie = alloc.alloc(used);
			if(newbie) {
				for(size_t i = 0; i < used; i++) {
					new (newbie + i) T(*(memory + i));
					(memory + i)->~T();
				}

				alloc.free(memory);

				memory = newbie;
				allocated = used;
				return true;
			}
		}

		return false;
	}
};



#endif /* HEADERS_MODCPP_CONTAINERS_VECTOR_HPP_ */
