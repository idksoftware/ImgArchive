#pragma once
#include "ConfigReader.h"
#include "ExifDateTime.h"

#define HOOK_SCRIPTS_PATH       "HookScripsPath" /*< Path to hook scripts */
#define CONFIG_PATH		"ConfigPath"	 /*< Main configuration path */ 
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
		static std::string m_logLevel;
		static bool m_dry_run;
		/// Log file path
		static std::string m_logPath;
		/// home path. This is the root path all default paths are made. 
		static std::string m_homePath;
		/// This is the path to the crc index database.
		static std::string m_indexPath;
		/// This is the path the history file are stored. These files are used for
		/// the history of the changes made to the archive.  
		static std::string m_historyPath;
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
		static std::string m_archivePath;

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
		CAppConfig();
	public:
		static CAppConfig &get();
		~CAppConfig();
		/// Gets the source path.
		const char *getSourcePath();
		/// Gets the archive path.
		const char *getArchivePath();
		/// Gets the temp file path.
		const char *getTempPath();
		/// Gets tools path
		const char *getToolsPath();
		/// Gets the hooks path
		const char *CAppConfig::getHookPath();
		/// Gets the path to the metadata template files.
		const char *getMetadataTemplatePath();
		/// Gets log file path
		const char *getLogPath();
		/// Gets the path to the crc index database.
		const char *getIndexPath();
		/// Gets the path the history file are stored. These files are used for
		/// the history of the changes made to the archive.
		const char *getHistoryPath();
		/// gets external Command line
		const char *getExternalCommandLine();
		/// Gets path to the Exif Map files. For example the Exiftool map
		/// that maps exiftool keywords to Simple Archive keywords.
		const char *getExifMapPath();
		const char *getConfigPath();
		/// Gets home path. This is the root path all default paths are made.
		const char *getHomePath();
		const char *getMasterViewPath();
		const char *getDatabasePath();
		const char *getBackupDestinationPath();
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
		bool isQuiet() const;
		bool isVerbose() const;
		const char *getLogLevel();
		
	private:
		void setDryRun(bool dryRun);
		void setQuiet(bool quiet);
		void setVerbose(bool verbose);
		void setArchivePath(const char *homePath);
		void setMasterViewPath(const char *viewPath);
		
		void setSourcePath(const char *sourcePath);
		void setLogLevel(const char *logLevel);
		/// Sets tools path
		void setToolsPath(const char *toolsPath);
		/// Sets home path. This is the root path all default paths are made. 
		void setHomePath(const char *homePath);
		bool validArchivePath();
		bool validSourcePath();
		bool validHomePath();
	};

}