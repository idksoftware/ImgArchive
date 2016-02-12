/* **************************************************
**
**    III                DDD  KKK
**    III                DDD  KKK
**                       DDD  KKK
**    III   DDDDDDDDDDD  DDD  KKK            KKK
**    III  DDD           DDD  KKK            KKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK   KKKKKKKKK
**    III  DDD           DDD  KKK        KKKKKK
**    III  DDD           DDD  KKK           KKK
**    III  DDD           DDD  KKK            KKK
**    III   DDDDDDDDDDDDDDDD  KKK            KKK
**
**
**     SSS         FF
**    S           F   T
**     SSS   OO   FF  TTT W   W  AAA  R RR   EEE
**        S O  O  F   T   W W W  AAAA RR  R EEEEE
**    S   S O  O  F   T   W W W A   A R     E
**     SSS   OO  FFF   TT  W W   AAAA R      EEE
**
**    Copyright: (c) 2015 IDK Software Ltd
**
****************************************************
**
**	Filename	: CRegString.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include "ImageLock.h"
#include "SAUtils.h"
#include <fstream>
#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

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
#ifdef _WIN32
	_unlink(filelockname.c_str());
#else
	unlink(filelockname.c_str());
#endif
	return true;
}

} /* namespace simplearchive */
