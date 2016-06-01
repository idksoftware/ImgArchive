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
#include "ImageGroup.h"
#include "TargetsList.h"
#include "FileInfo.h"
#include "BasicExifFactory.h"
#include "XMLWriter.h"
#include "ImagePath.h"
#include "ArchiveObject.h"
#include "CSVDBFile.h"
#include "HistoryEvent.h"
#include "History.h"
#include "ImageHistory.h"
#include "CSVDatabase.h"
#include "HookCmd.h"
#include "ViewManager.h"
#include "VersionControl.h"
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

#include <stdio.h>
#include <sstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

#define VERSION	"1.00"
#define BUILD	"040115.1749"
#define DB "c:/temp/test3.db"


namespace simplearchive {
	


	


/////////////////////////////////////////////////////////


	SIALib::SIALib()
	{
		m_ArchiveBuilder.reset(new ArchiveBuilder(ArchiveObject::get()));
		m_winsockRequired = false;
		m_socklibStarted = false;
		m_enableEvents = false;

		
	}


	SIALib::~SIALib()
	{
		if (m_socklibStarted) {
			WSACleanup();
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

	void SIALib::setUseDate(ExifDate &date) {
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
		return CLogger::getLogger().getLastCode();
	}
	const char *SIALib::getLastMessage() {
		return CLogger::getLogger().getLastMessage();
	}


	int SIALib::initalise() {
		CTerminalServerManager &terminalServerManager = CTerminalServerManager::getInstance();
		terminalServerManager.start();

		CAppConfig &config = CAppConfig::get();

		config.init();
		//AppOptions &appOptions = AppOptions::get();

		//CLogger::setLevel(CLogger::INFO);
		CLogger::setLevel(CLogger::FINE);
		CLogger::setLogPath(config.getLogPath());
		//ChangeLog::setLogPath(config.getHistoryPath());

		CLogger &logger = CLogger::getLogger();
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



		if (m_enableEvents == true && m_socklibStarted == true) {
			if (UDPOut::enableUDPOutput(m_udpPortNum, m_udpAddress.c_str()) == false) {
				return false;
			}
		}

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
		logger.log(LOG_STARTING, CLogger::SUMMARY, "Application starting at %s", date.Print().c_str());
		logger.log(LOG_OK, CLogger::INFO, "Home path is \"%s\"", config.getHomePath());

		try {
			std::string temp = config.getWorkspacePath();

			if (SAUtils::DirExists(temp.c_str()) == false) {

				logger.log(LOG_OK, CLogger::INFO, "Main repository folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(LOG_WORKSPACE_NOT_FOUND, CLogger::INFO, "Cannot create Main repository folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(LOG_OK, CLogger::INFO, "Main repository folder created at location: \"%s\"", temp.c_str());
			}

			temp = config.getShadowPath();
			if (SAUtils::DirExists(temp.c_str()) == false) {

				logger.log(LOG_OK, CLogger::INFO, "Shadow Repository folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(LOG_OK, CLogger::INFO, "Cannot create Shadow Repository folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(LOG_OK, CLogger::INFO, "Shadow repository folder created at location: \"%s\"", temp.c_str());
			}
			/* ArchiveObject
			if (ImagePath::settupMainArchiveFolders(config.getWorkspacePath(), config.getShadowPath(), config.getHomePath()) == false) {

				return -1;
			}
	
			VersionControl::setPathToArchives(config.getWorkspacePath(), config.getShadowPath());
			History::setPath(config.getHistoryPath());
			History &history = History::getHistory();
			std::string csvdbPath = config.getShadowPath();
			csvdbPath += "/.csvdb";
			CSVDatabase::setDBPath(csvdbPath.c_str());
			*/
			if (ArchiveObject::get().Initalise() == false) {
				return -1;
			}

			if (ImageExtentions::setExtentionsFilePath(config.getConfigPath()) == false) {
				logger.log(LOG_OK, CLogger::INFO, "Unable to find image extensions file path: \"%s\"", config.getConfigPath());
				return -1;
			}
			if (SAUtils::DirExists(temp.c_str()) == false) {
				logger.log(LOG_OK, CLogger::INFO, "Hidden .sia folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(LOG_OK, CLogger::INFO, "Cannot create Hidden .sia folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				if (SAUtils::setHidden(temp.c_str()) == false) {
					logger.log(LOG_OK, CLogger::INFO, "Cannot set Hidden .sia folder to a hidden folder \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(LOG_OK, CLogger::INFO, "Hidden .sia folder created at location: \"%s\"", temp.c_str());
			}

			logger.log(LOG_OK, CLogger::FINE, "Log path \"%s\"", config.getLogPath());
			temp = config.getHistoryPath();
			if (SAUtils::DirExists(temp.c_str()) == false) {
				logger.log(LOG_OK, CLogger::INFO, "History folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(LOG_OK, CLogger::INFO, "Cannot create History folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(LOG_OK, CLogger::INFO, "History folder created at location: \"%s\"", temp.c_str());
			}
			temp = config.getIndexPath();
			if (SAUtils::DirExists(temp.c_str()) == false) {
				logger.log(LOG_OK, CLogger::INFO, "Index folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(LOG_OK, CLogger::INFO, "Cannot create Index folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(LOG_OK, CLogger::INFO, "Index folder created at location: \"%s\"", temp.c_str());
			}
			if (ImageExtentions::setExtentionsFilePath(config.getConfigPath()) == false) {
				return -1;
			}
			HookCmd::setHookPath(config.getHookPath());
			m_ArchiveBuilder->setUseExternalExifTool(true);

			if (m_ArchiveBuilder->Init() == false) {
				return -1;
			}
			/* this needs looking at??
			if (MirrorManager::initalise(config.getShadowPath(), config.getConfigPath()) == false) {
				return -1;
			}
			*/
			if (ViewManager::initalise(config.getShadowPath(), config.getConfigPath()) == false) {
				return -1;
			}
			Database &db = Database::getInstance();

			if (db.open(DB, "", "") == false) {
				printf("database open returned %s", db.getError());
			}
		}
		catch (SIAAppException e) {
			logger.log(LOG_OK, CLogger::FATAL, "Failed to complete initalisation %s\n", e.what());
			return -1;
		}
		logger.log(LOG_OK, CLogger::SUMMARY, "Initalisation complete");
		return 0;
	}

	

	int SIALib::complete() {
		CLogger &logger = CLogger::getLogger();
		SummaryFile &summaryFile = SummaryFile::getSummaryFile();
		CIDKDate date;
		date.Now();
		logger.log(LOG_OK, CLogger::SUMMARY, "Application completed successfully at %s", date.Print().c_str());
		summaryFile.log(SummaryFile::SF_BRIEF, SummaryFile::SF_COMMENT, "Summary start");
		return 0;
	}

	bool SIALib::ImportFile(const char *filePath) {
		if (m_ArchiveBuilder->ImportFile(filePath) == false) {
			return false;
		}

		return true;
	}

	bool SIALib::Import() {
		CAppConfig &config = CAppConfig::get();
		if (m_ArchiveBuilder->Import(config.getSourcePath()) == false) {
			return false;
		}
		
		return true;
	}

	bool SIALib::exportImage(const char *distpath) {
		if (ArchiveObject::get().exportImages(distpath) == false) {
			return false;
		}

		return true;
	}


	bool SIALib::showCheckedOut(const char *filepath) {

		if (ArchiveObject::get().showCheckedOut(filepath) == false) {
			return false;
		}
		return true;
	}

	bool SIALib::showUncheckedOutChanges(const char *filepath) {
		if (ArchiveObject::get().showUncheckedOutChanges(filepath) == false) {
			return false;
		}
		return true;
	}

	bool SIALib::checkout(const char *filepath, const char *comment) {
		if (ArchiveObject::get().checkout(filepath, comment) == false) {
			return false;
		}
		
		return true;
	}

	bool SIALib::checkin(const char *filepath, const char *comment) {
		if (ArchiveObject::get().checkin(filepath, comment) == false) {
			return false;
		}
		return true;
	}

	bool SIALib::uncheckout(const char *filepath, const char *comment) {
		if (ArchiveObject::get().uncheckout(filepath, comment) == false) {
			return false;
		}
		return true;
		
	}

	

	bool SIALib::listContents(const char *addressScope) {
		if (ArchiveObject::get().listContents(addressScope) == false) {
			return false;
		}
		return true;
	}

	bool SIALib::view(const char *name) {
	
		/*
		if (name.compare("Master") == 0) {
		viewManager.initaliseMaster(config.getArchivePath(), config.getMasterViewPath());
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

	bool SIALib::mirror(const char *name) {
		
		/*
		if (name.compare("Master") == 0) {
		viewManager.initaliseMaster(config.getArchivePath(), config.getMasterViewPath());
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

		CAppConfig &config = CAppConfig::get();
		printf("Archive");
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

	bool SIALib::validate(const char *archivePath, const char *workspacePath, const char *homePath, Scope scope, bool repair) {
		IntegrityManager &im = IntegrityManager::get();
		if (repair) {
			switch (scope) {
			case Workspace:
				if (im.repair(true, false) == false) {
					return false;
				}
			case Shadow:
				if (im.repair(false, true) == false) {
					return false;
				}
			default:
				if (im.repair(true, true) == false) {
					return false;
				}
			}
			return true;
		}
		else {
			switch (scope) {
			case Workspace:
				if (im.validate(true, false) == false) {
					return false;
				}
			case Shadow:
				if (im.validate(false, true) == false) {
					return false;
				}
			default:
				if (im.validate(true, true) == false) {
					return false;
				}
			}
			return true;
		}
		return true;
	}
} // simplearchive