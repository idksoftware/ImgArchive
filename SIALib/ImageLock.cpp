/*
 * ImageLock.cpp
 *
 *  Created on: Sep 11, 2014
 *      Author: wzw7yn
 */

#include "ImageLock.h"
#include "SAUtils.h"
#include <fstream>
#include <stdio.h>

namespace simplearchive {

ImageLock::ImageLock() {
	// TODO Auto-generated constructor stub

}

ImageLock::~ImageLock() {
	// TODO Auto-generated destructor stub
}
bool ImageLock::isLocked(const char *filename) {
	std::string filelockname(filename);
	filelockname += ".lck";
	if (SAUtils::FileExists(filelockname.c_str()) == true) {
		return true;
	}
	return false;
}

bool ImageLock::lock(const char *filename) {
	std::string filelockname(filename);
	filelockname += ".lck";
	if (SAUtils::FileExists(filelockname.c_str()) == true) {
		return false; // already locked
	}

	std::ofstream file(filelockname.c_str());
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	file.close();
	return true;
}

bool ImageLock::unlock(const char *filename) {
	std::string filelockname(filename);
	filelockname += ".lck";
	if (SAUtils::FileExists(filelockname.c_str()) == false) {
			return false; // already locked
	}
#ifdef WIN32
	_unlink(filelockname.c_str());
#else
	unlink(filelockname.c_str());
#endif
	return true;
}

} /* namespace simplearchive */
