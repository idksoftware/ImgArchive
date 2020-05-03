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
**	Filename	: FileInfo.cpp
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#include "SIALib.h"
#include "AppConfig.h"
//#include "SIAArcAppOptions.h"
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
#include "ImagePath.h"
#include "ArchiveObject.h"
#include "CSVDBFile.h"
#include "HistoryEvent.h"
#include "History.h"
#include "CSVDatabase.h"
#include "HookCmd.h"
#include "ViewManager.h"
#include "SummaryFile.h"
#include "CIDKDate.h"
#include "MirrorManager.h"
#include "Database.h"
#include "CSVDatabase.h"
#include "MakeMedia.h"
#include "UDPOut.h"
#include "IntegrityManager.h"
#include "TerminalServer.h"
#include "ArchivePath.h"
#include "CheckoutStatus.h"
#include "IndexVisitor.h"
#include "RemoteServer.h"
#include <stdio.h>
#include <sstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

#define VERSION	"1.00"
#define BUILD	"040115.1749"

#undef FILECODE
#define FILECODE SIALIB_CPP

namespace simplearchive {
	


	


/////////////////////////////////////////////////////////


	SIALib::SIALib() :
		m_ArchiveBuilder(std::make_shared<ArchiveBuilder>(ArchiveObject::getInstance())),
		m_winsockRequired(false),
		m_socklibStarted(false),
		m_enableEvents(false) 
	{}


	SIALib::~SIALib()
	{
		if (m_socklibStarted) {
#ifdef _WIN32
			WSACleanup();
#endif
		}
	}

	void SIALib::setForceDate() {
		ArchiveDate::setForceDate(true);
	}

	void SIALib::setUseEXIFDate() {
		ArchiveDate::setUseEXIFDate(true);
	}

	void SIALib::setUseFileDate() {
		ArchiveDate::setUseFileDate(true);
	}

	void SIALib::setUseDateToday() {
		ArchiveDate::setUseDateToday(true);
	}

	void SIALib::setUseDate(const ExifDate &date) {
		ArchiveDate::setUseDate(date);
		ArchiveDate::setDefaultDateSet(true);
	}

	



	int SIALib::checkin() {
		return 99;
	}

	void  SIALib::enableEvents(const char *address, int port) {
		m_udpPortNum = port;
		m_udpAddress = address;
		m_enableEvents = true;
		m_winsockRequired = true;
	}

	void  SIALib::enableServer(int port) {
		m_tcpPortNum = port;
		m_enableServer = true;
		m_winsockRequired = true;
	}

	const int SIALib::getLastCode() {
		return CLogger::getLastCode();
	}
	const char *SIALib::getLastMessage() {
		return CLogger::getLastMessage();
	}


	int SIALib::initalise() {
		
		
		AppConfig &config = AppConfig::get();

		CLogger::setLogLevel(config.getLogLevel());
		CLogger::setConsoleLevel(config.getConsoleLevel());
		CLogger::setSilent(config.isSilent());
		CLogger::setLogPath(config.getLogPath());
		try {
			CLogger::startLogging();
		}
		catch (exception e) {
			printf("Failed to start logging");
			return -1;
		}
		

		CLogger &logger = CLogger::getLogger();

		
#ifdef _WIN32
		WSADATA wsa;
		if (m_winsockRequired) {
			//Initialise winsock
			if (m_socklibStarted == false) {
				
				if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
				{
					//printf("Failed. Error Code : %d", WSAGetLastError());
					return false;
				}
				
				m_socklibStarted = true;
			}
		}
#endif
		

		m_enableEvents = config.isEventsOn();
		if (m_enableEvents == true && m_socklibStarted == true) {
			m_udpPortNum = config.eventPort();
			m_udpAddress = config.eventAddress();
			if (UDPOut::enableUDPOutput(m_udpPortNum, m_udpAddress.c_str()) == false) {
				return false;
			}
		}


		m_enableServer = config.isServerOn();
		if (m_enableServer == true && m_socklibStarted == true) {
			if (UDPOut::enableUDPOutput(m_udpPortNum, m_udpAddress.c_str()) == false) {
				return false;
			}
		}

		
		SummaryFile::setPath(config.getHistoryPath());
		SummaryFile &summaryFile = SummaryFile::getSummaryFile();
		CIDKDate date;
		date.Now();
		summaryFile.log(SummaryFile::SF_BRIEF, SummaryFile::SF_COMMENT, "Summary start %s", date.Print().c_str());
		logger.log(LOG_STARTING, CLogger::Level::SUMMARY, "Application starting at %s", date.Print().c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "Home path is \"%s\"", config.getHomePath());

		if (ImageExtentions::setExtentionsFilePath(config.getConfigPath()) == false) {
			logger.log(LOG_OK, CLogger::Level::INFO, "Unable to find image extensions file path: \"%s\"", config.getConfigPath());
			return -1;
		}
		std::string temp = config.getWorkspacePath();
		if (SAUtils::DirExists(temp.c_str()) == false) {
			if (SAUtils::mkDir(temp.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Cannot create Workspace at location: \"%s\"", temp.c_str());
				return -1;
			}
		}

		if (SAUtils::DirExists(temp.c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::INFO, "Hidden .sia folder not found at location: \"%s\"", temp.c_str());

			if (SAUtils::mkDir(temp.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Cannot create Hidden .sia folder at location: \"%s\"", temp.c_str());
				return -1;
			}
			if (SAUtils::setHidden(temp.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::INFO, "Cannot set Hidden .sia folder to a hidden folder \"%s\"", temp.c_str());
				return -1;
			}
			logger.log(LOG_OK, CLogger::Level::INFO, "Hidden .sia folder created at location: \"%s\"", temp.c_str());
		}


		try {
			auto &archiveObject = ArchiveObject::getInstance();
			if (archiveObject.Initalise() == false) {
				return -1;
			}
			/* This is done by ArchiveObject -> ArchivePath
			

			if (SAUtils::DirExists(temp.c_str()) == false) {

				logger.log(LOG_OK, CLogger::Level::INFO, "Main repository folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(LOG_WORKSPACE_NOT_FOUND, CLogger::Level::INFO, "Cannot create Main repository folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(LOG_OK, CLogger::Level::INFO, "Main repository folder created at location: \"%s\"", temp.c_str());
			}
			

			temp = config.getMasterPath();
			if (SAUtils::DirExists(temp.c_str()) == false) {

				logger.log(LOG_OK, CLogger::Level::INFO, "Master Repository folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(LOG_OK, CLogger::Level::INFO, "Cannot create Master Repository folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(LOG_OK, CLogger::Level::INFO, "Master repository folder created at location: \"%s\"", temp.c_str());
			}
			*/
			/* ArchiveObject
			if (ImagePath::settupMainArchiveFolders(config.getWorkspacePath(), config.getMasterPath(), config.getHomePath()) == false) {

				return -1;
			}
			
			const RepositoryPath &rp = archiveObject.getMasterPath();
			VersionControl::setPathToArchives(archiveObject.getWorkspacePath().c_str(), rp.getRepositoryPath().c_str());
			History::setPath(config.getHistoryPath());
			History &history = History::getHistory();
			std::string csvdbPath = rp.getRepositoryPath().c_str();
			csvdbPath += "/.csvdb";
			CSVDatabase::setDBPath(csvdbPath.c_str());
			*/
			
			
			logger.log(LOG_OK, CLogger::Level::FINE, "Log path \"%s\"", config.getLogPath());
			temp = config.getHistoryPath();
			if (SAUtils::DirExists(temp.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::INFO, "History folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(LOG_OK, CLogger::Level::FATAL, "Cannot create History folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(LOG_OK, CLogger::Level::INFO, "History folder created at location: \"%s\"", temp.c_str());
			}
			logger.log(LOG_OK, CLogger::Level::FINE, "System path \"system\"");
			temp = config.getSystemPath();
			if (SAUtils::DirExists(temp.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::INFO, "System path not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(LOG_OK, CLogger::Level::FATAL, "Cannot create System path at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(LOG_OK, CLogger::Level::INFO, "System folder created at location: \"%s\"", temp.c_str());
			}
			
			/*
			temp = config.getIndexPath();
			if (SAUtils::DirExists(temp.c_str()) == false) {
				logger.log(LOG_OK, CLogger::Level::INFO, "Index folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(LOG_OK, CLogger::Level::FATAL, "Cannot create Index folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(LOG_OK, CLogger::Level::INFO, "Index folder created at location: \"%s\"", temp.c_str());
			}
			*/
			
			if (ImageExtentions::setExtentionsFilePath(config.getConfigPath()) == false) {
				return -1;
			}
			HookCmd::setHookPath(config.getHookPath());
			m_ArchiveBuilder->setUseExternalExifTool(true);

			if (m_ArchiveBuilder->Init() == false) {
				return -1;
			}
			/* this needs looking at??
			if (MirrorManager::initalise(config.getMasterPath(), config.getConfigPath()) == false) {
				return -1;
			}
			*/
			/*
			if (ViewManager::initalise(config.getMasterPath(), config.getConfigPath()) == false) {
				return -1;
			}
			*/
			if (config.isSQL()) {
				Database &db = Database::getInstance();
				std::string dbPath = config.getDatabasePath();
				dbPath += '/';
				dbPath += SQLITE3_DB;
				if (db.open(dbPath.c_str(), "", "") == false) {
					printf("database open returned %s", db.getError());
				}
			}
		}
		catch (SIAException e) {
			logger.log(LOG_OK, CLogger::Level::FATAL, "Failed to complete initalisation %s\n", e.what());
			return -1;
		}
		logger.log(LOG_INITALISATION, CLogger::Level::INFO, "Initalisation complete");
		return 0;
	}

	
	/*
	int SIALib::complete() {
		CLogger &logger = CLogger::getLogger();
		SummaryFile &summaryFile = SummaryFile::getSummaryFile();
		CIDKDate date;
		date.Now();
		logger.log(LOG_COMPLETED, CLogger::Level::SUMMARY, "Application completed successfully at %s", date.Print().c_str());
		//logger.log(LOG_SUMMARY, CLogger::Level::SUMMARY, "Completed Summary\n%s", completedSummary.getSummary());
		//logger.log(LOG_RESULT, CLogger::Level::SUMMARY, "Result: %s", completedSummary.getResult());
		summaryFile.log(SummaryFile::SF_BRIEF, SummaryFile::SF_COMMENT, "Summary start");
		return 0;
	}
	*/

	int SIALib::complete() {
		CLogger &logger = CLogger::getLogger();
		SummaryFile &summaryFile = SummaryFile::getSummaryFile();
		CIDKDate date;
		date.Now();
		logger.log(LOG_COMPLETED, CLogger::Level::SUMMARY, "Application completed successfully at %s\n", date.Print().c_str());
		summaryFile.log(SummaryFile::SF_BRIEF, SummaryFile::SF_COMMENT, "Summary start");
		return 0;
	}

	// using list of images in a file
	bool SIALib::ImportFile(const char *filePath) {
		if (m_ArchiveBuilder->ImportFile(filePath) == false) {
			return false;
		}

		return true;
	}

	bool SIALib::Import() {
		AppConfig &config = AppConfig::get();
		if (config.isLightroom()) {
			if (m_ArchiveBuilder->ImportLightroom(config.getLightroomPath()) == false) {
				return false;
			}
		}
		else {
			if (m_ArchiveBuilder->Import(config.getSourcePath()) == false) {
				return false;
			}
		}
		
		return true;
	}

	bool SIALib::show() {
		AppConfig &config = AppConfig::get();
		//config.setup();

		std::string str = config.toString();
		std::cout << "Using\n====================\n";
		
		//completedSummary.setSummary(str.c_str());
		//completedSummary.setResult("Completed Successfully");
		return true;
	}

	bool SIALib::exportImage(const char *distpath) {
		if (ArchiveObject::getInstance().exportImages(distpath) == false) {
			//completedSummary.setResult("Failed to complete");
			return false;
		}
		//completedSummary.setResult("Completed Successfully");
		return true;
	}


	bool SIALib::showCheckedOut(const char *filepath) {

		if (ArchiveObject::getInstance().showCheckedOut(filepath) == false) {
			//completedSummary.setResult("Failed to complete");
			return false;
		}
		//completedSummary.setResult("Completed Successfully");
		return true;
	}

	bool SIALib::showUncheckedOutChanges(const char *filepath) {
		if (ArchiveObject::getInstance().showUncheckedOutChanges(filepath) == false) {
			return false;
		}
		return true;
	}

	bool SIALib::get(const char *scope, const char *comment, bool force) {
		const char *version = "1, 2, [4-@]";
		if (ArchiveObject::getInstance().get(scope, version, comment, force) == false) {
			return false;
		}

		return true;
	}

	bool SIALib::checkout(const char *scope, const char *comment, bool force) {
		
		if (ArchiveObject::getInstance().checkout(scope, comment, force) == false) {
			return false;
		}
		
		return true;
	}

	bool SIALib::checkin(const char* scope, const char* comment, bool force) {
		if (ArchiveObject::getInstance().checkin(scope, comment, force) == false) {
			return false;
		}
		return true;
	}

	bool SIALib::uncheckout(const char* scope, const char* comment, bool force) {
		if (ArchiveObject::getInstance().uncheckout(scope, comment, force) == false) {
			return false;
		}
		return true;

	}

	bool SIALib::log(const char *filepath, LogDocument::FormatType& formatType, const char *filename) {
		
		if (ArchiveObject::getInstance().imageHistory(filepath, formatType, filename) == false) {
			return false;
		}
		return true;

	}

	bool SIALib::listContents(const char *addressScope) {
		if (ArchiveObject::getInstance().listContents(addressScope) == false) {
			return false;
		}
		return true;
	}

	bool SIALib::view(const char *name) {
	
		/*
		if (name.compare("Master") == 0) {
		viewManager.initaliseMaster(config.getArchivePath(), config.getMasterCataloguePath());
		//if (viewManager.processMaster() == false) {
		//	return false;
		//}
		}
		else {
		*/
		ViewManager& viewManager = ViewManager::GetInstance();
		if (viewManager.process() == false) {
			return false;
		}
		return true;
	}

	bool SIALib::status(const char *scope) {

		
		CheckoutStatus checkoutStatus;
		if (checkoutStatus.scopedStatus(scope) == false) {
			
			return false;
		}
		return true;
	}
	bool SIALib::mirror(const char *name) {
		
		/*
		if (name.compare("Master") == 0) {
		viewManager.initaliseMaster(config.getArchivePath(), config.getMasterCataloguePath());
		//if (viewManager.processMaster() == false) {
		//	return false;
		//}
		}
		else {
		*/
		MirrorManager mirrorManager;
		
		// Make mirror
		
		if (mirrorManager.mirror() == false) {
			return false;
		}
		
		if (mirrorManager.verifyMirror() == false) {
			return false;
		}
		return true;
	}
	bool SIALib::archive(const char *archivePath, const char *distPath, unsigned long sizeOfMedia, ExifDateTime *startDate, ExifDateTime *endDate) {
		
		if (archivePath == nullptr) {
		}

		if (distPath == nullptr) {
		}

		if (startDate == nullptr) {

		}

		if (endDate == nullptr) {

		}

		AppConfig &config = AppConfig::get();
		//DEBUG_PRINT("Archive");
		MakeMedia makeMedia;
		if (config.isToDateSet() || config.isFromDateSet()) {
			
			
			if (makeMedia.initalise(config.getWorkspacePath(), config.getBackupDestinationPath(), config.getBackupMediaSize(), startDate, endDate) == false) {
				return false;
			}
		}
		else {
			MakeMedia makeMedia;
			if (makeMedia.initalise(config.getWorkspacePath(), config.getBackupDestinationPath(), config.getBackupMediaSize()) == false) {
				return false;
			}
		}
		if (makeMedia.process() == false) {
			return false;
		}
		
		return true;
	}

	bool SIALib::checkDisk() {
		IntegrityManager &im = IntegrityManager::get();
		im.makeList();
		return true;
	}

	
	bool SIALib::validate(Scope scope, bool main, VerifyBackups verifyBackups, bool repair) {
		CLogger &logger = CLogger::getLogger();
		IntegrityManager &im = IntegrityManager::get();
		IMCompletedSummary imCompletedSummary;
		IntegrityManager::VerifyBackups imVerifyBackups;
		switch (verifyBackups) {
		case SIALib::VerifyBackups::Backup_1: imVerifyBackups = IntegrityManager::VerifyBackups::Backup_1; break;
		case SIALib::VerifyBackups::Backup_2: imVerifyBackups = IntegrityManager::VerifyBackups::Backup_1; break;
		case SIALib::VerifyBackups::Both: imVerifyBackups = IntegrityManager::VerifyBackups::Backup_1; break;
		case SIALib::VerifyBackups::None: imVerifyBackups = IntegrityManager::VerifyBackups::Backup_1; break;
		}
		
		switch (scope) {
		case Scope::Workspace:
			if (im.validate(imCompletedSummary, IntegrityManager::Scope::Workspace, main, imVerifyBackups, repair) == false) {
				return false;
			}
			break;
		case Scope::Master:
			if (im.validate(imCompletedSummary, IntegrityManager::Scope::Master, main, imVerifyBackups, repair) == false) {
				return false;
			}
			break;
		case Scope::Derivative:
			if (im.validate(imCompletedSummary, IntegrityManager::Scope::Derivative, main, imVerifyBackups, repair) == false) {
				return false;
			}
			break;
		case Scope::All:
			if (im.validate(imCompletedSummary, IntegrityManager::Scope::All, main, imVerifyBackups, repair) == false) {
				return false;
			}
			break;
		case Scope::Main:
			if (im.validate(imCompletedSummary, IntegrityManager::Scope::Main, main, imVerifyBackups, repair) == false) {
				return false;
			}
			break;
		default:
			if (im.validate(imCompletedSummary, IntegrityManager::Scope::All, main, imVerifyBackups, repair) == false) {
				return false;
			}
		}
		CompletedSummary completedSummary;
		completedSummary.setSummary(imCompletedSummary.getSummary());
		completedSummary.setResult(imCompletedSummary.getResult());
		return true;
		
		/*
		else {
			switch (scope) {
			case Workspace:
				logger.log(LOG_COMMAND, CLogger::Level::SUMMARY, "Validating Workspace");
				try {
					if (im.validate(imCompletedSummary, true, false) == false) {
						return false;
					}
				}
				catch (std::exception& e) {
					logger.log(LOG_COMMAND, CLogger::Level::FATAL, e.what());
					return false;
				}
				break;
			case Master:
				logger.log(LOG_COMMAND, CLogger::Level::SUMMARY, "Validating Master");
				if (im.validate(imCompletedSummary, false, true) == false) {
					return false;
				}
				break;
			default:
				logger.log(LOG_COMMAND, CLogger::Level::SUMMARY, "Validating Both Master and Workspace");
				if (im.validate(imCompletedSummary, true, true) == false) {
					return false;
				}
			}
			//completedSummary.setSummary(imCompletedSummary.getSummary());
			//completedSummary.setResult(imCompletedSummary.getResult());
			return true;
		}
		*/
		return true;
	}

	bool SIALib::sync(MainArchives mainArchives, Backups Backups)
	{
		CLogger& logger = CLogger::getLogger();

		IntegrityManager::MainArchives imArchives;
		IntegrityManager::Backups imBackups;

		bool master = false;
		bool derivative = false;
		switch (mainArchives) {
		case SIALib::MainArchives::Derivative: 
			imArchives = IntegrityManager::MainArchives::Derivative;
			logger.log(LOG_COMPLETED, CLogger::Level::SUMMARY, "Syncing Derivative archive");
			derivative = true;
			break;
		case SIALib::MainArchives::Master:
			logger.log(LOG_COMPLETED, CLogger::Level::SUMMARY, "Syncing Master archive");
			imArchives = IntegrityManager::MainArchives::Master;
			master = true;
			break;
		case SIALib::MainArchives::Both:
			logger.log(LOG_COMPLETED, CLogger::Level::SUMMARY, "Syncing Both Master and Derivative archives");
			imArchives = IntegrityManager::MainArchives::Both;
			master = true;
			derivative = true;
			break;
		}

		
		IntegrityManager& im = IntegrityManager::get();

		switch (Backups) {
		case SIALib::Backups::Backup_1:
			imBackups = IntegrityManager::Backups::Backup_1;
			logger.log(LOG_COMPLETED, CLogger::Level::SUMMARY, "Syncing archive with Backup 1");
			if ((master && !im.isMasterBackup1enabled()) || (derivative && !im.isDerivativeBackup1enabled())) {
				logger.log(LOG_COMPLETED, CLogger::Level::ERR, "Backup 1 not enabled cannot continue;");
					return false;
			}
			break;
		case SIALib::Backups::Backup_2:
			imBackups = IntegrityManager::Backups::Backup_2;
			logger.log(LOG_COMPLETED, CLogger::Level::SUMMARY, "Syncing archive with Backup 2");
			if ((master && !im.isMasterBackup2enabled()) || (derivative && !im.isDerivativeBackup2enabled())) {
				logger.log(LOG_COMPLETED, CLogger::Level::ERR, "Backup 2 not enabled cannot continue;");
					return false;
			}
			break;
		case SIALib::Backups::Both:
			imBackups = IntegrityManager::Backups::Both;
			logger.log(LOG_COMPLETED, CLogger::Level::SUMMARY, "Syncing archive with Both Backups");
			if ((master && !im.isMasterBackup1enabled()) || (derivative && !im.isDerivativeBackup1enabled())) {
				logger.log(LOG_COMPLETED, CLogger::Level::ERR, "Backup 1 not enabled cannot continue;");
					return false;
			}
			if ((master && !im.isMasterBackup2enabled()) || (derivative && !im.isDerivativeBackup2enabled())) {
				logger.log(LOG_COMPLETED, CLogger::Level::ERR, "Backup 2 not enabled cannot continue;");
					return false;
			}
			break;
		default:
			return false;
		}
		

		bool ret = im.sync(imArchives, imBackups);

		
		int imagesUpdated = im.imagesUpdated();
		if (imagesUpdated) {
			logger.log(LOG_COMPLETED, CLogger::Level::SUMMARY, "Sync: updated %d images", imagesUpdated);
		}
		else {
			logger.log(LOG_COMPLETED, CLogger::Level::SUMMARY, "Sync: no images updated, backup was found to be up-to-date");
		}
		
		return ret;
	}
	

	bool SIALib::remoteServer() {
		RemoteServer remoteServer;
		remoteServer.Connect(22);
		remoteServer.Run();
		/*
		CTerminalServer terminalServer;
		terminalServer.Init(22);
		terminalServer.Run();
		*/
		return true;
	}
} // simplearchive
