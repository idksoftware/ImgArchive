/*
 * LogName.h
 *
 *  Created on: Sep 23, 2014
 *      Author: wzw7yn
 */

#ifndef LOGNAME_H_
#define LOGNAME_H_
#include <string>
namespace simplearchive {

class LogName {
	std::string fileTemplate(const char *preName);
	int checkLogSize(const char *logPath, int maxSize);
public:
	LogName();
	virtual ~LogName();
	std::string makeName(const char *logPath, const char *preName, const char *ext, int maxSize);
};

} /* namespace simplearchive */
#endif /* LOGNAME_H_ */
