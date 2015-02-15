/*
 * IntegrityManager.h
 *
 *  Created on: Oct 7, 2014
 *      Author: wzw7yn
 */

#ifndef INTEGRITYMANAGER_H_
#define INTEGRITYMANAGER_H_

namespace simplearchive {

class IntegrityManager {
	static IntegrityManager *m_this;
	const char *m_archivePath;
public:
	IntegrityManager();
	virtual ~IntegrityManager();
	bool addDayFolder(const char *folderName);
	bool addFile(const char *folderPath, const char *fileName);
	bool makeList();
	static IntegrityManager &get(const char *archivePath);
	static IntegrityManager &get();
	void setArchivePath(const char* archivePath) {
		this->m_archivePath = archivePath;
	}
};

} /* namespace simplearchive */
#endif /* INTEGRITYMANAGER_H_ */
