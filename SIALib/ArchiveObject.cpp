
#include "AppConfig.h"
#include "AppPaths.h"
#include "SACmdArgs.h"
#include "ArchiveDate.h"
#include "ArchiveBuilder.h"
#include "StandardOut.h"
#include "ArchiveRepository.h"
#include "AddressScope.h"

#include "ImageExtentions.h"
#include "ExifObject.h"
#include "MetadataTemplate.h"
#include "MetadataObject.h"
#include "ImageFileReader.h"
#include "ImageIndex.h"
#include "SAUtils.h"
#include "CLogger.h"
//#include "ImageGroup.h"
#include "TargetsList.h"
#include "FileInfo.h"
#include "BasicMetadataFactory.h"
#include "XMLWriter.h"
#include "JSONWriter.h"
#include "HTMLWriter.h"
#include "ImagePath.h"
//#include "CSVDBFile.h"
#include "Database.h"
#include "SQLiteDB.h"
#include "HistoryEvent.h"
#include "History.h"
#include "pathcontroller.h"

#include "CSVDatabase.h"
#include "CSVDerivativeDatabase.h"
#include "IntegrityManager.h"
#include "HookCmd.h"
#include "ViewManager.h"
#include "Version.h"
#include "VersionControl.h"
#include "ImportJournal.h"
#include "CheckoutStatus.h"
#include "ContentsLister.h"
#include "ExportImages.h"
#include "CheckFile.h"
#include "ErrorCode.h"
#include "IndexVisitor.h"
#include "SQLiteDB.h"
#include "SequenceFileManager.h"

#include <stdio.h>
#include <sstream>
#include <vector>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

#ifdef _WIN32
#include <crtdbg.h>
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

#include "GetImagesManager.h"
#include "ArchiveObject.h"
#include "AppPaths.h"
#include "ArchivePath.h"
#include "SAUtils.h"
#include "VersionMetadataObject.h"
#include "CSVMasterDatabase.h"
#include "VersionControl.h"
#include "FileInfo.h"



#undef FILECODE
#define FILECODE ARCHIVEOBJECT_CPP
#define LOG_COMPLETED_OK						ARCHIVEOBJECT_CPP
#define LOG_CHECKOUT_FUNCTION					(LOG_COMPLETED_OK + 1)
#define LOG_UNABLE_TO_CHECKOUT_GENERAL			(LOG_CHECKOUT_FUNCTION + 1)
#define LOG_UNABLE_TO_CHECKIN_GENERAL			(LOG_UNABLE_TO_CHECKOUT_GENERAL + 1)
#define LOG_UNCHECKOUT_FUNCTION					(LOG_UNABLE_TO_CHECKIN_GENERAL + 1)
#define LOG_UNABLE_TO_UNCHECKOUT_GENERAL		(LOG_UNCHECKOUT_FUNCTION + 1)

namespace simplearchive {

	bool PrimaryIndexObject::init(PrimaryIndexPath &primaryIndexPath) {

		VersionControl& versionControl = VersionControl::getInstance();
		versionControl.setVersionPath(primaryIndexPath.getHistoryPath().c_str());
		m_primaryIndexTable->setPath(primaryIndexPath.getIdxDbPath().c_str());
		if (!m_imageIndex->init(primaryIndexPath.getImageIndexPath().c_str())) {
			return false;
		}

		*m_primaryIndexPath = primaryIndexPath;

		return true;
	}

	bool PrimaryIndexObject::addMasterImage(const BasicMetadata &basicMetadata, ImagePath &imagePath, int masterSeqNumber, int primarySeqNumber) {
		unsigned long seqNumber = m_primaryIndexTable->getNextIndex();

		ExifDate date;

		if (m_primaryIndexTable->insert(seqNumber, imagePath.getRelativePath().c_str(), imagePath.getImageName().c_str(), basicMetadata.getSize(), basicMetadata.getCrc(), basicMetadata.getMd5().c_str(),
			basicMetadata.getUuid().c_str(), basicMetadata.getVersion(), date, masterSeqNumber) == false) {
			return false;
		}
		//m_imageIndex->updateVersion(basicMetadata.getCrc(), imagePath.getRelativePath().c_str());
		//this->m_primaryIndexPath.get()->getImageIndexPath();
		VersionControl::getInstance().createMasterVersion(basicMetadata, imagePath.getYyyymmddStr().c_str(), masterSeqNumber, primarySeqNumber);
		m_imageIndex->updatePath(basicMetadata.getCrc(), imagePath.getRelativePath().c_str(), 0);
		return true;
	}

	bool PrimaryIndexObject::addDerivativeImage(FileInfo& fileInfo, const char *comment, int primarySeqNumber, int derivativeSeqNumber, const Version& version) {


		PathController pathController(fileInfo.getPath().c_str());
		if (pathController.split() == false) {
			return false;
		}
		ExifDate addDate;
		VersionControl& versionControl = VersionControl::getInstance();
		if (versionControl.createDerivativeVersion(fileInfo, derivativeSeqNumber, primarySeqNumber) == false) {
			return false;
		}

		unsigned long seqNumber = m_primaryIndexTable->getNextIndex();


		int versionInt = versionControl.getVersion();
		//	This is the rolling CSV table that holds all images in the database 
		// root path is IMGA/pi/index"
		if (m_primaryIndexTable->insert(primarySeqNumber, pathController.getYearday().c_str(), fileInfo.getName().c_str(), fileInfo.getSize(),
			fileInfo.getCrc(), fileInfo.getMd5().c_str(),
			fileInfo.getUuid().c_str(), versionInt, addDate, derivativeSeqNumber) == false) {
			return false;
		}

		m_imageIndex->updatePath(versionControl.getCRC(), std::to_string(versionInt).c_str(), versionInt);
		return true;
	}

	bool RepositoryObject::validate(const char *dbImage, unsigned int size, unsigned int crc, const char * md5)
	{
		CLogger &logger = CLogger::getLogger();

		RepositoryPath repositoryPath = getRepositoryPath();
		std::string path = repositoryPath.getRepositoryPath();
		PathController pathController(path.c_str());
		pathController.splitPathAndFile(dbImage);

		if (pathController.makeRelativePath(dbImage) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid path: \"%s\"?", dbImage);
			ErrorCode::setErrorCode(IMGA_ERROR::IMAGE_NOT_FOUND);
			return false;
		}

		std::string from = path.c_str();
		from += pathController.getRelativePath();
		from += "/images/";
		from += pathController.getImageName();
		pathController.setFullPath(from);

		if (pathController.isValid() == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid image path: \"%s\"?", from.c_str());
			return false;
		}
		if (SAUtils::FileExists(from.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid image path: \"%s\"?", from.c_str());
			return false;
		}

		FileInfo fileInfo(from);

		if (fileInfo.getSize() != size) {
			return false;
		}
		if (fileInfo.getCrc() != crc) {
			return false;
		}
		std::string thisMD5 = fileInfo.getMd5();
		if (thisMD5.compare(md5) != 0) {
			return false;
		}
		return true;
	}
	
	void RepositoryObject::init(RepositoryPath &repositoryPath) {
		*m_repositoryPath = repositoryPath;
	}

	bool MasterRepositoryObject::init(RepositoryPath &repositoryPath) {
		
		RepositoryObject::init(repositoryPath);
		CLogger &logger = CLogger::getLogger();

		m_masterIndexTable->setPath(repositoryPath.getCSVDatabasePath().c_str());

		if (ArchivePath::isMasterBackup1Enabled() == true) {
			logger.log(LOG_OK, CLogger::Level::SUMMARY, "Backup 1 enabled, using folder: \"%s\"", ArchivePath::getMasterBackup1Path().c_str());
			if (SAUtils::DirExists(ArchivePath::getMasterBackup1Path().c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Backup 1 folder: \"%s\" not accessable?", ArchivePath::getMasterBackup1Path().c_str());
				return false;
			}
			RepositoryPath& backupPath1 = ArchivePath::getMasterBackup1();
			if (backupPath1.settup() == false) {
				return false;
			}
			m_backup[0].init(backupPath1);
		}
		else {
			logger.log(LOG_OK, CLogger::Level::INFO, "Backup 1 not enabled");
		}
		if (ArchivePath::isMasterBackup2Enabled() == true) {
			logger.log(LOG_OK, CLogger::Level::SUMMARY, "Backup 2 enabled, using folder: \"%s\"", ArchivePath::getMasterBackup2Path().c_str());
			if (SAUtils::DirExists(ArchivePath::getMasterBackup2Path().c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Backup 2 folder: \"%s\" not accessable?", ArchivePath::getMasterBackup2Path().c_str());
				return false;
			}
			RepositoryPath& backupPath2 = ArchivePath::getMasterBackup2();
			if (backupPath2.settup() == false) {
				return false;
			}
			m_backup[1].init(backupPath2);
		}
		else {
			logger.log(LOG_OK, CLogger::Level::INFO, "Backup 2 not enabled");
		}
		IntegrityManager& integrityManager = IntegrityManager::get();
		integrityManager.setMasterBackupPaths(ArchivePath::getMasterBackup1Path().c_str(), ArchivePath::getMasterBackup2Path().c_str(),
									ArchivePath::isMasterBackup1Enabled(), ArchivePath::isMasterBackup2Enabled());
		return true;
	}

	bool MasterRepositoryObject::settupRelative(std::string &yyyymmddStr) {
		RepositoryObject::settupRelative(yyyymmddStr);
		
		if (ArchivePath::isMasterBackup1Enabled() == true) {
			if (m_backup[0].settupRelative(yyyymmddStr) == false) {
				return false;
			}
		}
		if (ArchivePath::isMasterBackup2Enabled() == true) {
			if (m_backup[1].settupRelative(yyyymmddStr) == false) {
				return false;
			}
		}
		return true;
	}

	bool MasterRepositoryObject::copyFile(const std::string &pathToSourceRoot, const std::string  &yyyymmddStr, const std::string &fileName, const std::string &sequenceName) {
		RepositoryObject::copyFile(pathToSourceRoot, fileName, sequenceName);
		
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

	bool MasterRepositoryObject::validate(const char * dbImage, const char *sourceImage)
	{
		CLogger &logger = CLogger::getLogger();
		
		if (sourceImage == nullptr) {
			// test between backups and master.
			RepositoryPath repositoryPath = getRepositoryPath();
			std::string path = repositoryPath.getRepositoryPath();
			PathController pathController(path.c_str());
			pathController.splitPathAndFile(dbImage);

			if (pathController.makeRelativePath(dbImage) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid path: \"%s\"?", dbImage);
				ErrorCode::setErrorCode(IMGA_ERROR::IMAGE_NOT_FOUND);
				return false;
			}
			std::string from = path.c_str();
			from += pathController.getRelativePath();
			from += "/images/";
			from += pathController.getImageName();
			pathController.setFullPath(from);
			if (pathController.isValid() == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid image path: \"%s\"?", from.c_str());
				return false;
			}
			if (SAUtils::FileExists(from.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid image path: \"%s\"?", from.c_str());
				return false;
			}
			FileInfo targetFileInfo(from);
			if (isBackup1Enabled() == true) {
				if (m_backup[0].validate(from.c_str(), targetFileInfo.getSize(), targetFileInfo.getCrc(), targetFileInfo.getMd5().c_str()) == false) {
					return false;
				}
			}
			if (isBackup2Enabled() == true) {
				if (m_backup[1].validate(from.c_str(), targetFileInfo.getSize(), targetFileInfo.getCrc(), targetFileInfo.getMd5().c_str()) == false) {
					return false;
				}
			}
		}
		else {
			std::string source = sourceImage;
			FileInfo targetFileInfo(source);
			if (RepositoryObject::validate(dbImage, targetFileInfo.getSize(), targetFileInfo.getCrc(), targetFileInfo.getMd5().c_str()) == false) {
				return false;
			}
			if (isBackup1Enabled() == true) {
				if (m_backup[0].validate(dbImage, targetFileInfo.getSize(), targetFileInfo.getCrc(), targetFileInfo.getMd5().c_str()) == false) {
					return false;
				}
			}
			if (isBackup2Enabled() == true) {
				if (m_backup[1].validate(dbImage, targetFileInfo.getSize(), targetFileInfo.getCrc(), targetFileInfo.getMd5().c_str()) == false) {
					return false;
				}
			}
		}
		
		return true;
	}

	bool RepositoryObject::writeMetadata(ImagePath &imagePath, MetadataObject &metadataObject) {
		std::string toxml = getRepositoryPath().getMetadataPath() + '/' + imagePath.getImageName() + ".xml";
		XMLWriter xmlWriter;
		if (xmlWriter.writeImage(metadataObject, toxml.c_str()) == false) {
			return false;
		}
		std::string tojson = getRepositoryPath().getMetadataPath() + '/' + imagePath.getImageName() + ".json";
		JSONWriter jsonWriter;
		if (jsonWriter.writeImage(metadataObject, tojson.c_str()) == false) {
			return false;
		}
		std::string tohtml = getRepositoryPath().getMetadataPath() + '/' + imagePath.getImageName() + ".html";
		HTMLWriter htmlWriter;
		if (htmlWriter.writeImage(metadataObject, tohtml.c_str()) == false) {
			return false;
		}
		return true;
	}

	bool RepositoryObject::settupRelative(std::string &yyyymmddStr) {
		return getRepositoryPath().settupRelative(yyyymmddStr);
	}

	bool RepositoryObject::copyFile(const std::string &pathToSourceRoot, const std::string &fileName, const std::string &sequenceName) {
		RepositoryPath &RepositoryPath = getRepositoryPath();
		IntegrityManager &integrityManager = IntegrityManager::get();
		std::string from = pathToSourceRoot + "/" + fileName;
		std::string to = RepositoryPath.getDataPath() + '/' + sequenceName;

		if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
			return false;
		}

		// this can only be done after the file copy
		integrityManager.addDayFolder(RepositoryPath.getRepositoryPath().c_str(), RepositoryPath.getYYYYMMDD().c_str());
		integrityManager.addFile(RepositoryPath.getRepositoryPath().c_str(), RepositoryPath.getYYYYMMDD().c_str(), sequenceName.c_str());
		
		return true;
	}

	

	void DerivativesObject::init(RepositoryPath &repositoryPath, const char *workspacePath) {
		*m_repositoryPath = repositoryPath;
		m_derivativeIndexTable->setPath(repositoryPath.getCSVDatabasePath().c_str());
		m_workspacePath = workspacePath;

		CSVDerivativeDatabase &derivativeRepository = CSVDerivativeDatabase::get();
		derivativeRepository.setDBPath(m_repositoryPath->getRepositoryPath().c_str());

		//derivativeRepository.setPathToActiveRoot(workspacePath);
		//if (!m_imageIndex->init(repositoryPath.getImageIndexPath().c_str())) {
		//	return false;
		//}
		IntegrityManager& integrityManager = IntegrityManager::get();
		//integrityManager.setMasterBackupPaths(ArchivePath::getMasterBackup1Path().c_str(), ArchivePath::getMasterBackup2Path().c_str(),
		//	ArchivePath::isMasterBackup1Enabled(), ArchivePath::isMasterBackup2Enabled());
	}

	bool DerivativesObject::checkin(FileInfo &fileInfo, const char *comment) {
		return true;
	}
	bool DerivativesObject::isBackup1Enabled() {
		return m_backup[0].getRepositoryPath().isEnabled();
	}
	bool DerivativesObject::isBackup2Enabled() {
		return m_backup[1].getRepositoryPath().isEnabled();
	}
	

	RepositoryObject & DerivativesObject::getBackup1Object()
	{
		return m_backup[0];
	}

	RepositoryObject & DerivativesObject::getBackup2Object()
	{
		return m_backup[1];
	}

	bool DerivativesObject::copyFile(const std::string &pathToSourceRoot, const std::string  &yyyymmddStr, const std::string &fileName, const std::string &sequenceName) {
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

	bool DerivativesObject::validate(const char * dbImage, const char * sourceImage)
	{
		return false;
	}

	bool DerivativesObject::addimage(const char *sourceRelPath, FileInfo& fileInfo, const char *comment, int primarySeqNumber, int derivativeSeqNumber, int version, MetadataObject& metadataObject) {

		CSVDerivativeDatabase &derivativeRepository = CSVDerivativeDatabase::get();
		m_derivativeMetadata = std::make_shared<DerivativeMetadata>();
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
		Version &versionObj = (Version &)versionControl.getCurrentVersion();
		std::string shortRelPath = versionObj.getShortRelPath();
		shortRelPath += '/';
		shortRelPath += versionObj.makeEncodedVersion();
		m_derivativeMetadata->columnAt(DB_FILEPATH) = shortRelPath.c_str();

		m_derivativeMetadata->columnAt(DB_CRC) = fileInfo.getCrc();
		m_derivativeMetadata->columnAt(DB_MD5) = fileInfo.getMd5().c_str();
		m_derivativeMetadata->columnAt(DB_UUID) = fileInfo.getUuid().c_str();
		m_derivativeMetadata->columnAt(DB_FILESIZE) = fileInfo.getSize();
		m_derivativeMetadata->columnAt(DB_DATEMODIFIED) = fileInfo.getModTime();
		m_derivativeMetadata->columnAt(DB_DATECREATE) = fileInfo.getCreateTime();
		ExifDateTime addTime;
		ExifDate addDate;
		m_derivativeMetadata->columnAt(DB_DATEADDED) = addTime;
		m_derivativeMetadata->columnAt(DB_VERSIONPATH) = addDate.toShortRelativePath().c_str();

		CLogger &logger = CLogger::getLogger();
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

		IntegrityManager &integrityManager = IntegrityManager::get();
		integrityManager.addDerivativeDayFolder(m_repositoryPath->getYYYYMMDD().c_str());
		integrityManager.addDerivativeFile(m_repositoryPath->getYYYYMMDD().c_str(), versionObj.makeEncodedVersion());
		return true;
	}


	bool getimage(const std::string &pathToTargetRoot, const char *filepath, const char *comment, int version) {

		CSVDerivativeDatabase &derivativeRepository = CSVDerivativeDatabase::get();
		return true;
	}

	bool MasterRepositoryObject::isBackup1Enabled() {
		return m_backup[0].getRepositoryPath().isEnabled();
	}
	bool MasterRepositoryObject::isBackup2Enabled() {
		return m_backup[1].getRepositoryPath().isEnabled();
	}

	RepositoryObject& MasterRepositoryObject::getBackup1Object() {
		return m_backup[0];
	}

	bool DerivativesObject::settupRelative(std::string &yyyymmddStr) {
		return getRepositoryPath().settupRelative(yyyymmddStr);
	}

	/*
	bool DerivativesObject::copy2Repos(const std::string &pathToSourceRoot, const std::string &file) {
		RepositoryPath &RepositoryPath = getRepositoryPath();
		//IntegrityManager &integrityManager = IntegrityManager::get();
		std::string from = pathToSourceRoot + "/" + file;
		std::string to = RepositoryPath.getDataPath() + '/' + file;

		if (SAUtils::FileExists(from.c_str()) == false) {
			return false;
		}

		if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
			return false;
		}
		// this can only be done after the file copy
		//integrityManager.addFile(m_yyyymmddStr.c_str(), file.c_str());
		return true;
	}
	*/
	//DerivativeTable &DerivativesObject::getDerivativeTable() {
	//	return *m_derivativeTable;
	//}

	ArchiveObject::ArchiveObject() noexcept :
		m_masterView(std::make_unique<MasterCatalogue>()),
		m_PrimaryIndexObject(std::make_unique<PrimaryIndexObject>())
	{}




	ArchiveObject::~ArchiveObject() {}

	

	bool ArchiveObject::isMasterEnabled() {
		return m_master.getRepositoryPath().isEnabled();
	}

	bool ArchiveObject::isWorkspaceEnabled() {
		return (m_workspacePath.empty() == false);
	}

	MasterCatalogue& ArchiveObject::getMasterCatalogue() {
		return *m_masterView;
	}

	DerivativesObject& ArchiveObject::getDerivativesObject() {
		return m_derivatives;
	}

	

	RepositoryObject& MasterRepositoryObject::getBackup2Object() {
		return m_backup[1];
	}

	MasterRepositoryObject& ArchiveObject::getMasterObject() {
		return m_master;
	}

	PrimaryIndexObject& ArchiveObject::getPrimaryIndexObject() {
		if (m_PrimaryIndexObject == nullptr) {
			throw std::exception();
		}
		return *m_PrimaryIndexObject;
	}


	//ArchiveObject& ArchiveObject::getInstance()
	//{
	//	if (m_this == nullptr) {
	//		m_this = std::make_unique<ArchiveObject>();
	//	}
	//	return *m_this;
	//}

	std::unique_ptr<ArchiveObject> ArchiveObject::m_this = nullptr;

	//bool ArchiveObject::Initalise(const char *pathToWorkspace, const char *pathToMaster, const char *pathToHome, const char *pathToHistory) {
	bool ArchiveObject::Initalise() {

		AppConfig &config = AppConfig::get();

		CLogger &logger = CLogger::getLogger();
		m_workspacePath = ArchivePath::getPathToWorkspace();

		//if (ArchivePath::isMasterEnabled() == true) {

		RepositoryPath& master = ArchivePath::getMaster();
		if (master.settup() == false) {
			return false;
		}
		if (SAUtils::DirExists(master.getRepositoryPath().c_str()) == false) {
			return false;
		}
		m_master.init(master);

		// Settup the file renaming manager with the master path 
		SequenceFileManager::setMasterPath(master.getRepositoryPath().c_str());


		RepositoryPath& derivative = ArchivePath::getDerivative();

		if (derivative.settup() == false) {
			return false;
		}
		if (SAUtils::DirExists(derivative.getRepositoryPath().c_str()) == false) {
			return false;
		}

		if (ViewManager::initalise(ArchivePath::getMasterPath().c_str(), config.getConfigPath()) == false) {
			return false;
		}

		SQLiteDB::enableSQLiteDB(config.isSQL());

		if (SQLiteDB::init(config.getDatabasePath(), true) == false) {
			return false;
		}

		std::string csvdbPath = ArchivePath::getDerivativePath();
		csvdbPath += CSVDB_PATH;
		CSVDerivativeDatabase::setDBPath(csvdbPath.c_str());


		/*
		if (master.isBackup1Enabled() == true) {
			logger.log(LOG_OK, CLogger::Level::SUMMARY, "Backup 1 enabled, using folder: \"%s\"", ArchivePath::getBackup1Path().c_str());
			if (SAUtils::DirExists(ArchivePath::getBackup1Path().c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Backup 1 folder: \"%s\" not accessable?", ArchivePath::getBackup1Path().c_str());
				return false;
			}
			RepositoryPath& backupPath1 = ArchivePath::getBackup1();
			if (backupPath1.settup() == false) {
				return false;
			}
			m_backup[0].init(backupPath1);
		}
		else {
			logger.log(LOG_OK, CLogger::Level::INFO, "Backup 1 not enabled");
		}
		if (master::isBackup2Enabled() == true) {
			logger.log(LOG_OK, CLogger::Level::INFO, "Backup 2 enabled, using folder: \"%s\"", ArchivePath::getBackup2Path().c_str());
			if (SAUtils::DirExists(ArchivePath::getBackup2Path().c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Backup 2 folder: \"%s\" not accessable?", ArchivePath::getBackup2Path().c_str());
				return false;
			}
			RepositoryPath& backupPath2 = ArchivePath::getBackup2();
			if (backupPath2.settup() == false) {
				return false;
			}
			m_backup[1].init(backupPath2);
		}
		else {
			logger.log(LOG_OK, CLogger::Level::INFO, "Backup 2 not enabled");
		}
		*/


		PrimaryIndexPath& primaryIndexPath = ArchivePath::getPrimaryIndex();
		std::string path = ArchivePath::getPathToHome() + PRIMARY_INDEX_PATH;
		primaryIndexPath.setRepositoryPath(path);
		primaryIndexPath.settup();
		try {
			if (!getPrimaryIndexObject().init(primaryIndexPath)) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to initalise the primary index", ArchivePath::getMasterBackup2Path().c_str());
				return false;
			}
			if (SAUtils::DirExists(ArchivePath::getPathToWorkspace().c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Workspace folder: \"%s\" not accessable?", ArchivePath::getPathToWorkspace().c_str());
				return false;
			}

			if (ImagePath::settupMainArchiveFolders(ArchivePath::getPathToWorkspace().c_str(), ArchivePath::getMasterPath().c_str(), ArchivePath::getDerivativePath().c_str(), ArchivePath::getPathToHome().c_str()) == false) {

				return false;
			}
		}
		catch (std::exception /*e */) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Initalisation Failed creating Primary Index");
			return false;
		}

		std::string csvpdbPath = primaryIndexPath.getCSVDatabasePath();
		CSVDatabase::setDBPath(csvpdbPath.c_str());
		History::setPaths(ArchivePath::getIndexHistory().c_str(), ArchivePath::getPathToWorkspace().c_str(), ArchivePath::getMainHistory().c_str());

		History& history = History::getHistory();
		history.init();

		IndexVisitor::Init(ArchivePath::getMasterPath().c_str(), ArchivePath::getPathToWorkspace().c_str(), primaryIndexPath.getCheckoutStatusPath().c_str());
		CheckoutStatus::Init(ArchivePath::getMasterPath().c_str(), ArchivePath::getPathToWorkspace().c_str(), primaryIndexPath.getCheckoutStatusPath().c_str());
		VersionControl::setPaths(primaryIndexPath.getPathToRepository().c_str(), ArchivePath::getMasterPath().c_str(), ArchivePath::getDerivativePath().c_str(), ArchivePath::getPathToWorkspace().c_str());
		MasterCatalogue& masterView = getMasterCatalogue();

		/* Testing
		masterView.setFileEnabled(true);
		masterView.setWWWEnabled(true);
		*/

		if (masterView.isWWWEnabled()) {
			if (masterView.settupWWW(config.getTempPath(), config.getTemplatePath(), config.getMasterWWWCataloguePath()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Initalisation Failed creating master WWW view");
				return false;
			}


			if (masterView.settupSystemWWW(primaryIndexPath.getPathToRepository().c_str(), config.getTemplatePath(), config.getMasterWWWCataloguePath(), ArchivePath::getMainHistory().c_str(),
																		master.getJournalPath().c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Initalisation Failed creating master WWW view system");
				return false;
			}
		}
		if (masterView.isFileEnabled()) {
			
			if (SAUtils::DirExists(config.getMasterCataloguePath()) == false) {
				if (SAUtils::mkDir(config.getMasterCataloguePath()) == false) {
					logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid master catalogue path");
					return false;
				}
			}
			if (masterView.settupFile(config.getTempPath(), config.getMasterCataloguePath()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Initalisation Failed creating master view");
				return false;
			}
		}
		m_derivatives.init(derivative, getWorkspacePath().c_str());
		return true;
	}

	bool ArchiveObject::OnCompletion() {
		MasterCatalogue& masterView = getMasterCatalogue();

		if (masterView.processWWWPages() == false) {
			return false;
		}

		return true;
	}

	bool ArchiveObject::writeDerivativeMetadata(std::string& rootPath, std::string &versionName, DerivativeMetadata &metadataObject, std::string &imageName) {

		CLogger &logger = CLogger::getLogger();

		std::string toxml = rootPath + '/' + imageName;
		if (SAUtils::DirExists(toxml.c_str()) == false) {
			if (SAUtils::mkDir(toxml.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to create version folder: \"%s\"?", toxml.c_str());
				return false;
			}

		}
		return writeDerivativeMetadata(toxml, versionName, metadataObject);
	}

	bool ArchiveObject::writeDerivativeMetadata(std::string& rootPath, std::string &versionName, DerivativeMetadata &metadataObject) {
		std::string toxml = rootPath;
		toxml += '/';
		toxml += versionName;
		toxml += ".xml";

		XMLWriter xmlWriter;
		if (xmlWriter.writeDerivativeMetadata(metadataObject, toxml.c_str()) == false) {
			return false;
		}
		/*
		std::string tojson = rootPath + '/' + imageName + ".json";
		JSONWriter jsonWriter;
		if (jsonWriter.writeDerivativeMetadata(metadataObject, tojson.c_str()) == false) {
			return false;
		}
		std::string tohtml = rootPath + '/' + imageName + ".html";
		HTMLWriter htmlWriter;
		if (htmlWriter.writeDerivativeMetadata(metadataObject, tohtml.c_str()) == false) {
			return false;
		}
		*/
		return true;
	}

	bool ArchiveObject::writeMetadata(std::string& rootPath, std::string &imageName, MetadataObject &metadataObject) {

		std::string toxml = rootPath + '/' + imageName + ".xml";
		XMLWriter xmlWriter;
		if (xmlWriter.writeImage(metadataObject, toxml.c_str()) == false) {
			return false;
		}
		std::string tojson = rootPath + '/' + imageName + ".json";
		JSONWriter jsonWriter;
		if (jsonWriter.writeImage(metadataObject, tojson.c_str()) == false) {
			return false;
		}
		std::string tohtml = rootPath + '/' + imageName + ".html";
		HTMLWriter htmlWriter;
		if (htmlWriter.writeImage(metadataObject, tohtml.c_str()) == false) {
			return false;
		}
		return true;
	}

	bool ArchiveObject::CreateImage(const BasicMetadata &BasicMetadata, ImagePath &imagePath, MetadataObject &metadataObject) {
		CLogger &logger = CLogger::getLogger();
		PrimaryIndexTable& primaryIndexTable = getPrimaryIndexObject().getPrimaryIndexTable();
		MasterRepositoryObject& masterObject = getMasterObject();
		MasterIndexTable& masterIndexTable = masterObject.getMasterIndexTable();


		// Note Both the Primary and the Master sequence numbers must be known before saving

		// Primary Index Object
		unsigned long primarySeqNumber = primaryIndexTable.getNextIndex();
		std::string seqNumberStr = std::to_string(primarySeqNumber);


		// Master Database Object
		unsigned long masterSeqNumber = masterIndexTable.getNextIndex();
		metadataObject.setSequenceId(primarySeqNumber);

		BasicMetadata.columnAt(DB_FILEPATH) = imagePath.getYyyymmddStr().c_str();
		metadataObject.columnAt(DB_FILEPATH) = imagePath.getYyyymmddStr().c_str();

		
		// This is adding the sequence file name
		SequenceFileManager& sequenceFileManager = SequenceFileManager::get();
		std::string sequenceName = sequenceFileManager.sequenceFile(imagePath.getYyyymmddStr().c_str(), primarySeqNumber, imagePath.getImageName().c_str());
		BasicMetadata.columnAt(DB_FILENAME) = sequenceName.c_str();
		BasicMetadata.columnAt(DB_ORGINALNAME) = imagePath.getImageName().c_str();
		imagePath.switchOrginalName(sequenceName.c_str());
		metadataObject.columnAt(DB_FILENAME) = sequenceName.c_str();
		

		unsigned long n = BasicMetadata.getSize();
		unsigned long crc = BasicMetadata.getCrc();
		ExifDate date;
		date.now();

		std::string yyyymmddStr = imagePath.getYyyymmddStr();
		if (settupRelativeMaster(yyyymmddStr) == false) {
			return false;
		}
		// Write the metadata to the workspace
		std::string name = imagePath.getImageName();
		if (this->writeMetadata2Workspace(imagePath, name, metadataObject) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" workspace", imagePath.getImageName().c_str());
			return false;
		}

		// Write the metadata to the primary index
		if (this->writeMetadata2PrimaryIndex(imagePath, name, metadataObject) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" primary index", imagePath.getImageName().c_str());
			return false;
		}

		// Write the metadata to the Master archive
		if (this->writeMetadata2MasterDatabase(name, metadataObject) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" master database", imagePath.getImageName().c_str());
			return false;
		}

		if (SQLiteDB::add(metadataObject) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to insert \"%s\" into SQL Database", imagePath.getImageName().c_str());
			return false;
		}

		/*
		std::string to = imagePath.getWorkspaceMetadataPath() + '/' + imagePath.getImageName() + ".xml";
		XMLWriter xmlWriter;
		if (xmlWriter.writeImage(metadataObject, to.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" metadata in XML format", imagePath.getImageName().c_str());
			return false;
		}
		*/


		if (masterIndexTable.insert(masterSeqNumber, imagePath.getRelativePath().c_str(), imagePath.getImageName().c_str(), n, crc, BasicMetadata.getMd5().c_str(),
			BasicMetadata.getUuid().c_str(), 0, date) == false) {
			return false;
		}
		std::string filepath = imagePath.getRelativePath() + '/' + imagePath.getImageName();
		std::string shortFilePath = imagePath.getYyyymmddStr() + "/" + imagePath.getImageName();
		// main
		if (copyFile2Master(imagePath.getCurrentSourcePath(), imagePath.getYyyymmddStr(), imagePath.getOrginalName(), imagePath.getImageName()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to copy image \"%s\" to Master Archive", imagePath.getImageName().c_str());
			return false;
		}
		if (imagePath.copyFile2Workspace(imagePath.getCurrentSourcePath(), imagePath.getOrginalName(), imagePath.getImageName()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to copy image \"%s\" to current Workspace", imagePath.getImageName().c_str());
			return false;
		}

		CSVDatabase &csvDatabase = CSVDatabase::get();
		try {
			csvDatabase.add(metadataObject, shortFilePath.c_str());
		}
		catch (std::exception /*&e*/) {

			logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to read database: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}

		PrimaryIndexObject& primaryIndexObject = getPrimaryIndexObject();
		if (primaryIndexObject.addMasterImage(BasicMetadata, imagePath, masterSeqNumber, primarySeqNumber) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to add master image: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}

		CheckoutStatus checkoutStatus;
		const char *comment = "";
		if (checkoutStatus.newImage(shortFilePath.c_str(), comment) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to add to checkout file: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}

		if (m_masterView->addImage(imagePath.getCurrentSourcePath().c_str(), metadataObject, filepath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::ERR, "Unable to preview file: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
		}



		return true;
	}

	bool ArchiveObject::settupRelativeDerivative(std::string &yyyymmddStr) {
		if (ArchivePath::isDerivativeEnabled() == true) {
			if (m_derivatives.settupRelative(yyyymmddStr) == false) {
				return false;
			}
		}
		return true;
	}

	bool ArchiveObject::settupRelativeMaster(std::string &yyyymmddStr) {
		if (ArchivePath::isMasterEnabled() == true) {
			if (m_master.settupRelative(yyyymmddStr) == false) {
				return false;
			}
		}
		/*
		if (ArchivePath::isBackup1Enabled() == true) {
			if (m_backup[0].settupRelative(yyyymmddStr) == false) {
				return false;
			}
		}
		if (ArchivePath::isBackup2Enabled() == true) {
			if (m_backup[1].settupRelative(yyyymmddStr) == false) {
				return false;
			}
		}
		*/
		return true;
	}
	bool ArchiveObject::writeMetadata2Workspace(ImagePath &imagePath, std::string &imageName, MetadataObject &metadataObject) {

		if (ArchiveObject::isWorkspaceEnabled() == true) {
			std::string toxml = m_workspacePath + '/';
			std::string relpath = imagePath.getRelativePath();
			toxml += imagePath.getYear(relpath);
			if (SAUtils::DirExists(toxml.c_str()) == false) {
				if (SAUtils::mkDir(toxml.c_str()) == false) {
					return false;
				}
			}
			toxml = m_workspacePath + '/';
			toxml += imagePath.getRelativePath();
			if (SAUtils::DirExists(toxml.c_str()) == false) {
				if (SAUtils::mkDir(toxml.c_str()) == false) {
					return false;
				}
			}
			toxml += "/.sia";
			if (SAUtils::DirExists(toxml.c_str()) == false) {
				if (SAUtils::mkDir(toxml.c_str()) == false) {
					return false;
				}
			}
			toxml += "/metadata";
			if (SAUtils::DirExists(toxml.c_str()) == false) {
				if (SAUtils::mkDir(toxml.c_str()) == false) {
					return false;
				}
			}
			if (writeMetadata(toxml, imageName, metadataObject) == false) {
				return false;
			}
		}


		return true;
	}

	bool ArchiveObject::writeDerivativeMetadat2Workspace(ImagePath &imagePath, std::string &versionName, DerivativeMetadata &derivativeMetadata, std::string &imageName) {
		if (ArchiveObject::isWorkspaceEnabled() == true) {
			std::string toxml = m_workspacePath + '/';
			std::string relpath = imagePath.getRelativePath();
			toxml += imagePath.getYear(relpath);
			if (SAUtils::DirExists(toxml.c_str()) == false) {
				if (SAUtils::mkDir(toxml.c_str()) == false) {
					return false;
				}
			}
			toxml = m_workspacePath + '/';
			toxml += imagePath.getRelativePath();
			if (SAUtils::DirExists(toxml.c_str()) == false) {
				if (SAUtils::mkDir(toxml.c_str()) == false) {
					return false;
				}
			}
			toxml += "/.sia";
			if (SAUtils::DirExists(toxml.c_str()) == false) {
				if (SAUtils::mkDir(toxml.c_str()) == false) {
					return false;
				}
			}
			toxml += "/metadata";
			if (SAUtils::DirExists(toxml.c_str()) == false) {
				if (SAUtils::mkDir(toxml.c_str()) == false) {
					return false;
				}
			}
			if (writeDerivativeMetadata(toxml, versionName, derivativeMetadata, imageName) == false) {
				return false;
			}
		}
		return true;
	}

	bool ArchiveObject::writeMetadata2PrimaryIndex(ImagePath &imagePath, std::string &imageName,  MetadataObject &metadataObject) {
		PrimaryIndexPath primaryIndexPath = getPrimaryIndexObject().getPrimaryIndexPath();
		std::string primaryIndexObjectMetadataPath = primaryIndexPath.getMetadataPath();

		if (SAUtils::DirExists(primaryIndexObjectMetadataPath.c_str()) == false) {
			return false;
		}
		std::string toxml = primaryIndexObjectMetadataPath + '/';
		std::string relpath = imagePath.getRelativePath();
		toxml += imagePath.getYear(relpath);
		if (SAUtils::DirExists(toxml.c_str()) == false) {
			if (SAUtils::mkDir(toxml.c_str()) == false) {
				return false;
			}
		}
		toxml = primaryIndexObjectMetadataPath + '/';
		toxml += imagePath.getRelativePath();
		if (SAUtils::DirExists(toxml.c_str()) == false) {
			if (SAUtils::mkDir(toxml.c_str()) == false) {
				return false;
			}
		}

		
		if (writeMetadata(toxml, imageName, metadataObject) == false) {
			return false;
		}
		return true;
	}
	bool ArchiveObject::writeMetadata2MasterDatabase(std::string &imageName, MetadataObject &metadataObject) {
		if (ArchivePath::isMasterEnabled() == true) {
			std::string path = m_master.getRepositoryPath().getMetadataPath();
			if (writeMetadata(path, imageName, metadataObject) == false) {
				return false;
			}
		}
		if (ArchivePath::isMasterBackup1Enabled() == true) {
			std::string path = m_master.getBackup1Object().getRepositoryPath().getMetadataPath();
			if (writeMetadata(path, imageName, metadataObject) == false) {
				return false;
			}
		}
		if (ArchivePath::isMasterBackup2Enabled() == true) {
			std::string path = m_master.getBackup2Object().getRepositoryPath().getMetadataPath();
			if (writeMetadata(path, imageName, metadataObject) == false) {
				return false;
			}
		}
		return true;
	}
	bool ArchiveObject::writeMetadata2DerivativesDatabase(std::string &versionName, DerivativeMetadata &metadataObject, std::string &imageName) {
		if (ArchivePath::isDerivativeEnabled() == true) {
			std::string path = m_derivatives.getRepositoryPath().getMetadataPath();
			if (writeDerivativeMetadata(path, versionName, metadataObject, imageName) == false) {
				return false;
			}
		}
		return true;
	}


	bool ArchiveObject::copyFile2Master(const std::string  &pathToSourceRoot, const std::string  &yyyymmddStr, const std::string &fileName, const std::string &sequenceName) {
		if (ArchivePath::isMasterEnabled() == true) {
			if (m_master.copyFile(pathToSourceRoot, yyyymmddStr, fileName, sequenceName) == false) {
				return false;
			}
		}
		
		return true;
	}
	

	bool ArchiveObject::processHistory(ImagePath &imagePath, const char *comment) {
		History& history = History::getHistory();
		if (history.newImage(imagePath.getImagePath().c_str(), comment) == false) {
			return false;
		}

		if (SQLiteDB::newImage2History(imagePath.getImagePath().c_str(), comment) == false) {
			return false;
		}
		return true;
	}
	
	

	bool ArchiveObject::showCheckedOut(const char *filepath) {
		PrimaryIndexObject& primaryIndexObject = getPrimaryIndexObject();
		
		VersionControl::getInstance().setCurrentVersion(filepath);
		std::string versionPath = VersionControl::getInstance().getCurrentVersionPath();
		CheckoutStatus checkoutStatus;
		if (checkoutStatus.showCheckedOut(filepath) == false) {
			return false;
		}
		return true;
	}

	bool ArchiveObject::showUncheckedOutChanges(const char *filepath) {
		CheckoutStatus checkoutStatus;
		if (checkoutStatus.showUncheckedOutChanges(filepath) == false) {
			return false;
		}
		return true;
	}

	/*
	class GetManager {
	public:
		bool makeList();
	};

	bool GetManager::makeList() {
		ArchiveVisitor archiveVisitor(new CatalogAction(m_catalogPath.c_str()));
		archiveVisitor.process(m_MasterPath.c_str());
		return true;
	}
	*/

	/*
	bool ArchiveObject::get(const char *filepath, const char *comment, bool force) {
		CLogger &logger = CLogger::getLogger();
		AppConfig &config = AppConfig::get();
		RepositoryObject& repositoryObject = getMasterObject();
		RepositoryPath& rp = repositoryObject.getRepositoryPath();
		GetImagesManager::setPaths("C:/temp/cat", rp.getRepositoryPath().c_str());
		GetImagesManager getImagesManager = GetImagesManager::get();
		getImagesManager.makeList();
		return true;
	}
	*/
	class GetAction : public IndexAction {
		/// On finding a directory, this function is run.
		virtual bool onImage();
		ArchiveObject *m_ArchiveObject;
		std::string m_versions;
	public:
		/// Constructor
		GetAction(ArchiveObject *archiveObject, const char *versions) {
			m_versions = versions;
			m_ArchiveObject = archiveObject;
		};
		/// Distructor
		virtual ~GetAction() {};

	};

	class CheckoutAction : public IndexAction {
		/// On finding a directory, this function is run.
		virtual bool onImage();
		ArchiveObject *m_ArchiveObject;
	public:
		/// Constructor
		CheckoutAction(ArchiveObject *archiveObject) {		
			m_ArchiveObject = archiveObject;
		};
		/// Distructor
		virtual ~CheckoutAction() {};

	};

	class CheckinAction : public IndexAction {
		virtual bool onImage();
		ArchiveObject *m_ArchiveObject;
	public:
		/// Constructor
		CheckinAction(ArchiveObject *archiveObject) {
			m_ArchiveObject = archiveObject;
		};
		/// Distructor
		virtual ~CheckinAction() {};

	};

	class UnCheckoutAction : public IndexAction {
		virtual bool onImage();
		ArchiveObject *m_ArchiveObject;
	public:
		/// Constructor
		UnCheckoutAction(ArchiveObject *archiveObject) {
			m_ArchiveObject = archiveObject;
		};
		/// Distructor
		virtual ~UnCheckoutAction() {};

	};

	bool ArchiveObject::get(const char *scope, const char *versions, const char *comment, bool force) {
		IndexVisitor IndexVisitor(std::make_shared<GetAction>(this, versions));
		if (!IndexVisitor.setScope(scope)) {
			return false;
		}
		IndexVisitor.process();

		return true;
	}

	bool ArchiveObject::checkout(const char *scope, const char *comment, bool force) {
		IndexVisitor IndexVisitor(std::make_shared<CheckoutAction>(this));
		if (!IndexVisitor.setScope(scope)) {
			return false;
		}
		IndexVisitor.process();
		
		return true;
	}

	bool ArchiveObject::checkin(const char *scope, const char *comment, bool force) {
		IndexVisitor IndexVisitor(std::make_shared<CheckinAction>(this));
		if (!IndexVisitor.setScope(scope)) {
			return false;
		}
		IndexVisitor.process();
		return true;
	}

	bool ArchiveObject::uncheckout(const char *scope, const char *comment, bool force) {
		IndexVisitor IndexVisitor(std::make_shared<UnCheckoutAction>(this));
		if (!IndexVisitor.setScope(scope)) {
			return false;
		}
		IndexVisitor.process();
		return true;
	}

	bool GetAction::onImage() {
		CLogger &logger = CLogger::getLogger();
		std::string path = m_currentRow->columnAt(DB_FILEPATH).getString();
		path += '/';
		path += m_currentRow->columnAt(DB_FILENAME).getString();

		VersionControl& versionControl = VersionControl::getInstance();

		if (versionControl.getVersions(path.c_str(), m_versions.c_str(), false) == false) {
			return false;
		}
		logger.status(LOG_COMPLETED_OK, ReporterEvent::Status::Completed, "Getting image was successfull \"%s\"", path.c_str());
		return true;
	}

	bool CheckoutAction::onImage() {
		CLogger &logger = CLogger::getLogger();
		std::string path = m_currentRow->columnAt(DB_FILEPATH).getString();
		path += '/';
		path += m_currentRow->columnAt(DB_FILENAME).getString();
			
		if (m_ArchiveObject->checkoutFile(path.c_str(), "", false) == false) {
			switch (ErrorCode::getErrorCode()) {
			case IMGA_ERROR::INVALID_PATH:
				//m_indexActionReporter->add(ReporterItem::Status::Error, "Failed to check out image \"%s\" %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				logger.status(LOG_UNABLE_TO_CHECKOUT_GENERAL, ReporterEvent::Status::Warning, "Failed to check out image \"%s\" Reason: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));

				break;
			case IMGA_ERROR::ALREADY_CHECKED_OUT:
				//m_indexActionReporter->add(ReporterItem::Status::Warning, "Failed to check out image \"%s\" %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				logger.status(LOG_UNABLE_TO_CHECKOUT_GENERAL, ReporterEvent::Status::Warning, "Failed to check out image \"%s\" Reason: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				return true;
			case IMGA_ERROR::WILL_OVERWRITE_CHANGES:
				logger.status(LOG_UNABLE_TO_CHECKOUT_GENERAL, ReporterEvent::Status::Warning, "Unable to checkout: \"%s\" Reason: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				return true;
			case IMGA_ERROR::CHANGE_MAY_BE_LOST:
				logger.status(LOG_UNABLE_TO_CHECKOUT_GENERAL, ReporterEvent::Status::Warning, "checked out but not copied\"%s\" Error: %s?", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				return true;
			}
			return false;
		}
		//m_indexActionReporter->add(ReporterItem::Status::Completed, "Image \"%s\"", path.c_str());
		logger.status(LOG_COMPLETED_OK, ReporterEvent::Status::Completed, "Checking out image was successfull \"%s\"", path.c_str());
		return true;
	}


	bool CheckinAction::onImage() {
		CLogger &logger = CLogger::getLogger();
		std::string path = m_currentRow->columnAt(DB_FILEPATH).getString();
		path += '/';
		path += m_currentRow->columnAt(DB_FILENAME).getString();
		
		if (m_ArchiveObject->checkinFile(path.c_str(), "", false) == false) {
			switch (ErrorCode::getErrorCode()) {
			case IMGA_ERROR::INVALID_PATH:
				//m_indexActionReporter->add(ReporterItem::Status::Error, "Failed to check out image \"%s\" %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				logger.status(LOG_UNABLE_TO_CHECKIN_GENERAL, ReporterEvent::Status::Warning, "Images \"%s\" not checked in? Reason: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));

				break;
			case IMGA_ERROR::ALREADY_CHECKED_IN:
				//m_indexActionReporter->add(ReporterItem::Status::Warning, "Failed to check out image \"%s\" %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				logger.status(LOG_UNABLE_TO_CHECKIN_GENERAL, ReporterEvent::Status::Warning, "Images \"%s\" not checked in? Reason: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				return true;
			case IMGA_ERROR::ALREADY_CHECKED_IN_NO_CHANGES:
				logger.status(LOG_UNABLE_TO_CHECKIN_GENERAL, ReporterEvent::Status::Warning, "Images \"%s\"? not checked in? Reason: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				return true;
			case IMGA_ERROR::ALREADY_CHECKED_IN_CHANGES:
				logger.status(LOG_UNABLE_TO_CHECKIN_GENERAL, ReporterEvent::Status::Warning, "Images \"%s\"? not checked in? Reason: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				return true;
			case IMGA_ERROR::NO_CHANGE_IN_IMAGE:
				logger.status(LOG_UNABLE_TO_CHECKIN_GENERAL, ReporterEvent::Status::Warning, "File not copied \"%s\"? Error: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				return true;
			case IMGA_ERROR::DUPLICATE_IMAGE:
				logger.status(LOG_UNABLE_TO_CHECKIN_GENERAL, ReporterEvent::Status::Warning, "Images \"%s\" not checked in? Reason: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				return true;	
			}
			logger.status(LOG_UNABLE_TO_CHECKOUT_GENERAL, ReporterEvent::Status::Warning, "Unable to checkout: \"%s\" Error: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}
		//m_indexActionReporter->add(ReporterItem::Status::Completed, "Image \"%s\"", path.c_str());
		logger.status(LOG_COMPLETED_OK, ReporterEvent::Status::Completed, "Checking in image was successfull \"%s\"", path.c_str());
		return true;
	}

	bool UnCheckoutAction::onImage() {
		CLogger &logger = CLogger::getLogger();
		std::string path = m_currentRow->columnAt(DB_FILEPATH).getString();
		path += '/';
		path += m_currentRow->columnAt(DB_FILENAME).getString();
		
		if (m_ArchiveObject->uncheckoutFile(path.c_str(), "", false) == false) {
			switch (ErrorCode::getErrorCode()) {
			case IMGA_ERROR::INVALID_PATH:
				//m_indexActionReporter->add(ReporterItem::Status::Error, "Failed to check out image \"%s\" %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				logger.status(LOG_UNABLE_TO_CHECKIN_GENERAL, ReporterEvent::Status::Warning, "Images \"%s\" not checked in? Reason: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));

				break;
			case IMGA_ERROR::ALREADY_CHECKED_IN:
				//m_indexActionReporter->add(ReporterItem::Status::Warning, "Failed to check out image \"%s\" %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				logger.status(LOG_UNABLE_TO_CHECKIN_GENERAL, ReporterEvent::Status::Warning, "Images \"%s\" not checked in? Reason: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
				return true;
			}
			logger.status(LOG_UNABLE_TO_CHECKOUT_GENERAL, ReporterEvent::Status::Warning, "Unable to uncheckout: \"%s\" Error: %s", path.c_str(), ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}
		logger.status(LOG_COMPLETED_OK, ReporterEvent::Status::Completed, "Un-checking out image was successfull \"%s\"", path.c_str());
		return true;
	}

	bool ArchiveObject::checkoutFile(const char *filepath, const char *comment, bool force) {
		CLogger &logger = CLogger::getLogger();

		VersionControl& versionControl = VersionControl::getInstance();

		if (versionControl.checkoutFile(filepath, comment, force) == false) {
			return false;
		}
		
		/* To be done
		// In the state that it can be checked out
		CDCheckInOutManager &cd = CDCheckInOutManager::get();
		try {

			if (cd.CheckOut(CDCheckInOutManager::DB::Workspace, filepath) == false) {
				logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				return false;
			}

		}
		catch (SIAAppException &e) {
			printf("Error: %s\n", e.what());
			if (checkoutStatus.checkinUpdate(filepath, comment) == false) {
				logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				return false;
			}
			return false;
		}
		*/
		History& history = History::getHistory();
		if (history.checkoutImage(filepath, versionControl.getVersion(), comment) == false) {
			logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to update checkout history for image: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}
		return true;
	}

	bool ArchiveObject::checkinFile(const char *filepath, const char *comment, bool force) {
		CLogger &logger = CLogger::getLogger();
		CDCheckInOutManager cd = CDCheckInOutManager::get();
		CheckoutStatus checkoutStatus;
		ImportJournal& importJournal = ImportJournalManager::GetJournal();
		// Note Both the Primary and the Master sequence numbers must be known before saving



		// Test the checkin status of the image
		bool alreadyCheckedIn = false;
		if (!force) {
			if (checkoutStatus.isCheckedOut(filepath) == false) {
				if (ErrorCode::getErrorCode() == IMGA_ERROR::ALREADY_CHECKED_IN || ErrorCode::getErrorCode() == IMGA_ERROR::NOT_BEEN_CHECKED_OUT) {
					// Needs chacking for changes beween the one in the workspace and the one checked in
					// even if is already chected in, the user needs to if this is the case.
					alreadyCheckedIn = true;

				}
				else {
					return false;
				}
			}
		}

		std::string workspacePath = getWorkspacePath();
		PathController pathController(workspacePath.c_str());
		pathController.splitShort(filepath);
		pathController.makeImagePath();
		ImagePath imagePath(filepath);
		PrimaryIndexObject& primaryIndexObject = getPrimaryIndexObject();

		
		VersionControl& versionControl = VersionControl::getInstance();
		versionControl.setCurrentVersion(filepath);
		int version = versionControl.getVersion();
		// Fill-in the details of the checkedin image
		std::string ddmmyy = pathController.getFullPath();
		FileInfo fileInfo(ddmmyy);
		if (!force) {
			if (checkoutStatus.isChanged(filepath, versionControl.getCRC(), versionControl.getMD5().c_str()) == false) {
				if (alreadyCheckedIn) {
					ErrorCode::setErrorCode(IMGA_ERROR::ALREADY_CHECKED_IN_NO_CHANGES);
					return false; // Just return now as not changes found.
				}
				// not checked in but no changes so just change state to checked-in
				logger.log(LOG_OK, CLogger::Level::INFO, "Already checked in and No changes have been made to image: \"%s\" Warning: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				if (checkoutStatus.checkinUpdate(filepath, comment) == false) {
					logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to checkin: \"%s\" Warning: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
					return false;
				}
				return false;
			}
			else {
				if (alreadyCheckedIn) {
					ErrorCode::setErrorCode(IMGA_ERROR::ALREADY_CHECKED_IN_CHANGES);
					logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::WARNING, "Already checked in: \"%s\" Reason: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
					return false; // changes found.
				}
			}

			if (TestForDuplicate(fileInfo, importJournal, false) == false) {
				if (ImportJournalManager::save() == false) {
					logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to save Journal File");
					return false;
				}
			}
		}
		// Note the new version will always go in the derivative database.
		DerivativesObject& derivativesObject = getDerivativesObject();
		DerivativeIndexTable& derivativeIndexTable = derivativesObject.getDerivativeIndexTable();

		// Primary Index Object
		PrimaryIndexTable& primaryIndexTable = primaryIndexObject.getPrimaryIndexTable();
		unsigned long primarySeqNumber = primaryIndexTable.getNextIndex();

		// Derivatives Object
		unsigned long derivativeSeqNumber = derivativeIndexTable.getNextIndex();

		// Get The old metadataObject
		CSVDatabase csvDatabase = CSVDatabase::get();
		SharedMTRow metadataRow = csvDatabase.get(pathController.getImageName().c_str(), pathController.getRelativePath().c_str());
		if (metadataRow == nullptr) {
			return false;
		}
 
		version++;
		

		MetadataObject metadataObject(*metadataRow);

		logger.log(LOG_OK, CLogger::Level::FINE, "Final metadata");
		MTTableSchema& mos = (MTTableSchema&)metadataObject.getSchema();
		for (std::vector<MTSchema>::iterator i = mos.begin(); i != mos.end(); i++) {
			MTSchema& columnInfo = *i;

			logger.log(LOG_OK, CLogger::Level::FINE, "%-20s %s\n", columnInfo.getName().c_str(), metadataObject.columnAt(columnInfo.getName().c_str()).toString().c_str());
		}

		
		//primaryIndexObject.

		derivativesObject.addimage(pathController.getYearday().c_str(), fileInfo, comment, primarySeqNumber, derivativeSeqNumber, version, metadataObject);
		if (ImportJournalManager::save() == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to save Journal File");
			return false;
		}
		//FileInfo fileInfo(pathController.getFullPath());
		metadataObject.update(fileInfo, derivativeSeqNumber, version);
		std::string yyyymmddStr = derivativesObject.getRepositoryPath().getYYYYMMDD();
		if (settupRelativeDerivative(yyyymmddStr) == false) {
			return false;
		}
		DerivativeMetadata& derivativeMetadata = derivativesObject.getDerivativeMetadata();
		std::string versionName = VersionControl::getInstance().getCurrentVersion().getVersionName();
		
		// Note the workspase needs the image name note the version image name
		std::string name = pathController.getImageName();
		if (this->writeMetadata2Workspace(imagePath, name, metadataObject) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" metadata to workspace", versionName.c_str());
			return false;
		}
		name = fileInfo.getName();
		if (this->writeDerivativeMetadat2Workspace(imagePath, versionName, derivativeMetadata, name) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" metadata to workspace", versionName.c_str());
			return false;
		}
		if (this->writeMetadata2PrimaryIndex(imagePath, versionName, metadataObject) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" metadata to primary index", versionName.c_str());
			return false;
		}
		if (this->writeMetadata2DerivativesDatabase(versionName, derivativeMetadata, name) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" metadata Derivatives Database", versionName.c_str());
			return false;
		}

		if (primaryIndexObject.addDerivativeImage(fileInfo, "A command", primarySeqNumber, derivativeSeqNumber, VersionControl::getInstance().getCurrentVersion()) == false) {
			if (ImportJournalManager::save() == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to save Journal File");
				return false;
			}
			return false;
		}

		versionControl.checkin(filepath, comment, force);

		if (checkoutStatus.checkinUpdate(filepath, comment) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to checkin: \"%s\" Warning: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}
		/* to be added later
		try {

		if (cd.CheckIn(CDCheckInOutManager::DB::Workspace, filepath) == false) {
		return false;
		}

		}
		catch (SIAAppException &e) {
		printf("Error: %s\n", e.what());
		if (checkoutStatus.checkin(filepath, comment) == false) {
		return false;
		}
		return false;
		}
		*/
		History& history = History::getHistory();
		history.checkinImage(pathController.getShortRelativePath().c_str(), version, comment);

		return true;
	}

	bool ArchiveObject::uncheckoutFile(const char *filepath, const char *comment, bool force) {
		CLogger &logger = CLogger::getLogger();
		VersionControl& versionControl = VersionControl::getInstance();
		if (versionControl.uncheckoutFile(filepath, comment, force) == false) {
			return false;
		}
		History& history = History::getHistory();
		if (history.uncheckoutImage(filepath, versionControl.getVersion(), comment) == false) {
			logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to update checkout history for image: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}
		return true;
		
	}


	bool ArchiveObject::TestForDuplicate(FileInfo& fileinfo, ImportJournal& importJournal, bool isForceChanges) {
		PrimaryIndexObject& primaryIndexObject = getPrimaryIndexObject();
		ImageIndex& imageIndex = primaryIndexObject.getimageIndex();
		CLogger &logger = CLogger::getLogger();
		if (isForceChanges) {
			if (imageIndex.IsDup(fileinfo.getCrc())) {
				//m_imageIndex->getData(BasicMetadata.getCrc());
				logger.log(LOG_OK, CLogger::Level::INFO, "Dup %s", fileinfo.getName().c_str());
				// reject image from import
				ErrorCode::setErrorCode(IMGA_ERROR::DUPLICATE_IMAGE);				
				return false;
			}
		}
		else {
			// normal operation. dups are rejected
			int pos = -1;
			if (pos = imageIndex.IsDup(fileinfo.getCrc())) {
				//m_imageIndex->getData(imageId->getCrc());
				logger.log(LOG_DUPLICATE, CLogger::Level::SUMMARY, "Image \"%s\" was found to be a duplicate. Rejecting from import", fileinfo.getName().c_str());
				// reject image from import
				ImageId imageId = imageIndex.findDup(fileinfo.getCrc());
				if (imageId.getName().empty()) {
					logger.log(LOG_OK, CLogger::Level::ERR, "Image indexing corrupt %s", fileinfo.getName().c_str());
					ErrorCode::setErrorCode(IMGA_ERROR::IMAGE_INDEXING_CORRUPT);
					throw std::exception();
				}
				else {
					importJournal.update(fileinfo.getPath().c_str(), ImportJournal::Result::Duplicate, imageId.getLocation().c_str());
					if (ImportJournalManager::save() == false) {
						logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to save Journal File");
						ErrorCode::setErrorCode(IMGA_ERROR::UNABLE_TO_SAVE_JOUNAL);
						return false;
					}
				}
				return false;
			}
			else {
				// Add To the Image Indexing (used to find duplicates) ImageIndex::add(const char *name, unsigned long crc, const char *md5) {
				//if (imageIndex.add(fileinfo) == false) {
				if (imageIndex.IsDup(fileinfo.getCrc()) == true) {
					logger.log(LOG_DUPLICATE, CLogger::Level::WARNING, "Image \"%s\" was found to be a duplicate. Rejecting from import", fileinfo.getName().c_str());
					// reject image from import
					ImageId imageId = imageIndex.findDup(fileinfo.getCrc());
					if (imageId.getName().empty()) {
						logger.log(LOG_OK, CLogger::Level::ERR, "Image indexing corrupt %s", fileinfo.getName().c_str());
					}
					else {
						importJournal.update(fileinfo.getPath().c_str(), ImportJournal::Result::Duplicate, imageId.getLocation().c_str());
						if (ImportJournalManager::save() == false) {
							logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to save Journal File");
							ErrorCode::setErrorCode(IMGA_ERROR::UNABLE_TO_SAVE_JOUNAL);
							return false;
						}
					}
					return false;
				}
				logger.log(LOG_OK, CLogger::Level::INFO, "Adding image to dups index %s", fileinfo.getName().c_str());
			}
		}
		return true;
	}
	
	bool ArchiveObject::imageHistory(const char *filepath, const LogDocument::FormatType& formatType) {
		CLogger &logger = CLogger::getLogger();
		History& history = History::getHistory();
		if (history.logImageHistory(filepath, formatType) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to process image history for image: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}
		
		return true;
	}
	
	bool ArchiveObject::exportImages(const char *dispPath) {
		ExportImages exportImages(m_masterPath.c_str());
		exportImages.process();
		return true;
	}

	bool ArchiveObject::listContents(const char *addressScope) {
		ContentsLister contentsLister(m_masterPath.c_str());
		contentsLister.List();
		return true;
	}

} // simplearchive
