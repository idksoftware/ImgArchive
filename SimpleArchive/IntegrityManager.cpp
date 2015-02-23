/*
 * IntegrityManager.cpp
 *
 *  Created on: Oct 7, 2014
 *      Author: wzw7yn
 */

#include <iostream>
#include <time.h>
#include "IntegrityManager.h"
#include "FolderList.h"
#include "CheckDisk.h"
#include "SAUtils.h"


namespace simplearchive {

IntegrityManager *IntegrityManager::m_this = 0;

IntegrityManager::IntegrityManager() {
	m_archivePath;
}

IntegrityManager::~IntegrityManager() {
}

bool IntegrityManager::addDayFolder(const char *folderName) {
	FolderList folderList(m_archivePath.c_str());
	folderList.addDayFolder(folderName);
	return true;
}

bool IntegrityManager::addFile(const char *folderPath, const char *fileName) {
	CheckDisk checkDisk;
	if (checkDisk.update(folderPath, fileName) == false) {
		return false;
	}
	FolderList folderList(m_archivePath.c_str());
	folderList.incFiles(folderPath);
	return true;
}

bool IntegrityManager::makeList() {
	FolderList folderList(m_archivePath.c_str());
	if (folderList.makeList() == false) {
		return false;
	}
	// make file list
	std::string path = m_archivePath + std::string("/.sia/chdsk");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	
	std::vector<std::string *> *filelist = SAUtils::getFiles(m_archivePath.c_str());
	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *year = *i;
		std::string filepath = m_archivePath + "/" + *year;

		char c = (*year)[0];
		if (c == '.') {
			continue;
		}
		std::cout << filepath << '\n';

		std::string yearfolder = m_archivePath + '/' + *year;
		std::vector<std::string *> *dayList = SAUtils::getFiles(yearfolder.c_str());

		for (std::vector<std::string *>::iterator i = dayList->begin(); i != dayList->end(); i++) {
			std::string *name = *i;
			

			char c = (*name)[0];
			if (c == '.') {
				continue;
			}
			std::string chdskpath = m_archivePath + "/" + *year + "/" + *name + "/.sia/chdsk";
			std::string dirpath = m_archivePath + "/" + *year + "/" + *name;
			std::cout << chdskpath << '\n';
			std::cout << dirpath << '\n';
			CheckDisk checkDisk;
			checkDisk.makeCheckData(dirpath.c_str());
		}
		
	}
	
	return true;
}

IntegrityManager &IntegrityManager::get(const char *archivePath) {
	if (m_this == 0) {
		m_this = new IntegrityManager();
	}
	m_this->setArchivePath(archivePath);
	return *m_this;
}

IntegrityManager &IntegrityManager::get() {
	if (m_this == 0) {
		throw std::exception();
	}
	return *m_this;
}

} /* namespace simplearchive */
