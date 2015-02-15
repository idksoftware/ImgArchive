/*
 * ChangeLog.h
 *
 *  Created on: May 21, 2014
 *      Author: wzw7yn
 */

#ifndef CHANGELOG_H_
#define CHANGELOG_H_
#include <fstream>
#include <iostream>
#include "HistoryEvent.h"

namespace simplearchive {

class ChangeLog {
public:
	static void setLogPath(const char *logpath) {
		m_logpath = logpath;
	}
	static ChangeLog &getLogger();
	bool log(const char *filename, const char *version, const char *comment, HistoryEvent &he);
private:

	ChangeLog();
	ChangeLog(const ChangeLog&) {};
	ChangeLog& operator = (const ChangeLog& ) { return *this; }

	static std::string m_filename;
	static ChangeLog* m_this;
	static std::ofstream m_logfile;
	static std::string m_logpath;
	virtual ~ChangeLog();
};

} /* namespace simplearchive */
#endif /* CLOGGER_H_ */
