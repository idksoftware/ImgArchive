
#include <iostream>
#include <vector>
#include <string>
#include "SetConfig.h"
#include "AppConfig.h"
#include "SAUtils.h"

OutputType::Type OutputType::get() {
	return m_type;
}

bool OutputType::parse(const char* str) {
	std::string arg = str;

	if (arg.compare("plain") == 0) {
		m_type = Type::plain;
		return true;
	}
	if (arg.compare("xml") == 0) {
		m_type = Type::xml;
		return true;
	}
	if (arg.compare("json") == 0) {
		m_type = Type::json;
		return true;
	}
	if (arg.compare("html") == 0) {
		m_type = Type::html;
		return true;
	}
	m_type = Type::unknown;
	return false;
}


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
	COMMANDS_ENABLED,
	EVENTS_PORT,
	EVENTS_ADDRESS,
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
	GENERAL,
	LOGGING,
	NETWORK,
	FOLDERS,
	MASTER,
	DERIVATIVE,
	BACKUP,
	EXIFTOOL,
	ALL,
	RAW,
	PICTURE,
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

// check if given string is a numeric string or not
bool isNumber(const std::string& str)
{
	// std::find_first_not_of searches the string for the first character 
	// that does not match any of the characters specified in its arguments
	return !str.empty() &&
		(str.find_first_not_of("[0123456789]") == std::string::npos);
}

// Function to split string str using given delimiter
std::vector<std::string> split(const std::string& str, char delim)
{
	auto i = 0;
	std::vector<std::string> list;

	auto pos = str.find(delim);

	while (pos != std::string::npos)
	{
		list.push_back(str.substr(i, pos - i));
		i = ++pos;
		pos = str.find(delim, pos);
	}

	list.push_back(str.substr(i, str.length()));

	return list;
}

// Function to validate an IP address
bool validateIP(std::string ip)
{
	// split the string into tokens
	std::vector<std::string> list = split(ip, '.');

	// if token size is not equal to four
	if (list.size() != 4)
		return false;

	// validate each token
	for (std::string str : list)
	{
		// verify that string is number or not and the numbers 
		// are in the valid range
		if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0)
			return false;
	}

	return true;
}

bool ParseOptions::checkIPAddress(const char* ip) {

	if (validateIP(ip) == false) {
		return false;
	}
	return true;
}


bool ParseOptions::checkPath(const char* path) {

	if (SAUtils::FileExists(path) == false) {
		return false;
	}
	return true;
}

bool ParseOptions::checkPath() {

	if (SAUtils::FileExists(m_value.c_str()) == false) {
		m_error = Error::Path_not_found;
		return false;
	}
	return true;
}

bool ParseOptions::checkBool() {
	bool ret = (SAUtils::isTrueFalse(m_value) != BoolOption::Invalid);
	if (ret) {
		m_error = Error::Invalid_argument;
	}
	return ret;
}

bool ParseOptions::checkNumber() {
	bool ret = SAUtils::isNumber(m_value);
	if (!ret) {
		m_error = Error::Invalid_argument;
	}
	return ret;
}

bool ParseOptions::checkIPAddress()
{
	bool ret = validateIP(m_value);
	if (ret) {
		m_error = Error::Invalid_argument;
	}
	return ret;
}

bool ParseOptions::processArgs(const char* ov) {
	std::string optionValueString = ov;
	size_t pos = optionValueString.find_first_of('=');
	if (pos == std::string::npos) {
		return false;
	}
	m_option = optionValueString.substr(0, pos);
	m_value = optionValueString.substr(pos + 1, optionValueString.length() - 1);

	return true;
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
	Option ret = processLoggingOptions(m_option);
	switch (ret) {
	case Option::LOG_LEVEL:
		m_option = LOG_LEVEL_LABEL;
		if (iequals(setLogLevel(m_value), "UNKNOWN")) {
			return false;
		}
		return true;

	case Option::CONSOLE_LEVEL:
		m_option = CONSOLE_LEVEL_LABEL;
		if (iequals(setLogLevel(m_value), "UNKNOWN")) {
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
		m_option = HOOK_SCRIPTS_PATH_LABEL;
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

bool SetConfig::checkIPAddress(const char* ip) {

	if (validateIP(ip) == false) {
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
	if (!ret) {
		m_error = Error::Invalid_argument;
	}
	return ret;
}

bool SetConfig::checkNumber() {
	bool ret = SAUtils::isNumber(m_value);
	if (!ret) {
		m_error = Error::Invalid_argument;
	}
	return ret;
}

bool SetConfig::checkIPAddress()
{
	bool ret = validateIP(m_value);
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
	Option ret = processDerivativeOptions(m_option);
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
	case Option::COMMANDS_ENABLED:
		m_option = COMMANDS_ENABLED_LABEL;
		return checkBool();
	case Option::COMMANDS_PORT:
		m_option = COMMANDS_PORT_LABEL;
		return checkNumber();
	case Option::EVENTS_ENABLED:
		m_option = EVENTS_ENABLED_LABEL;
		return checkBool();
	case Option::EVENTS_PORT:
		m_option = EVENTS_PORT_LABEL;
		return checkNumber();
	case Option::EVENTS_ADDRESS:
		m_option = EVENTS_ADDRESS_LABEL;
		return checkIPAddress();
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
	else if (iequals(optionString, COMMANDS_ENABLED_LABEL)) {
		return Option::COMMANDS_ENABLED;
	}
	else if (iequals(optionString, EVENTS_ENABLED_LABEL)) {
		return Option::EVENTS_ENABLED;
	}
	else if (iequals(optionString, EVENTS_PORT_LABEL)) {
		return Option::EVENTS_PORT;
	}
	else if (iequals(optionString, EVENTS_ADDRESS_LABEL)) {
		return Option::EVENTS_ADDRESS;
	}
	return Option::UNKNOWN;
}

bool SetSettings::parseSettingsOptions(const char* s)
{
	std::string optionString = s;

	Option ret = processSettingsOptions(optionString);
	switch (ret) {
	case Option::GENERAL:
		m_value = GENERAL_LABEL;
		return true;
	case Option::LOGGING:
		m_value = LOGGING_LABEL;
		return true;
	case Option::NETWORK:
		m_value = NETWORK_LABEL;
		return true;
	case Option::FOLDERS:
		m_value = FOLDERS_LABEL;
		return true;
	case Option::MASTER:
		m_value = MASTER_LABEL;
		return true;
	case Option::DERIVATIVE:
		m_value = DERIVATIVE_LABEL;
		return true;
	case Option::BACKUP:
		m_value = BACKUP_LABEL;
		return true;
	case Option::EXIFTOOL:
		m_value = EXIFTOOL_LABEL;
		return true;
	case Option::ALL:
		m_value = ALL_LABEL;
		return true;
	default:
		return false;
	}
	return true;
}

Option SetSettings::processSettingsOptions(std::string& optionString)
{
	if (iequals(optionString, GENERAL_LABEL)) {
		return Option::GENERAL;
	}
	if (iequals(optionString, LOGGING_LABEL)) {
		return Option::LOGGING;
	}
	if (iequals(optionString, NETWORK_LABEL)) {
		return Option::NETWORK;
	}
	if (iequals(optionString, FOLDERS_LABEL)) {
		return Option::FOLDERS;
	}
	if (iequals(optionString, MASTER_LABEL)) {
		return Option::MASTER;
	}
	if (iequals(optionString, DERIVATIVE_LABEL)) {
		return Option::DERIVATIVE;
	}
	if (iequals(optionString, BACKUP_LABEL)) {
		return Option::BACKUP;
	}
	if (iequals(optionString, EXIFTOOL_LABEL)) {
		return Option::EXIFTOOL;
	}
	if (iequals(optionString, ALL_LABEL)) {
		return Option::ALL;
	}
	return Option::UNKNOWN;
}

bool SetSettings::parseAllowedOptions(const char* s)
{
	std::string optionString = s;

	Option ret = processAllowedOptions(optionString);
	switch (ret) {
	case Option::RAW:
		m_value = RAW_LABEL;
		return true;
	case Option::PICTURE:
		m_value = PICTURE_LABEL;
		return true;
	case Option::ALL:
		m_value = ALL_LABEL;
		return true;
	default:
		return false;
	}
	return true;
}

Option SetSettings::processAllowedOptions(std::string& optionString)
{
	if (iequals(RAW_LABEL, optionString)) {
		return Option::RAW;
	}
	if (iequals(PICTURE_LABEL, optionString)) {
		return Option::PICTURE;
	}
	if (iequals(ALL_LABEL, optionString)) {
		return Option::ALL;
	}
	return Option::UNKNOWN;
}

