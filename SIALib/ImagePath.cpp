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
#include "pathcontroller.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

std::string ImagePath::m_mainMetadataPath;
std::string ImagePath::m_mainDupsPath;
std::string ImagePath::m_mainHistory;
std::string ImagePath::m_pathToWorkspace;

std::string ImagePath::m_pathToMaster;
std::string ImagePath::m_MasterMetadataPath;
std::string ImagePath::m_MasterSequenceNumberPath;

std::string ImagePath::m_MasterHistory;
std::string ImagePath::m_MasterCatalog;
std::string ImagePath::m_MasterJournalPath;




bool ImagePath::settupMainArchiveFolders(const char *pathToWorkspace, const char *pathToMaster, const char *pathToDerivative, const char *pathToHome) {

	if (SAUtils::DirExists(pathToWorkspace) == false) {
		return false;
	}
	if (SAUtils::DirExists(pathToMaster) == false) {
		return false;
	}
	m_pathToWorkspace = pathToWorkspace;
	m_pathToMaster = pathToMaster;
	IntegrityManager &integrityManager = IntegrityManager::get();
	integrityManager.setPaths(pathToMaster, pathToDerivative, pathToWorkspace, pathToHome);
	
	// Master Archive
	std::string m_dataFolder = m_pathToMaster;
	m_dataFolder += SYSTEM_PATH;
	if (SAUtils::DirExists(m_dataFolder.c_str()) == false) {
		SAUtils::mkDir(m_dataFolder.c_str());

	}
	if (m_MasterMetadataPath.empty() == true) {
		m_MasterMetadataPath = m_dataFolder + METADATA_PATH;
		m_MasterSequenceNumberPath = m_dataFolder + IMAGEID_PATH;
		m_MasterJournalPath = m_dataFolder + JOURNAL_PATH;
		m_MasterHistory = m_dataFolder + HISTORY_PATH;
		if (SAUtils::DirExists(m_MasterMetadataPath.c_str()) == false) {
			SAUtils::mkDir(m_MasterMetadataPath.c_str());
		}
		/*
		if (SAUtils::DirExists(m_MasterSequenceNumberPath.c_str()) == false) {
			SAUtils::mkDir(m_MasterSequenceNumberPath.c_str());
		}
		
		if (SAUtils::DirExists(m_MasterHistory.c_str()) == false) {
			SAUtils::mkDir(m_MasterHistory.c_str());
		}
		*/
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
	std::string tmp = s.str();
	init(tmp);
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
	
	m_yearStrPath = m_pathToWorkspace + '/' + m_yearStr;
	if (SAUtils::DirExists(m_yearStrPath.c_str()) == false) {
		SAUtils::mkDir(m_yearStrPath.c_str());

	}

	m_yyyymmddStrPath = m_yearStrPath + '/' + m_yyyymmddStr;

	if (SAUtils::DirExists(m_yyyymmddStrPath.c_str()) == false) {
		SAUtils::mkDir(m_yyyymmddStrPath.c_str());
	}

	createWorkspaceMetadataPath();

	m_relpath = getYearStr() + '/' + getYyyymmddStr();

	// Master
	m_MasterYearStrPath = m_pathToMaster + '/' + m_yearStr;
	if (SAUtils::DirExists(m_MasterYearStrPath.c_str()) == false) {
		SAUtils::mkDir(m_MasterYearStrPath.c_str());

	}

	m_MasterYyyymmddStrPath = m_MasterYearStrPath + '/' + m_yyyymmddStr;
	std::string localPath = m_MasterYyyymmddStrPath;

	createLocalPaths(localPath);
}

void ImagePath::createWorkspaceMetadataPath() {
	m_workspaceMetadataPath = m_yyyymmddStrPath + "/.sia";
	if (SAUtils::DirExists(m_workspaceMetadataPath.c_str()) == false) {
		SAUtils::mkDir(m_workspaceMetadataPath.c_str());
	}
	m_workspaceMetadataPath += "/metadata";
	if (SAUtils::DirExists(m_workspaceMetadataPath.c_str()) == false) {
		SAUtils::mkDir(m_workspaceMetadataPath.c_str());
	}
}


void ImagePath::createLocalPaths(std::string localPath)
{
	m_localMasterDataPath = localPath + MASTER_IMAGE_PATH;
	m_localMasterMetadataPath = localPath + METADATA_PATH;
	
	if (SAUtils::DirExists(localPath.c_str()) == false) {
		SAUtils::mkDir(localPath.c_str());
		if (SAUtils::DirExists(localPath.c_str()) == false) {
			throw std::exception();
		}
	}
	SAUtils::mkDir(m_localMasterDataPath.c_str());
	SAUtils::mkDir(m_localMasterMetadataPath.c_str());
	
	IntegrityManager &integrityManager = IntegrityManager::get();
	//integrityManager.addMasterDayFolder(m_yyyymmddStr.c_str());
}



ImagePath::ImagePath(const char *filepath) {
	//IntegrityManager &m_integrityManager = IntegrityManager::get();
	PathController pathController;
	pathController.splitShort(filepath);
	m_imageName = pathController.getImage();
	std::string path = pathController.getYearday();
	init(path);
}


bool ImagePath::copyFile2Workspace(std::string  pathToSourceRoot, const std::string& file, const std::string& sequenceFile) {
	IntegrityManager &integrityManager = IntegrityManager::get();
	std::string from = pathToSourceRoot + "/" + file;
	std::string to = m_yyyymmddStrPath + '/' + sequenceFile;
	
	// Working Archive
	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}

	// this can only be done after the file copy
	integrityManager.addMasterDayFolder(m_yyyymmddStr.c_str());
	integrityManager.addMasterFile(m_yyyymmddStr.c_str(), sequenceFile.c_str());
	return true;
}


ImagePath::~ImagePath() {

}


} /* namespace simplearchive */
