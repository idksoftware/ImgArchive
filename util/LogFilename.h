/*
 * LogFilename.h
 *
 *  Created on: Jun 16, 2014
 *      Author: wzw7yn
 */

#ifndef LOGFILENAME_H_
#define LOGFILENAME_H_
#include <string>
#include "ExifDate.h"

namespace simplearchive {

class LogFilename {
	std::string m_logfilePath;
	int m_maxSize; // in K
	int m_ver;
public:
	LogFilename(const char *logfilePath);
	virtual ~LogFilename();
	const std::string filename();
	std::string lastfile();

	int getMaxSize() const {
		return m_maxSize;
	}

	void setMaxSize(int maxSize) {
		m_maxSize = maxSize;
	}
};

} /* namespace simplearchive */
#endif /* LOGFILENAME_H_ */
