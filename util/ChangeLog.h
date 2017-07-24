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

#ifndef CHANGELOG_H_
#define CHANGELOG_H_
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include "HistoryEvent.h"

namespace simplearchive {

class ChangeLog {
public:
	static void setLogPath(const char *logpath) {
		m_logpath = logpath;
	}
	
	bool log(const char *filename, const char *version, const char *comment, HistoryEvent &he);
	ChangeLog();
	
	virtual ~ChangeLog();

	void ChangeLog::init();
private:

	
	
	ChangeLog(const ChangeLog& src);
	ChangeLog& operator=(const ChangeLog& rhs);




	static std::string m_filename;
	
	static std::ofstream m_logfile;
	static std::string m_logpath;
	
};

} /* namespace simplearchive */
#endif /* CLOGGER_H_ */
