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
#include <iostream>
#include "SAUtils.h"
#include "SystemHistory.h"
#include "HistoryEvent.h"
#include "LogName.h"
#include "History.h"
//#include "CIDKDate.h"
#include "ExifDateTime.h"
#include "CSVArgs.h"
#include "ArchivePath.h"
#include "pathcontroller.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

class ImageHistoryItem {
	std::string m_comment;
	std::string m_date;
	std::string m_event;
	std::string m_fileShortPath;
	std::string m_data;
	std::string m_version;


public:
	ImageHistoryItem();
	/*
	 * 14.06.12.12.16.12:56:first checkin:checkin
	 */
	ImageHistoryItem(const char *dataString) {
		m_data = dataString;
		CSVArgs csvArgs(':');
		csvArgs.process(dataString);

	
		m_date = csvArgs.at(0);
		m_fileShortPath = csvArgs.at(1);
		m_version = csvArgs.at(2);
		m_comment = csvArgs.at(3);
		m_event = csvArgs.at(4);

	}

	ImageHistoryItem(const char *date, const char *fileShortPath, const char *version, const char *comment, const char *event) {
		m_date = date;
		m_version = version;
		m_comment = comment;
		m_fileShortPath = fileShortPath;
		m_event = event;
	}

	//bool add(const char *date, const char *version, int comment, const char *event);

	std::string &toString() {
		m_data = m_date + ":" + m_fileShortPath + ":" + m_version + ":" + m_comment + ":" + m_event;
		return m_data;
	}

	const char* getDateString() {
		return m_date.c_str();
	}
};


std::ofstream SystemHistory::m_hstfile;
std::string SystemHistory::m_filename;
std::string SystemHistory::m_currentFilename;
std::string SystemHistory::m_primary;
std::string SystemHistory::m_backup1;
std::string SystemHistory::m_backup2;
std::string SystemHistory::m_index;

SystemHistory::SystemHistory() {}

SystemHistory::~SystemHistory() {
	m_hstfile.close();
}

bool SystemHistory::init() {

	//LogName logName;
	//m_currentFilename = logName.makeName(m_primary.c_str(), "", "hst", 256);
	//m_index = m_primary;
	//m_index += '/'; m_index += logName.getFilename();
		
	/*
	if (ArchivePath::isMasterBackup1Enabled() == true) {
		m_backup1 = ArchivePath::getMasterBackup1().getSystemHistory();
		if (SAUtils::DirExists(m_backup1.c_str()) == false) {
			if (SAUtils::mkDir(m_backup1.c_str()) == false) {
				return false;
			}
		}
		m_backup1 += '/'; m_backup1 += logName.getFilename();
	}
	if (ArchivePath::isMasterBackup2Enabled() == true) {
		m_backup2 = ArchivePath::getMasterBackup2().getSystemHistory();
		if (SAUtils::DirExists(m_backup2.c_str()) == false) {
			if (SAUtils::mkDir(m_backup2.c_str()) == false) {
				return false;
			}
		}
		m_backup2 += '/'; m_backup2 += logName.getFilename();
	}
	*/
	return true;
}
/**
 * This function adds history to an image.
 */
/*
bool SystemHistory::add(const char *filepath, int version, const char *comment, const HistoryEvent &he) {
	std::string buff = SAUtils::sprintf("%.4d", version);
	add(filepath, buff.c_str(), comment, he);
	return true;
}
*/

bool SystemHistory::add(const char *filepath, const char *version, const char *comment, const HistoryEvent &he) {

	ExifDateTime date;
	date.now();
	const char *event = he.getString();
	ImageHistoryItem historyItem(date.toLogString().c_str(), filepath, version, comment, event);

	//if (add(historyItem, m_currentFilename.c_str()) == false) {
	//	return false;
	//}
	
	if (ArchivePath::isMasterEnabled() == true) {
		if (add(historyItem) == false) {
			return false;
		}
	}
	/*
	if (ArchivePath::isMasterBackup1Enabled() == true) {
		if (add(historyItem) == false) {
			return false;
		}
	}
	if (ArchivePath::isMasterBackup2Enabled() == true) {
		if (add(historyItem) == false) {
			return false;
		}
	}
	*/
	return true;
}

bool SystemHistory::add(ImageHistoryItem &historyItem) {
	
	ExifDateTime date;
	date.now();
	std::string dateStr = date.toLogString();

	std::string yearStr = dateStr.substr(0, 4);
	std::string monthStr = dateStr.substr(5, 2);
	std::string dayStr = dateStr.substr(8, 2);
	
	std::string addressStr = yearStr + monthStr + dayStr;

	if (SAUtils::DirExists(m_primary.c_str()) == false) {
			return false;
	}

	std::string indexPath = m_primary + '/' + yearStr;

	if (SAUtils::DirExists(indexPath.c_str()) == false) {
		if (SAUtils::mkDir(indexPath.c_str()) == false) {
			return false;
		}
	}

	indexPath += '/' + addressStr;

	if (SAUtils::DirExists(indexPath.c_str()) == false) {
		if (SAUtils::mkDir(indexPath.c_str()) == false) {
			return false;
		}
	}

	LogName logName;
	std::string currentFilename = logName.makeName(indexPath.c_str(), "", "hst", 1);

	indexPath += '/' + currentFilename;

	m_hstfile.open(indexPath.c_str(), std::ios::out | std::ios::app);
	if (m_hstfile.is_open() == false) {
		return false;
	}
	m_hstfile << historyItem.toString() << '\n';
	m_hstfile.close();

	return true;
}

std::shared_ptr<SystemHistoryLog>  SystemHistory::getEntries() {

	/*
	PathController indexController;
	indexController.setRoot(m_index.c_str());
	indexController.split(filepath);
	indexController.makeImagePath("hst");
	*/

	//std::string indexPath = indexController.getFullPath();;

	std::shared_ptr<SystemHistoryLog> log = std::make_shared<SystemHistoryLog>();

	
	if (readLog(m_primary.c_str(), *log) == false) {
		log = nullptr;
		return log;
	}
	/*
	log->setTitle(indexController.getImageName().c_str());
	log->setDescription(filepath);
	*/
	return log;
}

bool SystemHistory::readLog(const char* logFile, SystemHistoryLog& historyLog) {

	std::ifstream file(logFile);
	//file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	if (!file.is_open()) {
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		//std::cout << line << '\n';
		historyLog.push_back(line);
	}
	//HistoryLog
	return true;
}

/*
bool SystemHistory::read(const char *filepath) {
	char text[256];
	std::ifstream file(filepath);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		m_eventList->push_back(SystemHistoryItem(text));
	}
	file.close();

	return true;
}
bool SystemHistory::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<SystemHistoryItem>::iterator i = m_eventList->begin(); i != m_eventList->end(); i++) {
		SystemHistoryItem item = *i;
		std::string &line = item.toString();
		//printf("%s\n", line.c_str());
		file << line << '\n';

	}
	file.close();
	return true;
}
*/

/*
std::shared_ptr<SystemHistoryLog>  SystemHistory::get() {
	std::string str;
	std::shared_ptr<SystemHistoryLog> SystemHistoryLog(std::make_shared<SystemHistoryLog>());
	if (read(mpath.c_str()) == false) {
		return SystemHistoryLog;
	}

	for (std::vector<SystemHistoryItem>::iterator i = m_eventList->begin(); i != m_eventList->end(); i++) {
		SystemHistoryItem item = *i;
		std::string &line = item.toString();
		SystemHistoryLog->push_back(line);
	}
	return SystemHistoryLog;
}
*/
/*
const char *SystemHistory::getEventString(HistoryEvent he) {
	switch (he) {
	case IMPORT:
		return "added";
	case CHECKOUT:
		return "checkout";
	case CHECKIN:
		return "checkin";
	case EXPORT:
		return "export";
	case ERROR:
		return "error";
	case UNCHECKOUT:
		return "uncheckout";
	}
	return "error";
}
*/

/*
HistoryEvent SystemHistory::getEvent(const char *he) {
	std::string eventstr = he;
	if (eventstr.compare("add") == 0) {
		return IMPORT;
	} else if(eventstr.compare("checkout") == 0) {
		return CHECKOUT;
	} else if(eventstr.compare("checkin") == 0) {
		return CHECKIN;
	} else if(eventstr.compare("export") == 0) {
		return EXPORT;
	}
	return ERROR;
}
*/



} /* namespace simplearchive */
