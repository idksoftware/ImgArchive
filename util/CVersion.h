/*
 * CVersion.h
 *
 *  Created on: May 20, 2014
 *      Author: wzw7yn
 */

#ifndef CVERSION_H_
#define CVERSION_H_
#include <string>
namespace simplearchive {

class CVersion {
	int m_version;
	std::string m_nameonly;
	std::string m_currentName;
public:
	CVersion();
	virtual ~CVersion();
	const char *newVersion(const char *name);
	int fileLastVersion(const char *name);
	int versionIndex(const char *name);
	std::string nameOnly(const char *name);
	int getVersion() const {
		return m_version;
	}

	void setVersion(int version) {
		m_version = version;
	}

	const std::string& getCurrentName() const {
		return m_currentName;
	}

	const std::string& getNameonly() const {
		return m_nameonly;
	}
};

} /* namespace simplearchive */
#endif /* CVERSION_H_ */
