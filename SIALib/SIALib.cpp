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
#include "FileId.h"
#include "ImageId.h"
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
		logger.log(CLogger::INFO, "=== Application starting at %s ===", date.Print().c_str());
		logger.log(CLogger::INFO, "Home path is \"%s\"", config.getHomePath());

		std::string temp = config.getArchivePath();

		if (SAUtils::DirExists(temp.c_str()) == false) {

			logger.log(CLogger::INFO, "Repository folder not found at location: \"%s\"", temp.c_str());

			if (SAUtils::mkDir(temp.c_str()) == false) {
				logger.log(CLogger::INFO, "Cannot create Repository folder at location: \"%s\"", temp.c_str());
				return false;
			}
			logger.log(CLogger::INFO, "Repository folder created at location: \"%s\"", temp.c_str());
		}

		if (ImagePath::settupMainArchiveFolders(config.getArchivePath()) == false) {

			return false;
		}

		History &history = History::getHistory();
		history.setPath(config.getHistoryPath());
		if (SAUtils::DirExists(temp.c_str()) == false) {
			logger.log(CLogger::INFO, "Hidden .sia folder not found at location: \"%s\"", temp.c_str());

			if (SAUtils::mkDir(temp.c_str()) == false) {
				logger.log(CLogger::INFO, "Cannot create Hidden .sia folder at location: \"%s\"", temp.c_str());
				return false;
			}
			if (SAUtils::setHidden(temp.c_str()) == false) {
				logger.log(CLogger::INFO, "Cannot set Hidden .sia folder to a hidden folder \"%s\"", temp.c_str());
				return false;
			}
			logger.log(CLogger::INFO, "Hidden .sia folder created at location: \"%s\"", temp.c_str());
		}

		logger.log(CLogger::FINE, "Log path \"%s\"", config.getLogPath());
		temp = config.getHistoryPath();
		if (SAUtils::DirExists(temp.c_str()) == false) {
			logger.log(CLogger::INFO, "History folder not found at location: \"%s\"", temp.c_str());

			if (SAUtils::mkDir(temp.c_str()) == false) {
				logger.log(CLogger::INFO, "Cannot create History folder at location: \"%s\"", temp.c_str());
				return false;
			}
			logger.log(CLogger::INFO, "History folder created at location: \"%s\"", temp.c_str());
		}
		temp = config.getIndexPath();
		if (SAUtils::DirExists(temp.c_str()) == false) {
			logger.log(CLogger::INFO, "Index folder not found at location: \"%s\"", temp.c_str());

			if (SAUtils::mkDir(temp.c_str()) == false) {
				logger.log(CLogger::INFO, "Cannot create Index folder at location: \"%s\"", temp.c_str());
				return false;
			}
			logger.log(CLogger::INFO, "Index folder created at location: \"%s\"", temp.c_str());
		}
		if (ImageExtentions::setExtentionsFilePath(config.getConfigPath()) == false) {
			return false;
		}
		HookCmd::setHookPath(config.getHookPath());
		m_ArchiveBuilder->setUseExternalExifTool(true);

		if (m_ArchiveBuilder->Init() == false) {
			return false;
		}
		if (MirrorManager::initalise(config.getArchivePath(), config.getConfigPath()) == false) {
			return false;
		}
		if (ViewManager::initalise(config.getArchivePath(), config.getConfigPath()) == false) {
			return false;
		}
		Database &db = Database::getInstance();

		if (db.open(DB, "", "") == false) {
			printf("database open returned %s", db.getError());
		}

		logger.log(CLogger::INFO, "+++ Initalisation complete +++");
		return 0;
	}

	int SIALib::complete() {
		CLogger &logger = CLogger::getLogger();
		SummaryFile &summaryFile = SummaryFile::getSummaryFile();
		CIDKDate date;
		date.Now();
		logger.log(CLogger::INFO, "+++ Application completed successfully at %s +++", date.Print().c_str());
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
	}

} // simplearchive