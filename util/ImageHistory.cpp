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
#include "ImageHistory.h"
#include "HistoryEvent.h"
#include "LogName.h"
#include "CIDKDate.h"
#include "CSVArgs.h"
#include "ArchivePath.h"
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

};

ImageHistory *ImageHistory::m_this = nullptr;
std::ofstream ImageHistory::m_hstfile;
std::string ImageHistory::m_filename;
std::string ImageHistory::m_primary;
std::string ImageHistory::m_backup1;
std::string ImageHistory::m_backup2;
std::string ImageHistory::m_shadow;

ImageHistory::ImageHistory() {}

ImageHistory::~ImageHistory() {
	m_hstfile.close();
}

ImageHistory &ImageHistory::getImageHistory() {
	if (m_this == NULL) {
		m_this = new ImageHistory();
		
		std::string primary = ArchivePath::getMainHistory();
		LogName logName;
		m_primary = logName.makeName(primary.c_str(), "", "hst", 256);
		if (ArchivePath::isShadowEnabled() == true) {
			m_shadow = ArchivePath::getShadow().getLogHistory();
			m_shadow += '/'; m_shadow += logName.getFilename();
		}
		if (ArchivePath::isBackup1Enabled() == true) {
			m_backup1 = ArchivePath::getBackup1().getLogHistory();
			m_backup1 += '/'; m_backup1 += logName.getFilename();
		}
		if (ArchivePath::isBackup2Enabled() == true) {
			m_backup2 = ArchivePath::getBackup2().getLogHistory();
			m_backup2 += '/'; m_backup2 += logName.getFilename();
		}
		
		
	}
	return *m_this;
}
/**
 * This function adds history to an image.
 */
bool ImageHistory::add(const char *filepath, int version, const char *comment, const HistoryEvent &he) {
	char buff[50];
	sprintf_s(buff, 50, "%.4d", version);
	add(filepath, buff, comment, he);
	return true;
}

bool ImageHistory::add(const char *filepath, const char *version, const char *comment, const HistoryEvent &he) {
	CIDKDate date;
	date.Now();
	const char *event = he.getString();
	ImageHistoryItem historyItem(date.Print().c_str(), filepath, version, comment, event);
	if (add(historyItem, m_primary.c_str()) == false) {
		return false;
	}
	
	if (ArchivePath::isShadowEnabled() == true) {
		if (add(historyItem, m_shadow.c_str()) == false) {
			return false;
		}
	}
	if (ArchivePath::isBackup1Enabled() == true) {
		if (add(historyItem, m_backup1.c_str()) == false) {
			return false;
		}
	}
	if (ArchivePath::isBackup2Enabled() == true) {
		if (add(historyItem, m_backup2.c_str()) == false) {
			return false;
		}
	}
	return true;
}

bool ImageHistory::add(ImageHistoryItem &historyItem, const char *historyFile) {
	
	m_hstfile.open(historyFile, std::ios::out | std::ios::app);
	if (m_hstfile.is_open() == false) {
		return false;
	}
	m_hstfile << historyItem.toString() << '\n';
	m_hstfile.close();

	return true;
}

/*
bool ImageHistory::read(const char *filepath) {
	char text[256];
	std::ifstream file(filepath);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		m_eventList->push_back(ImageHistoryItem(text));
	}
	file.close();

	return true;
}
bool ImageHistory::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<ImageHistoryItem>::iterator i = m_eventList->begin(); i != m_eventList->end(); i++) {
		ImageHistoryItem item = *i;
		std::string &line = item.toString();
		//printf("%s\n", line.c_str());
		file << line << '\n';

	}
	file.close();
	return true;
}
*/

/*
std::auto_ptr<ImageHistoryLog>  ImageHistory::get() {
	std::string str;
	std::auto_ptr<ImageHistoryLog> imageHistoryLog(new ImageHistoryLog);
	if (read(mpath.c_str()) == false) {
		return imageHistoryLog;
	}

	for (std::vector<ImageHistoryItem>::iterator i = m_eventList->begin(); i != m_eventList->end(); i++) {
		ImageHistoryItem item = *i;
		std::string &line = item.toString();
		imageHistoryLog->push_back(line);
	}
	return imageHistoryLog;
}
*/
/*
const char *ImageHistory::getEventString(HistoryEvent he) {
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
HistoryEvent ImageHistory::getEvent(const char *he) {
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
