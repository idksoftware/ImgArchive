/*
 * VerifyMirror.h
 *
 *  Created on: Dec 1, 2014
 *      Author: wzw7yn
 */

#ifndef VERIFYMIRROR_H_
#define VERIFYMIRROR_H_
#include <string>

namespace simplearchive {

class VerifyMirror {
	std::string makeFilePath(const char *fileStr);
public:
	VerifyMirror();
	virtual ~VerifyMirror();
	bool verifyMirror(const char *mirrorRoot, const char *chkRoot);
	bool verifySource(const char *sourceRoot, const char *chkRoot);
};

} /* namespace simplearchive */
#endif /* VERIFYMIRROR_H_ */
