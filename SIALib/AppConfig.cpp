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

#include "AppConfig.h"
#include "SAUtils.h"

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
	std::string CAppConfig::m_archivePath;
	std::string CAppConfig::m_shadowArchivePath;
	std::string CAppConfig::m_sourcePath;
	std::string CAppConfig::m_configPath;
	std::string CAppConfig::m_tempPath;
	std::string CAppConfig::m_logPath;
	std::string CAppConfig::m_homePath;
	std::string CAppConfig::m_indexPath;
	std::string CAppConfig::m_historyPath;
	std::string CAppConfig::m_ExternalCommandLine;
	std::string CAppConfig::m_ExifMapPath;
	std::string CAppConfig::m_MetadataTemplatePath;
	std::string CAppConfig::m_backupDestinationPath;
	std::string CAppConfig::m_masterViewPath;
	std::string CAppConfig::m_DatabasePath;


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
#define HOOK_SCRIPTS_PATH       "HookScripsPath"
#define CONFIG_PATH				"ConfigPath"
#define TOOLS_PATH           	"ToolsPath"
#define TEMP_PATH           	"TempPath"
#define SOURCE_PATH         	"SourcePath"
#define ARCHIVE_PATH         	"ArchivePath"
#define LOG_PATH				"LogPath"
#define HOME_PATH				"HomePath"
#define INDEX_PATH				"IndexPath"
#define HISTORY_PATH			"HistoryPath"
#define EXTERNAL_COMMAND_LINE	"ExternalCommandLine"
#define EXIF_MAP_PATH			"ExifMapPath"
#define METADATA_TEMPLATE_PATH	"MetadataTemplatePath"
*/
	void CAppConfig::setToolsPath(const char *toolsPath) {
		m_toolsPath = toolsPath;
	}
	const char *CAppConfig::getToolsPath() {
		if (m_toolsPath.empty() == true) {
			if (value("ToolsPath", m_toolsPath) == false) {
				std::string temp = SAUtils::GetEnvironment("HOMEPATH");
				m_toolsPath = temp + "/tools";
			}
		}
		return m_toolsPath.c_str();

	}
	void CAppConfig::setHomePath(const char *homePath) {
		m_homePath = homePath;
	}
	const char *CAppConfig::getHomePath() {
		if (m_homePath.empty() == true) {
			if (value("HomePath", m_homePath) == false) {
				m_homePath = SAUtils::GetEnvironment("HOMEPATH");
			}
		}
		return m_homePath.c_str();

	}
	/// Gets the archive path.
	const char *CAppConfig::getArchivePath() {
		if (m_archivePath.empty() == true) {
			if (value("ArchivePath", m_archivePath) == false) {
				std::string temp = SAUtils::GetEnvironment("USERPROFILE");
					m_archivePath = temp + "/SIAImages"; 
			}
		}
		return m_archivePath.c_str();

	}
	/// Gets the archive path.
	const char *CAppConfig::getShadowPath() {
		if (m_shadowArchivePath.empty() == true) {
			if (value("ShadowPath", m_shadowArchivePath) == false) {
				std::string temp = m_homePath;
				m_shadowArchivePath = temp + "/SIAShadow";
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

	void CAppConfig::setArchivePath(const char *path) {
		m_archivePath = path;
	}

	void CAppConfig::setShadowPath(const char *path) {
		m_shadowArchivePath = path;
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
			m_indexPath = m_shadowArchivePath + "/root/index";
		}
		return m_indexPath.c_str();
	}

	const char *CAppConfig::getHistoryPath() {
		if (value("HistoryPath", m_logPath) == false) {
			m_historyPath = m_shadowArchivePath + "/root/history";
		}
		return 	m_historyPath.c_str();
	}

	const char *CAppConfig::getConfigPath() {
		if (value("ConfigPath", m_configPath) == false) {
			m_configPath = m_homePath + "/conf";
		}
		return 	m_configPath.c_str();
	}

	const char *CAppConfig::getExternalCommandLine() {
		if (value("ExifCommandLine", m_ExternalCommandLine) == false) {
			m_ExternalCommandLine = "exiftool.exe \"[input]\" > \"[output]\"";
			return m_ExternalCommandLine.c_str();
		}
		return 	m_ExternalCommandLine.c_str();
	}

	const char *CAppConfig::getExifMapPath() {
		if (value("ExifMapPath", m_ExternalCommandLine) == false) {
			m_ExifMapPath = m_homePath + "/conf";
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

	bool CAppConfig::validArchivePath() {
		return true;
	}
	bool CAppConfig::validSourcePath() {
		return true;
	}
	bool CAppConfig::validHomePath() {
		return true;
	}
}
