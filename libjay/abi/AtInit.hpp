/*
 * AtInit.hpp
 *
 *  Created on: 2017. 12. 10.
 *      Author: jaehoon
 */

#ifndef LIBJAY_ABI_ATINIT_HPP_
#define LIBJAY_ABI_ATINIT_HPP_

namespace jayabi {
	class AtInit {
	public:
		typedef void (*constructor_t)(void);

	public:
		static int doInit();
	};
}

#endif /* LIBJAY_ABI_ATINIT_HPP_ */
