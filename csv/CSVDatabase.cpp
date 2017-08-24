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
#include "CSVDatabase.h"
#include "MetaType.h"
#include "SAUtils.h"
#include "CSVDBVisitor.h"
#include "PathController.h"
#include "DBDefines.h"
#include "MetadataObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {
	
class MirrorDB {
	std::string m_path;
	std::string m_primary;
	std::string m_backup1;
	std::string m_backup2;
	std::string m_master;
	std::string makeFolders(std::string &pasePath, std::string &relPath);
	bool copy(std::string &from, std::string &to);
	bool copyDB(std::string &from, std::string &to, const char *name);

public:
	MirrorDB(const char *rootPath);
	~MirrorDB() {};
	bool process(std::string &shortPath);
};

/*
class MetadataSet {
	
	MTTable& m_Metadata;
	
	
	std::string m_path;
	MTDatabase &m_db;
public:
	MetadataSet(MTDatabase db, const char *path) : m_db(db), m_path(path),
		m_Metadata(db.getTable(MetadataTable)) {}
	~MetadataSet() {};
	void add(MetadataObject &metadataObject);
	std::shared_ptr<MetadataObject> get(const char *image);
	bool put(const char *image, MetadataObject &mo);
};


void MetadataSet::add(MetadataObject &metadataObject) {

	std::string filename = m_Metadata.getSchema().getName() + ".csv";
	m_Metadata.read(m_path.c_str(), filename.c_str());
	m_Metadata.addRow(metadataObject);
	m_Metadata.write(m_path.c_str(), filename.c_str());

	
	
}

std::shared_ptr<MetadataObject> MetadataSet::get(const char *image) {
	std::shared_ptr<MetadataObject> mo(new MetadataObject);
	
	m_assetProperties.read(m_path.c_str());


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

bool MetadataSet::put(const char *image, MetadataObject &mo) {
	
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

*/

class MetadataPartition : public MTTable {
public:
	MetadataPartition() : MTTable(new MetadataSchema) {};
	virtual ~MetadataPartition() {};
};

std::shared_ptr<CSVDatabase> CSVDatabase::m_this(0);
std::string CSVDatabase::m_dbpath;

//std::shared_ptr<CSVVersionDatabase> CSVVersionDatabase::m_this(0);
//std::string CSVVersionDatabase::m_dbpath;

CSVDatabase::CSVDatabase() {
	
	//m_mirrorDB.reset(new MirrorDB(m_dbpath.c_str()));
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
	
	
	MetadataPartition metadataPartition;
	std::string filename = metadataPartition.getSchema().getName() + ".csv";
	if (metadataPartition.read(fullPath.c_str(), filename.c_str()) == false) {
		if (ErrorCode::getErrorCode() != SIA_ERROR::OPEN_ERROR) {
			// file may not exist
			throw std::exception();
		}
	}
	if (metadataPartition.addRow(metadataObject) == false) {
		throw std::exception();
	}
	if (metadataPartition.write(fullPath.c_str(), filename.c_str()) == false) {
		throw std::exception();
	}

	//m_mirrorDB->process(relPath);
}

const MetadataObject *CSVDatabase::get(unsigned int idx, const char *path) {
	return 0;
}

SharedMTRow CSVDatabase::get(const char *name, const char *path) {
	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		throw std::exception();
	}

	PathController pathController(m_dbpath.c_str());
	if (pathController.isValid() == false) {
		return false;
	}
	pathController.setRelativePath(path);
	
	pathController.makeImagePath();
	std::string fullPath = pathController.getFullPath();
	if (pathController.splitPathAndFile(fullPath.c_str()) == false) {
		return false;
	}
	MetadataPartition metadataPartition;
	std::string filename = metadataPartition.getSchema().getName() + ".csv";
	std::string rootPath = pathController.getRoot();
	std::string imagename = pathController.getImage();
	if (metadataPartition.read(rootPath.c_str(), filename.c_str()) == false) {
		if (ErrorCode::getErrorCode() != SIA_ERROR::OPEN_ERROR) {
			// file may not exist
			throw std::exception();
		}
	}
	const MTSchema& info = metadataPartition.getSchema(static_cast<int>(MetadataObjectIndex::MD_FILENAME_IDX));
	MTColumn match(info);
	match.set(name);
	metadataPartition.find(match);
	SharedMTRow row = metadataPartition.getCurrentRow();
	SharedMTRow tmprow = metadataPartition.makeRow();
	tmprow->join(*row);
	return tmprow;
	
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

	//MTDatabase &db = *m_mtDatabase;
	//MetadataSet metadataSet(db, fullPath.c_str());
	//metadataSet.put(name, mo);
	return true;
}

MirrorDB::MirrorDB(const char *rootPath) {
	m_path = rootPath;

	if (ArchivePath::isMasterEnabled() == true) {
		m_master = ArchivePath::getMaster().getCSVDatabasePath();
	}
	if (ArchivePath::isBackup1Enabled() == true) {
		m_backup1 = ArchivePath::getBackup1().getCSVDatabasePath();
		
	}
	if (ArchivePath::isBackup2Enabled() == true) {
		m_backup2 = ArchivePath::getBackup2().getCSVDatabasePath();;
	}

}

bool MirrorDB::copyDB(std::string &from, std::string &to, const char *name) {
	std::string fromfull = from;
	fromfull += '/'; fromfull += name;
	std::string tofull = to;
	tofull += '/'; tofull += name;
	if (SAUtils::copy(fromfull.c_str(), tofull.c_str()) == false) {
		return false;
	}
	return true;
}

bool MirrorDB::copy(std::string &from, std::string &to) {
	if (copyDB(from, to, MetadataTableFilename) == false) {
		return false;
	}
	
	return true;
}

std::string MirrorDB::makeFolders(std::string &basePath, std::string &relPath) {
	std::string dayStr = relPath.substr(0, 10);
	std::string yearStr = relPath.substr(0, 4);

	std::string fullPath = basePath;

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
	return fullPath;
}

bool MirrorDB::process(std::string &relPath) {
	
	std::string dayStr = relPath.substr(0, 10);
	std::string yearStr = relPath.substr(0, 4);
	std::string imagename = relPath.substr(11, relPath.length() - 11);
	std::string fromfull = m_path;
	fromfull += '/'; fromfull += yearStr;
	fromfull += '/'; fromfull += dayStr;
	
	
	if (ArchivePath::isMasterEnabled() == true) {
		std::string fullPath = makeFolders(m_master, relPath);
		if (copy(fromfull, fullPath) == false) {
			return false;
		}
	}
	if (ArchivePath::isBackup1Enabled() == true) {
		std::string fullPath = makeFolders(m_backup1, relPath);
		if (copy(fromfull, fullPath) == false) {
			return false;
		}
	}
	if (ArchivePath::isBackup2Enabled() == true) {
		std::string fullPath = makeFolders(m_backup2, relPath);
		if (copy(fromfull, fullPath) == false) {
			return false;
		}
	}

	return true;
}

/********************************/



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
	MetadataSet metadataSet(path);
	return MetadataSet.get(name);
}
*/
} /* namespace simplearchive */
