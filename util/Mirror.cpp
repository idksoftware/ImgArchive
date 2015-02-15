/*
 * Mirror.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: wzw7yn
 */
#include "stdio.h"
#include "Mirror.h"
#include "SAUtils.h"
#include "ConfigReader.h"
#include "StagingManager.h"
namespace simplearchive {

Mirror *Mirror::m_this = 0;
class MirrorItem {
public:
	typedef enum {
		MM_Unknown,
		MM_Direct,
		MM_Indirect
	} MirrorMode;

	typedef enum {
		MT_Unknown,
		MT_Mirror,
		MT_Staging
	} MirrorType;
private:
	bool m_error;
	/// Name of mirror
	std::string m_name;
	/// Path to mirror or staging area
	std::string m_path;
	/// Direct/Indirect Mode
	MirrorMode m_eMode;
	/// Direct Write/Staged Write Mode
	MirrorType m_eType;
	StagingManager *m_stagingManager;
public:
	MirrorItem(const char *root, const char *name, const char *dataString) {
		m_error = false;
		m_name = name;
		m_stagingManager = 0;
		std::string data = dataString;
		int delim1 = data.find_first_of(",");
		int delim2 = data.find_first_of(",", delim1+1);
		std::string mode = data.substr(0,delim1);
		m_path = data.substr(delim1+1, delim2-(delim1+1));
		std::string type = data.substr(delim2+1, data.length());
		if (mode.compare("direct") == 0) {
			m_eMode = MM_Direct;
		} else if (mode.compare("indirect") == 0) {
			m_eMode = MM_Indirect;
		} else {
			m_eMode = MM_Unknown;
			m_error = true;
		}
		if (type.compare("mirror") == 0) {
			m_eType = MT_Mirror;
		} else if (type.compare("stage") == 0) {
			m_eType = MT_Staging;
			if (m_eMode == MM_Direct) {
				m_stagingManager = new StagingManager(root, m_name.c_str(), m_path.c_str());
			}
		} else {
			m_eType = MT_Unknown;
			m_error = true;
		}

	}
	~MirrorItem() {
		if (m_stagingManager == 0) {
			delete m_stagingManager;
		}
	}

	bool isError() const {
		return m_error;
	}

	const char *getModeString() const {
		switch (m_eMode) {
		case MM_Direct: return "Direct";
		case MM_Indirect: return "Indirect";
		case MM_Unknown: return "Unknown";
		}
		return "Unknown";
	}

	MirrorMode getMode() const {
		return m_eMode;
	}

	const std::string& getName() const {
		return m_name;
	}

	const std::string& getPath() const {
		return m_path;
	}

	const char *getTypeString() const {
		switch (m_eType) {
		case MT_Mirror: return "Mirror";
		case MT_Staging: return "Staging";
		case MT_Unknown: return "Unknown";
		}
		return "Unknown";
	}

	MirrorType getType() const {
		return m_eType;
	}

	void print() {
		printf("Name: \"%s\"\n", m_name.c_str());
		printf("Path: \"%s\"\n", m_path.c_str());
		printf("Mode: \"%s\"\n", getModeString());
		printf("Type: \"%s\"\n", getTypeString());
	}

	StagingManager* getStagingManager() {
		return m_stagingManager;
	}
};

class MirrorItemContainer : public std::vector<MirrorItem> {};


Mirror::Mirror() {
	m_pContainer = new MirrorItemContainer;
}

Mirror::~Mirror() {
	delete m_pContainer;
}

bool Mirror::readConf() {
	ConfigReader configReader;
	Config config;
	configReader.read(m_confpath.c_str(), config);
	for (std::map<std::string, std::string>::iterator ii = config.begin(); ii != config.end(); ++ii) {
			MirrorItem mirrorItem(m_root.c_str(), (*ii).first.c_str(), (*ii).second.c_str());
			mirrorItem.print();
			m_pContainer->insert(m_pContainer->end(), mirrorItem);
		}
	return true;
}

Mirror &Mirror::get() {

	if (m_this == 0) {
		throw std::exception();
	}
	return *m_this;
}

void Mirror::initalise(const char *rootPath, const char *confpath) {
	if (m_this == 0) {
		m_this = new Mirror();

	}
	m_this->m_root = rootPath;
	m_this->m_confpath = confpath;
	m_this->readConf();
}

bool Mirror::mkDir(const char *path) {

	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		if (data.getMode() == MirrorItem::MM_Direct && data.getType() == MirrorItem::MT_Mirror) {
			std::string fullpath = data.getPath() + '/' + path;
			if (SAUtils::mkDir(fullpath.c_str()) == false) {
				return false;
			}
		}
	}
	return true;
}

bool Mirror::copy(const char *to) {
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		std::string fromFull = m_root + to;
		if (data.getMode() == MirrorItem::MM_Direct) {
			if (data.getType() == MirrorItem::MT_Mirror) {
				std::string fullpath = data.getPath() + '/' + to;
				if (SAUtils::copy(fromFull.c_str(), fullpath.c_str()) == false) {
					return false;
				}
			} else if (data.getType() == MirrorItem::MT_Staging) {
				StagingManager *stagingManager = 0;
				if ((stagingManager = data.getStagingManager()) != 0) {
					stagingManager->put(to);
				}
			}

		}
	}
	return true;
}

bool Mirror::verify(const char *to) {
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		std::string fromFull = m_root + to;
		std::string toFull = data.getPath() + '/' + to;
		if (data.getMode() == MirrorItem::MM_Direct) {
			if (data.getType() == MirrorItem::MT_Mirror) {
				std::string fullpath = data.getPath() + to;
				if (SAUtils::verify(fromFull.c_str(), fullpath.c_str()) == false) {
					return false;
				}
			}
		}



		//if (SAUtils::copy(from, fullpath.c_str()) == false) {
		//	return false;
		//}
	}
	return true;
}
bool Mirror::FileExists(const char *filename) {
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		std::string fullpath = data.getPath() + '/' + filename;
		if (SAUtils::FileExists(fullpath.c_str()) == false) {
			return false;
		}
	}
	return true;
}
bool Mirror::DirExists(const char *path) {
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		if (data.getMode() == MirrorItem::MM_Direct && data.getType() == MirrorItem::MT_Mirror) {
			std::string fullpath = data.getPath() + path;
			if (SAUtils::DirExists(fullpath.c_str()) == false) {
				return false;
			}
		}
	}
	return true;
}

bool Mirror::IsFile(const char *path) {
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		std::string fullpath = data.getPath() + '/' + path;
		if (SAUtils::IsFile(fullpath.c_str()) == false) {
			return false;
		}
	}
	return true;
}

} /* namespace simplearchive */
