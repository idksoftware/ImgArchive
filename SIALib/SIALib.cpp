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

#include <stdio.h>
#include <sstream>

#define VERSION	"1.00"
#define BUILD	"040115.1749"
#define DB "c:/temp/test3.db"


namespace simplearchive {

	SIALib::SIALib()
	{
		m_ArchiveBuilder.reset(new ArchiveBuilder);
	}


	SIALib::~SIALib()
	{
	}

	int SIALib::checkin() {
		return 99;
	}

	int SIALib::initalise() {
		
		CAppConfig &config = CAppConfig::get();
		//AppOptions &appOptions = AppOptions::get();

		//CLogger::setLevel(CLogger::INFO);
		CLogger::setLevel(CLogger::FINE);
		CLogger::setLogPath(config.getLogPath());
		//ChangeLog::setLogPath(config.getHistoryPath());

		CLogger &logger = CLogger::getLogger();
		SummaryFile::setPath(config.getHistoryPath());
		SummaryFile &summaryFile = SummaryFile::getSummaryFile();
		CIDKDate date;
		date.Now();
		summaryFile.log(SummaryFile::SF_BRIEF, SummaryFile::SF_COMMENT, "Summary start %s", date.Print().c_str());
		logger.log(CLogger::SUMMARY, "Application starting at %s", date.Print().c_str());
		logger.log(CLogger::INFO, "Home path is \"%s\"", config.getHomePath());

		try {
			std::string temp = config.getArchivePath();

			if (SAUtils::DirExists(temp.c_str()) == false) {

				logger.log(CLogger::INFO, "Main repository folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(CLogger::INFO, "Cannot create Main repository folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(CLogger::INFO, "Main repository folder created at location: \"%s\"", temp.c_str());
			}

			temp = config.getShadowPath();
			if (SAUtils::DirExists(temp.c_str()) == false) {

				logger.log(CLogger::INFO, "Shadow Repository folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(CLogger::INFO, "Cannot create Shadow Repository folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(CLogger::INFO, "Shadow repository folder created at location: \"%s\"", temp.c_str());
			}
			if (ImagePath::settupMainArchiveFolders(config.getArchivePath(), config.getShadowPath()) == false) {

				return -1;
			}
	
			VersionControl::setPathToArchives(config.getArchivePath(), config.getShadowPath());
			History::setPath(config.getHistoryPath());
			History &history = History::getHistory();
			std::string csvdbPath = config.getShadowPath();
			csvdbPath += "/.csvdb";
			CSVDatabase::setDBPath(csvdbPath.c_str());

			if (SAUtils::DirExists(temp.c_str()) == false) {
				logger.log(CLogger::INFO, "Hidden .sia folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(CLogger::INFO, "Cannot create Hidden .sia folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				if (SAUtils::setHidden(temp.c_str()) == false) {
					logger.log(CLogger::INFO, "Cannot set Hidden .sia folder to a hidden folder \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(CLogger::INFO, "Hidden .sia folder created at location: \"%s\"", temp.c_str());
			}

			logger.log(CLogger::FINE, "Log path \"%s\"", config.getLogPath());
			temp = config.getHistoryPath();
			if (SAUtils::DirExists(temp.c_str()) == false) {
				logger.log(CLogger::INFO, "History folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(CLogger::INFO, "Cannot create History folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(CLogger::INFO, "History folder created at location: \"%s\"", temp.c_str());
			}
			temp = config.getIndexPath();
			if (SAUtils::DirExists(temp.c_str()) == false) {
				logger.log(CLogger::INFO, "Index folder not found at location: \"%s\"", temp.c_str());

				if (SAUtils::mkDir(temp.c_str()) == false) {
					logger.log(CLogger::INFO, "Cannot create Index folder at location: \"%s\"", temp.c_str());
					return -1;
				}
				logger.log(CLogger::INFO, "Index folder created at location: \"%s\"", temp.c_str());
			}
			if (ImageExtentions::setExtentionsFilePath(config.getConfigPath()) == false) {
				return -1;
			}
			HookCmd::setHookPath(config.getHookPath());
			m_ArchiveBuilder->setUseExternalExifTool(true);

			if (m_ArchiveBuilder->Init() == false) {
				return -1;
			}
			if (MirrorManager::initalise(config.getShadowPath(), config.getConfigPath()) == false) {
				return -1;
			}
			if (ViewManager::initalise(config.getShadowPath(), config.getConfigPath()) == false) {
				return -1;
			}
			Database &db = Database::getInstance();

			if (db.open(DB, "", "") == false) {
				printf("database open returned %s", db.getError());
			}
		}
		catch (SIAAppException e) {
			logger.log(CLogger::FATAL, "Failed to complete initalisation %s\n", e.what());
			
		}
		logger.log(CLogger::SUMMARY, "Initalisation complete");
		return 0;
	}

	int SIALib::complete() {
		CLogger &logger = CLogger::getLogger();
		SummaryFile &summaryFile = SummaryFile::getSummaryFile();
		CIDKDate date;
		date.Now();
		logger.log(CLogger::SUMMARY, "Application completed successfully at %s", date.Print().c_str());
		summaryFile.log(SummaryFile::SF_BRIEF, SummaryFile::SF_COMMENT, "Summary start");
		return 0;
	}

	bool SIALib::Import() {
		CAppConfig &config = CAppConfig::get();
		if (m_ArchiveBuilder->Import(config.getSourcePath()) == false) {
			return false;
		}
		return true;
	}

	bool SIALib::checkout(const char *filepath, const char *comment) {
		if (m_ArchiveBuilder->checkout(filepath, comment) == false) {
			return false;
		}
		return true;
	}

	bool SIALib::checkin(const char *filepath, const char *comment) {
		if (m_ArchiveBuilder->checkin(filepath, comment) == false) {
			return false;
		}
		return true;
	}

	bool SIALib::uncheckout(const char *filepath, const char *comment) {
		if (m_ArchiveBuilder->uncheckout(filepath, comment) == false) {
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
		ViewManager viewManager;
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
		if (mirrorManager.mirror() == false) {
			return false;
		}
		return true;
	}
	bool SIALib::archive() {
		CAppConfig &config = CAppConfig::get();
		printf("Archive");
		if (config.isToDateSet() || config.isFromDateSet()) {
			//MakeMedia makeMedia(config.getArchivePath().c_str(), config.setBackupDestinationPath(), config.getBackupMediaSize(), CDate startDate, CDate endDate);
		}
		else {
			MakeMedia makeMedia;
			if (makeMedia.initalise(config.getArchivePath(), config.getBackupDestinationPath(), config.getBackupMediaSize()) == false) {
				return false;
			}
			if (makeMedia.process() == false) {
				return false;
			}
		}
		return true;
	}

} // simplearchive