/*
 * stack.hpp
 *
 *  Created on: 2017. 12. 11.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_CONTAINERS_STACK_HPP_
#define HEADERS_MODCPP_CONTAINERS_STACK_HPP_

#include <modcpp/base/typedefs.hpp>
#include "allocator.hpp"

template< typename T >
struct stack_entity {
	uint8_t mem[sizeof(T)];
	struct entity* prev;
};

template< typename T, typename ALLOC = basic_allocator< stack_entity<T> > >
class stack {
private:
	typedef stack_entity<T> entity;

public:
	stack()
	: data(0), recycles(0), used(0), allocated(0),
	  strategy(E_ALLOC_DEFAULT)
	{

	}

	~stack() {
		while(pop());
		this->collapse();
	}

private:
	entity*			data;
	entity* 		recycles;
	size_t			used;
	size_t			allocated;
	EAllocStrategy	strategy;
	ALLOC			alloc;

public:
	inline stack<T, ALLOC>& setStrategy(EAllocStrategy strategy) {
		this->strategy = strategy;
		return *this;
	}

	inline EAllocStrategy getStrategy() { return strategy; }

public:
	void swap(stack<T, ALLOC>& other) {
		size_t used = this->used;
		size_t allocated = this->allocated;
		EAllocStrategy strategy = this->strategy;
		entity* data = this->data;
		entity* recycles = this->recycles;

		this->used = other.used;
		this->allocated = other.allocated;
		this->strategy = other.strategy;
		this->data = other.data;
		this->recycles = other.recycles;

		other.used = used;
		other.allocated = allocated;
		other.strategy = strategy;
		other.data = data;
		other.recycles = recycles;
	}

public:
	size_t size() const {
		return used;
	}

	size_t available() const {
		return allocated;
	}

	stack<T, ALLOC>& clear() {
		while(pop());
		return *this;
	}

public:
	inline bool push(const T& value) {
		if(allocated <= used && !expand(1))
			return false;

		entity* newbie = recycles;
		recycles = recycles->prev;

		new (newbie->mem) T(value);

		newbie->prev = data;
		data = newbie;

		used++;
		return true;
	}

	inline bool pop() {
		if(data) {
			entity* p = data;
			data = data->prev;
			used--;

			((T*) p->mem)->~T();

			p->prev = recycles;
			recycles = p;
			return true;
		}

		return false;
	}

	inline bool peek(T& value) {
		if(used) {
			new (&value) T(*((T*) data->mem));
			return true;
		}

		return false;
	}

public:
	inline bool expand(size_t count) {
		if(allocated >= used + count)
			return true;

		entity* newbie = 0;
		count = count * strategy;

		while(count > 0) {
			newbie = alloc.alloc(1);
			if(newbie) {
				newbie->prev = recycles;
				recycles = newbie;
			}

			else break;
			allocated++;
			count--;
		}

		return (count == 0);
	}

	inline bool collapse() {
		entity* p = 0;

		while(recycles) {
			p = recycles;
			recycles = recycles->prev;

			alloc.free(p);
			allocated--;
		}

		return (allocated == used);
	}

};


#endif /* HEADERS_MODCPP_CONTAINERS_STACK_HPP_ */
