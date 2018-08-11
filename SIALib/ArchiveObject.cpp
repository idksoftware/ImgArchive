
#include "AppConfig.h"
#include "AppPaths.h"
#include "SACmdArgs.h"
#include "ArchiveDate.h"
#include "ArchiveBuilder.h"
#include "StandardOut.h"
#include "ArchiveRepository.h"

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
#include "PathController.h"

#include "CSVDatabase.h"
#include "CSVDerivativeDatabase.h"
#include "HookCmd.h"
#include "ViewManager.h"
#include "Version.h"
#include "ImportJournal.h"
#include "CheckoutStatus.h"
#include "ContentsLister.h"
#include "ExportImages.h"
#include "CheckFile.h"
#include "ErrorCode.h"
#include "IndexVisitor.h"
#include "SQLiteDB.h"

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

#define LOG_CHECKOUT_FUNCTION 200
#define LOG_UNABLE_TO_CHECKOUT_GENERAL (LOG_CHECKOUT_FUNCTION + 50)
#define LOG_UNCHECKOUT_FUNCTION 300
#define LOG_UNABLE_TO_UNCHECKOUT_GENERAL (LOG_UNCHECKOUT_FUNCTION + 50)

#define FILECODE ARCHIVEOBJECT_CPP

namespace simplearchive {

	bool PrimaryIndexObject::init(PrimaryIndexPath &primaryIndexPath) {
		
		m_versionIndex->setVersionPath(primaryIndexPath.getHistoryPath().c_str());
		m_primaryIndexTable->setPath(primaryIndexPath.getIdxDbPath().c_str());
		if (!m_imageIndex->init(primaryIndexPath.getImageIndexPath().c_str())) {
			return false;
		}
		
		*m_primaryIndexPath = primaryIndexPath;
		
		return true;
	}

	bool PrimaryIndexObject::addMasterImage(const BasicMetadata &basicMetadata, ImagePath &imagePath, int masterSeqNumber, int primarySeqNumber) {
		unsigned long seqNumber = m_primaryIndexTable->getNextIndex();
		std::string seqNumberStr = std::to_string(primarySeqNumber);
		ExifDate date;

		if (m_primaryIndexTable->insert(seqNumber, imagePath.getRelativePath().c_str(), imagePath.getImageName().c_str(), basicMetadata.getSize(), basicMetadata.getCrc(), basicMetadata.getMd5().c_str(),
			basicMetadata.getUuid().c_str(), basicMetadata.getVersion(), date, masterSeqNumber) == false) {
			return false;
		}
		//m_imageIndex->updateVersion(basicMetadata.getCrc(), imagePath.getRelativePath().c_str());
		//this->m_primaryIndexPath.get()->getImageIndexPath();
		m_versionIndex->createMasterVersion(basicMetadata, imagePath.getYyyymmddStr().c_str(), masterSeqNumber, primarySeqNumber);
		m_imageIndex->updatePath(basicMetadata.getCrc(), imagePath.getRelativePath().c_str(), 0);
		return true;
	}
	
	bool PrimaryIndexObject::addDerivativeImage(FileInfo& fileInfo, const char *comment, int primarySeqNumber, int derivativeSeqNumber, int version) {

		VersionMetadataObject vmo;
		PathController pathController(fileInfo.getPath().c_str());
		if (pathController.split() == false) {
			return false;
		}
		ExifDate addDate;
		vmo.columnAt(DB_SEQUENCEID) = primarySeqNumber;
		vmo.columnAt(DB_VERSION) = version;
		vmo.columnAt(DB_DATABASEID) = derivativeSeqNumber;
		vmo.columnAt(DB_FILENAME) = fileInfo.getName().c_str();
		vmo.columnAt(DB_ORGINALNAME) = fileInfo.getName().c_str();;
		vmo.columnAt(DB_VERSIONPATH) = addDate.toShortRelativePath();
		vmo.columnAt(DB_CRC) = fileInfo.getCrc();
		vmo.columnAt(DB_MD5) = fileInfo.getMd5().c_str();
		vmo.columnAt(DB_UUID) = fileInfo.getUuid().c_str();
		vmo.columnAt(DB_FILESIZE) = fileInfo.getSize();
		vmo.columnAt(DB_DATEMODIFIED) = fileInfo.getModTime();
		vmo.columnAt(DB_DATECREATE) = fileInfo.getCreateTime();
		ExifDateTime addTime;
		
		vmo.columnAt(DB_DATEADDED) = addTime;
		
		unsigned long seqNumber = m_primaryIndexTable->getNextIndex();
		//std::string seqNumberStr = std::to_string(primarySeqNumber); 
		
		
		//	This is the rolling CSV table that holds all images in the database 
		// root path is SIA/pi/index"
		if (m_primaryIndexTable->insert(primarySeqNumber, pathController.getYearday().c_str(), fileInfo.getName().c_str(), fileInfo.getSize(),
																		fileInfo.getCrc(), fileInfo.getMd5().c_str(),
																		fileInfo.getUuid().c_str(), version, addDate, derivativeSeqNumber) == false) {
			return false;
		}
		
		m_versionIndex->createDerivativeVersion(vmo, pathController.getYearday().c_str(), vmo.columnAt(DB_DATABASEID).getInt(), vmo.columnAt(DB_SEQUENCEID).getInt());
		m_imageIndex->updatePath(vmo.columnAt(DB_CRC).getInt(), std::to_string(version).c_str(), version);
		return true;
	}

	void RepositoryObject::init(RepositoryPath &repositoryPath) {
		*m_repositoryPath = repositoryPath;
		m_masterIndexTable->setPath(repositoryPath.getCSVDatabasePath().c_str());
		
		//if (!m_imageIndex->init(repositoryPath.getImageIndexPath().c_str())) {
		//	return false;
		//}
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

	bool RepositoryObject::copyFile(const std::string &pathToSourceRoot,const std::string &file) {
		RepositoryPath &RepositoryPath = getRepositoryPath();
		//IntegrityManager &integrityManager = IntegrityManager::get();
		std::string from = pathToSourceRoot + "/" + file;
		std::string to = RepositoryPath.getDataPath() + '/' + file;

		if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
			return false;
		}
		// this can only be done after the file copy
		//integrityManager.addFile(m_yyyymmddStr.c_str(), file.c_str());
		return true;
	}

	
	bool RepositoryObject::checkout(const char *pathToTargetRoot, const char *targetRelPath, const char *comment, bool force) {
		CLogger &logger = CLogger::getLogger();
		std::string root = m_repositoryPath->getRepositoryPath().c_str();

		PathController pathController(m_repositoryPath->getRepositoryPath().c_str());
		//pathController.setRelativePath(filepath);
		pathController.splitPathAndFile(targetRelPath);

		if (pathController.makeRelativePath(targetRelPath) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid version path: \"%s\"?", targetRelPath);
			return false;
		}
		std::string from = m_repositoryPath->getRepositoryPath().c_str();



		from += pathController.getRelativePath();
		from += "/data/";
		from += pathController.getImageName();
		pathController.setFullPath(from);
		if (pathController.isValid() == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid database path: \"%s\"?", from.c_str());
			return false;
		}
		if (pathController.makeRelativePath(targetRelPath) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid target path: \"%s\"?", from.c_str());
			return false;
		}
		std::string to = pathToTargetRoot;
		to += '/';
		to += pathController.getRelativePath();
		to += '/';
		to += pathController.getImage();

		if (!force) {
			if (SAUtils::FileExists(to.c_str()) == true) {
				
				FileInfo fileInfoFrom(from);
				FileInfo fileInfoTo(to);
				if (fileInfoFrom.getCrc() != fileInfoTo.getCrc()) {
					logger.log(LOG_OK, CLogger::Level::FATAL, "Changes may be lost by Checkout \"%s\"?", targetRelPath);
					return false;
				}
				logger.log(LOG_OK, CLogger::Level::INFO, "Target image is the same as checked in image: \"%s\"?", from.c_str());
				return true;
			}
		}
		
		if (SAUtils::FileExists(from.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid database path: \"%s\"?", from.c_str());
			return false;
		}
		
		if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to copy version to destination: \"%s\"?", to.c_str());
			return false;
		}
		
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
	}

	bool DerivativesObject::checkin(FileInfo &fileInfo, const char *comment) {
		return true;
	}

	bool DerivativesObject::checkout(const char *pathToTargetRoot, const char *targetRelPath, int v, const char *comment, const char *verstionPath, bool force) {
		CLogger &logger = CLogger::getLogger();
		std::string root = m_repositoryPath->getRepositoryPath().c_str();

		PathController pathController(m_repositoryPath->getRepositoryPath().c_str());
		//pathController.setRelativePath(filepath);
		pathController.splitPathAndFile(targetRelPath);
		Version version(pathController.getImageName().c_str(), v);
		std::string versionedFilename = version.makeVersion();
		if (pathController.makeRelativePath(verstionPath) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid version path: \"%s\"?", verstionPath);
			return false;
		}
		std::string from = m_repositoryPath->getRepositoryPath().c_str();



		from += pathController.getRelativePath();
		from += "/data/";
		from += versionedFilename;
		pathController.setFullPath(from);
		if (pathController.isValid() == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid database path: \"%s\"?", from.c_str());
			return false;
		}
		if (pathController.makeRelativePath(targetRelPath) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid target path: \"%s\"?", from.c_str());
			return false;
		}
		std::string to = pathToTargetRoot;
		to += '/';
		to += pathController.getRelativePath();
		to += '/';
		to += pathController.getImage();

		if (SAUtils::FileExists(from.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Invalid database path: \"%s\"?", from.c_str());
			return false;
		}

		if (SAUtils::FileExists(to.c_str()) == true) {
			
			FileInfo fileInfoFrom(from);
			FileInfo fileInfoTo(to);
			if (fileInfoFrom.getCrc() != fileInfoTo.getCrc()) {
				if (force != true) {
					logger.log(LOG_OK, CLogger::Level::FATAL, "Changes may be lost by Checkout \"%s\"?", targetRelPath);
					return false;
				}
				else {
					logger.log(LOG_OK, CLogger::Level::INFO, "Forcing copy. This will overwrite changes, \"%s\"?", targetRelPath);
				}
			}
			else {
				if (force != true) {
					logger.log(LOG_OK, CLogger::Level::INFO, "File not copied as the images are the same, \"%s\"?", targetRelPath);
					return true;
				}
				else {
					logger.log(LOG_OK, CLogger::Level::INFO, "Forcing copy. However the images are the same, \"%s\"?", targetRelPath);
					
				}
			}
		}
		

		if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to copy version to destination: \"%s\"?", to.c_str());
			return false;
		}
		return true;
	}

	bool DerivativesObject::addimage(FileInfo& fileInfo, const char *comment, int primarySeqNumber, int derivativeSeqNumber, int version, MetadataObject& metadataObject) {

		CSVDerivativeDatabase &derivativeRepository = CSVDerivativeDatabase::get();
		DerivativeMetadata dmo;
		PathController pathController(fileInfo.getPath().c_str());
		pathController.split();

		dmo.columnAt(DB_SEQUENCEID) = primarySeqNumber;
		dmo.columnAt(DB_VERSION) = version;
		dmo.columnAt(DB_DATABASEID) = derivativeSeqNumber;
		dmo.columnAt(DB_FILENAME) = fileInfo.getName().c_str();
		dmo.columnAt(DB_ORGINALNAME) = fileInfo.getName().c_str();
		dmo.columnAt(DB_FILEPATH) = pathController.getYearday().c_str();
		dmo.columnAt(DB_CRC) = fileInfo.getCrc();
		dmo.columnAt(DB_MD5) = fileInfo.getMd5().c_str();
		dmo.columnAt(DB_UUID) = fileInfo.getUuid().c_str();
		dmo.columnAt(DB_FILESIZE) = fileInfo.getSize();
		dmo.columnAt(DB_DATEMODIFIED) = fileInfo.getModTime();
		dmo.columnAt(DB_DATECREATE) = fileInfo.getCreateTime();
		ExifDateTime addTime;
		ExifDate addDate;
		dmo.columnAt(DB_DATEADDED) = addTime;
		dmo.columnAt(DB_VERSIONPATH) = addDate.toShortRelativePath();
		
		CLogger &logger = CLogger::getLogger();
		try {
			derivativeRepository.add(dmo);
		}
		catch (std::exception e) {

			return false;
		}

		if (SQLiteDB::add(dmo) == false) {
			return false;
		}
		
		DerivativeIndexTable& derivativeIndexTable = getDerivativeIndexTable();
		if (derivativeIndexTable.insert(derivativeSeqNumber, pathController.getYearday().c_str(), fileInfo.getName().c_str(), fileInfo.getSize(), fileInfo.getCrc(), fileInfo.getMd5().c_str(),
			fileInfo.getUuid().c_str(), version, addDate) == false) {
			return false;
		}

		//std::string yyyymmddStr = imagePath.getYyyymmddStr();
		if (settupRelative(addDate.toShortRelativePath()) == false) {
			return false;
		}
		Version versionObj(fileInfo.getName().c_str(), version);
		if (versionObj.checkin(m_repositoryPath->getDataPath().c_str(), fileInfo.getPath().c_str()) == false) {
			return false;
		}

		

		return true;
	}


	bool getimage(const std::string &pathToTargetRoot, const char *filepath, const char *comment, int version) {

		CSVDerivativeDatabase &derivativeRepository = CSVDerivativeDatabase::get();
		return true;
	}

	bool DerivativesObject::settupRelative(std::string &yyyymmddStr) {
		return getRepositoryPath().settupRelative(yyyymmddStr);
	}

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

	//DerivativeTable &DerivativesObject::getDerivativeTable() {
	//	return *m_derivativeTable;
	//}

	ArchiveObject::ArchiveObject() noexcept :
		m_masterView(std::make_unique<MasterCatalogue>()),
		m_PrimaryIndexObject(std::make_unique<PrimaryIndexObject>())
	{}

	ArchiveObject::~ArchiveObject() {}

	bool ArchiveObject::isBackup1Enabled() {
		return m_backup[0].getRepositoryPath().isEnabled();
	}
	bool ArchiveObject::isBackup2Enabled() {
		return m_backup[1].getRepositoryPath().isEnabled();
	}

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

	RepositoryObject& ArchiveObject::getBackup1Object() {
		return m_backup[0];
	}

	RepositoryObject& ArchiveObject::getBackup2Object() {
		return m_backup[1];
	}

	RepositoryObject& ArchiveObject::getMasterObject() {
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

		RepositoryPath& derivative = ArchivePath::getDerivative();

		if (derivative.settup() == false) {
			return false;
		}
		if (SAUtils::DirExists(derivative.getRepositoryPath().c_str()) == false) {
			return false;
		}

		
		//VersionControl::setPathToArchives(ArchivePath::getPathToWorkspace().c_str(), ArchivePath::getMasterPath().c_str());
		if (ViewManager::initalise(ArchivePath::getMasterPath().c_str(), config.getConfigPath()) == false) {
			return false;
		}
		SQLiteDB::enableSQLiteDB(config.isSQL());

		if (SQLiteDB::init(config.getDatabasePath(), true) == false) {
			return false;
		}
			
		

		//std::string csvdbPath = ArchivePath::getMasterPath();
		//csvdbPath += CSVDB_PATH;
		//CSVDatabase::setDBPath(csvdbPath.c_str());
		std::string csvdbPath = ArchivePath::getDerivativePath();
		csvdbPath += CSVDB_PATH;
		CSVDerivativeDatabase::setDBPath(csvdbPath.c_str());
			
		
		
		if (ArchivePath::isBackup1Enabled() == true) {
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
		if (ArchivePath::isBackup2Enabled() == true) {
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
		PrimaryIndexPath& primaryIndexPath = ArchivePath::getPrimaryIndex();
		primaryIndexPath.setRepositoryPath(ArchivePath::getPathToHome() + PRIMARY_INDEX_PATH);
		primaryIndexPath.settup();
		try {
			if (!getPrimaryIndexObject().init(primaryIndexPath)) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to initalise the primary index", ArchivePath::getBackup2Path().c_str());
				return false;
			}

			if (SAUtils::DirExists(ArchivePath::getPathToWorkspace().c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Workspace folder: \"%s\" not accessable?", ArchivePath::getPathToWorkspace().c_str());
				return false;
			}
			if (ImagePath::settupMainArchiveFolders(ArchivePath::getPathToWorkspace().c_str(), ArchivePath::getMasterPath().c_str(), ArchivePath::getPathToHome().c_str()) == false) {

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

		MasterCatalogue& masterView = getMasterCatalogue();

		// Testing
		masterView.setFileEnabled(false);
		masterView.setWWWEnabled(false);

		if (masterView.isWWWEnabled()) {
			if (masterView.settupWWW(config.getTempPath(), config.getTemplatePath(), config.getMasterWWWCataloguePath()) == false) {
				return false;
			}


			if (masterView.settupSystemWWW(primaryIndexPath.getPathToRepository().c_str(), config.getTemplatePath(), config.getMasterWWWCataloguePath(), ArchivePath::getMainHistory().c_str(), master.getJournalPath().c_str()) == false) {
				return false;
			}
		}
		if (masterView.isFileEnabled()) {
			if (masterView.settupFile(config.getTempPath(), config.getMasterCataloguePath()) == false) {
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

	bool ArchiveObject::writeMetadata(std::string rootPath, ImagePath &imagePath, MetadataObject &metadataObject) {
		
		std::string toxml = rootPath + '/' + imagePath.getImageName() + ".xml";
		XMLWriter xmlWriter;
		if (xmlWriter.writeImage(metadataObject, toxml.c_str()) == false) {
			return false;
		}
		std::string tojson = rootPath + '/' + imagePath.getImageName() + ".json";
		JSONWriter jsonWriter;
		if (jsonWriter.writeImage(metadataObject, tojson.c_str()) == false) {
			return false;
		}
		std::string tohtml = rootPath + '/' + imagePath.getImageName() + ".html";
		HTMLWriter htmlWriter;
		if (htmlWriter.writeImage(metadataObject, tohtml.c_str()) == false) {
			return false;
		}
		return true;
	}


	bool ArchiveObject::CreateImage(const BasicMetadata &BasicMetadata, ImagePath &imagePath, MetadataObject &metadataObject) {
		CLogger &logger = CLogger::getLogger();
		PrimaryIndexTable& primaryIndexTable = getPrimaryIndexObject().getPrimaryIndexTable();
		MasterIndexTable& masterIndexTable = this->getMasterObject().getMasterIndexTable();


		// Note Both the Primary and the Master sequence numbers must be known before saving

		// Primary Index Object
		unsigned long primarySeqNumber = primaryIndexTable.getNextIndex();
		std::string seqNumberStr = std::to_string(primarySeqNumber);
		

		// Master Database Object
		unsigned long masterSeqNumber = masterIndexTable.getNextIndex();
		metadataObject.setSequenceId(primarySeqNumber);

		BasicMetadata.columnAt(DB_FILEPATH) = imagePath.getYyyymmddStr().c_str();
		metadataObject.columnAt(DB_FILEPATH) = imagePath.getYyyymmddStr().c_str();
		unsigned long n = BasicMetadata.getSize();
		unsigned long crc = BasicMetadata.getCrc();
		ExifDate date;
		date.now();

		std::string yyyymmddStr = imagePath.getYyyymmddStr();
		if (settupRelative(yyyymmddStr) == false) {
			return false;
		}
		// Write the metadata to the workspace
		if (this->writeMetadata2Workspace(imagePath, metadataObject) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" workspace", imagePath.getImageName().c_str());
			return false;
		}

		// Write the metadata to the primary index
		if (this->writeMetadata2PrimaryIndex(imagePath, metadataObject) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" primary index", imagePath.getImageName().c_str());
			return false;
		}

		// Write the metadata to the Master archive
		if (this->writeMetadata2MasterDatabase(imagePath, metadataObject) == false) {
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
		if (copyFile(imagePath.getCurrentSourcePath(), imagePath.getImageName()) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to copy image \"%s\" to Master Archive", imagePath.getImageName().c_str());
			return false;
		}
		if (imagePath.copyFile(imagePath.getCurrentSourcePath(), imagePath.getImageName()) == false) {
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
			logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to add to chechout file: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}

		if (m_masterView->addImage(imagePath.getCurrentSourcePath().c_str(), metadataObject, filepath.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::ERR, "Unable to preview file: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
		}

		

		return true;
	}

	bool ArchiveObject::settupRelative(std::string &yyyymmddStr) {
		if (ArchivePath::isMasterEnabled() == true) {
			if (m_master.settupRelative(yyyymmddStr) == false) {
				return false;
			}
		}
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
		
		return true;
	}
	bool ArchiveObject::writeMetadata2Workspace(ImagePath &imagePath, MetadataObject &metadataObject) {

		if (ArchiveObject::isWorkspaceEnabled() == true) {
			if (writeMetadata(imagePath.getWorkspaceMetadataPath(), imagePath, metadataObject) == false) {
				return false;
			}
		}
		

		return true;
	}

	bool ArchiveObject::writeMetadata2PrimaryIndex(ImagePath &imagePath, MetadataObject &metadataObject) {
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


		if (writeMetadata(toxml, imagePath, metadataObject) == false) {
			return false;
		}
		return true;
	}
	bool ArchiveObject::writeMetadata2MasterDatabase(ImagePath &imagePath, MetadataObject &metadataObject) {
		if (ArchivePath::isMasterEnabled() == true) {
			if (writeMetadata(m_master.getRepositoryPath().getMetadataPath(), imagePath, metadataObject) == false) {
				return false;
			}
		}
		if (ArchivePath::isBackup1Enabled() == true) {
			if (writeMetadata(m_backup[0].getRepositoryPath().getMetadataPath(), imagePath, metadataObject) == false) {
				return false;
			}
		}
		if (ArchivePath::isBackup2Enabled() == true) {
			if (writeMetadata(m_backup[1].getRepositoryPath().getMetadataPath(), imagePath, metadataObject) == false) {
				return false;
			}
		}
		return true;
	}
	bool ArchiveObject::writeMetadata2DerivativesDatabase(ImagePath &imagePath, MetadataObject &metadataObject) {
		if (ArchivePath::isDerivativeEnabled() == true) {
			if (writeMetadata(m_derivatives.getRepositoryPath().getMetadataPath(), imagePath, metadataObject) == false) {
				return false;
			}
		}
		return true;
	}


	bool ArchiveObject::copyFile(const std::string  &pathToSourceRoot,const std::string &file) {
		if (ArchivePath::isMasterEnabled() == true) {
			if (m_master.copyFile(pathToSourceRoot, file) == false) {
				return false;
			}
		}
		if (ArchivePath::isBackup1Enabled() == true) {
			if (m_backup[0].copyFile(pathToSourceRoot, file) == false) {
				return false;
			}
		}
		if (ArchivePath::isBackup2Enabled() == true) {
			if (m_backup[1].copyFile(pathToSourceRoot, file) == false) {
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
		
		std::string versionPath = primaryIndexObject.getVersionIndex().getCurrentVersion(filepath);
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


	bool ArchiveObject::checkout(const char *scope, const char *comment, bool force) {
		IndexVisitor IndexVisitor(new CheckoutAction(this));
		if (!IndexVisitor.setScope(scope)) {
			return false;
		}
		IndexVisitor.process();
		return true;
	}

	bool CheckoutAction::onImage() {
		std::string path = m_currentRow->columnAt(DB_FILEPATH).getString();
		path += '/';
		path += m_currentRow->columnAt(DB_FILENAME).getString();
			/*
			add(MTSchema(MTSchema::Text, DB_FILENAME));
		add(MTSchema(MTSchema::Text, DB_FILEPATH));
		add(MTSchema(MTSchema::Integer, DB_EVENT));
		add(MTSchema(MTSchema::Integer, DB_VERSION));
		add(MTSchema(MTSchema::Date, DB_DATEADDED));
		add(MTSchema(MTSchema::Date, DB_COMMENT));
		*/
		m_ArchiveObject->checkoutFile(path.c_str(), "", false);
		return true;
	}

	bool ArchiveObject::checkoutFile(const char *filepath, const char *comment, bool force) {
		CLogger &logger = CLogger::getLogger();
		PrimaryIndexObject& primaryIndexObject = getPrimaryIndexObject();
		VersionIndex versionIndex = primaryIndexObject.getVersionIndex();
		std::string targetRootPath = getWorkspacePath();
		PathController pathController;
		if (pathController.splitShort(filepath) == false) {
			ErrorCode::setErrorCode(SIA_ERROR::INVALID_PATH);
			logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}
		pathController.makeImagePath();
		if (versionIndex.setRowCursor(filepath) == false) {
			logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}
		SharedMTRow row = versionIndex.getCurrentRow();
		int version = row->columnAt(DB_VERSION).getInt();
		std::string versionPath = row->columnAt(DB_VERSIONPATH).getString();
		//versionPath += '/';
		//versionPath += pathController.getImageName();
		// Test the checkout status of the image
		CheckoutStatus checkoutStatus;
		if (checkoutStatus.checkout(filepath, comment) == false) {
			if (ErrorCode::getErrorCode() == SIA_ERROR::TARGET_NOT_FOUND) {
				// Not an error but the image needs to be copied form the repository to the workspace
				
			}
			else if (ErrorCode::getErrorCode() == SIA_ERROR::WILL_OVERWRITE_CHANGES) {
				// This may be an option
				logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				return false;
			}
			else {
				logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				return false;
			}
		}
		
		
		if (version == 0) {
			// Original
			RepositoryObject& repositoryObject = getMasterObject();
			if (repositoryObject.checkout(targetRootPath.c_str(), filepath, comment, force) == false) {
				return false;
			}
			//master.
		}
		else {
			DerivativesObject& derivativesObject = getDerivativesObject();
			derivativesObject.checkout(targetRootPath.c_str(), filepath, version, comment, versionPath.c_str(), force);
		}
		// In the state that it can be checked out
		try {

			if (CheckFile::CheckOut(filepath) == false) {
				logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				return false;
			}

		}
		catch (SIAAppException &e) {
			printf("Error: %s\n", e.what());
			if (checkoutStatus.checkin(filepath, comment) == false) {
				logger.log(LOG_UNABLE_TO_CHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to checkout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				return false;
			}
			return false;
		}
		History& history = History::getHistory();
		history.checkoutImage(pathController.getShortRelativePath().c_str(), version, comment);
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
				return false;
			}
		}
		else {
			// normal operation. dups are rejected
			int pos = -1;
			if (pos = imageIndex.IsDup(fileinfo.getCrc())) {
				//m_imageIndex->getData(imageId->getCrc());
				logger.log(LOG_DUPLICATE, CLogger::Level::WARNING, "Image \"%s\" was found to be a duplicate. Rejecting from import", fileinfo.getName().c_str());
				// reject image from import
				ImageId imageId = imageIndex.findDup(fileinfo.getCrc());
				if (imageId.getName().empty()) {
					logger.log(LOG_OK, CLogger::Level::ERR, "Image indexing corrupt %s", fileinfo.getName().c_str());
				}
				else {
					importJournal.update(fileinfo.getPath().c_str(), ImportJournal::Duplicate, imageId.getLocation().c_str());
					if (ImportJournalManager::save() == false) {
						logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to save Journal File");
						return false;
					}
				}
				return false;
			}
			else {
				// Add To the Image Indexing (used to find duplicates) ImageIndex::add(const char *name, unsigned long crc, const char *md5) {
				if (imageIndex.add(fileinfo) == false) {
					logger.log(LOG_DUPLICATE, CLogger::Level::WARNING, "Image \"%s\" was found to be a duplicate. Rejecting from import", fileinfo.getName().c_str());
					// reject image from import
					ImageId imageId = imageIndex.findDup(fileinfo.getCrc());
					if (imageId.getName().empty()) {
						logger.log(LOG_OK, CLogger::Level::ERR, "Image indexing corrupt %s", fileinfo.getName().c_str());
					}
					else {
						importJournal.update(fileinfo.getPath().c_str(), ImportJournal::Duplicate, imageId.getLocation().c_str());
						if (ImportJournalManager::save() == false) {
							logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to save Journal File");
							return false;
						}
					}
					return false;;
				}
				logger.log(LOG_OK, CLogger::Level::INFO, "Adding image to dups index %s", fileinfo.getName().c_str());
			}
		}
		return true;
	}

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

	bool CheckinAction::onImage() {
		std::string path = m_currentRow->columnAt(DB_FILEPATH).getString();
		path += '/';
		path += m_currentRow->columnAt(DB_FILENAME).getString();
		/*
		add(MTSchema(MTSchema::Text, DB_FILENAME));
		add(MTSchema(MTSchema::Text, DB_FILEPATH));
		add(MTSchema(MTSchema::Integer, DB_EVENT));
		add(MTSchema(MTSchema::Integer, DB_VERSION));
		add(MTSchema(MTSchema::Date, DB_DATEADDED));
		add(MTSchema(MTSchema::Date, DB_COMMENT));
		*/
		m_ArchiveObject->checkinFile(path.c_str(), "", false);
		return true;
	}

	bool ArchiveObject::checkin(const char *scope, const char *comment, bool force) {
		IndexVisitor IndexVisitor(new CheckinAction(this));
		if (!IndexVisitor.setScope(scope)) {
			return false;
		}
		IndexVisitor.process();
		return true;
	}

	bool ArchiveObject::checkinFile(const char *filepath, const char *comment, bool force) {
		CLogger &logger = CLogger::getLogger();
		
		CheckoutStatus checkoutStatus;
		ImportJournal& importJournal = ImportJournalManager::GetJournal();
		// Note Both the Primary and the Master sequence numbers must be known before saving

		
		
		// Test the checkin status of the image
		if (!force) {
			if (checkoutStatus.checkin(filepath, comment) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to checkin: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				return false;
			}
		}
		try {
			
			if (CheckFile::CheckIn(filepath) == false) {
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
		
		

		std::string workspacePath = getWorkspacePath();
		
		PathController pathController(workspacePath.c_str());
		pathController.splitShort(filepath);
		pathController.makeImagePath();

		ImagePath imagePath(filepath);

		PrimaryIndexObject& primaryIndexObject = getPrimaryIndexObject();
		
		
		VersionIndex& versionIndex = primaryIndexObject.getVersionIndex();

		DerivativesObject& derivativesObject = getDerivativesObject();		
		DerivativeIndexTable& derivativeIndexTable = derivativesObject.getDerivativeIndexTable();
		
		// Primary Index Object
		unsigned long primarySeqNumber = derivativeIndexTable.getNextIndex();
		
		// Derivatives Object
		unsigned long derivativeSeqNumber = derivativeIndexTable.getNextIndex();
		
		if (versionIndex.setRowCursor(filepath) == false) {
			return false;
		}
		SharedMTRow row = versionIndex.getCurrentRow();
		int version = row->columnAt(DB_VERSION).getInt();
		
		// =========================
		// This updates the version
		// =========================
		version++;

		// Fill-in the details of the checkedin image
		FileInfo fileInfo(pathController.getFullPath());
		
		if (TestForDuplicate(fileInfo, importJournal, false) == false) {
			if (ImportJournalManager::save() == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to save Journal File");
				return false;
			}
		}

		// Get The old metadataObject
		CSVDatabase csvDatabase = CSVDatabase::get();
		SharedMTRow metadataRow = csvDatabase.get(pathController.getImageName().c_str(), pathController.getRelativePath().c_str());
		if (metadataRow == nullptr) {
			return false;
		}
		
		// 

		if (primaryIndexObject.addDerivativeImage(fileInfo, "A command", primarySeqNumber, derivativeSeqNumber, version) == false) {
			if (ImportJournalManager::save() == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to save Journal File");
				return false;
			}
			return false;
		}
		
		MetadataObject metadataObject(*metadataRow);
		
		logger.log(LOG_OK, CLogger::Level::FINE, "Final metadata");
		MTTableSchema& mos = (MTTableSchema&)metadataObject.getSchema();
		for (std::vector<MTSchema>::iterator i = mos.begin(); i != mos.end(); i++) {
			MTSchema& columnInfo = *i;
			
			logger.log(LOG_OK, CLogger::Level::FINE, "%-20s %s\n", columnInfo.getName().c_str(), metadataObject.columnAt(columnInfo.getName().c_str()).toString().c_str());
		}

		metadataObject.update(fileInfo, derivativeSeqNumber, version);


		if (this->writeMetadata2Workspace(imagePath, metadataObject) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" metadata to workspace", imagePath.getImageName().c_str());
			return false;
		}
		if (this->writeMetadata2PrimaryIndex(imagePath, metadataObject) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" metadata to primary index", imagePath.getImageName().c_str());
			return false;
		}
		if (this->writeMetadata2DerivativesDatabase(imagePath, metadataObject) == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Fataled to write image \"%s\" metadata Derivatives Database", imagePath.getImageName().c_str());
			return false;
		}
		//primaryIndexObject.

		derivativesObject.addimage(fileInfo, comment, primarySeqNumber, derivativeSeqNumber, version, metadataObject);
		if (ImportJournalManager::save() == false) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Unable to save Journal File");
			return false;
		}
		
		History& history = History::getHistory();
		history.checkinImage(pathController.getShortRelativePath().c_str(), version,  comment);
		
		return true;
	}

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

	bool UnCheckoutAction::onImage() {
		std::string path = m_currentRow->columnAt(DB_FILEPATH).getString();
		path += '/';
		path += m_currentRow->columnAt(DB_FILENAME).getString();
		/*
		add(MTSchema(MTSchema::Text, DB_FILENAME));
		add(MTSchema(MTSchema::Text, DB_FILEPATH));
		add(MTSchema(MTSchema::Integer, DB_EVENT));
		add(MTSchema(MTSchema::Integer, DB_VERSION));
		add(MTSchema(MTSchema::Date, DB_DATEADDED));
		add(MTSchema(MTSchema::Date, DB_COMMENT));
		*/
		m_ArchiveObject->uncheckoutFile(path.c_str(), "", false);
		return true;
	}

	bool ArchiveObject::uncheckout(const char *scope, const char *comment, bool force) {
		IndexVisitor IndexVisitor(new CheckinAction(this));
		if (!IndexVisitor.setScope(scope)) {
			return false;
		}
		IndexVisitor.process();
		return true;
	}

	bool ArchiveObject::uncheckoutFile(const char *filepath, const char *comment, bool force) {
		CLogger &logger = CLogger::getLogger();

		CheckoutStatus checkoutStatus;
		if (checkoutStatus.checkin(filepath, comment) == false) {
			logger.log(LOG_UNABLE_TO_UNCHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to uncheckout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
			return false;
		}
		try {

			if (CheckFile::CheckIn(filepath) == false) {
				logger.log(LOG_UNABLE_TO_UNCHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to uncheckout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				return false;
			}

		}
		catch (SIAAppException &e) {
			printf("Error: %s\n", e.what());
			if (checkoutStatus.checkin(filepath, comment) == false) {
				logger.log(LOG_UNABLE_TO_UNCHECKOUT_GENERAL, CLogger::Level::FATAL, "Unable to uncheckout: \"%s\" Error: %s", filepath, ErrorCode::toString(ErrorCode::getErrorCode()));
				return false;
			}
			return false;
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
