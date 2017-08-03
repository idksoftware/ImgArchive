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

#pragma once
#include "ConfigReader.h"
#include "ExifDateTime.h"

/**
	@brief These #defines define the labels the hook files will use
	to reference envronment varibles.
*/
#define HOOK_SCRIPTS_PATH_LABEL			"HookScripsPath" /*< Path to hook scripts */
#define CONFIG_PATH_LABEL				"ConfigPath"	 /*< Main configuration path */ 
#define TOOLS_PATH_LABEL           		"ToolsPath"
#define TEMP_PATH_LABEL           		"TempPath"
#define SOURCE_PATH_LABEL         		"SourcePath"
#define ARCHIVE_PATH_LABEL         		"ArchivePath"
#define LOG_PATH_LABEL					"LogPath"
#define HOME_PATH_LABEL					"HomePath"
#define INDEX_PATH_LABEL				"IndexPath"
#define HISTORY_PATH_LABEL				"HistoryPath"
#define EXTERNAL_COMMAND_LINE_LABEL		"ExternalCommandLine"
#define EXIF_MAP_PATH_LABEL				"ExifMapPath"
#define METADATA_TEMPLATE_PATH_LABEL	"MetadataTemplatePath"
#define CATALOG_PATH_LABEL				"CatalogPath"

namespace simplearchive {

	class AppOptions;
	/**
	* @brief This is the main configuration object. It is used for the
	* primary configuration options.
	*
	*/
	class CAppConfig : public Config
	{
	
	private:
		friend class AppOptions;

		static CAppConfig *m_this;
		static bool m_verbose; //< -v --Verbose
		static bool m_quiet;
		static bool m_silent;
		static std::string m_logLevel;
		static bool m_dry_run;
		/// Log file path
		static std::string m_logPath;
		/// home path. This is the root path all default paths are made. 
		static std::string m_homePath;
		/// System Path
		static std::string m_systemPath;
		/// This is the path to the crc index database.
		static std::string m_indexPath;
		/// This is the path the history file are stored. These files are used for
		/// the history of the changes made to the archive.  
		static std::string m_historyPath;
		/// External Exif tool
		static std::string m_ExternalExifTool;
		/// External Command line
		static std::string m_ExternalCommandLine;
		/// This is path to the Exif Map files. For example the Exiftool map
		/// that maps exiftool keywords to Simple Archive keywords.
		static std::string m_ExifMapPath;
		/// This is the path to the metadata template files.
		static std::string m_MetadataTemplatePath;
		/// This is the temp file path.
		static std::string m_tempPath;
		/// This is the archive repository file path.
		static std::string m_workspacePath;
		static std::string m_masterPath;
		static std::string m_derivativePath;
		static std::string m_masterViewPath;

		static std::string m_sourcePath;

		static std::string m_configPath;

		static std::string m_hookPath;

		static std::string m_toolsPath;
		static std::string m_DatabasePath;
		static std::string m_backupDestinationPath;
		static bool m_useDatabase;
		static long m_backupMediaSize;
		
		static ExifDateTime m_fromDate;
		static bool m_isFromDate;
		static ExifDateTime m_toDate;
		static bool m_isToDate;

		static std::string m_backup1;
		static std::string m_backup2;
		static bool m_backup1Enabled;
		static bool m_backup2Enabled;

		static bool m_eventsOn; // UDP events
		static bool m_serverOn;

		static int m_tcpPortNum;
		static int m_udpPortNum;
		static std::string m_udpAddress;


		CAppConfig();

		/// @brief Gets the Master archive path
		/// user definable
		const char *getMasterPath();

	public:
		static CAppConfig &get();
		~CAppConfig();

		/// @brief Initalises the config object
		void init(const char *homePath = nullptr);
		void settup();
		void fileBasedValues();
		/// @brief Gets the source path.
		const char *getSourcePath();
		/// @brief Gets the archive path.
		/// user definable
		const char *getWorkspacePath();
		
		/// @brief Gets the Derivative archive path
		/// user definable
		const char *getDerivativePath();
		/// @brief Gets the temp file path.
		/// user definable
		const char *getTempPath();
		/// @brief Gets tools path
		const char *getToolsPath();
		/// @brief Gets the hooks path
		/// user definable
		/// 
		const char *getHookPath();
		/// @brief Gets the path to the metadata template files.
		const char *getMetadataTemplatePath();
		/// @brief Gets log file path
		const char *getLogPath();
		/// System path
		const char *getSystemPath();
		/// @brief Gets the path to the crc index database.
		const char *getIndexPath();
		/// @brief Gets the path the history file are stored. These files are used for
		/// @brief the history of the changes made to the archive.
		const char *getHistoryPath();
		/// @brief gets external Exif tool
		const char *getExternalExifTool();
		/// @brief gets external Command line
		const char *getExternalCommandLine();
		/// @brief Gets path to the Exif Map files. For example the Exiftool map
		/// that maps exiftool keywords to Simple Archive keywords.
		const char *getExifMapPath();
		const char *getConfigPath();
		/// @brief Gets home path. This is the root path all default paths are made.
		const char *getHomePath();
		const char *getMasterViewPath();
		const char *getDatabasePath();
		// This is for media backups
		const char *getBackupDestinationPath();

		const char *getBackup1();
		const char *getBackup2();

		bool isBackup1Enabled() { return m_backup1Enabled; };
		bool isBackup2Enabled() { return m_backup2Enabled; };

		void setBackupDestinationPath(const char *path);
		long getBackupMediaSize();
		void setBackupMediaSize(const char *path);

		ExifDateTime &getFromDate();
		bool isFromDateSet();
		void setFromDate(const char *dateStr);

		ExifDateTime &getToDate();
		bool isToDateSet();
		void setToDate(const char *dateStr);

		bool isDryRun() const;
		bool isSilent() const;
		bool isQuiet() const;
		bool isVerbose() const;
		const char *getLogLevel();

		bool isEventsOn();
		int eventPort();
		const char *eventAddress();
		bool isServerOn();
		int serverPort();

		std::string toString();
		std::string toXMLString();
		
		void setDryRun(bool dryRun);
		void setQuiet(bool quiet);
		void setSilent(bool silent);
		void setVerbose(bool verbose);
		void setWorkspacePath(const char *homePath);
		void setMasterPath(const char *homePath);
		void setDerivativePath(const char *homePath);
		void setMasterViewPath(const char *viewPath);
		void setBackup1(const char *backupPath);
		void setBackup2(const char *backupPath);
		void setSourcePath(const char *sourcePath);
		void setLogLevel(const char *logLevel);

		void setEventsOn(bool evt);
		void setEventPort(int port);
		void setEventAddress(const char *address);

		void isServerOn(bool on);
		void setServerPort(int port);
		/// Sets tools path
		void setToolsPath(const char *toolsPath);
		/// Sets home path. This is the root path all default paths are made. 
		void setHomePath(const char *homePath);
		bool validWorkspacePath();
		bool validSourcePath();
		bool validHomePath();
	};

}
