/*
 * ImageLock.cpp
 *
 *  Created on: Sep 11, 2014
 *      Author: wzw7yn
 */

#include "ImageLock.h"
#include "SAUtils.h"
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
#ifdef WIN32
	errno_t err;
	FILE *source;
	fopen_s(&source, filelockname.c_str(), "w");
	if (err != 0) {
		return false;
	}
#else
	FILE *source = fopen(filelockname.c_str(), "w");
#endif
	fclose(source);
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
