/*
 * ManagedMirror.h
 *
 *  Created on: Jan 19, 2015
 *      Author: wzw7yn
 */

#ifndef MANAGEDMIRROR_H_
#define MANAGEDMIRROR_H_
#include <string>

namespace simplearchive {

class ManagedMirror {
	std::string m_rootFolder;
	std::string m_configFile;
public:
	ManagedMirror();
	virtual ~ManagedMirror();
	bool initalise(const char *rootFolder, const char *configFile);
	bool process();
};

} /* namespace simplearchive */
#endif /* MANAGEDMIRROR_H_ */
