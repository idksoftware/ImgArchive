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

void IntegrityManager::setMasterBackupPaths(const char* backupPath1, const char* backupPath2, bool backup1enabled, bool backup2enabled)
{
	m_masterBackup1enabled = backup1enabled;
	m_masterBackup2enabled = backup2enabled;
	if (backup1enabled) {
		m_masterBackupPath1 = backupPath1;
	}
	if (backup2enabled) {
		m_masterBackupPath2 = backupPath2;
	}
}

void IntegrityManager::setDerivativeBackupPaths(const char* backupPath1, const char* backupPath2, bool backup1enabled, bool backup2enabled)
{
	m_derivativeBackup1enabled = backup1enabled;
	m_derivativeBackup2enabled = backup2enabled;
	if (backup1enabled) {
		m_derivativeBackupPath1 = backupPath1;
	}
	if (backup2enabled) {
		m_derivativeBackupPath2 = backupPath2;
	}
}

bool IntegrityManager::addDerivativeDayFolder(const char *folderName) {
	return addDayFolder(m_derivativePath.c_str(), folderName);
}

bool IntegrityManager::addMasterDayFolder(const char *folderName) {
	return addDayFolder(m_archivePath.c_str(), folderName);
}

bool IntegrityManager::addDayFolder(const char *rootName, const char *folderName) {
	FolderList folderList(rootName);
	folderList.addDayFolder(m_archivePath.c_str(), folderName);
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
	folderList.incFiles(m_archivePath.c_str(), folderPath);
	return true;
}

bool IntegrityManager::validate(IMCompletedSummary& imCompletedSummary, Scope scope, bool main, VerifyBackups verifyBackups, bool repair) {
//	FolderList folderList(m_archivePath.c_str());
	std::string tmp = m_archivePath;
	if (SAUtils::DirExists(tmp.c_str()) == false) {
		throw std::exception(); //"Archive path not found");
	}
	tmp += "/system/journal";
	if (SAUtils::DirExists(tmp.c_str()) == false) {
		throw std::exception(); //"Journal path not found");
		return false;
	}
	tmp += "/checkdisk";
	if (SAUtils::DirExists(tmp.c_str()) == false) {
		if (SAUtils::mkDir(tmp.c_str()) == false) {
			throw std::exception(); //"checkdisk path not found");
		}
	}
	ValidateReportingObject::setPath(tmp.c_str());
	FolderList folderList(m_workspacePath.c_str());
	
	bool m_validateMaster = false;
	bool m_validateDerivative = false;
	bool m_valideteWorkspace = false;
	
	bool m_validateBackup_1 = false;
	bool m_validateBackup_2 = false;
	bool m_validateMasterBackup_1 = false;
	bool m_validateMasterBackup_2 = false;
	bool m_validateDerivativeBackup_1 = false;
	bool m_validateDerivativeBackup_2 = false;
	
	switch (verifyBackups) {
	case IntegrityManager::VerifyBackups::Backup_1:
		m_validateBackup_1 = true;
		break;
	case IntegrityManager::VerifyBackups::Backup_2:
		m_validateBackup_2 = true;
		break;
	case IntegrityManager::VerifyBackups::Both:
		m_validateBackup_1 = true;
		m_validateBackup_2 = true;
		break;
	case IntegrityManager::VerifyBackups::None:
		break;
	};

	switch(scope) {
	case Scope::Workspace:	
		m_valideteWorkspace = true;
		break;
	case Scope::Master:
		m_validateMaster = true;
		break;
	case Scope::Derivative:
		m_validateDerivative = true;
		break;
	case Scope::All:
		m_validateMaster = true;
		m_validateDerivative = true;
		m_valideteWorkspace = true;
		break;
	case Scope::Main:
		m_validateMaster = true;
		m_validateDerivative = true;
		break;
	}
	
	if (m_validateMaster == true) {
		if (m_validateBackup_1 == true) {
			if (m_masterBackup1enabled) {
				m_validateMasterBackup_1 = true;
			}
		}
		else if (m_validateBackup_2 == true) {
			if (m_masterBackup2enabled) {
				m_validateMasterBackup_2 = true;
			}
		}
	} 
	else if (m_validateDerivative == true) {
		if (m_validateBackup_1 == true) {
			if (m_derivativeBackup1enabled) {
				m_validateDerivativeBackup_1 = true;
			}
		}
		else if (m_validateBackup_2 == true) {
			if (m_derivativeBackup2enabled) {
				m_validateDerivativeBackup_2 = true;
			}
		}
	}
	
	if (m_validateMaster == true) {
		if (folderList.validate(m_archivePath.c_str(), imCompletedSummary, repair) == false) {
			return false;
		}
	} 
	else if (m_validateDerivative == true) {
		if (folderList.validate(m_derivativePath.c_str(), imCompletedSummary, repair) == false) {
			return false;
		}
	}
	else if (m_valideteWorkspace == true) {
		if (folderList.validate(m_workspacePath.c_str(), imCompletedSummary, repair) == false) {
			return false;
		}
	}
	else if (m_validateMasterBackup_1 == true) {
		if (folderList.validate(m_masterBackupPath1.c_str(), imCompletedSummary, repair) == false) {
			return false;
		}
	}
	else if (m_validateMasterBackup_2 == true) {
		if (folderList.validate(m_masterBackupPath2.c_str(), imCompletedSummary, repair) == false) {
			return false;
		}
	}
	else if (m_validateDerivativeBackup_1 == true) {
		if (folderList.validate(m_derivativeBackupPath1.c_str(), imCompletedSummary, repair) == false) {
			return false;
		}
	}
	else if (m_validateDerivativeBackup_2 == true) {
		if (folderList.validate(m_derivativeBackupPath2.c_str(), imCompletedSummary, repair) == false) {
			return false;
		}
	}
	return true;
}

/*
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
*/

bool IntegrityManager::makeList() {
	FolderList folderList(m_archivePath.c_str());
	if (folderList.makeList(m_archivePath.c_str()) == false) {
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
