/*
 * IntegrityManager.h
 *
 *  Created on: Oct 7, 2014
 *      Author: wzw7yn
 */

#ifndef CATALOGMANAGER_H_
#define CATALOGMANAGER_H_
#include <memory>

namespace simplearchive {

class CatalogManager {
	static std::auto_ptr<CatalogManager> m_this;
	const char *m_archivePath;
public:
	CatalogManager();
	virtual ~CatalogManager();
	bool addDayFolder(const char *folderName);
	bool addFile(const char *folderPath, const char *fileName);
	bool makeList();
	static CatalogManager &get(const char *archivePath);
	static CatalogManager &get();
	void setArchivePath(const char* archivePath) {
		this->m_archivePath = archivePath;
	}
};

} /* namespace simplearchive */
#endif /* CATALOGMANAGER_H_ */
