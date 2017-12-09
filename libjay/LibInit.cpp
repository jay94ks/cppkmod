/*
 * LibInit.cpp
 *
 *  Created on: 2017. 12. 9.
 *      Author: jaehoon
 */

#include <modcpp.h>

#include "../libjay/abi/AtExit.hpp"
#include "../libjay/abi/AtInit.hpp"
using namespace jayabi;

KMOD_FORWARD int koInit(void) {
	return AtInit::doInit();
}

KMOD_FORWARD int koCleanup(void) {
	return AtExit::cleanUp(0);
}
