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

/*

 */

#include "stdio.h"
#include "Mirror.h"
#include "SAUtils.h"
#include "ConfigReader.h"
#include "StagingManager.h"
#include "CheckDisk.h"
#include "VerifyMirror.h"
#include "CSVArgs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

Mirror *Mirror::m_this = 0;
MirrorItemContainer *Mirror::m_pContainer = 0;

class MirrorItem {
public:

	/// Direct or Indirect copies.
	/// Direct copies do not use a script to copy
	/// Uses a script to copy. This is used to copy to say Google Drive,
	typedef enum {
		MM_Unknown,
		MM_Direct,
		MM_Indirect
	} MirrorMode;

	/// Mirror Type
	/// Direct mirror
	/// Staging mirror
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
	/// Copy script name
	std::string m_remotePath;
	/// include file name
	std::string m_includeName;
	/// Copy script name
	std::string m_scriptName;
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

		std::string type;
		std::string opt1;
		std::string opt2;
		std::string opt3;
		std::string mode;

		//bool complete = false;
		CSVArgs csvArgs(',');
		csvArgs.process(dataString);
		int size = csvArgs.size();
		
		if (size > 0) {
			mode = csvArgs.at(0);
		}
		if (size > 1) {
			m_path = csvArgs.at(1);
		}
		if (size > 2) {
			type = csvArgs.at(2);
		}
		if (size > 3) {
			opt1 = csvArgs.at(3);
		}
		if (size > 4) {
			opt2 = csvArgs.at(4);
		}
		if (size > 5) {
			opt3 = csvArgs.at(5);
		}

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
	ConfigBlock config;
	if (configReader.read(m_confpath.c_str(), config) == false) {
		return false;
	}
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

bool Mirror::initalise(const char *rootPath, const char *confpath) {
	if (m_this == 0) {
		m_this = new Mirror();

	}
	m_this->m_root = rootPath;
	m_this->m_confpath = confpath;
	if (m_this->readConf() == false) {
		return false;
	}

	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		if (SAUtils::DirExists(data.getPath().c_str()) == false) {
			std::string errString;
			//SAUtils::sprintf(errString, "Mirror name:%s cannot find path \"%s\"", data.getName().c_str(), data.getPath().c_str());
			//sprintf_s(errString, "Mirror name:%s cannot find path \"%s\"", data.getName().c_str(), data.getPath().c_str());
			//throw MirrorException("Mirror: %s, Path not found %s", data.getName().c_str(), data.getPath().c_str());
			return false;
		}
	}
	return true;
}

bool Mirror::mkDir(const char *path) {

	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		if (data.getMode() == MirrorItem::MM_Direct && data.getType() == MirrorItem::MT_Mirror) {
			std::string fullpath = data.getPath() + '/' + path;
			if (SAUtils::DirExists(data.getPath().c_str()) == false) {
				if (SAUtils::mkDir(fullpath.c_str()) == false) {
					return false;
				}
			}
		}
	}
	return true;
}

bool Mirror::copy(const char *to) {
	std::string toRel = to;
	int idx = toRel.find_last_of('/');
	std::string toPath = toRel.substr(0, idx);
	
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		std::string fromFull = m_root + '/' + to;
		if (data.getMode() == MirrorItem::MM_Direct) {
			if (data.getType() == MirrorItem::MT_Mirror) {
				std::string fullpath = data.getPath() + '/' + to;
				if (SAUtils::makePath(data.getPath().c_str(), toPath.c_str()) == false) {
					return false;
				}
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

bool Mirror::syncFile(const char *to) {
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		std::string fromFull = m_root + '/' + to;
		if (data.getMode() == MirrorItem::MM_Direct) {
			if (data.getType() == MirrorItem::MT_Mirror) {
				std::string fullpath = data.getPath() + '/' + to;
				if (SAUtils::FileExists(fullpath.c_str()) == false) {
					printf("Copying %s to %s\n", fromFull.c_str(), fullpath.c_str());
					if (SAUtils::copy(fromFull.c_str(), fullpath.c_str()) == false) {
						return false;
					}
				} else if (SAUtils::verify(fromFull.c_str(), fullpath.c_str()) == false) {
					printf("Copying %s to %s\n", fromFull.c_str(), fullpath.c_str());
					if (SAUtils::copy(fromFull.c_str(), fullpath.c_str()) == false) {
						return false;
					}
				}

			/*
			} else if (data.getType() == MirrorItem::MT_Staging) {
				StagingManager *stagingManager = 0;
				if ((stagingManager = data.getStagingManager()) != 0) {
					stagingManager->put(to);
				}
			}
			*/
			}
		}
	}
	return true;
}

bool Mirror::syncDir(const char *to) {
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		std::string fromFull = m_root + '/' + to;
		if (data.getMode() == MirrorItem::MM_Direct) {
			if (data.getType() == MirrorItem::MT_Mirror) {
				std::string fullpath = data.getPath() + '/' + to;
				if (SAUtils::DirExists(fullpath.c_str()) == false) {
					printf("Making directory %s\n", fullpath.c_str());
					if (SAUtils::mkDir(fullpath.c_str()) == false) {
						return false;
					}
				}
			/*
			} else if (data.getType() == MirrorItem::MT_Staging) {
				StagingManager *stagingManager = 0;
				if ((stagingManager = data.getStagingManager()) != 0) {
					stagingManager->put(to);
				}
			}
			*/
			}
		}
	}
	return true;
}
bool Mirror::mapFileVerify(const char *path) {
	CheckDisk m_checkDisk;
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		if (data.getMode() == MirrorItem::MM_Direct && data.getType() == MirrorItem::MT_Mirror) {
			std::string fullSavePath = data.getPath() + path + "/.chk";
			if (SAUtils::DirExists(fullSavePath.c_str()) == false) {
				if (SAUtils::mkDir(fullSavePath.c_str()) == false) {

					return false;
				}
			}
			std::string fullTargetPath = m_root + path;
			// needs working on
			//if (m_checkDisk.check(fullTargetPath.c_str(), fullSavePath.c_str()) == true) {
			//	return false;
			//}
		}
	}
	return true;
}

bool Mirror::verifyMirrors() {
	VerifyMirror verifyMirror;
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		std::string fullSavePath = data.getPath() + "/.chk";
		if (data.getMode() == MirrorItem::MM_Direct) {
			if (data.getType() == MirrorItem::MT_Mirror) {
				verifyMirror.verifyMirror(data.getPath().c_str(), fullSavePath.c_str());
			}
		}

	}
	return true;
}

bool Mirror::verifySource() {
	bool ret = true;
	VerifyMirror verifyMirror;
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		std::string fullSavePath = data.getPath() + "/.chk";
		if (data.getMode() == MirrorItem::MM_Direct) {
			if (data.getType() == MirrorItem::MT_Mirror) {
				if (verifyMirror.verifySource(m_root.c_str(), fullSavePath.c_str()) == false) {
					printf("Errors found in %s\n", m_root.c_str());
					ret = false;
				}
			}
		}

	}
	return ret;
}

bool Mirror::verifyMirrorName(const char *to) {
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		//std::string fromFull = m_root + '/' + to;
		std::string toFull = data.getPath() + '/' + to;
		if (data.getMode() == MirrorItem::MM_Direct) {
			if (data.getType() == MirrorItem::MT_Mirror) {
				if (SAUtils::FileExists(toFull.c_str()) == false) {
					return false;
				}
			}
		}

	}
	return true;
}

bool Mirror::verifySourceName(const char *to) {
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		//MirrorItem &data = *i;
		std::string fromFull = m_root + '/' + to;
		//std::string toFull = data.getPath() + '/' + to;
		//if (data.getMode() == MirrorItem::MM_Direct) {
		//	if (data.getType() == MirrorItem::MT_Mirror) {
				if (SAUtils::FileExists(fromFull.c_str()) == false) {
					return false;
				}
		//	}
		//}

	}
	return true;
}


bool Mirror::verifyContents(const char *to) {
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		std::string fromFull = m_root + '/' + to;
		std::string toFull = data.getPath() + '/' + to;
		if (data.getMode() == MirrorItem::MM_Direct) {
			if (data.getType() == MirrorItem::MT_Mirror) {

				if (SAUtils::verify(fromFull.c_str(), toFull.c_str()) == false) {
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

bool Mirror::makeCheckfile(const char *path) {
	CheckDisk m_checkDisk;
	for (std::vector<MirrorItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		MirrorItem &data = *i;
		if (data.getMode() == MirrorItem::MM_Direct && data.getType() == MirrorItem::MT_Mirror) {
			std::string fullSavePath = data.getPath() + "/.chk";
			if (SAUtils::DirExists(fullSavePath.c_str()) == false) {
				if (SAUtils::mkDir(fullSavePath.c_str()) == false) {
					return false;
				}
			}
			fullSavePath += '/';
			fullSavePath += makeFileName(path);
			if (SAUtils::DirExists(fullSavePath.c_str()) == false) {
				if (SAUtils::mkDir(fullSavePath.c_str()) == false) {
					return false;
				}
			}
			std::string fullTargetPath = m_root + path;

			if (m_checkDisk.makeCheckData(fullTargetPath.c_str(), fullSavePath.c_str()) == false) {
				return false;
			}
		}
	}
	return true;
}
#define DIR_DELIM '@'
std::string Mirror::makeFileName(const char *path) {
	std::string tmp = path;
	std::string out;
	if (tmp.length() == 0) {
		out.insert(out.end(), DIR_DELIM);
	}
	for (std::string::iterator i = tmp.begin(); i != tmp.end(); i++) {
		char c = *i;
		if (c == '/') {
			c = DIR_DELIM;
		} else if (c == DIR_DELIM) {
			out.insert(out.end(), '?');
		}
		out.insert(out.end(), c);
	}
	printf("dir %s\n", out.c_str());
	return out;
}

} /* namespace simplearchive */
