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

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstdarg>
#include "ChangeLog.h"
#include "ExifDateTime.h"
#include "LogFilename.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

using namespace std;
namespace simplearchive {

class ChangeLogItem {
	std::string m_comment;
	std::string m_filePath;
	std::string m_date;
	std::string m_event;
	std::string m_data;
	std::string m_version;
public:
	ChangeLogItem();
	/*
	 * 14.06.12.12.16.12:56:file:first checkin:checkin
	 */
	ChangeLogItem(const char *dataString) {
		m_data = dataString;
		int delim1 = m_data.find_first_of(':');
		int delim2 = m_data.find_first_of(':', delim1+1);
		int delim3 = m_data.find_first_of(':', delim2+1);
		int delim4 = m_data.find_first_of(':', delim3 + 1);
		m_date = m_data.substr(0,delim1);
		m_filePath = m_data.substr(delim1+1, (delim2-delim1)-1);
		m_version = m_data.substr(delim2 + 1, (delim3 - delim2) - 1);
		m_comment = m_data.substr(delim3+1, (delim4-delim3)-1);
		m_event = m_data.substr(delim4+1, m_data.length());

	}

	ChangeLogItem(const char *date, const char *filePath, const char *version, const char *comment, const char *event) {
		m_date = date;
		m_filePath = filePath;
		m_version = version;
		m_comment = comment;
		m_event = event;
	}

	//bool add(const char *date, const char *version, int comment, const char *event);

	std::string &toString() {
		m_data = m_date + ":" + m_filePath + ":" + m_version + ":" + m_comment + ":" + m_event;
		return m_data;
	}

};

std::unique_ptr<ChangeLog> ChangeLog::m_this;
std::once_flag ChangeLog::m_onceFlag;

std::string ChangeLog::m_filename;
std::ofstream ChangeLog::m_logfile;
std::string ChangeLog::m_logpath;

ChangeLog::ChangeLog() {

}

ChangeLog &ChangeLog::getLogger() {

	std::call_once(m_onceFlag,
			[] {
		m_this.reset(new ChangeLog);
		LogFilename logFilename(m_logpath.c_str());
		m_filename = logFilename.filename();
		m_logfile.open(m_filename.c_str(), ios::out | ios::app);
	});

	return *m_this.get();

}

ChangeLog::~ChangeLog() {
	m_logfile.close();
}

bool ChangeLog::log(const char *filepath, const char *version, const char *comment, HistoryEvent &he) {
	std::string filepathstr = filepath;
	std::string filename;
	std::string path;
	int slashpos = filepathstr.find_last_of("/");
	if (slashpos != -1) {
		filename = filepathstr.substr(slashpos+1, filepathstr.length() - slashpos);
		path = filepathstr.substr(0, slashpos);
	} else {
		filename = filepathstr;
	}
	ExifDateTime exifDateTime;
	std::string date = exifDateTime.toLogString();
	const char *event = he.getString();
	ChangeLogItem *changeLogItem = new ChangeLogItem(date.c_str(), filename.c_str(), version, comment, event);
	m_logfile << changeLogItem->toString().c_str() << '\n';

	return true;
}
} /* namespace simplearchive */
