/*
 * atexit.hpp
 *
 *  Created on: 2017. 12. 10.
 *      Author: jaehoon
 */

#ifndef LIBJAY_ABI_ATEXIT_HPP_
#define LIBJAY_ABI_ATEXIT_HPP_

#include <modcpp.h>
#include <modcpp/kthread.hpp>

namespace jayabi {
	class AtExit {
	protected:
		AtExit();
		~AtExit();

	private:
		AtExit* next;
		AtExit* prev;

	public:
		typedef void (* destructor_t)(void* obj);

	private:
		destructor_t	dtor;
		void*			target;
		void*			handle;

	private:
		static AtExit*	front;
		static AtExit*	last;
		static kmutex*	mutex;

	private:
		void append(AtExit* newbie);

	public:
		/*
		 * this method will be refered by koCleanup().
		 * */
		static int registerDtor(destructor_t dtor, void* obj, void* handle);
		static int cleanUp(void* handle);
	};
}

#endif /* LIBJAY_ABI_ATEXIT_HPP_ */
