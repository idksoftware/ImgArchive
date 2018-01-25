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

namespace simplearchive {

	CSIAArcAppConfig *CSIAArcAppConfig::m_this = NULL;
	
	bool CSIAArcAppConfig::m_verbose = false;
	bool CSIAArcAppConfig::m_quiet = true;
	bool CSIAArcAppConfig::m_silent = false;
	std::string CSIAArcAppConfig::m_logLevel = "SUMMARY";
	bool CSIAArcAppConfig::m_dry_run = false;
	bool CSIAArcAppConfig::m_useDatabase = true;

	bool CSIAArcAppConfig::m_eventsOn = false; // UDP events
	bool CSIAArcAppConfig::m_serverOn = false;

	int CSIAArcAppConfig::m_tcpPortNum = 11000;
	int CSIAArcAppConfig::m_udpPortNum = 11001;
	std::string CSIAArcAppConfig::m_udpAddress = "127.0.0.1";

	std::string CSIAArcAppConfig::m_hookPath;
	std::string CSIAArcAppConfig::m_toolsPath;
	std::string CSIAArcAppConfig::m_workspacePath;
	std::string CSIAArcAppConfig::m_derivativePath;
	std::string CSIAArcAppConfig::m_masterPath;
	std::string CSIAArcAppConfig::m_sourcePath;
	std::string CSIAArcAppConfig::m_catalogPath;
	std::string CSIAArcAppConfig::m_configPath;
	std::string CSIAArcAppConfig::m_tempPath;
	std::string CSIAArcAppConfig::m_logPath;
	std::string CSIAArcAppConfig::m_consoleLevel;
	std::string CSIAArcAppConfig::m_homePath;
	std::string CSIAArcAppConfig::m_systemPath;
	std::string CSIAArcAppConfig::m_indexPath;
	std::string CSIAArcAppConfig::m_historyPath;
	std::string CSIAArcAppConfig::m_ExternalExifTool;
	std::string CSIAArcAppConfig::m_ExternalCommandLine;
	std::string CSIAArcAppConfig::m_ExifMapPath;
	std::string CSIAArcAppConfig::m_ExifMapFile;
	std::string CSIAArcAppConfig::m_templatePath;
	std::string CSIAArcAppConfig::m_backupDestinationPath;
	
	std::string CSIAArcAppConfig::m_DatabasePath;
	std::string CSIAArcAppConfig::m_backup1;
	std::string CSIAArcAppConfig::m_backup2;

	std::string CSIAArcAppConfig::m_masterCataloguePath;
	std::string CSIAArcAppConfig::m_masterWWWCataloguePath;
	bool CSIAArcAppConfig::m_masterViewEnabled = true;
	bool CSIAArcAppConfig::m_masterViewFullSizeOn = true;
	bool CSIAArcAppConfig::m_masterViewPreview1On = true;
	bool CSIAArcAppConfig::m_masterViewThumbnailOn = true;

	bool CSIAArcAppConfig::m_backup1Enabled = false;
	bool CSIAArcAppConfig::m_backup2Enabled = false;

	long CSIAArcAppConfig::m_backupMediaSize;
	ExifDateTime CSIAArcAppConfig::m_fromDate;
	ExifDateTime CSIAArcAppConfig::m_toDate;
	bool CSIAArcAppConfig::m_isFromDate = false;
	bool CSIAArcAppConfig::m_isToDate = false;

	

	CSIAArcAppConfig::CSIAArcAppConfig()
	{
	
	}


	CSIAArcAppConfig::~CSIAArcAppConfig()
	{
	}

	CSIAArcAppConfig &CSIAArcAppConfig::get() {
		if (m_this == NULL) {
			m_this = new CSIAArcAppConfig;
		}
		return *m_this;
	}

	

	std::shared_ptr<ConfigBlock> CSIAArcAppConfig::getMasterArchive() {
		return getConfigBlocks(MASTER_ARCHIVE);
	}

	std::shared_ptr<ConfigBlock> CSIAArcAppConfig::getSystemFolders() {
		return getConfigBlocks(SYSTEM_FOLDERS);
	}

	/*
		Default paths based on UserDrive and UserHome

	*/
	bool CSIAArcAppConfig::init(const char *homePath) {
		CLogger &logger = CLogger::getLogger();
		
		logger.log(LOG_OK, CLogger::Level::INFO, "Initalising configuration");
		// Home Path 
		if (homePath != nullptr) {
			m_homePath = homePath;
		} else if (m_homePath.empty() == true) { // use ProgramData default
			std::string tempProgramData = SAUtils::GetPOSIXEnv("ProgramData");
			m_homePath = tempProgramData + DEFAULT_HOME_PATH;
			if (SAUtils::DirExists(m_homePath.c_str()) == false) {
				m_homePath = SAUtils::GetPOSIXEnv("HOMEDRIVE");
				m_homePath += SAUtils::GetPOSIXEnv("HOMEPATH");
				m_homePath += DEFAULT_HOME_PATH;
				if (SAUtils::DirExists(m_homePath.c_str()) == false) {
					return false;
				}
			}
			
		}
		ArchivePath::setPathToHome(m_homePath);
		logger.log(LOG_OK, CLogger::Level::INFO, "Home path                 \"%s\"", m_homePath.c_str());

		// Backup 1
		if (m_backup1.empty() == true) {
			if (getRoot().value("BackupOne", m_backup1) == true) {
				ArchivePath::setBackup1Path(m_backup1);
				m_backup1Enabled = true;
			}
		}
		else {
			ArchivePath::setBackup1Path(m_backup1);
		}
		
		// Backup 2
		if (m_backup2.empty() == true) {
			if (getRoot().value("BackupTwo", m_backup2) == true) {
				ArchivePath::setBackup2Path(m_backup2);
				m_backup2Enabled = true;
			}
		}
		else {
			ArchivePath::setBackup2Path(m_backup2);
		}

		// Repository Archive Path
		if (m_masterPath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("MasterPath", m_masterPath) == false) {
				std::string temp = m_homePath;
				m_masterPath = temp + MASTER_PATH;
				
			}
		}
		ArchivePath::setMasterPath(m_masterPath);
		logger.log(LOG_OK, CLogger::Level::INFO, "Master path               \"%s\"", m_masterPath.c_str());

		// Repository Archive Path
		if (m_derivativePath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("DerivativePath", m_derivativePath) == false) {
				std::string temp = m_homePath;
				m_derivativePath = temp + DERIVATIVE_PATH;
				
			}
		}
		ArchivePath::setDerivativePath(m_derivativePath);
		logger.log(LOG_OK, CLogger::Level::INFO, "Derivative path           \"%s\"", m_derivativePath.c_str());

		// History Path
		if (m_historyPath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("HistoryPath", m_historyPath) == false) {
				std::string temp = SAUtils::GetPOSIXEnv("HOMEPATH");
				m_historyPath = m_homePath + HISTORY_PATH;
				
			}
		}
		ArchivePath::setMainHistory(m_historyPath);
		logger.log(LOG_OK, CLogger::Level::INFO, "History path              \"%s\"", m_historyPath.c_str());

		// m_masterViewPath
		if (m_masterWWWCataloguePath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("MasterWWWCataloguePath", m_masterWWWCataloguePath) == false) {
				std::string temp = SAUtils::GetPOSIXEnv("HOMEPATH");
				m_masterWWWCataloguePath = m_homePath + MASTER_WWW_CATALOGUE_PATH;

			}
		}
		logger.log(LOG_OK, CLogger::Level::INFO, "Master Web catalogue path \"%s\"", m_masterWWWCataloguePath.c_str());

		if (m_masterCataloguePath.empty() == true) {
			// read from config file
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("MasterCataloguePath", m_masterCataloguePath) == false) {
				// if not found read from SIA_WORKSPACE environment variable
				std::string temp = SAUtils::GetPOSIXEnv("SIA_MASTER_CATALOGUE");
				if (temp.empty() == false) {
					m_masterCataloguePath = temp;
				}
				else {
					std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
					std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
					m_masterCataloguePath = tempHomeDrive + tempHomePath + DEFAULT_MASTER_CATALOGUE_PATH;
				}
			}
		}
		//ArchivePath::setMasterCataloguePath(m_masterViewPath);
		logger.log(LOG_OK, CLogger::Level::INFO, "Master catalogue path     \"%s\"", m_masterCataloguePath.c_str());

		// Workspace Path
		// This is used in siaadm.exe
		if (m_workspacePath.empty() == true) {
			// read from config file
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("WorkspacePath", m_workspacePath) == false) {
				// if not found read from SIA_WORKSPACE environment variable
				std::string temp = SAUtils::GetPOSIXEnv("SIA_WORKSPACE");
				if (temp.empty() == false) {
					m_workspacePath = temp;
				}
				else {
					std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
					std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
					m_workspacePath = tempHomeDrive + tempHomePath + DEFAULT_WORKSPACE_PATH;
				}
			}
		}
		ArchivePath::setPathToWorkspace(m_workspacePath);
		logger.log(LOG_OK, CLogger::Level::INFO, "Workspace path            \"%s\"", m_workspacePath.c_str());
		
		std::string temp = SAUtils::GetPOSIXEnv("SIA_SOURCE");
		if (temp.empty() == false) {
			setSourcePath(temp.c_str());
		}
		temp = SAUtils::GetPOSIXEnv("SIA_LOGLEVEL");
		if (temp.empty() == false) {
			setLogLevel(temp.c_str());
		}
		return true;
	}

	void CSIAArcAppConfig::settup() {
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


	
	void CSIAArcAppConfig::fileBasedValues(const char *home) {
		CLogger &logger = CLogger::getLogger();
		logger.log(LOG_OK, CLogger::Level::INFO, "Reading configuration file ");
		// Home Path (The path to this file will be based on the home path)
		std::string homePath = home;
		// Backup 1
		if (getRoot().value("BackupOne", m_backup1) == true) {
			ArchivePath::setBackup1Path(m_backup1);
			m_backup1Enabled = true;
		}
		// Backup 2
		if (getRoot().value("BackupTwo", m_backup2) == true) {
			ArchivePath::setBackup2Path(m_backup2);
			m_backup2Enabled = true;
		}
		

		// Master Archive Path
		//std::shared_ptr<ConfigBlock> folders = getSystemFolders();
		//if (folders == nullptr || getRoot().value("MasterPath", m_masterPath) == true) {
		//	ArchivePath::setMasterPath(m_masterPath);
		//}
		//
//#define CONFIG_PATH_LABEL				"ConfigPath"	 
//#define TOOLS_PATH_LABEL           		"ToolsPath"
		
		setSystemFolders(TEMP_PATH_LABEL, m_tempPath, homePath + TEMP_PATH);
//#define SOURCE_PATH_LABEL         		"SourcePath"
		setSystemFolders(SYSTEM_PATH_LABEL, m_systemPath, homePath + SYSTEM_PATH);
		setSystemFolders(LOG_PATH_LABEL, m_logPath, homePath + MASTER_PATH);
		setSystemFolders(MASTER_PATH_LABEL, m_masterPath, homePath + MASTER_PATH);
		setSystemFolders(DERIVATIVE_PATH_LABEL, m_derivativePath, homePath + DERIVATIVE_PATH);
		setSystemFolders(TOOLS_PATH_LABEL, m_toolsPath, homePath + TOOLS_PATH);
		setSystemFolders(HOOK_SCRIPTS_PATH_LABEL, m_hookPath, homePath + HOOKS_PATH);
//#define HOME_PATH_LABEL					"HomePath"

		setSystemFolders(HISTORY_PATH_LABEL, m_historyPath, homePath + TOOLS_PATH);	
		setSystemFolders(TEMPLATE_PATH_LABEL, m_templatePath, homePath + TEMPLATE_PATH);
		setSystemFolders(CATALOG_PATH_LABEL, m_catalogPath, homePath + TOOLS_PATH);
		
		std::string emptyString;
		setSystemFolders(EXIF_MAP_PATH_LABEL, m_ExifMapPath, homePath + TOOLS_PATH);
		setConfigBlock(EXIF_MAP_FILE_LABEL, m_ExifMapFile, emptyString, EXIFTOOL_BLOCK);


		// Workspace Path	
		std::string wtemp = SAUtils::GetPOSIXEnv("SIA_WORKSPACE");
		if (wtemp.empty() == true) {
			std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
			std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
			wtemp = tempHomeDrive + tempHomePath + DEFAULT_WORKSPACE_PATH;
		}
		
		setSystemFolders(WORKSPACE_PATH_LABEL, m_workspacePath, wtemp);
		ArchivePath::setPathToWorkspace(m_workspacePath);

		// Workspace Path	
		std::string ctemp = SAUtils::GetPOSIXEnv("SIA_MASTER_CATALOGUE");
		if (ctemp.empty() == true) {
			std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
			std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
			ctemp = tempHomeDrive + tempHomePath + DEFAULT_MASTER_CATALOGUE_PATH;
		}
		setSystemFolders(MASTER_VIEW_PATH_LABEL, m_masterCataloguePath, ctemp);
		
		logger.log(LOG_OK, CLogger::Level::INFO, "    System paths");
		logger.log(LOG_OK, CLogger::Level::INFO, "        System path \"%s\"", m_systemPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Log path \"%s\"", m_logPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Master path \"%s\"", m_masterPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Derivetive path \"%s\"", m_derivativePath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Tools path \"%s\"", m_toolsPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Hook path \"%s\"", m_hookPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        History path \"%s\"", m_historyPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Template path \"%s\"", m_templatePath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Catalog path \"%s\"", m_catalogPath.c_str());
		
		logger.log(LOG_OK, CLogger::Level::INFO, "    External Exif Tool");
		logger.log(LOG_OK, CLogger::Level::INFO, "        Exif map path \"%s\"", m_ExifMapPath.c_str());
		logger.log(LOG_OK, CLogger::Level::INFO, "        Exif map file \"%s\"", m_ExifMapFile.c_str());

		std::string dry_run = "false";
		setGeneral(DRY_RUN_LABEL, dry_run, dry_run);
		
		if (stricmp(dry_run.c_str(), "true") == 0)
		{
			m_dry_run = true;
		}
		else {
			m_dry_run = false;
		}
	}

	void CSIAArcAppConfig::setToolsPath(const char *toolsPath) {
		m_toolsPath = toolsPath;
	}
	const char *CSIAArcAppConfig::getToolsPath() {
		if (m_toolsPath.empty() == true) {
			auto folders = getSystemFolders();
			if (folders == nullptr || getRoot().value("ToolsPath", m_toolsPath) == false) {
				std::string temp = m_homePath;
				m_toolsPath = temp + TOOLS_PATH;

			}
		}
		return m_toolsPath.c_str();

	}
	void CSIAArcAppConfig::setHomePath(const char *homePath) {
		m_homePath = homePath;
		ArchivePath::setPathToHome(m_homePath);
		m_configPath = homePath;
		m_configPath += CONFIG_PATH;
	}
	
	const char *CSIAArcAppConfig::getHomePath() {
		
		return m_homePath.c_str();

	}

	const char *CSIAArcAppConfig::getBackup1() {
		if (m_backup1.empty() == true) {
			if (getRoot().value("BackupOne", m_backup1) == true) {
				ArchivePath::setBackup1Path(m_backup1);
				m_backup1Enabled = true;
			}
		}
		return m_backup1.c_str();
	}

	const char *CSIAArcAppConfig::getBackup2() {
		if (m_backup2.empty() == true) {
			if (getRoot().value("BackupTwo", m_backup2) == true) {
				ArchivePath::setBackup2Path(m_backup2);
				m_backup2Enabled = true;
			}
		}
		return m_backup2.c_str();
	}

	


	/// Gets the archive path.
	const char *CSIAArcAppConfig::getWorkspacePath() {
		
		return m_workspacePath.c_str();

	}
	/// Gets the archive path.
	const char *CSIAArcAppConfig::getMasterPath() {
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
	
	const char *CSIAArcAppConfig::getDerivativePath() {
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

	
	const char *CSIAArcAppConfig::getSourcePath() {
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

	const char *CSIAArcAppConfig::getHookPath() {
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
	void CSIAArcAppConfig::setArchivePath(const char *path) {
		m_homePath = path;
	}

	void CSIAArcAppConfig::setMasterCataloguePath(const char *path) {
		m_masterCataloguePath = path;
	}

	void CSIAArcAppConfig::setWorkspacePath(const char *path) {
		m_workspacePath = path;
		ArchivePath::setPathToWorkspace(m_workspacePath);
	}

	void CSIAArcAppConfig::setMasterPath(const char *path) {
		m_masterPath = path;
		ArchivePath::setMasterPath(m_masterPath);
	}


	void CSIAArcAppConfig::setDerivativePath(const char *path) {
		m_derivativePath = path;
	}

	void CSIAArcAppConfig::setSourcePath(const char *path) {
		m_sourcePath = path;
	}

	const char *CSIAArcAppConfig::getBackupDestinationPath() {
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

	const char *CSIAArcAppConfig::getMasterCataloguePath() {
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

	const char *CSIAArcAppConfig::getMasterWWWCataloguePath() {
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

	void CSIAArcAppConfig::setBackupDestinationPath(const char *path) {
		m_backupDestinationPath = path;
	}

	long CSIAArcAppConfig::getBackupMediaSize() {
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

	void CSIAArcAppConfig::setBackupMediaSize(const char *sizeStr) {
		m_backupMediaSize = strtoul(sizeStr, 0, 10);
	}

	ExifDateTime &CSIAArcAppConfig::getFromDate() {
		return m_fromDate;
	}

	void CSIAArcAppConfig::setFromDate(const char *dateStr) {
		ExifDateTime tmp(dateStr);
		m_fromDate = tmp;
	}

	ExifDateTime &CSIAArcAppConfig::getToDate() {
		return m_toDate;
	}

	void CSIAArcAppConfig::setToDate(const char *dateStr) {
		ExifDateTime tmp(dateStr);
		m_toDate = tmp;
	}

	bool CSIAArcAppConfig::isFromDateSet() {
		return m_isFromDate;
	}

	bool CSIAArcAppConfig::isToDateSet() {
		return m_isToDate;
	}
	
	const char *CSIAArcAppConfig::getDatabasePath() {
		if (getRoot().value("DatabasePath", m_DatabasePath) == false) {
			std::string temp = SAUtils::GetPOSIXEnv("HOMEPATH");
			m_DatabasePath = m_homePath + DATABASE_PATH;
		}
		return m_DatabasePath.c_str();
	}

	const char *CSIAArcAppConfig::getTempPath() {
		/*
		if (getRoot().value("TempPath", m_tempPath) == false) {
			m_tempPath = m_homePath + "/tmp";
		}
		*/
		return m_tempPath.c_str();
	}

	const char *CSIAArcAppConfig::getLogPath() {
		if (getRoot().value("LogPath", m_logPath) == false) {
			m_logPath = m_homePath + "/logs";
		}
		return m_logPath.c_str();
	}

	const char *CSIAArcAppConfig::getLogLevel() {
		return m_logLevel.c_str();
	}

	void CSIAArcAppConfig::setLogLevel(const char *logLevel) {
		m_logLevel = logLevel;
	}

	const char *CSIAArcAppConfig::getConsoleLevel() {
		return m_consoleLevel.c_str();
	}

	void CSIAArcAppConfig::setConsoleLevel(const char *logLevel) {
		m_consoleLevel = logLevel;
	}
	
	bool CSIAArcAppConfig::setGeneral(const char* name, std::string &value, std::string &defaultValue) {
		return setConfigBlock(name, value, defaultValue, GENERAL_BLOCK);
	}

	bool CSIAArcAppConfig::setSystemFolders(const char* name, std::string &value, std::string &defaultValue) {
		return setConfigBlock(name, value, defaultValue, FOLDERS_BLOCK);
	}

	bool CSIAArcAppConfig::setExternalExifTool(const char* name, std::string &value, std::string &defaultValue) {
		return setConfigBlock(name, value, defaultValue, EXIFTOOL_BLOCK);
	}

	/*
	const char *CSIAArcAppConfig::getSystemPath() {
		std::shared_ptr<ConfigBlock> cb = find(FOLDERS_BLOCK)->second;
		if (cb == nullptr && cb->value("SystemPath", m_systemPath) == true) {
			return m_systemPath.c_str();
		}
		m_systemPath = m_masterPath + MASTER_SYSTEM_FOLDER;
		
	}
	*/
	const char *CSIAArcAppConfig::getSystemPath() {
		setSystemFolders("SystemPath", m_systemPath, m_masterPath + SYSTEM_PATH);
		return m_systemPath.c_str();
	}


	const char *CSIAArcAppConfig::getIndexPath() {	
		m_indexPath = m_systemPath + "/index";
		return m_indexPath.c_str();
	}

	const char *CSIAArcAppConfig::getHistoryPath() {
		if (m_historyPath.empty() == true) {
			if (getRoot().value("HistoryPath", m_logPath) == false) {
				std::string temp = SAUtils::GetEnv("HOMEPATH");
				m_historyPath = m_homePath + HISTORY_PATH;
				ArchivePath::setMainHistory(m_historyPath);
			}
		}
		return 	m_historyPath.c_str();
	}

	const char *CSIAArcAppConfig::getConfigPath() {
		if (getRoot().value("ConfigPath", m_configPath) == false) {
			m_configPath = m_homePath + "/config";
		}
		return 	m_configPath.c_str();
	}

	const char *CSIAArcAppConfig::getExternalExifTool() {
		if (getRoot().value("ExifTool", m_ExternalExifTool) == false) {
			m_ExternalExifTool = "exiftool.exe";
			return m_ExternalExifTool.c_str();
		}
		return 	m_ExternalCommandLine.c_str();
	}

	const char *CSIAArcAppConfig::getExternalCommandLine() {
		if (getRoot().value("ExifCommandLine", m_ExternalCommandLine) == false) {
			m_ExternalCommandLine = EXTERAL_EXIF_COMMAND_LINE;
			return m_ExternalCommandLine.c_str();
		}
		return 	m_ExternalCommandLine.c_str();
	}

	const char *CSIAArcAppConfig::getExifMapPath() {
		if (getRoot().value("ExifMapPath", m_ExternalCommandLine) == false) {
			m_ExifMapPath = m_homePath + CONFIG_PATH;
			return m_ExifMapPath.c_str();
		}
		return 	m_ExifMapPath.c_str();
	}
	
	const char *CSIAArcAppConfig::getTemplatePath() {
		//if (getRoot().value("TemplatePath", m_templatePath) == false) {
		//	m_templatePath = m_homePath + "/templates";
		//	return m_templatePath.c_str();
		//}
		return 	m_templatePath.c_str();
	}

	const char *CSIAArcAppConfig::getExifMapFile() {
		if (getRoot().value("ExifMapFile", m_ExifMapFile) == false) {
			return m_ExifMapFile.c_str();
		}
		return 	m_ExifMapPath.c_str();
	}

	bool CSIAArcAppConfig::isDryRun() const {
		return m_dry_run;
	}

	void CSIAArcAppConfig::setDryRun(bool dryRun) {
		m_dry_run = dryRun;
	}


	bool CSIAArcAppConfig::isQuiet() const {
		return m_quiet;
	}

	void CSIAArcAppConfig::setQuiet(bool quiet) {
		m_quiet = quiet;
	}

	bool CSIAArcAppConfig::isSilent() const {
		return m_silent;
	}

	void CSIAArcAppConfig::setSilent(bool silent) {
		m_silent = silent;
	}

	bool CSIAArcAppConfig::isVerbose() const {
		return m_verbose;
	}

	void CSIAArcAppConfig::setVerbose(bool verbose) {
		m_verbose = verbose;
	}

	bool CSIAArcAppConfig::isEventsOn() {
		return m_eventsOn;
	}

	int CSIAArcAppConfig::eventPort() {
		return m_udpPortNum;

	}
	const char *CSIAArcAppConfig::eventAddress() {
		return m_udpAddress.c_str();
	}

	bool CSIAArcAppConfig::isServerOn() {
		return m_serverOn;
	}

	int CSIAArcAppConfig::serverPort() {
		return m_tcpPortNum;

	}

	void CSIAArcAppConfig::setEventsOn(bool evt) {
		m_eventsOn = evt;
	}

	void CSIAArcAppConfig::setEventPort(int port) {
		m_eventsOn = true;
		m_udpPortNum = port;
	}

	void CSIAArcAppConfig::setEventAddress(const char *address) {
		m_eventsOn = true;
		m_udpAddress = address;
	}

	void CSIAArcAppConfig::isServerOn(bool on) {
		m_serverOn = on;
	}

	void CSIAArcAppConfig::setServerPort(int port) {
		m_tcpPortNum = port;
	}

	//void setSourcePath(const char *sourcePath);

	bool CSIAArcAppConfig::validWorkspacePath() {
		return true;
	}
	bool CSIAArcAppConfig::validSourcePath() {
		return true;
	}
	bool CSIAArcAppConfig::validHomePath() {
		return true;
	}

	std::string CSIAArcAppConfig::toString() {
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

	std::string CSIAArcAppConfig::toXMLString() {
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
