/*
 * AppException.h
 *
 *  Created on: May 12, 2014
 *      Author: wzw7yn
 */

#ifndef APPEXCEPTION_H_
#define APPEXCEPTION_H_
#include <string>

namespace simplearchive {

struct AppException : public std::exception {
	std::string s;
	AppException(std::string ss) : s(ss) {};
	virtual ~AppException() throw () ;
	const char *message() const throw() {
		return s.c_str();
	}
};

} /* namespace simplearchive */
#endif /* APPEXCEPTION_H_ */
