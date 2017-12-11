/*
 * queue.hpp
 *
 *  Created on: 2017. 12. 11.
 *      Author: jaehoon
 */

#ifndef HEADERS_MODCPP_CONTAINERS_QUEUE_HPP_
#define HEADERS_MODCPP_CONTAINERS_QUEUE_HPP_

#include <modcpp/base/typedefs.hpp>
#include "allocator.hpp"

template< typename T >
struct queue_entity {
	uint8_t mem[sizeof(T)];
	struct entity* next;
};

template< typename T, typename ALLOC = basic_allocator< queue_entity<T> > >
class queue {
private:
	typedef queue_entity<T> entity;

public:
	queue()
	: _enqueue(0), _dequeue(0), used(0)
	{
	}

	~queue() {
	}

private:
	entity* _enqueue;
	entity* _dequeue;
	size_t used;
	ALLOC alloc;

public:
	inline bool enqueue(const T& value) {
		entity* newbie = alloc.alloc(1);

		if(newbie == 0)
			return false;

		new (newbie->mem) T(value);
		newbie->next = 0;

		if(_enqueue)
			_enqueue->next = newbie;

		_enqueue = newbie;
		if(!_dequeue)
			_dequeue = newbie;

		used++;
		return true;
	}

	inline bool dequeue(T& value) {
		if(_dequeue) {
			entity* p = _dequeue;
			_dequeue = _dequeue->next;

			new (&value) T(*((T*) p->mem));

			used--;
			alloc.free(p);
			return true;
		}

		return false;
	}

	inline size_t size() {
		return used;
	}
};

#endif /* HEADERS_MODCPP_CONTAINERS_QUEUE_HPP_ */
