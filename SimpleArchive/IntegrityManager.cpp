/*
 * IntegrityManager.cpp
 *
 *  Created on: Oct 7, 2014
 *      Author: wzw7yn
 */

#include "IntegrityManager.h"
#include "FolderList.h"
#include "CheckDisk.h"

namespace simplearchive {

IntegrityManager *IntegrityManager::m_this = 0;

IntegrityManager::IntegrityManager() {
	m_archivePath = 0;
}

IntegrityManager::~IntegrityManager() {
}

bool IntegrityManager::addDayFolder(const char *folderName) {
	FolderList folderList(m_archivePath);
	folderList.addDayFolder(folderName);
	return true;
}

bool IntegrityManager::addFile(const char *folderPath, const char *fileName) {
	CheckDisk checkDisk;
	if (checkDisk.update(folderPath, fileName) == false) {
		return false;
	}
	FolderList folderList(m_archivePath);
	folderList.incFiles(folderPath);
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
