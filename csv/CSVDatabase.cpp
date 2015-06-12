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

#include <memory>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "MediaProperties.h"
#include "CSVDatabase.h"
#include "AssetProperties.h"
#include "CameraInformation.h"

#include "GPSProperties.h"

#include "CopyrightProperties.h"
#include "SAUtils.h"
#include "CSVDBVisitor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

class FolderSet {
	AssetProperties m_assetProperties;
	CameraInformation m_cameraInformation;
	GPSProperties m_GPSProperties;
	MediaProperties m_MediaProperties;
	CopyrightProperties m_copyrightProperties;
	std::string m_path;
public:
	FolderSet(const char *path) {
		m_path = path;
	}
	~FolderSet() {

	}
	void add(MetadataObject &metadataObject);
	std::auto_ptr<MetadataObject> get(const char *image);
	bool put(const char *image, MetadataObject &mo);
};


void FolderSet::add(MetadataObject &metadataObject) {

	m_assetProperties.read(m_path.c_str());
	m_assetProperties.add(metadataObject);
	m_assetProperties.write(m_path.c_str());
	m_cameraInformation.read(m_path.c_str());
	m_cameraInformation.add(metadataObject);
	m_cameraInformation.write(m_path.c_str());
	m_GPSProperties.read(m_path.c_str());
	m_GPSProperties.add(metadataObject);
	m_GPSProperties.write(m_path.c_str());
	m_MediaProperties.read(m_path.c_str());
	m_MediaProperties.add(metadataObject);
	m_MediaProperties.write(m_path.c_str());
	m_copyrightProperties.read(m_path.c_str());
	m_copyrightProperties.add(metadataObject);
	m_copyrightProperties.write(m_path.c_str());
}

std::auto_ptr<MetadataObject> FolderSet::get(const char *image) {
	m_assetProperties.read(m_path.c_str());

	std::auto_ptr<MetadataObject> mo(new MetadataObject);
	unsigned int row = m_assetProperties.findImage(image, 1);
	if (row == std::string::npos) {
		mo.reset();
		return mo;
	}

	m_assetProperties.load(row, *mo);
	m_cameraInformation.read(m_path.c_str());
	m_cameraInformation.load(row, *mo);
	m_GPSProperties.read(m_path.c_str());
	m_GPSProperties.load(row, *mo);
	m_copyrightProperties.read(m_path.c_str());
	m_copyrightProperties.load(row, *mo);
	m_MediaProperties.read(m_path.c_str());
	m_MediaProperties.load(row, *mo);
	return mo;
}

bool FolderSet::put(const char *image, MetadataObject &mo) {
	m_assetProperties.read(m_path.c_str());
	unsigned int row = m_assetProperties.findImage(image, 1);
	if (row == std::string::npos) {
		return false;
	}
	m_assetProperties.save(row, mo);
	m_cameraInformation.save(row, mo);
	m_GPSProperties.save(row, mo);
	m_copyrightProperties.save(row, mo);
	m_MediaProperties.save(row, mo);
	return true;
}


std::auto_ptr<CSVDatabase> CSVDatabase::m_this(0);
std::string CSVDatabase::m_dbpath;

CSVDatabase::CSVDatabase() {
}

CSVDatabase::~CSVDatabase() {
}

CSVDatabase &CSVDatabase::get() {
	if (!m_this.get()) {
		m_this.reset(new CSVDatabase());
	}
	return *(m_this.get());
}

void CSVDatabase::setDBPath(const char *path) {
	m_dbpath = path;
}

void CSVDatabase::addold(MetadataObject &metadataObject, const char *path) {
	// check path exists
	FolderSet folderSet(path);
	folderSet.add(metadataObject);
}





void CSVDatabase::add(MetadataObject &metadataObject, const char *relpath) {
	// check path exists
	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		if (SAUtils::mkDir(m_dbpath.c_str()) == false) {
			throw std::exception();
		}
	}

	std::string relPath = relpath;
	std::string dayStr = relPath.substr(0,10);
	std::string yearStr = relPath.substr(0,4);
	std::string imagename = relPath.substr(11,relPath.length() - 11);
	std::string fullPath = m_dbpath;
	fullPath += '/';
	fullPath += yearStr;
	if (SAUtils::DirExists(fullPath.c_str()) == false) {
		if (SAUtils::mkDir(fullPath.c_str()) == false) {
			throw std::exception();
		}
	}
	fullPath += '/';
	fullPath += dayStr;
	if (SAUtils::DirExists(fullPath.c_str()) == false) {
		if (SAUtils::mkDir(fullPath.c_str()) == false) {
			throw std::exception();
		}
	}
	FolderSet folderSet(fullPath.c_str());
	folderSet.add(metadataObject);
}

const MetadataObject *CSVDatabase::get(unsigned int idx, const char *path) {
	return 0;
}

std::auto_ptr<MetadataObject> CSVDatabase::get(const char *name, const char *path) {
	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		throw std::exception();
	}

	std::string relPath = path;
	std::string dayStr = relPath.substr(0,10);
	std::string yearStr = relPath.substr(0,4);
	std::string imagename = relPath.substr(11,relPath.length() - 11);
	std::string fullPath = m_dbpath;
	fullPath += '/';
	fullPath += yearStr;
	if (SAUtils::DirExists(fullPath.c_str()) == false) {
		throw std::exception();
	}
	fullPath += '/';
	fullPath += dayStr;
	if (SAUtils::DirExists(fullPath.c_str()) == false) {
		throw std::exception();
	}
	FolderSet folderSet(fullPath.c_str());
	return folderSet.get(name);
}

bool CSVDatabase::put(const char *name, const char *path, MetadataObject &mo) {
	std::string relPath = path;
	std::string dayStr = relPath.substr(0,10);
	std::string yearStr = relPath.substr(0,4);
	std::string imagename = relPath.substr(11,relPath.length() - 11);
	std::string fullPath = m_dbpath;
	fullPath += '/';
	fullPath += yearStr;
	if (SAUtils::DirExists(fullPath.c_str()) == false) {
		throw std::exception();
	}
	fullPath += '/';
	fullPath += dayStr;
	if (SAUtils::DirExists(fullPath.c_str()) == false) {
		throw std::exception();
	}

	FolderSet folderSet(fullPath.c_str());
	folderSet.put(name, mo);
	return true;
}

class CSVDBCopyAction : public CSVDBAction {
	std::string m_copyPath;
	std::string m_sourcePath;
	std::string m_currCopyPath;
	std::string m_currSourcePath;
	std::string m_currYear;
protected:
        /// On the start of each directory found, this function is run.
	virtual void onStart() {
		m_currCopyPath = m_copyPath;
		m_currSourcePath = m_sourcePath;
		//printf("onStart %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
	};
	   /// At the end of each directory found, this function is run.
	virtual void onEnd() {
		printf("onEnd\n");
	};
        /// On finding a file, this function is run.
	virtual void onYearFolder(const char *name) {
		m_currYear = name;
		m_currCopyPath += '/';
		m_currCopyPath += name;
		m_currSourcePath += '/';
		m_currSourcePath += name;
		//printf("onYearFolder %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
		if (SAUtils::DirExists(m_currCopyPath.c_str()) == false) {
			if (SAUtils::mkDir(m_currCopyPath.c_str()) == false) {
				throw std::exception();
			}
		}
		//printf("onYearFolder %s: \n", name);
	};
    /// On finding a file, this function is run.
	virtual void onYearEnd() {
		printf("onYearEnd\n");
	};
        /// On finding a directory, this function is run.
	virtual void onDayFolder(const char *name) {
		m_currCopyPath += '/';
		m_currCopyPath += name;
		m_currSourcePath += '/';
		m_currSourcePath += name;
		//printf("onDayFolder %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
		if (SAUtils::DirExists(m_currCopyPath.c_str()) == false) {
			if (SAUtils::mkDir(m_currCopyPath.c_str()) == false) {
				throw std::exception();
			}
		}
		//printf("onDayFolder %s: \n", name);
	};
    /// On finding a directory, this function is run.
	virtual void onDayEnd() {
		m_currCopyPath = m_copyPath;
		m_currSourcePath = m_sourcePath;
		m_currCopyPath += '/';
		m_currCopyPath += m_currYear;
		m_currSourcePath += '/';
		m_currSourcePath += m_currYear;
		//printf("onDayEnd %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
		//printf("onDayEnd\n");
	};
	   /// On finding a directory, this function is run.
	virtual void onDBFile(const char *path, const char *name) {
		std::string currCopyPath = m_currCopyPath;
		currCopyPath += '/';
		currCopyPath += name;
		std::string currSourcePath = m_currSourcePath;
		currSourcePath += '/';
		currSourcePath += name;
		//printf("tonDBFile %s %s\n", currSourcePath.c_str(), currCopyPath.c_str());
		if (SAUtils::copy(currSourcePath.c_str(), currCopyPath.c_str()) == false) {
			throw std::exception();
		}
		//printf("\t\tonDBFile %s: %s\n", path, name);

	};
public:
	CSVDBCopyAction(const char *sourcePath, const char *copyPath) {
		m_copyPath = copyPath;
		m_sourcePath = sourcePath;
	}
};

class CSVDBValidateAction : public CSVDBAction {
	std::string m_copyPath;
	std::string m_sourcePath;
	std::string m_currCopyPath;
	std::string m_currSourcePath;
	std::string m_currYear;
protected:
        /// On the start of each directory found, this function is run.
	virtual void onStart() {
		m_currCopyPath = m_copyPath;
		m_currSourcePath = m_sourcePath;
		//printf("onStart %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
	};
	   /// At the end of each directory found, this function is run.
	virtual void onEnd() {
		printf("onEnd\n");
	};
        /// On finding a file, this function is run.
	virtual void onYearFolder(const char *name) {
		m_currYear = name;
		m_currCopyPath += '/';
		m_currCopyPath += name;
		m_currSourcePath += '/';
		m_currSourcePath += name;
		//printf("onYearFolder %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
		if (SAUtils::DirExists(m_currCopyPath.c_str()) == false) {
			printf("Validate failed %s\n", m_currCopyPath.c_str());
		}
		//printf("onYearFolder %s: \n", name);
	};
    /// On finding a file, this function is run.
	virtual void onYearEnd() {
		printf("onYearEnd\n");
	};
        /// On finding a directory, this function is run.
	virtual void onDayFolder(const char *name) {
		m_currCopyPath += '/';
		m_currCopyPath += name;
		m_currSourcePath += '/';
		m_currSourcePath += name;
		//printf("onDayFolder %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
		if (SAUtils::DirExists(m_currCopyPath.c_str()) == false) {
			printf("Validate failed %s\n", m_currCopyPath.c_str());
		}
		//printf("onDayFolder %s: \n", name);
	};
    /// On finding a directory, this function is run.
	virtual void onDayEnd() {
		m_currCopyPath = m_copyPath;
		m_currSourcePath = m_sourcePath;
		m_currCopyPath += '/';
		m_currCopyPath += m_currYear;
		m_currSourcePath += '/';
		m_currSourcePath += m_currYear;
		//printf("onDayEnd %s %s\n", m_currSourcePath.c_str(), m_currCopyPath.c_str());
		//printf("onDayEnd\n");
	};
	   /// On finding a directory, this function is run.
	virtual void onDBFile(const char *path, const char *name) {
		std::string currCopyPath = m_currCopyPath;
		currCopyPath += '/';
		currCopyPath += name;
		std::string currSourcePath = m_currSourcePath;
		currSourcePath += '/';
		currSourcePath += name;
		printf("tonDBFile %s %s\n", currSourcePath.c_str(), currCopyPath.c_str());
		if (SAUtils::verify(currSourcePath.c_str(), currCopyPath.c_str()) == false) {
			printf("Validate failed %s\n", m_currCopyPath.c_str());
		}
		//printf("\t\tonDBFile %s: %s\n", path, name);

	};
public:
	CSVDBValidateAction(const char *sourcePath, const char *copyPath) {
		m_copyPath = copyPath;
		m_sourcePath = sourcePath;
	}
};

bool CSVDatabase::copy(const char *path) {
	CSVDBVisitor csvdbVisitor(new CSVDBCopyAction(m_dbpath.c_str(), path));
	csvdbVisitor.process(m_dbpath.c_str());
	return true;
}

bool CSVDatabase::validate(const char *path) {
	CSVDBVisitor csvdbVisitor(new CSVDBValidateAction(m_dbpath.c_str(), path));
	csvdbVisitor.process(m_dbpath.c_str());
	return true;
}
/*
std::auto_ptr<MetadataObject> CSVDatabase::get(const char *name, const char *path) {
	FolderSet folderSet(path);
	return folderSet.get(name);
}
*/
} /* namespace simplearchive */
