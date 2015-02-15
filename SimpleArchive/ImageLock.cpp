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

	FILE *source = fopen(filelockname.c_str(), "w");

	fclose(source);
	return true;
}

bool ImageLock::unlock(const char *filename) {
	std::string filelockname(filename);
	filelockname += ".lck";
	if (SAUtils::FileExists(filelockname.c_str()) == false) {
			return false; // already locked
	}
	unlink(filelockname.c_str());
	return true;
}

} /* namespace simplearchive */
