#pragma once
#include <string>

enum class Option;

/*
//constexpr auto  CONFIG_GENERAL_BLOCK    = "General";
constexpr auto  CONFIG_LOGGING_BLOCK    = "Logging";
constexpr auto  CONFIG_NETWORK_BLOCK    = "Network";
//constexpr auto  CONFIG_FOLDERS_BLOCK    = "Folders"; // This contains the folder paths that the system uses.
constexpr auto  CONFIG_MASTER_BLOCK     = "Master"; // This archive contains the master images.This section controls this archive.
constexpr auto  CONFIG_DERIVATIVE_BLOCK = "Derivative";
constexpr auto  CONFIG_BACKUP_BLOCK     = "Backup";
constexpr auto  CONFIG_EXIFTOOL_BLOCK   = "ExifTool"; //External Exif Tool
*/

class SetConfig
{
	std::string m_optionBlock;
	std::string m_option;
	std::string m_value;
	Option processGeneralOptions(std::string& optionString);
	Option processFolderOptions(std::string& optionString);
	bool processArgs(const char* ov);
public:
	SetConfig() = default;
	bool parseGeneralOptions(const char* optionString);
	bool parseFolderOptions(const char* optionString);
	bool parseExifToolOptions(const char* optionString);
	bool parseMasterOptions(const char* optionString);
	bool parseNetworkOptions(const char* optionString);
	std::string& getOption() {
		return m_option;
	}
	std::string& getValue() {
		return m_value;
	}
	std::string& getOptionBlock() {
		return m_optionBlock;
	}
};

