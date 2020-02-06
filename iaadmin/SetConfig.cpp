#include "SetConfig.h"
#include "AppConfig.h"
#include "SAUtils.h"

enum class Option {
	QUIET,
	SILENT,
	LOG_LEVEL,
	LIGHTROOM,
	SERVER_MODE,
	CONSOLE_LEVEL,
	FILE_CAT,
	WWW_CAT,
	EVENTS_ENABLED,
	COMMANDS_PORT,
	BACKUP_ONE,
	BACKUP_TWO,
	SQL,
	HOOK_SCRIPTS_PATH, // Path to hook scripts
	CONFIG_PATH,	 // Main configuration path
	TOOLS_PATH,
	TEMP_PATH,
	SOURCE_PATH,
	SYSTEM_PATH,
	MASTER_PATH,
	MASTER_VIEW_PATH,
	DERIVATIVE_PATH,
	SQL_DATABASE_PATH,
	LOG_PATH,
	HOME_PATH,
	INDEX_PATH,
	HISTORY_PATH,
	LIGHTROOM_PATH,
	ENABLED,
	BACKUP_ONE_ENABLED,
	BACKUP_TWO_ENABLED,
	EXTERNAL_COMMAND_LINE,
	EXIF_MAP_PATH,
	EXIF_MAP_FILE,
	EXIF_TOOL,
	EXIF_COMMANDLINE,
	TEMPLATE_PATH,
	CATALOG_PATH,
	WORKSPACE_PATH,
	DRY_RUN,
	
	UNKNOWN
};

bool iequals(const std::string& a, const std::string& b)
{
	return std::equal(a.begin(), a.end(),
		b.begin(), b.end(),
		[](char a, char b) {
			return tolower(a) == tolower(b);
		});
}

/*
bool isTrueFalse(std::string& s) {
	if (iequals("true", s) || iequals("false", s) || iequals("on", s) ||
		iequals("off", s) || iequals("yes", s) || iequals("no", s)) {
		s = "True";
		return true;
	}
	s = "False";
	return false;
}
*/
/*

*/

std::string setLogLevel(std::string s) {
	
	if (iequals("TRACE", s)) {
		return "TRACE";
	}
	else if (iequals("FINE", s)) {
		return "FINE";
	}
	else if (iequals("INFO", s)) {
		return "INFO";
	}
	else if (iequals("STATUS", s)) {
		return "STATUS";
	}
	else if (iequals("SUMMARY", s)) {
		return "SUMMARY";
	}
	else if (iequals("WARNING", s)) {
		return "WARNING";
	}
	else if (iequals("ERROR", s)) {
		return "ERROR";
	}
	else if (iequals("FATAL", s)) {
		return "FATAL";
	}
	return "UNKNOWN";

}



bool SetConfig::processArgs(const char* ov) {
	std::string optionValueString = ov;
	size_t pos = optionValueString.find_first_of('=');
	if (pos == std::string::npos) {
		return false;
	}
	m_option = optionValueString.substr(0, pos);
	m_value = optionValueString.substr(pos + 1, optionValueString.length() - 1);

	return true;
}


bool SetConfig::parseGeneralOptions(const char* ov)
{
	m_optionBlock = GENERAL_BLOCK;
	if (!processArgs(ov)) {
		return false;
	}
	Option ret = processGeneralOptions(m_option);
	switch (ret) {
	case Option::QUIET:
		m_option = QUIET_LABEL;
		return (SAUtils::isTrueFalse(m_value) != BoolOption::Invalid);
	case Option::SILENT:
		m_option = SILENT_LABEL;
		return (SAUtils::isTrueFalse(m_value) != BoolOption::Invalid);
	case Option::LIGHTROOM:
		m_option = LIGHTROOM_LABEL;
		return (SAUtils::isTrueFalse(m_value) != BoolOption::Invalid);

	
	case Option::FILE_CAT:
		m_option = FILE_CAT_LABEL;
		return (SAUtils::isTrueFalse(m_value) != BoolOption::Invalid);
	case Option::WWW_CAT:
		m_option = WWW_CAT_LABEL;
		return (SAUtils::isTrueFalse(m_value) != BoolOption::Invalid);
	
	
	case Option::SQL:
		m_option = SQL_LABEL;
		return (SAUtils::isTrueFalse(m_value) != BoolOption::Invalid);
	case Option::UNKNOWN:
	default:
		return false;
	}
	return true;
}

bool SetConfig::parseLoggingOptions(const char* optionString)
{
	if (!processArgs(optionString)) {
		return false;
	}
	m_optionBlock = LOGGING_BLOCK;
	Option ret = processMasterOptions(m_option);
	switch (ret) {
	case Option::LOG_LEVEL:
		m_option = LOG_LEVEL_LABEL;
		if (iequals(setLogLevel(m_option), "UNKNOWN")) {
			return false;
		}
		return true;

	case Option::CONSOLE_LEVEL:
		m_option = CONSOLE_LEVEL_LABEL;
		if (iequals(setLogLevel(m_option), "UNKNOWN")) {
			return false;
		}
		break;
	case Option::LOG_PATH:
		m_option = LOG_PATH_LABEL;
		return true;
	default:
		return false;
	}
	return true;
}

bool SetConfig::parseFolderOptions(const char* ov)
{
	if (!processArgs(ov)) {
		return false;
	}
	m_optionBlock = FOLDERS_BLOCK;
	Option ret = processFolderOptions(m_option);
	switch (ret) {
	case Option::HOOK_SCRIPTS_PATH: // Path to hook scripts
		m_option = BACKUP_TWO_LABEL;
		return true;
	case Option::CONFIG_PATH:	 // Main configuration path
		m_option = CONFIG_PATH_LABEL;
		return true;
	case Option::TOOLS_PATH:
		m_option = TOOLS_PATH_LABEL;
		return true;
	case Option::TEMP_PATH:
		m_option = TEMP_PATH_LABEL;
		return true;
	case Option::SOURCE_PATH:
		m_option = SOURCE_PATH_LABEL;
		return true;
	case Option::SYSTEM_PATH:
		m_option = SYSTEM_PATH_LABEL;
		return true;
	case Option::MASTER_PATH:
		m_option = MASTER_PATH_LABEL;
		return true;
	case Option::MASTER_VIEW_PATH:
		m_option = MASTER_VIEW_PATH_LABEL;
		return true;
	case Option::DERIVATIVE_PATH:
		m_option = DERIVATIVE_PATH_LABEL;
		return true;
	case Option::SQL_DATABASE_PATH:
		m_option = SQL_DATABASE_PATH_LABEL;
		return true;
	
	case Option::HOME_PATH:
		m_option = HOME_PATH_LABEL;
		return true;
	case Option::INDEX_PATH:
		m_option = INDEX_PATH_LABEL;
		return true;
	case Option::HISTORY_PATH:
		m_option = HISTORY_PATH_LABEL;
		return true;
	case Option::LIGHTROOM_PATH:
		m_option = LIGHTROOM_LABEL;
		return true;
	default:
		return false;
	}
	return true;
}

bool SetConfig::parseExifToolOptions(const char* ov)
{
	if (!processArgs(ov)) {
		return false;
	}
	m_optionBlock = EXIFTOOL_BLOCK;
	Option ret = processExifToolOptions(m_option);
	switch (ret) {
	case Option::EXIF_MAP_PATH:
		m_option = EXIF_MAP_PATH_LABEL;
		return true;
	case Option::EXIF_MAP_FILE:
		m_option = EXIF_MAP_FILE_LABEL;
		return true;
	case Option::EXIF_TOOL:
		m_option = EXIF_TOOL_LABEL;
		return true;
	case Option::EXIF_COMMANDLINE:
		m_option = EXIF_COMMANDLINE_LABEL;
		return true;
		break;

	default:
		return false;
	}
	return true;
}

bool SetConfig::checkPath(const char *path) {
	
	if (SAUtils::FileExists(path) == false) {
		return false;
	}
	return true;
}

bool SetConfig::checkPath() {

	if (SAUtils::FileExists(m_value.c_str()) == false) {
		m_error = Error::Path_not_found;
		return false;
	}
	return true;
}

bool SetConfig::checkBool() {
	bool ret = (SAUtils::isTrueFalse(m_value) != BoolOption::Invalid);
	if (ret) {
		m_error = Error::Invalid_argument;
	}
	return ret;
}

bool SetConfig::parseMasterOptions(const char* optionString)
{
	if (!processArgs(optionString)) {
		return false;
	}
	m_optionBlock = MASTER_BLOCK;
	Option ret = processMasterOptions(m_option);
	switch (ret) {
	case Option::BACKUP_ONE_ENABLED:
		m_option = BACKUP_ONE_ENABLED_LABEL;
		return checkBool();
	case Option::BACKUP_TWO_ENABLED:
		m_option = BACKUP_TWO_ENABLED_LABEL;
		return checkBool();
	case Option::BACKUP_ONE:
		m_option = BACKUP_ONE_LABEL;
		return checkPath();
	case Option::BACKUP_TWO:
		m_option = BACKUP_TWO_LABEL;
		return checkPath();
	default:
		return false;
	}
	return true;
}

bool SetConfig::parseDerivativeOptions(const char* optionString)
{
	if (!processArgs(optionString)) {
		return false;
	}
	m_optionBlock = DERIVATIVE_BLOCK;
	Option ret = processMasterOptions(m_option);
	switch (ret) {
	case Option::BACKUP_ONE_ENABLED:
		m_option = BACKUP_ONE_ENABLED_LABEL;
		return checkBool();
	case Option::BACKUP_TWO_ENABLED:
		m_option = BACKUP_TWO_ENABLED_LABEL;
		return checkBool();
	case Option::BACKUP_ONE:
		m_option = BACKUP_ONE_LABEL;
		return checkPath();
	case Option::BACKUP_TWO:
		m_option = BACKUP_TWO_LABEL;
		return checkPath();
	default:
		return false;
	}
	return true;
}

bool SetConfig::parseBackupOptions(const char* optionString)
{
	if (!processArgs(optionString)) {
		return false;
	}
	m_optionBlock = BACKUP_BLOCK;
	Option ret = processMasterOptions(m_option);
	switch (ret) {
	case Option::QUIET:
		break;

	default:
		return false;
	}
	return true;
}

bool SetConfig::parseNetworkOptions(const char* ov)
{
	if (!processArgs(ov)) {
		return false;
	}
	m_optionBlock = NETWORK_BLOCK;
	Option ret = processNetworkOptions(m_option);
	switch (ret) {
	case Option::SERVER_MODE:
		m_option = SERVER_MODE_LABEL;
		return (SAUtils::isTrueFalse(m_value) != BoolOption::Invalid);
	case Option::EVENTS_ENABLED:
		m_option = EVENTS_ENABLED_LABEL;
		return (SAUtils::isTrueFalse(m_value) != BoolOption::Invalid);
	case Option::COMMANDS_PORT:
		m_option = COMMANDS_PORT_LABEL;
		return (SAUtils::isNumber(m_value));
	default:
		return false;
	}
	return true;
}



Option SetConfig::processGeneralOptions(std::string& optionString)
{
	if (iequals(optionString, QUIET_LABEL)) {
		return Option::QUIET;
	} 
	else if (iequals(optionString, SILENT_LABEL)) {
		return Option::SILENT;
	}
	else if (iequals(optionString, LIGHTROOM_LABEL)) {
		return Option::LIGHTROOM;
	}
	
	else if (iequals(optionString, FILE_CAT_LABEL)) {
		return Option::FILE_CAT;
	}
	else if (iequals(optionString, WWW_CAT_LABEL)) {
		return Option::WWW_CAT;
	}
	
	
	else if (iequals(optionString, BACKUP_ONE_ENABLED_LABEL)) {
		return Option::BACKUP_ONE_ENABLED;
	}
	else if (iequals(optionString, BACKUP_TWO_ENABLED_LABEL)) {
		return Option::BACKUP_TWO_ENABLED;
	}
	else if (iequals(optionString, SQL_LABEL)) {
		return Option::SQL;
	}
	
	return Option::UNKNOWN;
}

Option SetConfig::processLoggingOptions(std::string& optionString)
{
	if (iequals(optionString, LOG_LEVEL_LABEL)) {
		return Option::LOG_LEVEL;
	}
	else if (iequals(optionString, CONSOLE_LEVEL_LABEL)) {
		return Option::CONSOLE_LEVEL;
	}
	else if (iequals(optionString, LOG_PATH_LABEL)) {
		return Option::LOG_PATH;
	}
	return Option::UNKNOWN;
}

Option SetConfig::processFolderOptions(std::string& optionString)
{
	if (iequals(optionString, QUIET_LABEL)) {
		return Option::HOOK_SCRIPTS_PATH;
	}
	else if (iequals(optionString, CONFIG_PATH_LABEL)) {
		return Option::CONFIG_PATH;	 // Main configuration path
	}
	else if (iequals(optionString, TOOLS_PATH_LABEL)) {
		return Option::TOOLS_PATH;
	}
	else if (iequals(optionString, TEMP_PATH_LABEL)) {
		return Option::TEMP_PATH;
	}
	else if (iequals(optionString, SOURCE_PATH_LABEL)) {
		return Option::SOURCE_PATH;
	}
	else if (iequals(optionString, SYSTEM_PATH_LABEL)) {
		return Option::SYSTEM_PATH;
	}
	else if (iequals(optionString, MASTER_PATH_LABEL)) {
		return Option::MASTER_PATH;
	}
	else if (iequals(optionString, MASTER_VIEW_PATH_LABEL)) {
		return Option::MASTER_VIEW_PATH;
	}
	else if (iequals(optionString, DERIVATIVE_PATH_LABEL)) {
		return Option::DERIVATIVE_PATH;
	}
	else if (iequals(optionString, SQL_DATABASE_PATH_LABEL)) {
		return Option::SQL_DATABASE_PATH;
	}
	
	else if (iequals(optionString, HOME_PATH_LABEL)) {
		return Option::HOME_PATH;
	}
	else if (iequals(optionString, INDEX_PATH_LABEL)) {
		return Option::INDEX_PATH;
	}
	else if (iequals(optionString, HISTORY_PATH_LABEL)) {
		return Option::HISTORY_PATH;
	}
	else if (iequals(optionString, LIGHTROOM_PATH_LABEL)) {
		return Option::LIGHTROOM_PATH;
	}
	return Option::UNKNOWN;
}

Option SetConfig::processExifToolOptions(std::string& optionString)
{

	if (iequals(optionString, EXIF_MAP_PATH_LABEL)) {
		return Option::EXIF_MAP_PATH;
	}
	else if (iequals(optionString, EXIF_MAP_FILE_LABEL)) {
		return Option::EXIF_MAP_FILE;	 // Main configuration path
	}
	else if (iequals(optionString, EXIF_TOOL_LABEL)) {
		return Option::EXIF_TOOL;	 // Main configuration path
	}
	else if (iequals(optionString, EXIF_COMMANDLINE_LABEL)) {
		return Option::EXIF_COMMANDLINE;	 // Main configuration path
	}
	return Option::UNKNOWN;
}

Option SetConfig::processMasterOptions(std::string& optionString)
{
	if (iequals(optionString, BACKUP_ONE_ENABLED_LABEL)) {
		return Option::BACKUP_ONE_ENABLED;
	}
	else if (iequals(optionString, BACKUP_TWO_ENABLED_LABEL)) {
		return Option::BACKUP_TWO_ENABLED;
	}
	else if (iequals(optionString, BACKUP_ONE_LABEL)) {
		return Option::BACKUP_ONE;	 
	}
	else if (iequals(optionString, BACKUP_TWO_LABEL)) {
		return Option::BACKUP_TWO;	
	}
	return Option::UNKNOWN;
}

Option SetConfig::processDerivativeOptions(std::string& optionString)
{
	if (iequals(optionString, BACKUP_ONE_ENABLED_LABEL)) {
		return Option::BACKUP_ONE_ENABLED;
	}
	else if (iequals(optionString, BACKUP_TWO_ENABLED_LABEL)) {
		return Option::BACKUP_TWO_ENABLED;
	}
	else if (iequals(optionString, BACKUP_ONE_LABEL)) {
		return Option::BACKUP_ONE;
	}
	else if (iequals(optionString, BACKUP_TWO_LABEL)) {
		return Option::BACKUP_TWO;
	}
	return Option::UNKNOWN;
}

Option SetConfig::processBackupOptions(std::string& optionString)
{
	if (iequals(optionString, QUIET_LABEL)) {
		return Option::HOOK_SCRIPTS_PATH;
	}
	else if (iequals(optionString, CONFIG_PATH_LABEL)) {
		return Option::CONFIG_PATH;	 // Main configuration path
	}
	return Option::UNKNOWN;
}

Option SetConfig::processNetworkOptions(std::string& optionString)
{
	if (iequals(optionString, COMMANDS_PORT_LABEL)) {
	return Option::COMMANDS_PORT;
	}
	else if (iequals(optionString, SERVER_MODE_LABEL)) {
		return Option::SERVER_MODE;
	}
	else if (iequals(optionString, EVENTS_ENABLED_LABEL)) {
		return Option::EVENTS_ENABLED;
	}
	return Option::UNKNOWN;
}
