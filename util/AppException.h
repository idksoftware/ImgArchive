/*
 * AppException.h
 *
 *  Created on: May 12, 2014
 *      Author: wzw7yn
 */

#ifndef APPEXCEPTION_H_
#define APPEXCEPTION_H_
#include <sstream>

namespace simplearchive {

class AppException : public std::exception {
	std::string s;

public:
	AppException(std::string ss) : s(ss) {}
	//AppException(const char *file, const int line, const char *msg) {
	//AppException(const int line, const char *file, const char *msg) {
	/*
		std::stringstream ss;
		ss << "Internal error: " << msg << file  << line;
		s = ss.str();
	*/
	//}
	virtual ~AppException() throw () ;
	const char *message() const throw() {
		return s.c_str();
	}
};

} /* namespace simplearchive */
#endif /* APPEXCEPTION_H_ */
