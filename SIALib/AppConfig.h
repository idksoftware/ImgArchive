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


#define SYSTEM_FOLDERS			"System Folders" /*< Path to hook scripts */
#define MASTER_ARCHIVE			"Master Archive"	 /*< Main configuration path */ 
//#define TOOLS_PATH_LABEL           		"ToolsPath"


/**
	@brief These #defines define the labels the hook files will use
	to reference envronment varibles.
*/
#define HOOK_SCRIPTS_PATH_LABEL			"HookScripsPath" /*< Path to hook scripts */
#define CONFIG_PATH_LABEL				"ConfigPath"	 /*< Main configuration path */ 
#define TOOLS_PATH_LABEL           		"ToolsPath"
#define TEMP_PATH_LABEL           		"TempPath"
#define SOURCE_PATH_LABEL         		"SourcePath"
#define SYSTEM_PATH_LABEL         		"SystemPath"
#define MASTER_PATH_LABEL         		"MasterPath"
#define SQL_LABEL         				"SQL"
#define MASTER_VIEW_PATH_LABEL         	"MasterCataloguePath"
#define DERIVATIVE_PATH_LABEL         	"DerivativePath"
#define SQL_DATABASE_PATH_LABEL         "SQLDatabasePath"
#define LOG_PATH_LABEL					"LogPath"
#define HOME_PATH_LABEL					"HomePath"
#define INDEX_PATH_LABEL				"IndexPath"
#define HISTORY_PATH_LABEL				"HistoryPath"
#define LIGHTROOM_PATH_LABEL			"LightroomPath"
#define ENABLED_LABEL					"Enabled"
#define BACKUP_ONE_ENABLED_LABEL		"BackupOneEnabled"
#define BACKUP_TWO_ENABLED_LABEL		"BackupTwoEnabled"
#define EXTERNAL_COMMAND_LINE_LABEL		"ExternalCommandLine"
#define EXIF_MAP_PATH_LABEL				"ExifMapPath"
#define EXIF_MAP_FILE_LABEL				"ExifMapFile"
#define EXIF_TOOL_LABEL					"ExifTool"
#define EXIF_COMMANDLINE_LABEL			"ExifCommandLine"
#define TEMPLATE_PATH_LABEL				"TemplatePath"
#define CATALOG_PATH_LABEL				"CatalogPath"
#define WORKSPACE_PATH_LABEL			"WorkspacePath"
#define DRY_RUN_LABEL					"DryRun"
#define LOG_LEVEL_LABEL					"LogLevel"
#define LIGHTROOM_LABEL					"Lightroom"
#define SERVER_MODE_LABEL				"RemoteServerMode"
#define CONSOLE_LEVEL_LABEL				"ConsoleLevel"
#define FILE_CAT_LABEL				"FileCatalogue"
#define WWW_CAT_LABEL				"WWWCatalogue"
#define EVENTS_ENABLED_LABEL		"EventsEnabled"
#define COMMANDS_PORT_LABEL			"CommandPort"
#define BACKUP_ONE_LABEL			"BackupOne"
#define BACKUP_TWO_LABEL			"BackupTwo"

namespace simplearchive {

	class AppOptions;
	/**
	* @brief This is the main configuration object. It is used for the
	* primary configuration options.
	*
	*/
	class AppConfig 
	{
	
	private:
		friend class AppOptions;
		friend class SharedConfig;
		friend class SIAARCConfig;

		static std::unique_ptr<AppConfig> m_this;
		static bool m_verbose; //< -v --Verbose
		static bool m_quiet;
		static bool m_silent;
		static bool m_sql_on;
		static bool m_file_cat_on;
		static bool m_www_cat_on;
		static std::string m_logLevel;
		static std::string m_consoleLevel;
		static bool m_lightroom;
		static bool m_serverMode;
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
		static bool m_externalExifToolEnabled;
		/// External Exif tool
		static std::string m_ExternalExifTool;
		/// External Exif Command line
		static std::string m_ExternalCommandLine;
		/// This is path to the Exif Map files. For example the Exiftool map
		/// that maps exiftool keywords to Simple Archive keywords.
		static std::string m_ExifMapPath;
		static std::string m_ExifMapFile;
		static std::string m_ExifFileDelim;
		/// This is the path to the template files.
		static std::string m_templatePath;
		/// This is the temp file path.
		static std::string m_tempPath;
		/// This is the archive repository file path.
		static std::string m_workspacePath;
		static std::string m_masterPath;
		static std::string m_derivativePath;
		static std::string m_masterCataloguePath;
		static std::string m_masterWWWCataloguePath;
		static std::string m_catalogPath;
		static std::string m_lightroomPath;
		static std::string m_sourcePath;

		static std::string m_configPath;
		
		static std::string m_hookPath;
		static std::string m_toolsPath;
		static std::string m_DatabasePath;
		static std::string m_backupDestinationPath;
		
		static long m_backupMediaSize;
		
		static ExifDateTime m_fromDate;
		static bool m_isFromDate;
		static ExifDateTime m_toDate;
		static bool m_isToDate;

		static std::string m_backup1;
		static std::string m_backup2;
		static bool m_backup1Enabled;
		static bool m_backup2Enabled;
		static bool m_masterViewEnabled;
		static bool m_masterViewFullSizeOn;
		static bool m_masterViewPreview1On;
		static bool m_masterViewThumbnailOn;
		static bool m_eventsOn; // UDP events
		static bool m_serverOn;

		static int m_tcpPortNum;
		static int m_udpPortNum;
		static std::string m_udpAddress;

		static std::string m_filenaming;

	
	public:
		AppConfig() noexcept {}
		~AppConfig() {}
		void settup();

		static AppConfig &get() {
			static AppConfig    instance; // Guaranteed to be destroyed.
											  // Instantiated on first use.
			return instance;
		}
		
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
		const char *getTemplatePath();
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
		bool isExternalExifToolEnabled() { return m_externalExifToolEnabled; }
		const char *getExifMapPath();
		const char *getExifMapFile();
		const char *getExifFileDelim();
		const char *getConfigPath();
		/// @brief Gets home path. This is the root path all default paths are made.
		const char *getHomePath();
		bool isMasterCatalogueEnabled();
		bool isMasterWWWCatalogueEnabled();
		const char *getMasterCataloguePath();
		const char *getMasterWWWCataloguePath();
		const char *getDatabasePath();
		// This is for media backups
		const char *getBackupDestinationPath();
		bool isSQL();
		/// @brief Gets the Master archive path
		/// user definable
		const char *getMasterPath();
		const char *getLightroomPath();

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
		const char *getConsoleLevel();

		bool isLightroom() {
			return m_lightroom;
		}

		bool isServerMode() {
			return m_serverMode;
		}

		bool isEventsOn();
		int eventPort();
		const char *eventAddress();
		bool isServerOn();
		int serverPort();

		std::string toString();
		std::string toXMLString();
		
		
		void isServerOn(bool on);
		void setServerPort(int port);
		/// Sets tools path
		void setToolsPath(const char *toolsPath);
		
		
		bool validWorkspacePath();
		bool validSourcePath();
		bool validHomePath();
	};

	

	class SharedConfig : public AppConfigBase {
		

		ConfigBlock &getRoot() { // The root will always exist 
			std::shared_ptr<ConfigBlock> configBlock = find(ROOT_BLOCK)->second;
			return (*configBlock);
		}

		std::shared_ptr<ConfigBlock> getLogging() { // The root will always exist 
			
			auto p = find(LOGGING_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(LOGGING_BLOCK)->second;
			return configBlock;
		}

		std::shared_ptr<ConfigBlock> getNetwork() { // The root will always exist 
			
			auto p = find(NETWORK_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(NETWORK_BLOCK)->second;
			return configBlock;
		}

		std::shared_ptr<ConfigBlock> getFolders() { // The root will always exist 
			auto p = find(FOLDERS_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(FOLDERS_BLOCK)->second;
			return configBlock;
		}

		std::shared_ptr<ConfigBlock> getMaster() { // The root will always exist 
			auto p = find(BACKUP_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(MASTER_BLOCK)->second;
			return configBlock;
		}

		std::shared_ptr<ConfigBlock> getDerivative() { // The root will always exist
			auto p = find(BACKUP_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(DERIVATIVE_BLOCK)->second;
			return configBlock;
		}

		std::shared_ptr<ConfigBlock> getBackup() { // The root will always exist
			auto p = find(BACKUP_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(BACKUP_BLOCK)->second;
			return configBlock;
		}

		std::shared_ptr<ConfigBlock> getExifTool() { // The root will always exist 
			auto p = find(EXIFTOOL_BLOCK);
			if (p == end()) {
				return nullptr;
			}
			std::shared_ptr<ConfigBlock> configBlock = find(EXIFTOOL_BLOCK)->second;
			return configBlock;
		}

		bool setGeneral(const char* name, std::string &value, std::string &defaultValue) {
			return setConfigBlock(name, value, defaultValue, GENERAL_BLOCK);
		}

		bool setSystemFolders(const char* name, std::string &value, std::string &defaultValue) {
			return setConfigBlock(name, value, defaultValue, FOLDERS_BLOCK);
		}

		bool setExternalExifTool(const char* name, std::string &value, std::string &defaultValue) {
			return setConfigBlock(name, value, defaultValue, EXIFTOOL_BLOCK);
		}

		bool setMaster(const char* name, std::string &value, std::string &defaultValue) {
			return setConfigBlock(name, value, defaultValue, MASTER_BLOCK);
		}

		bool setNetwork(const char* name, std::string &value, std::string &defaultValue) {
			return setConfigBlock(name, value, defaultValue, NETWORK_BLOCK);
		}

		std::shared_ptr<ConfigBlock> getMasterArchive() {
			return getConfigBlocks(MASTER_ARCHIVE);
		}

		std::shared_ptr<ConfigBlock> getSystemFolders() {
			return getConfigBlocks(SYSTEM_FOLDERS);
		}


	public:

		/// Sets home path. This is the root path all default paths are made. 
		void setHomePath(const char *homePath);
		void setWorkspacePath(const char *path);

		//void setMasterPath(const char *path);

		void setDryRun(bool dryRun);
		void setQuiet(bool quiet);
		void setSilent(bool silent);
		void setVerbose(bool verbose);
		void setArchivePath(const char *path);
		
		void setMasterPath(const char *path);
		void setDerivativePath(const char *path);
		void setMasterCataloguePath(const char *path);
		void setBackup1(const char *path);
		void setBackup2(const char *path);
		void setSourcePath(const char *path);
		void setLogLevel(const char *logLevel);
		void setConsoleLevel(const char *logLevel);
		void setEventsOn(bool evt);
		void setEventPort(int port);
		void setEventAddress(const char *address);
		void setBackupDestinationPath(const char *str);
		void setBackupMediaSize(const char *str);
		void setFromDate(const char *str);
		void setToDate(const char *str);

		SharedConfig() noexcept {};

		/// @brief Initalises the config object
		bool init(const char *homePath = nullptr);
		
		void fileBasedValues(const char *homePath);
	};

	class SIAARCConfig : public SharedConfig {
	public:
		SIAARCConfig() = default;
		bool getLightroom() {
			return AppConfig::m_lightroom;
		}
		void setLightroom() {
			AppConfig::m_lightroom = true;
		}
		void setServerModeON() {
			AppConfig::m_serverMode = true;
		}
	};

	class AdminConfig : public SharedConfig {

	public:
		AdminConfig() = default;
		
	};


}
