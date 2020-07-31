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
#include "DerivativeRepository.h"

bool DerivativesObject::init(RepositoryPath& repositoryPath, const char* workspacePath) {
	CLogger& logger = CLogger::getLogger();

	*m_repositoryPath = repositoryPath;
	m_derivativeIndexTable->setPath(repositoryPath.getCSVDatabasePath().c_str());
	m_workspacePath = workspacePath;

	CSVDerivativeDatabase& derivativeRepository = CSVDerivativeDatabase::get();
	derivativeRepository.setDBPath(m_repositoryPath->getRepositoryPath().c_str());

	if (ArchivePath::isDerivativeBackup1Enabled() == true) {
		logger.log(LOG_OK, CLogger::Level::SUMMARY, "Backup 1 enabled, using folder: \"%s\"", ArchivePath::getMasterBackup1Path().c_str());
		if (SAUtils::DirExists(ArchivePath::getDerivativeBackup1Path().c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Backup 1 folder: \"%s\" not accessable?", ArchivePath::getMasterBackup1Path().c_str());
			return false;
		}
		RepositoryPath& backupPath1 = ArchivePath::getDerivativeBackup1();
		if (backupPath1.settup() == false) {
			return false;
		}
		m_backup[0].init(backupPath1);
	}
	else {
		logger.log(LOG_OK, CLogger::Level::INFO, "Backup 1 not enabled");
	}

	if (ArchivePath::isDerivativeBackup2Enabled() == true) {
		logger.log(LOG_OK, CLogger::Level::SUMMARY, "Backup 2 enabled, using folder: \"%s\"", ArchivePath::getMasterBackup1Path().c_str());
		if (SAUtils::DirExists(ArchivePath::getDerivativeBackup1Path().c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Backup 2 folder: \"%s\" not accessable?", ArchivePath::getMasterBackup1Path().c_str());
			return false;
		}
		RepositoryPath& backupPath2 = ArchivePath::getDerivativeBackup2();
		if (backupPath2.settup() == false) {
			return false;
		}
		m_backup[1].init(backupPath2);
	}
	else {
		logger.log(LOG_OK, CLogger::Level::INFO, "Backup 2 not enabled");
	}

	//derivativeRepository.setPathToActiveRoot(workspacePath);
	//if (!m_imageIndex->init(repositoryPath.getImageIndexPath().c_str())) {
	//	return false;
	//}

	IntegrityManager& integrityManager = IntegrityManager::get();
	integrityManager.setDerivativeBackupPaths(ArchivePath::getDerivativeBackup1Path().c_str(),
		ArchivePath::getDerivativeBackup2Path().c_str(),
		ArchivePath::isDerivativeBackup1Enabled(),
		ArchivePath::isDerivativeBackup2Enabled());
	return true;
}

bool DerivativesObject::checkin(FileInfo& fileInfo, const char* comment) {
	return true;
}
bool DerivativesObject::isBackup1Enabled() {
	return m_backup[0].getRepositoryPath().isEnabled();
}
bool DerivativesObject::isBackup2Enabled() {
	return m_backup[1].getRepositoryPath().isEnabled();
}


RepositoryObject& DerivativesObject::getBackup1Object()
{
	return m_backup[0];
}

RepositoryObject& DerivativesObject::getBackup2Object()
{
	return m_backup[1];
}

bool DerivativesObject::copyFile(const std::string& pathToSourceRoot, const std::string& yyyymmddStr, const std::string& fileName, const std::string& sequenceName) {
	//m_repositoryObject->copyFile(pathToSourceRoot, fileName, sequenceName);

	if (isBackup1Enabled() == true) {
		if (m_backup[0].copyFile(pathToSourceRoot, fileName, sequenceName) == false) {
			return false;
		}
	}
	if (isBackup2Enabled() == true) {
		if (m_backup[1].copyFile(pathToSourceRoot, fileName, sequenceName) == false) {
			return false;
		}
	}

	// this can only be done after the file copy
	//integrityManager.addFile(m_yyyymmddStr.c_str(), file.c_str());
	return true;
}

bool DerivativesObject::validate(const char* dbImage, const char* sourceImage)
{
	return false;
}

bool DerivativesObject::addimage(const char* sourceRelPath, FileInfo& fileInfo, const char* comment, int primarySeqNumber, int derivativeSeqNumber, int version, MetadataObject& metadataObject) {

	CSVDerivativeDatabase& derivativeRepository = CSVDerivativeDatabase::get();
	m_derivativeMetadata = std::make_shared<DerivativeMetadataRow>();
	PathController pathController(fileInfo.getPath().c_str());
	pathController.split();

	VersionControl& versionControl = VersionControl::getInstance();
	versionControl.setCurrentVersion(m_repositoryPath->getRepositoryPath().c_str(), sourceRelPath, fileInfo.getName().c_str(), version);
	m_repositoryPath->setImageName(versionControl.getCurrentVersion().getVersionName().c_str());

	m_derivativeMetadata->columnAt(DB_SEQUENCEID) = primarySeqNumber;
	m_derivativeMetadata->columnAt(DB_VERSION) = version;
	m_derivativeMetadata->columnAt(DB_DATABASEID) = derivativeSeqNumber;
	m_derivativeMetadata->columnAt(DB_FILENAME) = versionControl.getCurrentVersion().getVersionName().c_str();
	m_derivativeMetadata->columnAt(DB_ORGINALNAME) = fileInfo.getName().c_str();
	Version& versionObj = (Version&)versionControl.getCurrentVersion();
	std::string shortRelPath = versionObj.getShortRelPath();
	shortRelPath += '/';
	shortRelPath += versionObj.makeEncodedVersion();
	m_derivativeMetadata->columnAt(DB_FILEPATH) = shortRelPath.c_str();

	m_derivativeMetadata->columnAt(DB_CRC) = fileInfo.getCrc();
	m_derivativeMetadata->columnAt(DB_MD5) = fileInfo.getMd5().c_str();
	m_derivativeMetadata->columnAt(DB_UUID) = fileInfo.getUuid().c_str();
	m_derivativeMetadata->columnAt(DB_FILESIZE) = (int)fileInfo.getSize();
	m_derivativeMetadata->columnAt(DB_DATEMODIFIED) = fileInfo.getModTime();
	m_derivativeMetadata->columnAt(DB_DATECREATE) = fileInfo.getCreateTime();
	ExifDateTime addTime;
	ExifDate addDate;
	m_derivativeMetadata->columnAt(DB_DATEADDED) = addTime;
	m_derivativeMetadata->columnAt(DB_VERSIONPATH) = addDate.toShortRelativePath().c_str();

	CLogger& logger = CLogger::getLogger();
	try {
		derivativeRepository.add(*m_derivativeMetadata);
	}
	catch (std::exception e) {

		return false;
	}

	if (SQLiteDB::add(*m_derivativeMetadata) == false) {
		return false;
	}



	DerivativeIndexTable& derivativeIndexTable = getDerivativeIndexTable();
	if (derivativeIndexTable.insert(derivativeSeqNumber, pathController.getYearday().c_str(), fileInfo.getName().c_str(), fileInfo.getSize(), fileInfo.getCrc(), fileInfo.getMd5().c_str(),
		fileInfo.getUuid().c_str(), version, addDate) == false) {
		return false;
	}


	std::string ddmmyy = addDate.toShortRelativePath();
	if (settupRelative(ddmmyy) == false) {
		return false;
	}

	if (versionControl.checkinCurrentVersion(m_repositoryPath->getDataPath().c_str(), fileInfo.getPath().c_str()) == false) {
		return false;
	}

	IntegrityManager& integrityManager = IntegrityManager::get();
	integrityManager.addDerivativeDayFolder(m_repositoryPath->getYYYYMMDD().c_str());
	integrityManager.addDerivativeFile(m_repositoryPath->getYYYYMMDD().c_str(), versionObj.makeEncodedVersion());
	return true;
}
*/