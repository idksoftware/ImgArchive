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

#include <iostream>
#include <time.h>
#include "IntegrityManager.h"
#include "FolderList.h"
#include "CheckDisk.h"
#include "SAUtils.h"
#include "ValidateReportingObject.h"
#include "CheckoutStatus.h"
#include "CheckDiskJournal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {



void IntegrityManager::setPaths(const char* archivePath, const char *derivativePath, const char* workspacePath, const char* homePath) {
	m_archivePath = archivePath;
	m_derivativePath = derivativePath;
	m_workspacePath = workspacePath;
	m_homePath = homePath;
	
	//CheckDisk::setArchivePath(archivePath);
	
}

bool IntegrityManager::addDerivativeDayFolder(const char *folderName) {
	return addDayFolder(m_derivativePath.c_str(), folderName);
}

bool IntegrityManager::addMasterDayFolder(const char *folderName) {
	return addDayFolder(m_archivePath.c_str(), folderName);
}

bool IntegrityManager::addDayFolder(const char *rootName, const char *folderName) {
	FolderList folderList(rootName);
	folderList.addDayFolder(folderName);
	return true;
}

bool IntegrityManager::addDerivativeFile(const char *folderPath, const char *fileName) {
	return addFile(m_derivativePath.c_str(), folderPath, fileName);
}

bool IntegrityManager::addMasterFile(const char *folderPath, const char *fileName) {
	return addFile(m_archivePath.c_str(), folderPath, fileName);
}

bool IntegrityManager::addFile(const char *rootPath, const char *folderPath, const char *fileName) {
	CheckDisk checkDisk;
	if (checkDisk.update(rootPath, folderPath, fileName) == false) {
		return false;
	}
	FolderList folderList(rootPath);
	folderList.incFiles(folderPath);
	return true;
}

bool IntegrityManager::validate(IMCompletedSummary& imCompletedSummary, bool workspace, bool Master) {
//	FolderList folderList(m_archivePath.c_str());
	std::string tmp = m_archivePath;
	tmp += "/system/journal";
	if (SAUtils::DirExists(tmp.c_str()) == false) {
		throw std::exception();
		return false;
	}
	tmp += "/checkdisk";
	if (SAUtils::DirExists(tmp.c_str()) == false) {
		if (SAUtils::mkDir(tmp.c_str()) == false) {
			throw std::exception();
		}
	}
	ValidateReportingObject::setPath(tmp.c_str());
	FolderList folderList(m_archivePath.c_str(), m_workspacePath.c_str());
	if (workspace && Master) {
		folderList.SetAction(FolderList::READING_BOTH);
	}
	else if (workspace) {
		folderList.SetAction(FolderList::READING_WORKSPACE);
	}
	else if (Master) {
		folderList.SetAction(FolderList::READING_MASTER);
	}
	else {
		return false;
	}
	if (folderList.validate(imCompletedSummary) == false) {
		return false;
	}
	return true;
}

bool IntegrityManager::repair(IMCompletedSummary& imCompletedSummary, bool workspace, bool Master) {
	//	FolderList folderList(m_archivePath.c_str());
	std::string tmp = m_archivePath;
	tmp += "/system/journal";
	if (SAUtils::DirExists(tmp.c_str()) == false) {
		throw std::exception();
	}
	tmp += "/checkdisk";
	if (SAUtils::DirExists(tmp.c_str()) == false) {
		if (SAUtils::mkDir(tmp.c_str()) == false) {
			throw std::exception();
		}
	}
	ValidateReportingObject::setPath(tmp.c_str());
	FolderList folderList(m_archivePath.c_str(), m_workspacePath.c_str());
	if (workspace && Master) {
		folderList.SetAction(FolderList::READING_BOTH);
	}
	else if (workspace) {
		folderList.SetAction(FolderList::READING_WORKSPACE);
	}
	else if (Master) {
		folderList.SetAction(FolderList::READING_MASTER);
	}
	else {
		return false;
	}
	
	if (folderList.validateAndRepair(imCompletedSummary) == false) {
		return false;
	}
	return true;
}


bool IntegrityManager::makeList() {
	FolderList folderList(m_archivePath.c_str());
	if (folderList.makeList() == false) {
		return false;
	}
	// make file list
	std::string path = m_archivePath + std::string("/chdsk");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	
	FileList_Ptr filelist = SAUtils::getFiles_(m_archivePath.c_str());
	for (auto i = filelist->begin(); i != filelist->end(); i++) {
		std::string year = *i;
		std::string filepath = m_archivePath + "/" + year;

		char c = (year)[0];
		if (c == '.') {
			continue;
		}
		std::cout << filepath << '\n';

		std::string yearfolder = m_archivePath + '/' + year;
		FileList_Ptr dayList = SAUtils::getFiles_(yearfolder.c_str());

		for (auto i = dayList->begin(); i != dayList->end(); i++) {
			std::string name = *i;
			

			char c = (name)[0];
			if (c == '.') {
				continue;
			}
			std::string chdskpath = m_archivePath + "/" + year + "/" + name + "/chdsk";
			std::string dirpath = m_archivePath + "/" + year + "/" + name;
			std::cout << chdskpath << '\n';
			std::cout << dirpath << '\n';
			CheckDisk checkDisk;
			checkDisk.makeCheckData(dirpath.c_str());
		}
		
	}
	
	return true;
}

/*
IntegrityManager &IntegrityManager::get(const char *archivePath, const char *workspacePath, const char *homePath) {
	IntegrityManager integrityManager = IntegrityManager::get();
	static bool first = true;
	if (first) {
		integrityManager.setPaths(archivePath, workspacePath, homePath);
	}
	return integrityManager;
}
*/


IntegrityManager &IntegrityManager::get() {
	static IntegrityManager integrityManager;
	
	return integrityManager;
}

} /* namespace simplearchive */
