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

#include <sstream>
#include <iomanip>
#include "AppConfig.h"
#include "SAUtils.h"
#include "ArchivePath.h"
#include "CLogger.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

#define FILECODE APPCONFIG_CPP

namespace simplearchive {

	AppConfig *AppConfig::m_this = NULL;
	
	bool AppConfig::m_verbose = false;
	bool AppConfig::m_quiet = true;
	bool AppConfig::m_silent = false;
	std::string AppConfig::m_logLevel = "SUMMARY";
	bool AppConfig::m_dry_run = false;
	bool AppConfig::m_useDatabase = true;

	bool AppConfig::m_eventsOn = false; // UDP events
	bool AppConfig::m_serverOn = false;

	int AppConfig::m_tcpPortNum = 11000;
	int AppConfig::m_udpPortNum = 11001;
	std::string AppConfig::m_udpAddress = "127.0.0.1";

	std::string AppConfig::m_hookPath;
	std::string AppConfig::m_toolsPath;
	std::string AppConfig::m_workspacePath;
	std::string AppConfig::m_derivativePath;
	std::string AppConfig::m_masterPath;
	std::string AppConfig::m_sourcePath;
	std::string AppConfig::m_catalogPath;
	std::string AppConfig::m_configPath;
	std::string AppConfig::m_tempPath;
	std::string AppConfig::m_logPath;
	std::string AppConfig::m_consoleLevel;
	std::string AppConfig::m_homePath;
	std::string AppConfig::m_systemPath;
	std::string AppConfig::m_indexPath;
	std::string AppConfig::m_historyPath;	
	std::string AppConfig::m_ExternalExifTool;
	std::string AppConfig::m_ExternalCommandLine;
	std::string AppConfig::m_ExifMapPath;
	std::string AppConfig::m_ExifMapFile;
	std::string AppConfig::m_templatePath;
	std::string AppConfig::m_backupDestinationPath;
	
	std::string AppConfig::m_DatabasePath;
	std::string AppConfig::m_backup1;
	std::string AppConfig::m_backup2;

	std::string AppConfig::m_masterCataloguePath;
	std::string AppConfig::m_masterWWWCataloguePath;
	bool AppConfig::m_externalExifToolEnabled = false;
	bool AppConfig::m_masterViewEnabled = true;
	bool AppConfig::m_masterViewFullSizeOn = true;
	bool AppConfig::m_masterViewPreview1On = true;
	bool AppConfig::m_masterViewThumbnailOn = true;

	bool AppConfig::m_backup1Enabled = false;
	bool AppConfig::m_backup2Enabled = false;

	long AppConfig::m_backupMediaSize;
	ExifDateTime AppConfig::m_fromDate;
	ExifDateTime AppConfig::m_toDate;
	bool AppConfig::m_isFromDate = false;
	bool AppConfig::m_isToDate = false;

	

	

	AppConfig &AppConfig::get() {
		if (m_this == NULL) {
			m_this = new AppConfig;
		}
		return *m_this;
	}

	

	

	/*
		Default paths based on UserDrive and UserHome

	*/
	bool SIAARCConfig::init(const char *homePath) {
		CLogger &logger = CLogger::getLogger();
		
		logger.log(LOG_OK, CLogger::Level::INFO, "Initalising configuration");
		// Home Path 
		if (homePath != nullptr) {
			AppConfig::m_homePath = homePath;
		} else if (AppConfig::m_homePath.empty() == true) { // use ProgramData default
			std::string tempProgramData = SAUtils::GetPOSIXEnv("ProgramData");
			AppConfig::m_homePath = tempProgramData + DEFAULT_HOME_PATH;
			if (SAUtils::DirExists(AppConfig::m_homePath.c_str()) == false) {
				AppConfig::m_homePath = SAUtils::GetPOSIXEnv("HOMEDRIVE");
				AppConfig::m_homePath += SAUtils::GetPOSIXEnv("HOMEPATH");
				AppConfig::m_homePath += DEFAULT_HOME_PATH;
				if (SAUtils::DirExists(AppConfig::m_homePath.c_str()) == false) {
					return false;
				}
			}
			
		}
		ArchivePath::setPathToHome(AppConfig::m_homePath);
		logger.log(LOG_OK, CLogger::Level::INFO, "Home path                 \"%s\"", AppConfig::m_homePath.c_str());

		// Backup 1
		if (AppConfig::m_backup1.empty() == true) {
			if (getRoot().value("BackupOne", AppConfig::m_backup1) == true) {
				ArchivePath::setBackup1Path(AppConfig::m_backup1);
				AppConfig::m_backup1Enabled = true;
			}
		}
		else {
			ArchivePath::setBackup1Path(AppConfig::m_backup1);
		}
		
		// Backup 2
		if (AppConfig::m_backup2.empty() == true) {
			if (getRoot().value("BackupTwo", AppConfig::m_backup2) == true) {
				ArchivePath::setBackup2Path(AppConfig::m_backup2);
				AppConfig::m_backup2Enabled = true;
			}
		}
		else {
			ArchivePath::setBackup2Path(AppConfig::m_backup2);
		}

		// Repository Archive Path
		if (AppConfig::m_masterPath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("MasterPath", AppConfig::m_masterPath) == false) {
				std::string temp = AppConfig::m_homePath;
				AppConfig::m_masterPath = temp + MASTER_PATH;
				
			}
		}
		ArchivePath::setMasterPath(AppConfig::m_masterPath);
		logger.log(LOG_OK, CLogger::Level::INFO, "Master path               \"%s\"", AppConfig::m_masterPath.c_str());

		// Repository Archive Path
		if (AppConfig::m_derivativePath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("DerivativePath", AppConfig::m_derivativePath) == false) {
				std::string temp = AppConfig::m_homePath;
				AppConfig::m_derivativePath = temp + DERIVATIVE_PATH;
				
			}
		}
		ArchivePath::setDerivativePath(AppConfig::m_derivativePath);
		logger.log(LOG_OK, CLogger::Level::INFO, "Derivative path           \"%s\"", AppConfig::m_derivativePath.c_str());

		// History Path
		if (AppConfig::m_historyPath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("HistoryPath", AppConfig::m_historyPath) == false) {
				std::string temp = SAUtils::GetPOSIXEnv("HOMEPATH");
				AppConfig::m_historyPath = AppConfig::m_homePath + HISTORY_PATH;
				
			}
		}
		ArchivePath::setMainHistory(AppConfig::m_historyPath);
		logger.log(LOG_OK, CLogger::Level::INFO, "History path              \"%s\"", AppConfig::m_historyPath.c_str());

		// AppConfig::m_masterViewPath
		if (AppConfig::m_masterWWWCataloguePath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("MasterWWWCataloguePath", AppConfig::m_masterWWWCataloguePath) == false) {
				std::string temp = SAUtils::GetPOSIXEnv("HOMEPATH");
				AppConfig::m_masterWWWCataloguePath = AppConfig::m_homePath + MASTER_WWW_CATALOGUE_PATH;

			}
		}
		logger.log(LOG_OK, CLogger::Level::INFO, "Master Web catalogue path \"%s\"", AppConfig::m_masterWWWCataloguePath.c_str());

		if (AppConfig::m_masterCataloguePath.empty() == true) {
			// read from config file
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("MasterCataloguePath", AppConfig::m_masterCataloguePath) == false) {
				// if not found read from SIA_WORKSPACE environment variable
				std::string temp = SAUtils::GetPOSIXEnv("SIA_MASTER_CATALOGUE");
				if (temp.empty() == false) {
					AppConfig::m_masterCataloguePath = temp;
				}
				else {
					std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
					std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
					AppConfig::m_masterCataloguePath = tempHomeDrive + tempHomePath + DEFAULT_MASTER_CATALOGUE_PATH;
				}
			}
		}
		//ArchivePath::setMasterCataloguePath(AppConfig::m_masterViewPath);
		logger.log(LOG_OK, CLogger::Level::INFO, "Master catalogue path     \"%s\"", AppConfig::m_masterCataloguePath.c_str());

		// Workspace Path
		// This is used in siaadm.exe
		if (AppConfig::m_workspacePath.empty() == true) {
			// read from config file
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("WorkspacePath", AppConfig::m_workspacePath) == false) {
				// if not found read from SIA_WORKSPACE environment variable
				std::string temp = SAUtils::GetPOSIXEnv("SIA_WORKSPACE");
				if (temp.empty() == false) {
					AppConfig::m_workspacePath = temp;
				}
				else {
					std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
					std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
					AppConfig::m_workspacePath = tempHomeDrive + tempHomePath + DEFAULT_WORKSPACE_PATH;
				}
			}
		}
		ArchivePath::setPathToWorkspace(AppConfig::m_workspacePath);
		logger.log(LOG_OK, CLogger::Level::INFO, "Workspace path            \"%s\"", AppConfig::m_workspacePath.c_str());
		
		std::string temp = SAUtils::GetPOSIXEnv("SIA_SOURCE");
		if (temp.empty() == false) {
			AppConfig::m_sourcePath = temp.c_str();
		}
		temp = SAUtils::GetPOSIXEnv("SIA_LOGLEVEL");
		if (temp.empty() == false) {
			AppConfig::m_logLevel = temp.c_str();
		}
		return true;
	}

	void AppConfig::settup() {
		getWorkspacePath();
		getMasterPath();
		getDerivativePath();
		getTempPath();
		getToolsPath();
		getHookPath();
		getTemplatePath();
		getLogPath();
		getIndexPath();
		getHistoryPath();
		getExternalCommandLine();
		getExifMapPath();
		getConfigPath();
	    getHomePath();
		getMasterCataloguePath();
		getDatabasePath();
		getBackupDestinationPath();
		getBackupMediaSize();
		if (isFromDateSet() == true) {
			getFromDate().toString();
		}
		if (isToDateSet() == true) {
			getToDate().toString();
		}
		isDryRun();
		isQuiet();
		isVerbose();
		getLogLevel();
	}


	
	void SIAARCConfig::fileBasedValues(const char *home) {
		CLogger &logger = CLogger::getLogger();
		logger.log(LOG_OK, CLogger::Level::INFO, "Reading configuration file ");
		// Home Path (The path to this file will be based on the home path)
		std::string homePath = home;
		setHomePath(home);

		// Backup 1
		if (getRoot().value("BackupOne", AppConfig::m_backup1) == true) {
			ArchivePath::setBackup1Path(AppConfig::m_backup1);
			AppConfig::m_backup1Enabled = true;
		}
		// Backup 2
		if (getRoot().value("BackupTwo", AppConfig::m_backup2) == true) {
			ArchivePath::setBackup2Path(AppConfig::m_backup2);
			AppConfig::m_backup2Enabled = true;
		}
	
	// General	
		std::string dry_run = "false";
		setGeneral(DRY_RUN_LABEL, dry_run, dry_run);
		AppConfig::m_dry_run = (stricmp(dry_run.c_str(), "true") == 0);


	// System Folders
		// Master Archive Path
		//std::shared_ptr<ConfigBlock> folders = getSystemFolders();
		//if (folders == nullptr || getRoot().value("MasterPath", AppConfig::m_masterPath) == true) {
		//	ArchivePath::setMasterPath(AppConfig::m_masterPath);
		//}
		//
//#define CONFIG_PATH_LABEL				"ConfigPath"	 
//#define TOOLS_PATH_LABEL           		"ToolsPath"
		
		setSystemFolders(TEMP_PATH_LABEL, AppConfig::m_tempPath, homePath + TEMP_PATH);
//#define SOURCE_PATH_LABEL         		"SourcePath"
		setSystemFolders(SYSTEM_PATH_LABEL, AppConfig::m_systemPath, homePath + SYSTEM_PATH);
		setSystemFolders(LOG_PATH_LABEL, AppConfig::m_logPath, homePath + MASTER_PATH);
		setSystemFolders(MASTER_PATH_LABEL, AppConfig::m_masterPath, homePath + MASTER_PATH);
		setSystemFolders(DERIVATIVE_PATH_LABEL, AppConfig::m_derivativePath, homePath + DERIVATIVE_PATH);
		setSystemFolders(TOOLS_PATH_LABEL, AppConfig::m_toolsPath, homePath + TOOLS_PATH);
		setSystemFolders(HOOK_SCRIPTS_PATH_LABEL, AppConfig::m_hookPath, homePath + HOOKS_PATH);
//#define HOME_PATH_LABEL					"HomePath"

		setSystemFolders(HISTORY_PATH_LABEL, AppConfig::m_historyPath, homePath + HISTORY_PATH);	
		/*
		if (m_historyPath.empty() == true) {
		if (getRoot().value("HistoryPath", m_logPath) == false) {
		std::string temp = SAUtils::GetEnv("HOMEPATH");
		m_historyPath = m_homePath + HISTORY_PATH;
		ArchivePath::setMainHistory(m_historyPath);
		}
		}
		*/

		setSystemFolders(TEMPLATE_PATH_LABEL, AppConfig::m_templatePath, homePath + TEMPLATE_PATH);
		setSystemFolders(CATALOG_PATH_LABEL, AppConfig::m_catalogPath, homePath + TOOLS_PATH);
		

	// External Exif Tool
		std::string emptyString;
		setExternalExifTool(EXIF_MAP_PATH_LABEL, AppConfig::m_ExifMapPath, homePath + CONFIG_PATH);
		setExternalExifTool(EXIF_MAP_FILE_LABEL, AppConfig::m_ExifMapFile, emptyString);
		std::string externalExifToolEnabledStr;
		emptyString = "False";
		setExternalExifTool(ENABLED_LABEL, externalExifToolEnabledStr, emptyString);
		AppConfig::m_externalExifToolEnabled = (stricmp(externalExifToolEnabledStr.c_str(), "true") == 0);
		
	// Master
		std::string backup1Enabled = "false";
		setMaster(BACKUP_ONE_ENABLED_LABEL, backup1Enabled, backup1Enabled);
		AppConfig::m_backup1Enabled = (stricmp(backup1Enabled.c_str(), "true") == 0);
		
		std::string backup2Enabled = "false";
		setMaster(BACKUP_ONE_ENABLED_LABEL, backup2Enabled, backup2Enabled);
		AppConfig::m_backup2Enabled = (stricmp(backup2Enabled.c_str(), "true") == 0);

	// Network
		std::string eventsOn = "false";
		setMaster(BACKUP_ONE_ENABLED_LABEL, eventsOn, eventsOn);
		AppConfig::m_eventsOn = (stricmp(eventsOn.c_str(), "true") == 0);

		std::string m_udpPortNum = "127.0.0.1";
		setMaster(BACKUP_ONE_ENABLED_LABEL, m_udpPortNum, m_udpPortNum);
		//AppConfig::m_udpAddress = std::to

		AppConfig::m_udpAddress;
		AppConfig::m_udpPortNum;
		

		AppConfig::m_serverOn;
		AppConfig::m_tcpPortNum;

		setNetwork(BACKUP_ONE_ENABLED_LABEL, backup2Enabled, backup2Enabled);
		
		// Workspace Path	
		std::string wtemp = SAUtils::GetPOSIXEnv("SIA_WORKSPACE");
		if (wtemp.empty() == true) {
			std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
			std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
			wtemp = tempHomeDrive + tempHomePath + DEFAULT_WORKSPACE_PATH;
		}
		setSystemFolders("SystemPath", AppConfig::m_systemPath, AppConfig::m_masterPath + SYSTEM_PATH);
		setSystemFolders(WORKSPACE_PATH_LABEL, AppConfig::m_workspacePath, wtemp);
		ArchivePath::setPathToWorkspace(AppConfig::m_workspacePath);

		// Workspace Path	
		std::string ctemp = SAUtils::GetPOSIXEnv("SIA_MASTER_CATALOGUE");
		if (ctemp.empty() == true) {
			std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
			std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
			ctemp = tempHomeDrive + tempHomePath + DEFAULT_MASTER_CATALOGUE_PATH;
		}
		// todo
		setSystemFolders(MASTER_VIEW_PATH_LABEL, AppConfig::m_masterCataloguePath, ctemp);
		setWorkspacePath(AppConfig::m_workspacePath.c_str());
		setMasterPath(AppConfig::m_masterPath.c_str());

		logger.log(LOG_OK, CLogger::Level::INFO, "    General");
		logger.log(LOG_OK, CLogger::Level::INFO, "        Dry run enabled\"%s\"", (AppConfig::m_dry_run) ? "True" : "False");
		logger.log(LOG_OK, CLogger::Level::INFO, "    System paths");
		logger.log(LOG_OK, CLogger::Level::INFO, "        System path \"%s\"", AppConfig::m_systemPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Log path \"%s\"", AppConfig::m_logPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Master path \"%s\"", AppConfig::m_masterPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Derivetive path \"%s\"", AppConfig::m_derivativePath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Tools path \"%s\"", AppConfig::m_toolsPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Hook path \"%s\"", AppConfig::m_hookPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        History path \"%s\"", AppConfig::m_historyPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Template path \"%s\"", AppConfig::m_templatePath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Catalog path \"%s\"", AppConfig::m_catalogPath.c_str());
		
		logger.log(LOG_OK, CLogger::Level::INFO, "    External Exif Tool");
		logger.log(LOG_OK, CLogger::Level::INFO, "        External Exif tool enabled \"%s\"", (AppConfig::m_externalExifToolEnabled)?"True":"False");
		logger.log(LOG_OK, CLogger::Level::INFO, "        Exif map path \"%s\"", AppConfig::m_ExifMapPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Exif map file \"%s\"", AppConfig::m_ExifMapFile.c_str());

		
		
	}

	void SIAARCConfig::setWorkspacePath(const char *path) {
		AppConfig::m_workspacePath = path;
		ArchivePath::setPathToWorkspace(AppConfig::m_workspacePath);
	}

	void SIAARCConfig::setMasterPath(const char *path) {
		AppConfig::m_masterPath = path;
		ArchivePath::setMasterPath(AppConfig::m_masterPath);
	}



	void AppConfig::setToolsPath(const char *toolsPath) {
		AppConfig::m_toolsPath = toolsPath;
	}
	const char *AppConfig::getToolsPath() {
		/*
		if (m_toolsPath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("ToolsPath", m_toolsPath) == false) {
				std::string temp = m_homePath;
				m_toolsPath = temp + TOOLS_PATH;

			}
		}
		*/
		return m_toolsPath.c_str();

	}
	void SIAARCConfig::setHomePath(const char *homePath) {
		AppConfig::m_homePath = homePath;
		ArchivePath::setPathToHome(AppConfig::m_homePath);
		AppConfig::m_configPath = homePath;
		AppConfig::m_configPath += CONFIG_PATH;
	}
	
	const char *AppConfig::getHomePath() {
		
		return m_homePath.c_str();

	}

	const char *AppConfig::getBackup1() {
		/*
		if (m_backup1.empty() == true) {
			if (getRoot().value("BackupOne", m_backup1) == true) {
				ArchivePath::setBackup1Path(m_backup1);
				m_backup1Enabled = true;
			}
		}
		*/
		return m_backup1.c_str();
	}

	const char *AppConfig::getBackup2() {
		/*
		if (m_backup2.empty() == true) {
			if (getRoot().value("BackupTwo", m_backup2) == true) {
				ArchivePath::setBackup2Path(m_backup2);
				m_backup2Enabled = true;
			}
		}
		*/
		return m_backup2.c_str();
	}

	


	/// Gets the archive path.
	const char *AppConfig::getWorkspacePath() {
		
		return m_workspacePath.c_str();

	}
	/// Gets the archive path.
	const char *AppConfig::getMasterPath() {
		/*
		if (m_masterPath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("MasterPath", m_masterPath) == false) {
				// if not found read from SIA_WORKSPACE environment variable
				std::string temp = SAUtils::GetPOSIXEnv("SIA_MASTER");
				if (temp.empty() == false) {
					m_masterPath = temp;
				}
				else {
					std::string tempProgramData = SAUtils::GetPOSIXEnv("ProgramData");
					tempProgramData += DEFAULT_HOME_PATH;
					m_masterPath = tempProgramData + MASTER_PATH;
					ArchivePath::setMasterPath(m_masterPath);
				}
			}
		}
		*/
		return m_masterPath.c_str();

	}
	
	const char *AppConfig::getDerivativePath() {
		/*
		if (m_derivativePath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("DerivativePath", m_derivativePath) == false) {
				// if not found read from SIA_WORKSPACE environment variable
				std::string temp = SAUtils::GetPOSIXEnv("SIA_Master");
				if (temp.empty() == false) {
					m_derivativePath = temp;
				}
				else {
					std::string tempProgramData = SAUtils::GetPOSIXEnv("ProgramData");
					tempProgramData += DEFAULT_HOME_PATH;
					m_derivativePath = tempProgramData + DERIVATIVE_PATH;
					ArchivePath::setDerivativePath(m_derivativePath);
				}
			}
		}
		*/
		return m_derivativePath.c_str();

	}

	
	const char *AppConfig::getSourcePath() {
		/*
		if (m_sourcePath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("SourcePath", m_sourcePath) == false) {
				std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
				std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
				m_sourcePath = tempHomeDrive + tempHomePath + DEFAULT_SOURCE_PATH;
			}
		}
		*/
		return m_sourcePath.c_str();

	}

	const char *AppConfig::getHookPath() {
		/*
		auto folders = getSystemFolders();
		if (folders == nullptr || m_hookPath.empty() == true) {
			if (getRoot().value("HookPath", m_hookPath) == false) {
				std::string temp = SAUtils::GetPOSIXEnv("HOMEPATH");
				m_hookPath = m_homePath + HOOKS_PATH;
			}
		}
		*/
		return m_hookPath.c_str();

	}
	void SIAARCConfig::setArchivePath(const char *path) {
		AppConfig::m_homePath = path;
	}

	void SIAARCConfig::setMasterCataloguePath(const char *path) {
		AppConfig::m_masterCataloguePath = path;
	}

	void SIAARCConfig::setLogLevel(const char *logLevel) {
		AppConfig::m_logLevel = logLevel;
	}

	void SIAARCConfig::setConsoleLevel(const char *logLevel) {
		AppConfig::m_consoleLevel = logLevel;
	}



	void SIAARCConfig::setDerivativePath(const char *path) {
		AppConfig::m_derivativePath = path;
	}

	void SIAARCConfig::setSourcePath(const char *path) {
		AppConfig::m_sourcePath = path;
	}

	void SIAARCConfig::setDryRun(bool dryRun) {
		AppConfig::m_dry_run = dryRun;
	}

	void SIAARCConfig::setQuiet(bool quiet) {
		AppConfig::m_quiet = quiet;
	}

	

	void SIAARCConfig::setSilent(bool silent) {
		AppConfig::m_silent = silent;
	}
	void SIAARCConfig::setVerbose(bool verbose) {
		AppConfig::m_verbose = verbose;
	}

	void SIAARCConfig::setEventsOn(bool evt) {
		AppConfig::m_eventsOn = evt;
	}

	void SIAARCConfig::setEventPort(int port) {
		AppConfig::m_eventsOn = true;
		AppConfig::m_udpPortNum = port;
	}

	void SIAARCConfig::setEventAddress(const char *address) {
		AppConfig::m_eventsOn = true;
		AppConfig::m_udpAddress = address;
	}

	const char *AppConfig::getBackupDestinationPath() {
		/*
		if (m_backupDestinationPath.empty() == true) {
			if (getRoot().value("BackupDestinationPath", m_backupDestinationPath) == false) {
				std::string temp = SAUtils::GetPOSIXEnv("HOMEPATH");
				m_backupDestinationPath = temp + BACKUPS_PATH;
			}
		}
		*/
		return m_backupDestinationPath.c_str();

	}

	const char *AppConfig::getMasterCataloguePath() {
		/*
		if (m_masterViewPath.empty() == true) {
		if (getRoot().value("BackupDestinationPath", m_masterViewPath) == false) {
		std::string temp = SAUtils::GetPOSIXEnv("USERPROFILE");
		m_masterViewPath = temp + MASTER_VIEW_PATH;
		}
		}
		*/
		return m_masterCataloguePath.c_str();

	}

	const char *AppConfig::getMasterWWWCataloguePath() {
		/*
		if (m_masterViewPath.empty() == true) {
			if (getRoot().value("BackupDestinationPath", m_masterViewPath) == false) {
				std::string temp = SAUtils::GetPOSIXEnv("USERPROFILE");
				m_masterViewPath = temp + MASTER_VIEW_PATH;
			}
		}
		*/
		return m_masterWWWCataloguePath.c_str();

	}

	void AppConfig::setBackupDestinationPath(const char *path) {
		m_backupDestinationPath = path;
	}

	long AppConfig::getBackupMediaSize() {
		/*
		if (m_backupMediaSize == 0) {
			std::string tmp;
		
			if (getRoot().value("BackupMediaSize", tmp) == false) {
					m_backupMediaSize = 700; // size of a DVD
			}
			else {
				m_backupMediaSize = strtoul(tmp.c_str(), 0, 10);
			}	
		}
		*/
		return m_backupMediaSize;
	}

	void AppConfig::setBackupMediaSize(const char *sizeStr) {
		m_backupMediaSize = strtoul(sizeStr, 0, 10);
	}

	ExifDateTime &AppConfig::getFromDate() {
		return m_fromDate;
	}

	void AppConfig::setFromDate(const char *dateStr) {
		ExifDateTime tmp(dateStr);
		m_fromDate = tmp;
	}

	ExifDateTime &AppConfig::getToDate() {
		return m_toDate;
	}

	void AppConfig::setToDate(const char *dateStr) {
		ExifDateTime tmp(dateStr);
		m_toDate = tmp;
	}

	bool AppConfig::isFromDateSet() {
		return m_isFromDate;
	}

	bool AppConfig::isToDateSet() {
		return m_isToDate;
	}
	
	const char *AppConfig::getDatabasePath() {
		/*
		if (getRoot().value("DatabasePath", m_DatabasePath) == false) {
			std::string temp = SAUtils::GetPOSIXEnv("HOMEPATH");
			m_DatabasePath = m_homePath + DATABASE_PATH;
		}
		*/
		return m_DatabasePath.c_str();
	}

	const char *AppConfig::getTempPath() {
		/*
		if (getRoot().value("TempPath", m_tempPath) == false) {
			m_tempPath = m_homePath + "/tmp";
		}
		*/
		return m_tempPath.c_str();
	}

	const char *AppConfig::getLogPath() {
		/*
		if (getRoot().value("LogPath", m_logPath) == false) {
			m_logPath = m_homePath + "/logs";
		}
		*/
		return m_logPath.c_str();
	}

	const char *AppConfig::getLogLevel() {
		return m_logLevel.c_str();
	}

	const char *AppConfig::getConsoleLevel() {
		return m_consoleLevel.c_str();
	}

	/*
	const char *AppConfig::getSystemPath() {
		std::shared_ptr<ConfigBlock> cb = find(FOLDERS_BLOCK)->second;
		if (cb == nullptr && cb->value("SystemPath", m_systemPath) == true) {
			return m_systemPath.c_str();
		}
		m_systemPath = m_masterPath + MASTER_SYSTEM_FOLDER;
		
	}
	*/
	const char *AppConfig::getSystemPath() {
		
		return m_systemPath.c_str();
	}


	const char *AppConfig::getIndexPath() {	
		//m_indexPath = m_systemPath + "/index";
		return m_indexPath.c_str();
	}

	const char *AppConfig::getHistoryPath() {
		
		return 	m_historyPath.c_str();
	}

	const char *AppConfig::getConfigPath() {
		/*
		if (getRoot().value("ConfigPath", m_configPath) == false) {
			m_configPath = m_homePath + "/config";
		}
		*/
		return 	m_configPath.c_str();
	}

	const char *AppConfig::getExternalExifTool() {
		/*
		if (getRoot().value("ExifTool", m_ExternalExifTool) == false) {
			m_ExternalExifTool = "exiftool.exe";
			return m_ExternalExifTool.c_str();
		}
		*/
		return 	m_ExternalCommandLine.c_str();
	}

	const char *AppConfig::getExternalCommandLine() {
		/*
		if (getRoot().value("ExifCommandLine", m_ExternalCommandLine) == false) {
			m_ExternalCommandLine = EXTERAL_EXIF_COMMAND_LINE;
			return m_ExternalCommandLine.c_str();
		}
		*/
		return 	m_ExternalCommandLine.c_str();
	}

	const char *AppConfig::getExifMapPath() {
		/*
		if (getRoot().value("ExifMapPath", m_ExternalCommandLine) == false) {
			m_ExifMapPath = m_homePath + CONFIG_PATH;
			return m_ExifMapPath.c_str();
		}
		*/
		return 	m_ExifMapPath.c_str();
	}
	
	const char *AppConfig::getTemplatePath() {
		//if (getRoot().value("TemplatePath", m_templatePath) == false) {
		//	m_templatePath = m_homePath + "/templates";
		//	return m_templatePath.c_str();
		//}
		return 	m_templatePath.c_str();
	}

	const char *AppConfig::getExifMapFile() {
		/*
		if (getRoot().value("ExifMapFile", m_ExifMapFile) == false) {
			return m_ExifMapFile.c_str();
		}
		*/
		return 	m_ExifMapPath.c_str();
	}

	bool AppConfig::isDryRun() const {
		return m_dry_run;
	}

	bool AppConfig::isQuiet() const {
		return m_quiet;
	}

	bool AppConfig::isSilent() const {
		return m_silent;
	}
	
	bool AppConfig::isVerbose() const {
		return m_verbose;
	}

	

	bool AppConfig::isEventsOn() {
		return m_eventsOn;
	}

	int AppConfig::eventPort() {
		return m_udpPortNum;

	}
	const char *AppConfig::eventAddress() {
		return m_udpAddress.c_str();
	}

	bool AppConfig::isServerOn() {
		return m_serverOn;
	}

	int AppConfig::serverPort() {
		return m_tcpPortNum;

	}



	void AppConfig::isServerOn(bool on) {
		m_serverOn = on;
	}

	void AppConfig::setServerPort(int port) {
		m_tcpPortNum = port;
	}

	//void setSourcePath(const char *sourcePath);

	bool AppConfig::validWorkspacePath() {
		return true;
	}
	bool AppConfig::validSourcePath() {
		return true;
	}
	bool AppConfig::validHomePath() {
		return true;
	}

	std::string AppConfig::toString() {
		/// @brief Gets the source path.
		///const char *getSourcePath();

		/// @brief Gets the archive path.
		/// user definable
		std::stringstream str;
		str << "Configuration" << '\n';
		str << "=============" << '\n';
		str << "Workspace path:          " << getWorkspacePath() << '\n';
		/// @brief Gets the Master archive path
		/// user definable
		str << "Master path:             " << getMasterPath() << '\n';
		/// @brief Gets the temp file path.
		str << "Derivative path:         " << getDerivativePath() << '\n';
		/// @brief Gets the temp file path.
		/// user definable
		str << "Temp path:               " << getTempPath() << '\n';
		/// @brief Gets tools path
		str << "Tools path:              " << getToolsPath() << '\n';
		/// @brief Gets the hooks path
		/// user definable
		/// 
		str << "Hook path:               " << getHookPath()  << '\n';
		/// @brief Gets the path to the template files.
		str << "Metadata template path:  " << getTemplatePath() << '\n';
		/// @brief Gets log file path
		str << "Log path:                " << getLogPath() << '\n';
		/// @brief Gets the path to the crc index database.
		str << "Index path:              " << getIndexPath() << '\n';
		/// @brief Gets the path the history file are stored. These files are used for
		/// @brief the history of the changes made to the archive.
		str << "History path:            " << getHistoryPath() << '\n';
		/// @brief gets external Command line
		str << "Exif tool command        " << getExternalCommandLine() << '\n';
		/// @brief Gets path to the Exif Map files. For example the Exiftool map
		/// that maps exiftool keywords to Simple Archive keywords.
		str << "Exif map path:           " << getExifMapPath() << '\n';
		str << "Config path:             " << getConfigPath() << '\n';
		/// @brief Gets home path. This is the root path all default paths are made.
		str << "Home path:               " << getHomePath() << '\n';
		str << "Master view path:        " << getMasterCataloguePath() << '\n';
		str << "Database path:           " << getDatabasePath() << '\n';
		str << "Backup destination path: " << getBackupDestinationPath() << '\n';
		str << "Backup media size:       " << getBackupMediaSize() << '\n';
		if (isFromDateSet() == true) {
			str << "From date:               " << getFromDate().toString() << '\n';
		}
		if (isToDateSet() == true) {
			str << "To date:                 " << getToDate().toString() << '\n';
		}
		str << "Is dryrun:               " << (isDryRun()?"true":"false") << '\n';
		str << "Is quiet:                " << (isQuiet() ? "true" : "false") << '\n';
		str << "Is verbose:              " << (isVerbose() ? "true" : "false") << '\n';
		str << "Log level:               " << getLogLevel() << '\n';

		return str.str();
	}

	std::string AppConfig::toXMLString() {
		/// @brief Gets the source path.
		///const char *getSourcePath();

		/// @brief Gets the archive path.
		/// user definable
		std::stringstream str;
		str << "<Configuration>" << '\n';
		str << "<WorkspacePath>" << getWorkspacePath() << "</WorkspacePath>" << '\n';
		/// @brief Gets the Master archive path
		/// user definable
		str << "<MasterPath>" << getMasterPath() << "</MasterPath>" << '\n';
		/// @brief Gets the Master archive path
		/// user definable
		str << "<DerivativePath>" << this->getDerivativePath() << "</DerivativePath>" << '\n';
		/// @brief Gets the temp file path.
		/// user definable
		str << "<TempPath>" << getTempPath() << "</TempPath>" << '\n';
		/// @brief Gets tools path
		str << "<ToolsPath>" << getToolsPath() << "</ToolsPath>" << '\n';
		/// @brief Gets the hooks path
		/// user definable
		/// 
		str << "<HookPath>" << getHookPath() << "</HookPath>" << '\n';
		/// @brief Gets the path to the metadata template files.
		str << "<TemplatePath>" << getTemplatePath() << "</TemplatePath>" << '\n';
		/// @brief Gets log file path
		str << "<LogPath>" << getLogPath() << "</LogPath>" << '\n';
		/// @brief Gets the path to the crc index database.
		str << "<IndexPath>" << getIndexPath() << "</IndexPath>" << '\n';
		/// @brief Gets the path the history file are stored. These files are used for
		/// @brief the history of the changes made to the archive.
		str << "<HistoryPath>" << getHistoryPath() << "</HistoryPath>" << '\n';
		/// @brief gets external Command line
		str << "<ExternalCommandLine>" << getExternalCommandLine() << "</ExternalCommandLine>" << '\n';
		/// @brief Gets path to the Exif Map files. For example the Exiftool map
		/// that maps exiftool keywords to Simple Archive keywords.
		str << "<ExifMapPath>" << getExifMapPath() << "</ExifMapPath>" << '\n';
		str << "<ConfigPath>" << getConfigPath() << "</ConfigPath>" << '\n';
		/// @brief Gets home path. This is the root path all default paths are made.
		str << "<HomePath>" << getHomePath() << "</HomePath>" << '\n';
		str << "<MasterCataloguePath>" << getMasterCataloguePath() << "</MasterCataloguePath>" << '\n';
		str << "<DatabasePath>" << getDatabasePath() << "</DatabasePath>" << '\n';
		str << "<BackupDestinationPath>" << getBackupDestinationPath() << "</BackupDestinationPath>" << '\n';
		str << "<BackupMediaSize>" << getBackupMediaSize() << "</BackupMediaSize>" << '\n';
		if (isFromDateSet() == true) {
			str << "<FromDate>" << getFromDate().toString() << "</FromDate>" << '\n';
		}
		if (isToDateSet() == true) {
			str << "<FromDate>" << getToDate().toString() << "</FromDate>" << '\n';
		}
		str << "<IsDryRun>" << (isDryRun() ? "true" : "false") << "</IsDryRun>" << '\n';
		str << "<IsQuiet>" << (isQuiet() ? "true" : "false") << "</IsQuiet>" << '\n';
		str << "<IsVerbose>" << (isVerbose() ? "true" : "false") << "</IsVerbose>" << '\n';
		str << "<LogLevel>" << getLogLevel() << "</LogLevel>" << '\n';
		str << "</Configuration>" << '\n';
		return str.str();
	}

}
