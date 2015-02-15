/*
 * SetEnv.cpp
 *
 *  Created on: Oct 30, 2014
 *      Author: wzw7yn
 */

#include "SetEnv.h"
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#endif

namespace simplearchive {

SetEnv::SetEnv() {
	// TODO Auto-generated constructor stub

}

SetEnv::~SetEnv() {
	// TODO Auto-generated destructor stub
}

void SetEnv::process() {
	for (std::vector<EnvItem>::iterator i = begin(); i != end(); i++) {
		EnvItem &data = *i;
#ifdef WIN32
		//::SetEnvironmentVariable(data.getName().c_str(), data.getValue().c_str());
		std::string tmp = data.getName() + '=' + data.getValue().c_str();
		_putenv(tmp.c_str());

#else
		setenv(data.getName().c_str(),data.getValue().c_str(), true);
#endif
	}
}

} /* namespace simplearchive */
