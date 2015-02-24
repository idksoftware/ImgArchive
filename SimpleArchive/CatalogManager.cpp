/*
 * IntegrityManager.cpp
 *
 *  Created on: Oct 7, 2014
 *      Author: wzw7yn
 */

#include "CatalogManager.h"
#include "Catalog.h"
#include "CheckDisk.h"

namespace simplearchive {

std::auto_ptr<CatalogManager> CatalogManager::m_this(0);

CatalogManager::CatalogManager() {
	m_archivePath = 0;

}

CatalogManager::~CatalogManager() {
}

bool CatalogManager::addDayFolder(const char *folderName) {
	Catalog catalog(m_archivePath);
	catalog.addDayFolder(folderName);
	return true;
}

bool CatalogManager::addFile(const char *folderPath, const char *fileName) {

	Catalog catalog(m_archivePath);
	catalog.incFiles(folderPath);
	return true;
}

CatalogManager &CatalogManager::get(const char *archivePath) {

	if (!m_this.get()) {
		m_this.reset(new CatalogManager());

	}

	m_this->setArchivePath(archivePath);
	return *(m_this.get());
}

CatalogManager &CatalogManager::get() {
	if (!m_this.get()) {
		throw std::exception();
	}
	return *m_this;
}

} /* namespace simplearchive */
