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
	bool CAppConfig::m_quiet = false;
	std::string CAppConfig::m_logLevel = "INFO";
	bool CAppConfig::m_dry_run = false;
	bool CAppConfig::m_useDatabase = true;

	std::string CAppConfig::m_hookPath;
	std::string CAppConfig::m_toolsPath;
	std::string CAppConfig::m_workspacePath;
	std::string CAppConfig::m_shadowArchivePath;
	std::string CAppConfig::m_sourcePath;
	std::string CAppConfig::m_configPath;
	std::string CAppConfig::m_tempPath;
	std::string CAppConfig::m_logPath;
	std::string CAppConfig::m_homePath;
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

	void CAppConfig::init() {
		if (m_homePath.empty() == true) {
			if (value("HomePath", m_homePath) == false) {
				m_homePath = SAUtils::GetEnvironment("HOMEPATH");
			}
		}
		ArchivePath::setPathToHome(m_homePath);

		if (m_backup1.empty() == true) {
			if (value("BackupOne", m_backup1) == true) {
				ArchivePath::setBackup1Path(m_backup1);
				m_backup1Enabled = true;
			}
		}
		else {
			ArchivePath::setBackup1Path(m_backup1);
		}
		
		if (m_backup2.empty() == true) {
			if (value("BackupTwo", m_backup2) == true) {
				ArchivePath::setBackup2Path(m_backup2);
				m_backup2Enabled = true;
			}
		}
		else {
			ArchivePath::setBackup2Path(m_backup2);
		}

		if (m_shadowArchivePath.empty() == true) {
			if (value("ShadowPath", m_shadowArchivePath) == false) {
				std::string temp = m_homePath;
				m_shadowArchivePath = temp + "/shadow";
				ArchivePath::setPathToShadow(m_shadowArchivePath);
			}
		}
		ArchivePath::setPathToShadow(m_shadowArchivePath);

		if (m_historyPath.empty() == true) {
			if (value("HistoryPath", m_logPath) == false) {
				std::string temp = SAUtils::GetEnvironment("HOMEPATH");
				m_historyPath = m_homePath + "/history";
				
			}
		}
		ArchivePath::setMainHistory(m_historyPath);

		if (m_workspacePath.empty() == true) {
			if (value("WorkspacePath", m_workspacePath) == false) {
				std::string temp = SAUtils::GetEnvironment("USERPROFILE");
				m_workspacePath = temp + "/SIA Workspace";

			}
		}
		ArchivePath::setPathToWorkspace(m_workspacePath);
		
	}
	
	void CAppConfig::setToolsPath(const char *toolsPath) {
		m_toolsPath = toolsPath;
	}
	const char *CAppConfig::getToolsPath() {
		if (m_toolsPath.empty() == true) {
			if (value("ToolsPath", m_toolsPath) == false) {
				std::string temp = m_homePath;
				m_toolsPath = temp + "/tools";

			}
		}
		return m_toolsPath.c_str();

	}
	void CAppConfig::setHomePath(const char *homePath) {
		m_homePath = homePath;
		ArchivePath::setPathToHome(m_homePath);
	}
	
	const char *CAppConfig::getHomePath() {
		if (m_homePath.empty() == true) {
			if (value("HomePath", m_homePath) == false) {
				m_homePath = SAUtils::GetEnvironment("SIA_HOME");
				ArchivePath::setPathToHome(m_homePath);
			}
		}
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
		if (m_workspacePath.empty() == true) {
			if (value("WorkspacePath", m_workspacePath) == false) {
				std::string temp = SAUtils::GetEnvironment("USERPROFILE");
				m_workspacePath = temp + "/SIA Workspace";
					
			}
		}
		return m_workspacePath.c_str();

	}
	/// Gets the archive path.
	const char *CAppConfig::getShadowPath() {
		if (m_shadowArchivePath.empty() == true) {
			if (value("ShadowPath", m_shadowArchivePath) == false) {
				std::string temp = m_homePath;
				m_shadowArchivePath = temp + "/shadow";
				ArchivePath::setPathToShadow(m_shadowArchivePath);
			}
		}
		return m_shadowArchivePath.c_str();

	}
	
	const char *CAppConfig::getSourcePath() {
		if (m_sourcePath.empty() == true) {
			if (value("SourcePath", m_sourcePath) == false) {
				std::string temp = SAUtils::GetEnvironment("HOMEPATH");
				m_sourcePath = temp + "/Pictures";
			}
		}
		return m_sourcePath.c_str();

	}

	const char *CAppConfig::getHookPath() {
		if (m_hookPath.empty() == true) {
			if (value("HookPath", m_hookPath) == false) {
				std::string temp = SAUtils::GetEnvironment("HOMEPATH");
				m_hookPath = m_homePath + "/hooks";
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

	void CAppConfig::setShadowPath(const char *path) {
		m_shadowArchivePath = path;
		ArchivePath::setPathToShadow(m_shadowArchivePath);
	}

	void CAppConfig::setSourcePath(const char *path) {
		m_sourcePath = path;
	}

	const char *CAppConfig::getBackupDestinationPath() {
		if (m_backupDestinationPath.empty() == true) {
			if (value("BackupDestinationPath", m_backupDestinationPath) == false) {
				std::string temp = SAUtils::GetEnvironment("HOMEPATH");
				m_backupDestinationPath = temp + "/Image Backups";
			}
		}
		return m_backupDestinationPath.c_str();

	}

	const char *CAppConfig::getMasterViewPath() {
		if (m_masterViewPath.empty() == true) {
			if (value("BackupDestinationPath", m_masterViewPath) == false) {
				std::string temp = SAUtils::GetEnvironment("USERPROFILE");
				m_masterViewPath = temp + "/SIA Pictures";
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
			std::string temp = SAUtils::GetEnvironment("HOMEPATH");
			m_DatabasePath = m_homePath + "/db";
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
	
	const char *CAppConfig::getIndexPath() {
		if (value("IndexPath", m_indexPath) == false) {
			m_indexPath = m_shadowArchivePath + "/system/index";
		}
		return m_indexPath.c_str();
	}

	const char *CAppConfig::getHistoryPath() {
		if (m_historyPath.empty() == true) {
			if (value("HistoryPath", m_logPath) == false) {
				std::string temp = SAUtils::GetEnvironment("HOMEPATH");
				m_historyPath = m_homePath + "/history";
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
			m_ExternalCommandLine = "\"[input]\" > \"[output]\"";
			return m_ExternalCommandLine.c_str();
		}
		return 	m_ExternalCommandLine.c_str();
	}

	const char *CAppConfig::getExifMapPath() {
		if (value("ExifMapPath", m_ExternalCommandLine) == false) {
			m_ExifMapPath = m_homePath + "/config";
			return m_ExifMapPath.c_str();
		}
		return 	m_ExifMapPath.c_str();
	}

	const char *CAppConfig::getMetadataTemplatePath() {
		if (value("MetadataTemplatePath", m_MetadataTemplatePath) == false) {
			m_MetadataTemplatePath = m_homePath + "/template";
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

	bool CAppConfig::isVerbose() const {
		return m_verbose;
	}

	void CAppConfig::setVerbose(bool verbose) {
		m_verbose = verbose;
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
		str << "<Configuration>" << '\n';
		str << "<WorkspacePath>" << getWorkspacePath() << "</WorkspacePath>" << '\n';
		/// @brief Gets the shadow archive path
		/// user definable
		str << "<ShadowPath>" << getShadowPath() << "</ShadowPath>" << '\n';
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
		str << "<IsDryRun>" << (isDryRun()?"true":"false") << "</IsDryRun>" << '\n';
		str << "<IsQuiet>" << (isQuiet() ? "true" : "false") << "</IsQuiet>" << '\n';
		str << "<IsVerbose>" << (isVerbose() ? "true" : "false") << "</IsVerbose>" << '\n';
		str << "<LogLevel>" << getLogLevel() << "</LogLevel>" << '\n';
		str << "</Configuration>" << '\n';
		return str.str();
	}
}
