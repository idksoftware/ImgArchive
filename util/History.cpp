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
#include <vector>
#include <fstream>

#include <streambuf>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstdarg>
#include "History.h"
#include "LogName.h"
#include "ExifDateTime.h"
#include "SAUtils.h"
#include "HistoryLog.h"
#include "CSVArgs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {
class HistoryItem {
	std::string m_comment;
	std::string m_date;
	std::string m_filepath;
	std::string m_event;
	std::string m_data;
	std::string m_version;
public:
	HistoryItem();
	/*
	 * 14.06.12.12.16.12:56:first checkin:checkin
	 */
	HistoryItem(const char *dataString) {
		m_data = dataString;
		CSVArgs csvArgs(':');
		csvArgs.process(dataString);
		m_date = csvArgs.at(0);
		m_version = csvArgs.at(1);
		m_filepath = csvArgs.at(2);
		m_comment = csvArgs.at(3);
		m_event = csvArgs.at(4);
		/*
		int delim1 = m_data.find_first_of(':');
		int delim2 = m_data.find_first_of(':', delim1+1);
		int delim3 = m_data.find_first_of(':', delim2+1);
		int delim4 = m_data.find_first_of(':', delim2+1);
		m_date = m_data.substr(0,delim1);
		m_version = m_data.substr(delim1+1, (delim2-delim1)-1);
		m_filepath = m_data.substr(delim2+1, (delim3-delim2)-1);
		m_comment = m_data.substr(delim3+1, (delim4-delim3)-1);
		m_event = m_data.substr(delim4+1, m_data.length());
		*/

	}

	HistoryItem(const char *date, const char *version, const char *filepath, const char *comment, const char *event) {
		m_date = date;
		m_version = version;
		m_filepath = filepath;
		m_comment = comment;
		m_event = event;
	}

	//bool add(const char *date, const char *version, int comment, const char *event);

	std::string &toString() {
		m_data = m_date + ":" + m_version + ":" + m_filepath +":" + m_comment +":" + m_event;
		return m_data;
	}

};
class EventList : public std::vector<HistoryItem> {};


bool History::m_isOpen = false;

History::History() {
	m_eventList = new EventList;

}

History::~History() {
	if (m_isOpen) {
		m_logfile.close();
	}
	delete m_eventList;
}

std::string History::m_filename = "";
std::auto_ptr<History> History::m_this(0);
std::ofstream History::m_logfile;
std::string History::m_folder;

void History::setPath(const char *path) {
	LogName logName;
	m_folder = path;
	m_filename = logName.makeName(path, "hist", "log", 256);
}

History &History::getHistory() {

	if (!m_this.get()) {
		m_this.reset(new History());
		m_logfile.open(m_filename.c_str(), std::ios::out | std::ios::app);
		if (m_logfile.is_open()) {
			m_isOpen = true;
		}
	}
	return *(m_this.get());
}


/**
 * This function adds history to an image.
 */
bool History::add(const char *filepath, const char *version, const char *comment, const HistoryEvent &he) {
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
	//read(m_filename.c_str());
	ExifDateTime date;
	date.now();
	//CDate dateNow = CDate::timeNow();
	std::string dateStr = date.current();
	//std::string date =
	const char *event = he.getString();
	HistoryItem historyItem(dateStr.c_str(), filepath, version, comment, event);
	m_logfile << historyItem.toString() << '\n';

	return true;
}

bool History::read(const char *filepath) {
	char text[256];
	std::ifstream file(filepath);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		m_eventList->push_back(HistoryItem(text));
	}
	file.close();

	return true;
}

/*
std::string History::getHistory(CDate from, CDate to) {
	std::string str;
	return str;
}
*/
std::auto_ptr<HistoryLog>  History::getEntries(int daysAgo) {
	CDate date = CDate::daysAgo(daysAgo);

	std::vector<std::string> fileList;
	std::string filepath = "X";
	FileList_Ptr filelist = SAUtils::getFiles_(m_folder.c_str());
	std::string dateString = LogName::dateString(date);
	std::auto_ptr<HistoryLog> historyLog(new HistoryLog);
	for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string logFile = *i;

		if (logFile.at(0) == '.') {
			continue;
		}
		std::string logFileNoExt = SAUtils::getFilenameNoExt(logFile.c_str());
		std::string datePart = logFileNoExt.substr(4,8);
		//printf("datePart: %s dateString: %s\n", datePart.c_str(), dateString.c_str());
		if (dateString.compare(datePart) == 0) {
			//printf("%s\n", logFile->c_str());
			readLog(logFile.c_str(), *historyLog);

		}
	}
	return historyLog;
}

bool History::readLog(const char *logFile, HistoryLog &historyLog) {
	std::string path = m_folder;
	path += '/';
	path += logFile;
	std::ifstream file(path.c_str());
	//file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	if (!file.is_open()) {
		return false;
	}
	std::string line;
	while(getline(file, line)) {
		//std::cout << line << '\n';
		historyLog.push_back(line);
	}
	//HistoryLog
	return true;
}
/*
std::string History::getHistory(int entrys) {
	std::string str;
	return str;
}
*/


} /* namespace simplearchive */
