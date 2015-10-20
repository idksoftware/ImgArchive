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

#include "ImagePath.h"
#include "SAUtils.h"
#include "CDate.h"
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include "IntegrityManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

std::string ImagePath::m_mainMetadataPath;
//std::string ImagePath::m_userDefinedSequenceNumberPath;
std::string ImagePath::m_mainDupsPath;
std::string ImagePath::m_mainHistory;
std::string ImagePath::m_pathToArchive;

std::string ImagePath::m_pathToShadow;
std::string ImagePath::m_shadowMetadataPath;
std::string ImagePath::m_shadowSequenceNumberPath;

std::string ImagePath::m_shadowHistory;
std::string ImagePath::m_shadowCatalog;
std::string ImagePath::m_shadowJournalPath;

bool ImagePath::settupMainArchiveFolders(const char *pathToArchive, const char *pathToShadow, const char *pathToHome) {
	if (SAUtils::DirExists(pathToArchive) == false) {
		return false;
	}
	if (SAUtils::DirExists(pathToShadow) == false) {
		return false;
	}
	m_pathToArchive = pathToArchive;
	m_pathToShadow = pathToShadow;
	IntegrityManager &integrityManager = IntegrityManager::get(pathToShadow, pathToArchive, pathToHome);
	// Shadow Archive
	std::string m_dataFolder = m_pathToShadow;
	m_dataFolder += "/root";
	if (SAUtils::DirExists(m_dataFolder.c_str()) == false) {
		SAUtils::mkDir(m_dataFolder.c_str());

	}
	if (m_shadowMetadataPath.empty() == true) {
		m_shadowMetadataPath = m_dataFolder + "/metadata";
		m_shadowSequenceNumberPath = m_dataFolder + "/imageid";
		m_shadowJournalPath = m_dataFolder + "/journal";
		m_shadowHistory = m_dataFolder + "/history";
		if (SAUtils::DirExists(m_shadowMetadataPath.c_str()) == false) {
			SAUtils::mkDir(m_shadowMetadataPath.c_str());
		}
		if (SAUtils::DirExists(m_shadowSequenceNumberPath.c_str()) == false) {
			SAUtils::mkDir(m_shadowSequenceNumberPath.c_str());
		}
		
		if (SAUtils::DirExists(m_shadowHistory.c_str()) == false) {
			SAUtils::mkDir(m_shadowHistory.c_str());
		}
	}
	return true;
}

ImagePath::ImagePath(time_t time) {

	CDate date(time);
	
	int year = date.getYear();
	int month = date.getMonth();
	int day = date.getDay();
	//printf("Time: %.4d %.2d %.2d", year, month, day);

	m_yearStr = SAUtils::to_string(year);
	std::stringstream s;
	s << year << '-' << std::setw(2) << std::setfill('0') << month
		<< '-' << std::setw(2) << std::setfill('0') << day;
	init(s.str());
}

ImagePath::ImagePath(std::string &yyyymmddStr) {
	init(yyyymmddStr);
}

void ImagePath::init(std::string &yyyymmddStr) {

	m_yearStr = getYear(yyyymmddStr);
	m_yyyymmddStr = yyyymmddStr;

	//if (m_yyyymmddStr.find_first_of('/') == std::string::npos) {
	//	m_yyyymmddStr = m_yearStr + '/' + m_yyyymmddStr;
	//}
	IntegrityManager &integrityManager = IntegrityManager::get();
	m_yearStrPath = m_pathToArchive + '/' + m_yearStr;
	if (SAUtils::DirExists(m_yearStrPath.c_str()) == false) {
		SAUtils::mkDir(m_yearStrPath.c_str());

	}

	m_yyyymmddStrPath = m_yearStrPath + '/' + m_yyyymmddStr;

	if (SAUtils::DirExists(m_yyyymmddStrPath.c_str()) == false) {
		SAUtils::mkDir(m_yyyymmddStrPath.c_str());
	}
	
	m_relpath = getYearStr() + '/' + getYyyymmddStr();
	
	
	// shadow
	m_shadowYearStrPath = m_pathToShadow + '/' + m_yearStr;
	if (SAUtils::DirExists(m_shadowYearStrPath.c_str()) == false) {
		SAUtils::mkDir(m_shadowYearStrPath.c_str());

	}
	
	m_shadowYyyymmddStrPath = m_shadowYearStrPath + '/' + m_yyyymmddStr;

	m_localShadowDataPath = m_shadowYyyymmddStrPath + "/data";
	m_localShadowMetadataPath = m_shadowYyyymmddStrPath + "/metadata";
	m_localShadowDBPath = m_shadowYyyymmddStrPath + "/db";
	m_localShadowHistoryPath = m_shadowYyyymmddStrPath + "/history";
	if (SAUtils::DirExists(m_shadowYyyymmddStrPath.c_str()) == false) {
		SAUtils::mkDir(m_shadowYyyymmddStrPath.c_str());
		if (SAUtils::DirExists(m_shadowYyyymmddStrPath.c_str()) == false) {
			throw std::exception();
		}
	}
	SAUtils::mkDir(m_localShadowDataPath.c_str());
	SAUtils::mkDir(m_localShadowMetadataPath.c_str());
	SAUtils::mkDir(m_localShadowDBPath.c_str());
	SAUtils::mkDir(m_localShadowHistoryPath.c_str());
	integrityManager.addDayFolder(m_yyyymmddStr.c_str());
}



ImagePath::ImagePath(const char *filepath) {
	IntegrityManager &m_integrityManager = IntegrityManager::get();
	
	std::string tmp = filepath;
	std::string filename;
	std::string path;

	// This fines the filename from path NEEDS TO BE MOVED INTO UTILS
	int slashpos = tmp.find_last_of("/");
	if (slashpos != -1) {
		filename = tmp.substr(slashpos + 1, tmp.length() - slashpos);
		setImageName(filename.c_str());
		path = tmp.substr(0, slashpos);
	} else {
		//printf("Path to Image in achive invalid \"%s\"", filepath);
	}
	init(path);
}

bool ImagePath::copyFile(std::string  pathToSourceRoot, std::string file) {
	IntegrityManager &integrityManager = IntegrityManager::get();
	std::string from = pathToSourceRoot + "/" + file;
	std::string to = m_yyyymmddStrPath + '/' + file;
	
	// Working Archive
	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}
	// Shadow Archive
	to = m_localShadowDataPath + '/' + file;
	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}
	// this can only be done after the file copy
	integrityManager.addFile(m_yyyymmddStr.c_str(), file.c_str());
	return true;
}

ImagePath::~ImagePath() {
	// TODO Auto-generated destructor stub
}

} /* namespace simplearchive */
