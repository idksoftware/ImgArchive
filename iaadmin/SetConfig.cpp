#include "SetConfig.h"
#include "AppConfig.h"

enum class Option {
	QUIET_LABEL
	Silent_LABEL					"Silent"
	LOG_LEVEL_LABEL					"LogLevel"
	LIGHTROOM_LABEL					"Lightroom"
	SERVER_MODE_LABEL				"RemoteServerMode"
	CONSOLE_LEVEL_LABEL				"ConsoleLevel"
	FILE_CAT_LABEL				"FileCatalogue"
	WWW_CAT_LABEL				"WWWCatalogue"
	EVENTS_ENABLED_LABEL		"EventsEnabled"
	COMMANDS_PORT_LABEL			"CommandPort"
	BACKUP_ONE_LABEL			"BackupOne"
	BACKUP_TWO_LABEL			"BackupTwo"
	HOOK_SCRIPTS_PATH_LABEL			"HookScripsPath" // Path to hook scripts
	CONFIG_PATH_LABEL				"ConfigPath"	 // Main configuration path
	TOOLS_PATH_LABEL           		"ToolsPath"
	TEMP_PATH_LABEL           		"TempPath"
	SOURCE_PATH_LABEL         		"SourcePath"
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
#define QUIET_LABEL						"Quiet"
#define Silent_LABEL					"Silent"
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
};


bool SetConfig::parseGeneralOptions(const char* ov)
{
	std::string optionValueString = ov;
	size_t pos = optionValueString.find_first_of('=');
	if (pos == std::string::npos) {
		return false;
	}
	std::string option = optionValueString.substr(0, pos);
	processGeneralOptions(std::string option)
}
bool SetConfig::processGeneralOptions(const char* ov)
{
	std::string optionValueString = ov;
	size_t pos = optionValueString.find_first_of('=');
	if (pos == std::string::npos) {
		return false;
	}
	
	if (option.compare(QUIET_LABEL)) {

		return true
	}
	if QUIET_LABEL
#define Silent_LABEL					"Silent"
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
	/*
	#define HOOK_SCRIPTS_PATH_LABEL			"HookScripsPath" // Path to hook scripts 
#define CONFIG_PATH_LABEL				"ConfigPath"	 // Main configuration path  
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
#define QUIET_LABEL						"Quiet"
#define Silent_LABEL					"Silent"
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

	static bool m_verbose;					//< -v --Verbose
	static bool m_quiet;					//
	static bool m_silent;					//
		static bool m_sql_on;					// Sqlite2 On/Off
		static bool m_file_cat_on;				// File cat On/Off
		static bool m_www_cat_on;				// WWW cat On/Off (web site)
		static std::string m_logLevel;			// Logging Level
		static std::string m_consoleLevel;		// Logging level to console
		static bool m_lightroom;				// Lightroom intergration On/Off
		static bool m_serverMode;				// Server mode On/Off
		static bool m_dry_run;					// Dry run		On/Off
		static bool m_backup1Enabled;			// Backup 1 Enabled
		static bool m_backup2Enabled;			// Backup 2 Enabled
		static bool m_workspaceEnabled;			// Workspace Enabled
		static bool m_externalExifToolEnabled;	// External Exif Tool Enabled

		static bool m_masterViewEnabled;
		static bool m_masterViewFullSizeOn;
		static bool m_masterViewPreview1On;    // Previews Enabled
		static bool m_masterViewThumbnailOn;
		static bool m_eventsOn; // UDP events
		static bool m_serverOn;
	*/
	return false;
}
