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
#include "ImageHistory.h"
#include "LogName.h"
#include "ExifDateTime.h"
#include "SAUtils.h"
#include "History.h"
#include "CSVArgs.h"
#include "SystemHistory.h"
#include "ArchivePath.h"
#include "ErrorCode.h"
#include "PathController.h"




#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	


// This class is used to write the log in normal operation. However reading the 
// log will be done by the MTRow 



class HistoryItem {
	std::string m_comment;
	std::string m_date;
	std::string m_filepath;
	std::string m_event;
	std::string m_data;
	std::string m_version;
public:
	HistoryItem();
	
	// 14.06.12.12.16.12:56:first checkin:checkin
	
	HistoryItem(const char *dataString) {
		m_data = dataString;
		CSVArgs csvArgs(',');
		csvArgs.process(dataString);
		m_date = csvArgs.at(0);
		m_version = csvArgs.at(1);
		m_filepath = csvArgs.at(2);
		m_comment = csvArgs.at(3);
		m_event = csvArgs.at(4);
		

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
		m_data = m_date + "," + m_version + "," + m_filepath +"," + m_comment +"," + m_event;
		return m_data;
	}

	HistoryItem::~HistoryItem() {
		
	}
};



ImageHistory::ImageHistory() {
}

ImageHistory::~ImageHistory() {
}

std::string ImageHistory::m_workspace;
std::string ImageHistory::m_index;
std::string ImageHistory::m_backup1;
std::string ImageHistory::m_backup2;

/*
bool ImageHistory::init() {

	if (ArchivePath::isBackup1Enabled() == true) {
		ImageHistory::setBackup1Path(ArchivePath::getBackup1().getHistory().c_str());
	}
	if (ArchivePath::isBackup2Enabled() == true) {
		ImageHistory::setBackup2Path(ArchivePath::getBackup2().getHistory().c_str());
	}
	
	return true;
}
*/

bool ImageHistory::init() {


	LogName logName;
	//m_currentFilename = logName.makeName(m_primary.c_str(), "", "hst", 256);

	m_index = ArchivePath::getPrimaryIndex().getHistoryPath();
	m_index += '/'; m_index += logName.getFilename();

	if (ArchivePath::isBackup1Enabled() == true) {
		m_backup1 = ArchivePath::getBackup1().getImageHistory();
		if (SAUtils::DirExists(m_backup1.c_str()) == false) {
			if (SAUtils::mkDir(m_backup1.c_str()) == false) {
				return false;
			}
		}
		
	}
	if (ArchivePath::isBackup2Enabled() == true) {
		m_backup2 = ArchivePath::getBackup2().getImageHistory();
		if (SAUtils::DirExists(m_backup2.c_str()) == false) {
			if (SAUtils::mkDir(m_backup2.c_str()) == false) {
				return false;
			}
		}
		
	}
	return true;
}


bool ImageHistory::add(const char *filename, const char *comment) {
	return add(filename, "0000", comment, HistoryEvent::Event::ADDED);
}
/**
 * This function adds history to an image.
 */
bool ImageHistory::add(const char *filepath, int version, const char *comment, const HistoryEvent &he) {
	std::string buff = SAUtils::sprintf("%.4d", version);
	add(filepath, buff.c_str(), comment, he);
	return true;
}
/**
	filepath i.e 2015-11-10/DSC1236.jpg
*/
bool ImageHistory::add(const char *filepath, const char *version, const char *comment, const HistoryEvent &he) {
	
	ExifDateTime date;
	date.now();
	std::string dateStr = date.current();
	const char *event = he.getString();

	HistoryItem historyItem(dateStr.c_str(), version, filepath, comment, event);

	
	PathController workspaceController;
	workspaceController.setRoot(m_workspace.c_str());
	workspaceController.splitShort(filepath);
	workspaceController.makePath();
	std::string workspacePath = workspaceController.getFullPath();
	workspacePath += "/.sia/"; workspacePath += workspaceController.getImage();
	workspacePath += ".hst";

	if (writeLog(historyItem, workspacePath.c_str()) == false) {
		return false;
	}

	//PathController pathController;
	
	PathController indexController;
	indexController.setRoot(m_index.c_str());
	indexController.splitShort(filepath);
	indexController.makeImagePath();
	
	std::string indexPath = indexController.getFullPath();;
	/*
	indexPath += "/history";
	
	if (SAUtils::DirExists(indexPath.c_str()) == false) {
		if (SAUtils::mkDir(indexPath.c_str()) == false) {
			return false;
		}
	}
	
	indexPath += '/';
	indexPath += indexController.getFilename();
	*/
	indexPath += ".hst";

	if (writeLog(historyItem, indexPath.c_str()) == false) {
		return false;
	}
	
	/*
	if (ArchivePath::isBackup1Enabled()) {
		std::string backup1Path = ArchivePath::getBackup1().getRepositoryPath();
		backup1Path += '/'; backup1Path += m_localPath.substr(0, 4);
		backup1Path += '/'; backup1Path += m_localPath;
		backup1Path += "/history/"; backup1Path += filename;
		backup1Path += ".hst";

		if (writeLog(historyItem, backup1Path.c_str()) == false) {
			return false;
		}
	}
	if (ArchivePath::isBackup2Enabled()) {
		std::string backup2Path = ArchivePath::getBackup2().getRepositoryPath();
		backup2Path += '/'; backup2Path += m_localPath.substr(0, 4);
		backup2Path += '/'; backup2Path += m_localPath;
		backup2Path += "/history/"; backup2Path += filename;
		backup2Path += ".hst";

		if (writeLog(historyItem, backup2Path.c_str()) == false) {
			return false;
		}
	}
	*/
	return true;
	/*
	std::ofstream file;
	file.open(filepathstr.c_str(), std::ios::out | std::ios::app);
	if (file.is_open() == false) {
		return false;
	}
	file << historyItem.toString().c_str() << '\n';
	file.close();
	return true;
	*/
}

bool ImageHistory::writeLog(HistoryItem &item, const char *path) {
	std::ofstream file;
	file.open(path, std::ios::out | std::ios::app);
	if (file.is_open() == false) {
		ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
		return false;
	}
	file << item.toString().c_str() << '\n';
	file.close();
	return true;
}




/*
std::string History::getHistory(CDate from, CDate to) {
	std::string str;
	return str;
}
*/

std::shared_ptr<ImageHistoryLog>  ImageHistory::getEntries(const char *filepath) {
	
	PathController indexController;
	indexController.setRoot(m_index.c_str());
	indexController.split(filepath);
	indexController.makeImagePath("hst");

	std::string indexPath = indexController.getFullPath();;
	
	std::shared_ptr<ImageHistoryLog> log = std::make_shared<ImageHistoryLog>();

	if (readLog(indexPath.c_str(), *log) == false) {
		log = nullptr;
		return log;
	}
	log->setTitle(indexController.getImageName().c_str());
	log->setDescription(filepath);
	return log;
}

bool ImageHistory::readLog(const char *logFile, ImageHistoryLog &historyLog) {
	
	std::ifstream file(logFile);
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
