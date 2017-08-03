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

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	CAppConfig *CAppConfig::m_this = NULL;
	
	bool CAppConfig::m_verbose = false;
	bool CAppConfig::m_quiet = true;
	bool CAppConfig::m_silent = false;
	std::string CAppConfig::m_logLevel = "SUMMARY";
	bool CAppConfig::m_dry_run = false;
	bool CAppConfig::m_useDatabase = true;

	bool CAppConfig::m_eventsOn = false; // UDP events
	bool CAppConfig::m_serverOn = false;

	int CAppConfig::m_tcpPortNum = 11000;
	int CAppConfig::m_udpPortNum = 11001;
	std::string CAppConfig::m_udpAddress = "127.0.0.1";

	std::string CAppConfig::m_hookPath;
	std::string CAppConfig::m_toolsPath;
	std::string CAppConfig::m_workspacePath;
	std::string CAppConfig::m_derivativePath;
	std::string CAppConfig::m_masterPath;
	std::string CAppConfig::m_sourcePath;
	std::string CAppConfig::m_configPath;
	std::string CAppConfig::m_tempPath;
	std::string CAppConfig::m_logPath;
	std::string CAppConfig::m_homePath;
	std::string CAppConfig::m_systemPath;
	std::string CAppConfig::m_indexPath;
	std::string CAppConfig::m_historyPath;
	std::string CAppConfig::m_ExternalExifTool;
	std::string CAppConfig::m_ExternalCommandLine;
	std::string CAppConfig::m_ExifMapPath;
	std::string CAppConfig::m_MetadataTemplatePath;
	std::string CAppConfig::m_backupDestinationPath;
	std::string CAppConfig::m_masterViewPath;
	std::string CAppConfig::m_DatabasePath;
	std::string CAppConfig::m_backup1;
	std::string CAppConfig::m_backup2;

	bool CAppConfig::m_backup1Enabled = false;
	bool CAppConfig::m_backup2Enabled = false;

	long CAppConfig::m_backupMediaSize;
	ExifDateTime CAppConfig::m_fromDate;
	ExifDateTime CAppConfig::m_toDate;
	bool CAppConfig::m_isFromDate = false;
	bool CAppConfig::m_isToDate = false;

	

	CAppConfig::CAppConfig()
	{
	
	}


	CAppConfig::~CAppConfig()
	{
	}

	CAppConfig &CAppConfig::get() {
		if (m_this == NULL) {
			m_this = new CAppConfig;
		}
		return *m_this;
	}

	/*
		Default paths based on UserDrive and UserHome

	*/
	void CAppConfig::init(const char *homePath) {

		// Home Path 
		if (homePath != nullptr) {
			m_homePath = homePath;
		} else if (m_homePath.empty() == true) {
			if (value("HomePath", m_homePath) == false) {
				m_homePath = SAUtils::GetPOSIXEnv("SIA_HOME");
				
			}
			else {
				std::string tempProgramData = SAUtils::GetPOSIXEnv("ProgramData");
				m_homePath = tempProgramData + DEFAULT_HOME_PATH;
				if (SAUtils::DirExists(m_homePath.c_str()) == false) {
					m_homePath = SAUtils::GetPOSIXEnv("HOMEDRIVE");
					m_homePath += SAUtils::GetPOSIXEnv("HOMEPATH");
					m_homePath += DEFAULT_HOME_PATH;
				}
			}
		}
		ArchivePath::setPathToHome(m_homePath);

		// Backup 1
		if (m_backup1.empty() == true) {
			if (value("BackupOne", m_backup1) == true) {
				ArchivePath::setBackup1Path(m_backup1);
				m_backup1Enabled = true;
			}
		}
		else {
			ArchivePath::setBackup1Path(m_backup1);
		}
		
		// Backup 2
		if (m_backup2.empty() == true) {
			if (value("BackupTwo", m_backup2) == true) {
				ArchivePath::setBackup2Path(m_backup2);
				m_backup2Enabled = true;
			}
		}
		else {
			ArchivePath::setBackup2Path(m_backup2);
		}

		// Repository Archive Path
		if (m_masterPath.empty() == true) {
			if (value("RepositoryPath", m_masterPath) == false) {
				std::string temp = m_homePath;
				m_masterPath = temp + MASTER_PATH;
				ArchivePath::setMasterPath(m_masterPath);
			}
		}
		ArchivePath::setMasterPath(m_masterPath);

		// History Path
		if (m_historyPath.empty() == true) {
			if (value("HistoryPath", m_logPath) == false) {
				std::string temp = SAUtils::GetPOSIXEnv("HOMEPATH");
				m_historyPath = m_homePath + HISTORY_PATH;
				
			}
		}
		ArchivePath::setMainHistory(m_historyPath);

		// Workspace Path
		if (m_workspacePath.empty() == true) {
			// read from config file
			if (value("WorkspacePath", m_workspacePath) == false) {
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

		
		std::string temp = SAUtils::GetPOSIXEnv("SIA_SOURCE");
		if (temp.empty() == false) {
			setSourcePath(temp.c_str());
		}
		temp = SAUtils::GetPOSIXEnv("SIA_LOGLEVEL");
		if (temp.empty() == false) {
			setLogLevel(temp.c_str());
		}
		
	}

	void CAppConfig::settup() {
		getWorkspacePath();
		getMasterPath();
		getDerivativePath();
		getTempPath();
		getToolsPath();
		getHookPath();
		getMetadataTemplatePath();
		getLogPath();
		getIndexPath();
		getHistoryPath();
		getExternalCommandLine();
		getExifMapPath();
		getConfigPath();
	    getHomePath();
		getMasterViewPath();
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


	/*
		File values override the default and envroment 
	*/
	void CAppConfig::fileBasedValues() {

		// Home Path (The path to this file will be based on the home path)
		
		// Backup 1
		if (value("BackupOne", m_backup1) == true) {
			ArchivePath::setBackup1Path(m_backup1);
			m_backup1Enabled = true;
		}
		// Backup 2
		if (value("BackupTwo", m_backup2) == true) {
			ArchivePath::setBackup2Path(m_backup2);
			m_backup2Enabled = true;
		}
		

		// Master Archive Path
		if (value("MasterPath", m_masterPath) == true) {
			ArchivePath::setMasterPath(m_masterPath);
		}
		

		// History Path
		if (value("HistoryPath", m_logPath) == true) {
			ArchivePath::setMainHistory(m_logPath);
		}
		
		// Workspace Path	
		if (value("WorkspacePath", m_workspacePath) == true) {
				ArchivePath::setPathToWorkspace(m_workspacePath);
		}
		
	}

	void CAppConfig::setToolsPath(const char *toolsPath) {
		m_toolsPath = toolsPath;
	}
	const char *CAppConfig::getToolsPath() {
		if (m_toolsPath.empty() == true) {
			if (value("ToolsPath", m_toolsPath) == false) {
				std::string temp = m_homePath;
				m_toolsPath = temp + TOOLS_PATH;

			}
		}
		return m_toolsPath.c_str();

	}
	void CAppConfig::setHomePath(const char *homePath) {
		m_homePath = homePath;
		ArchivePath::setPathToHome(m_homePath);
		m_configPath = homePath;
		m_configPath += CONFIG_PATH;
	}
	
	const char *CAppConfig::getHomePath() {
		
		return m_homePath.c_str();

	}

	const char *CAppConfig::getBackup1() {
		if (m_backup1.empty() == true) {
			if (value("BackupOne", m_backup1) == true) {
				ArchivePath::setBackup1Path(m_backup1);
				m_backup1Enabled = true;
			}
		}
		return m_backup1.c_str();
	}

	const char *CAppConfig::getBackup2() {
		if (m_backup2.empty() == true) {
			if (value("BackupTwo", m_backup2) == true) {
				ArchivePath::setBackup2Path(m_backup2);
				m_backup2Enabled = true;
			}
		}
		return m_backup2.c_str();
	}

	


	/// Gets the archive path.
	const char *CAppConfig::getWorkspacePath() {
		
		return m_workspacePath.c_str();

	}
	/// Gets the archive path.
	const char *CAppConfig::getMasterPath() {
		if (m_masterPath.empty() == true) {
			if (value("MasterPath", m_masterPath) == false) {
				// if not found read from SIA_WORKSPACE environment variable
				std::string temp = SAUtils::GetPOSIXEnv("SIA_MASTER");
				if (temp.empty() == false) {
					m_masterPath = temp;
				}
				else {
					std::string tempProgramData = SAUtils::GetPOSIXEnv("ProgramData");
					m_masterPath = tempProgramData + DEFAULT_MASTER_PATH;
					ArchivePath::setMasterPath(m_masterPath);
				}
			}
		}
		return m_masterPath.c_str();

	}
	
	const char *CAppConfig::getDerivativePath() {
		if (m_derivativePath.empty() == true) {
			if (value("DerivativePath", m_derivativePath) == false) {
				// if not found read from SIA_WORKSPACE environment variable
				std::string temp = SAUtils::GetPOSIXEnv("SIA_Master");
				if (temp.empty() == false) {
					m_derivativePath = temp;
				}
				else {
					std::string tempProgramData = SAUtils::GetPOSIXEnv("ProgramData");
					m_derivativePath = tempProgramData + DEFAULT_DERIVATIVE_PATH;
					ArchivePath::setDerivativePath(m_derivativePath);
				}
			}
		}
		return m_derivativePath.c_str();

	}

	
	const char *CAppConfig::getSourcePath() {
		if (m_sourcePath.empty() == true) {
			if (value("SourcePath", m_sourcePath) == false) {
				std::string tempHomeDrive = SAUtils::GetPOSIXEnv("HOMEDRIVE");
				std::string tempHomePath = SAUtils::GetPOSIXEnv("HOMEPATH");
				m_sourcePath = tempHomeDrive + tempHomePath + DEFAULT_SOURCE_PATH;
			}
		}
		return m_sourcePath.c_str();

	}

	const char *CAppConfig::getHookPath() {
		if (m_hookPath.empty() == true) {
			if (value("HookPath", m_hookPath) == false) {
				std::string temp = SAUtils::GetPOSIXEnv("HOMEPATH");
				m_hookPath = m_homePath + HOOKS_PATH;
			}
		}
		return m_hookPath.c_str();

	}
	
	void CAppConfig::setMasterViewPath(const char *path) {
		m_masterViewPath = path;
	}

	void CAppConfig::setWorkspacePath(const char *path) {
		m_workspacePath = path;
		ArchivePath::setPathToWorkspace(m_workspacePath);
	}

	void CAppConfig::setMasterPath(const char *path) {
		m_masterPath = path;
		ArchivePath::setMasterPath(m_masterPath);
	}


	void CAppConfig::setDerivativePath(const char *path) {
		m_derivativePath = path;
	}

	void CAppConfig::setSourcePath(const char *path) {
		m_sourcePath = path;
	}

	const char *CAppConfig::getBackupDestinationPath() {
		if (m_backupDestinationPath.empty() == true) {
			if (value("BackupDestinationPath", m_backupDestinationPath) == false) {
				std::string temp = SAUtils::GetPOSIXEnv("HOMEPATH");
				m_backupDestinationPath = temp + BACKUPS_PATH;
			}
		}
		return m_backupDestinationPath.c_str();

	}

	const char *CAppConfig::getMasterViewPath() {
		if (m_masterViewPath.empty() == true) {
			if (value("BackupDestinationPath", m_masterViewPath) == false) {
				std::string temp = SAUtils::GetPOSIXEnv("USERPROFILE");
				m_masterViewPath = temp + MASTER_VIEW_PATH;
			}
		}
		return m_masterViewPath.c_str();

	}

	void CAppConfig::setBackupDestinationPath(const char *path) {
		m_backupDestinationPath = path;
	}

	long CAppConfig::getBackupMediaSize() {
		if (m_backupMediaSize == 0) {
			std::string tmp;
		
			if (value("BackupMediaSize", tmp) == false) {
					m_backupMediaSize = 700; // size of a DVD
			}
			else {
				m_backupMediaSize = strtoul(tmp.c_str(), 0, 10);
			}	
		}
		return m_backupMediaSize;
	}

	void CAppConfig::setBackupMediaSize(const char *sizeStr) {
		m_backupMediaSize = strtoul(sizeStr, 0, 10);
	}

	ExifDateTime &CAppConfig::getFromDate() {
		return m_fromDate;
	}

	void CAppConfig::setFromDate(const char *dateStr) {
		ExifDateTime tmp(dateStr);
		m_fromDate = tmp;
	}

	ExifDateTime &CAppConfig::getToDate() {
		return m_toDate;
	}

	void CAppConfig::setToDate(const char *dateStr) {
		ExifDateTime tmp(dateStr);
		m_toDate = tmp;
	}

	bool CAppConfig::isFromDateSet() {
		return m_isFromDate;
	}

	bool CAppConfig::isToDateSet() {
		return m_isToDate;
	}
	
	const char *CAppConfig::getDatabasePath() {
		if (value("DatabasePath", m_DatabasePath) == false) {
			std::string temp = SAUtils::GetPOSIXEnv("HOMEPATH");
			m_DatabasePath = m_homePath + DATABASE_PATH;
		}
		return m_DatabasePath.c_str();
	}

	const char *CAppConfig::getTempPath() {
		if (value("LogPath", m_tempPath) == false) {
			m_tempPath = m_homePath + "/tmp";
		}
		return m_tempPath.c_str();
	}

	const char *CAppConfig::getLogPath() {
		if (value("LogPath", m_logPath) == false) {
			m_logPath = m_homePath + "/logs";
		}
		return m_logPath.c_str();
	}

	const char *CAppConfig::getLogLevel() {
		return m_logLevel.c_str();
	}

	void CAppConfig::setLogLevel(const char *logLevel) {
		m_logLevel = logLevel;
	}
	
	const char *CAppConfig::getSystemPath() {
		if (value("SystemPath", m_systemPath) == false) {
			m_systemPath = m_masterPath + MASTER_SYSTEM_FOLDER;
		}
		return m_systemPath.c_str();
	}

	const char *CAppConfig::getIndexPath() {	
		m_indexPath = m_systemPath + "/index";
		return m_indexPath.c_str();
	}

	const char *CAppConfig::getHistoryPath() {
		if (m_historyPath.empty() == true) {
			if (value("HistoryPath", m_logPath) == false) {
				std::string temp = SAUtils::GetEnv("HOMEPATH");
				m_historyPath = m_homePath + HISTORY_PATH;
				ArchivePath::setMainHistory(m_historyPath);
			}
		}
		return 	m_historyPath.c_str();
	}

	const char *CAppConfig::getConfigPath() {
		if (value("ConfigPath", m_configPath) == false) {
			m_configPath = m_homePath + "/config";
		}
		return 	m_configPath.c_str();
	}

	const char *CAppConfig::getExternalExifTool() {
		if (value("ExifTool", m_ExternalExifTool) == false) {
			m_ExternalExifTool = "exiftool.exe";
			return m_ExternalExifTool.c_str();
		}
		return 	m_ExternalCommandLine.c_str();
	}

	const char *CAppConfig::getExternalCommandLine() {
		if (value("ExifCommandLine", m_ExternalCommandLine) == false) {
			m_ExternalCommandLine = EXTERAL_EXIF_COMMAND_LINE;
			return m_ExternalCommandLine.c_str();
		}
		return 	m_ExternalCommandLine.c_str();
	}

	const char *CAppConfig::getExifMapPath() {
		if (value("ExifMapPath", m_ExternalCommandLine) == false) {
			m_ExifMapPath = m_homePath + CONFIG_PATH;
			return m_ExifMapPath.c_str();
		}
		return 	m_ExifMapPath.c_str();
	}

	const char *CAppConfig::getMetadataTemplatePath() {
		if (value("MetadataTemplatePath", m_MetadataTemplatePath) == false) {
			m_MetadataTemplatePath = m_homePath + "/templates";
			return m_MetadataTemplatePath.c_str();
		}
		return 	m_MetadataTemplatePath.c_str();
	}

	bool CAppConfig::isDryRun() const {
		return m_dry_run;
	}

	void CAppConfig::setDryRun(bool dryRun) {
		m_dry_run = dryRun;
	}


	bool CAppConfig::isQuiet() const {
		return m_quiet;
	}

	void CAppConfig::setQuiet(bool quiet) {
		m_quiet = quiet;
	}

	bool CAppConfig::isSilent() const {
		return m_silent;
	}

	void CAppConfig::setSilent(bool silent) {
		m_silent = silent;
	}

	bool CAppConfig::isVerbose() const {
		return m_verbose;
	}

	void CAppConfig::setVerbose(bool verbose) {
		m_verbose = verbose;
	}

	bool CAppConfig::isEventsOn() {
		return m_eventsOn;
	}

	int CAppConfig::eventPort() {
		return m_udpPortNum;

	}
	const char *CAppConfig::eventAddress() {
		return m_udpAddress.c_str();
	}

	bool CAppConfig::isServerOn() {
		return m_serverOn;
	}

	int CAppConfig::serverPort() {
		return m_tcpPortNum;

	}

	void CAppConfig::setEventsOn(bool evt) {
		m_eventsOn = evt;
	}

	void CAppConfig::setEventPort(int port) {
		m_eventsOn = true;
		m_udpPortNum = port;
	}

	void CAppConfig::setEventAddress(const char *address) {
		m_eventsOn = true;
		m_udpAddress = address;
	}

	void CAppConfig::isServerOn(bool on) {
		m_serverOn = on;
	}

	void CAppConfig::setServerPort(int port) {
		m_tcpPortNum = port;
	}

	//void setSourcePath(const char *sourcePath);

	bool CAppConfig::validWorkspacePath() {
		return true;
	}
	bool CAppConfig::validSourcePath() {
		return true;
	}
	bool CAppConfig::validHomePath() {
		return true;
	}

	std::string CAppConfig::toString() {
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
		/// @brief Gets the path to the metadata template files.
		str << "Metadata template path:  " << getMetadataTemplatePath() << '\n';
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
		str << "Master view path:        " << getMasterViewPath() << '\n';
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

	std::string CAppConfig::toXMLString() {
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
		str << "<MetadataTemplatePath>" << getMetadataTemplatePath() << "</MetadataTemplatePath>" << '\n';
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
		str << "<MasterViewPath>" << getMasterViewPath() << "</MasterViewPath>" << '\n';
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
