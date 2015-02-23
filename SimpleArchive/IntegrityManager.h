/*
 * IntegrityManager.h
 *
 *  Created on: Oct 7, 2014
 *      Author: wzw7yn
 */

#ifndef INTEGRITYMANAGER_H_
#define INTEGRITYMANAGER_H_
#include <string>

namespace simplearchive {

class IntegrityManager {
	static IntegrityManager *m_this;
	std::string m_archivePath;
	IntegrityManager();
public:
	
	virtual ~IntegrityManager();
	bool addDayFolder(const char *folderName);
	bool addFile(const char *folderPath, const char *fileName);
	bool makeList();
	static IntegrityManager &get(const char *archivePath);
	static IntegrityManager &get();
	void setArchivePath(const char* archivePath) {
		m_archivePath = archivePath;
	}
};

} /* namespace simplearchive */
#endif /* INTEGRITYMANAGER_H_ */
