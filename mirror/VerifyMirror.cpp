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

#include <string>
#include <stdio.h>
#include "VerifyMirror.h"
#include "SAUtils.h"
#include "CheckDisk.h"


namespace simplearchive {

VerifyMirror::VerifyMirror() {
	// TODO Auto-generated constructor stub

}

VerifyMirror::~VerifyMirror() {
	// TODO Auto-generated destructor stub
}

bool VerifyMirror::verifyMirror(const char *mirrorRoot, const char *chkRoot) {
	CheckDisk checkDisk;
	// Read the list of check files
	std::vector<std::string *> *filelist = SAUtils::getFiles(chkRoot);
	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string *name = *i;
			std::string dirname = *name;
			std::string chkpath = mirrorRoot;
			chkpath += "/.chk/";
			chkpath += dirname.c_str();
			std::string filesPath = mirrorRoot;
			filesPath += makeFilePath(dirname.c_str());
			if (checkDisk.check(filesPath.c_str(), chkpath.c_str()) == true) {
				// True == there are errors
				printf("Errors found in %s", filesPath.c_str());
				return false;
			}
	}
	return true;
}

bool VerifyMirror::verifySource(const char *mirrorRoot, const char *chkRoot) {
	bool errors = false;
	CheckDisk checkDisk;
	std::vector<std::string *> *filelist = SAUtils::getFiles(chkRoot);
	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string *name = *i;
			if (name->compare(".") == 0 || name->compare("..") == 0 ) {
				continue;
			}
			std::string dirname = *name;
			std::string chkpath = chkRoot;
			chkpath += "/";
			chkpath += dirname.c_str();
			std::string filesPath = mirrorRoot;
			filesPath += makeFilePath(dirname.c_str());
			if (checkDisk.check(filesPath.c_str(), chkpath.c_str()) == false) {
				// True == there are errors
				printf("Verify: Errors found in %s\n", filesPath.c_str());
				errors = true;
			}
	}
	return (!errors);
}

#define DIR_DELIM '@'
std::string VerifyMirror::makeFilePath(const char *fileStr) {
	std::string tmp = fileStr;
	std::string out;
	if (tmp.length() == 0) {
		out.insert(out.end(), DIR_DELIM);
	}
	for (std::string::iterator i = tmp.begin(); i != tmp.end(); i++) {
		char c = *i;
		if (c == DIR_DELIM) {
			c = '/';
		//} else if (c == DIR_DELIM) {
		//	out.insert(out.end(), '?');
		}
		out.insert(out.end(), c);
	}
	printf("dir %s\n", out.c_str());
	return out;
}

} /* namespace simplearchive */
