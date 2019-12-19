#pragma once
#include <string>

enum class Option;

#define GENERAL_BLOCK	    "General"
#define LOGGING_BLOCK		"Logging"
#define NETWORK_BLOCK		"Network"
#define FOLDERS_BLOCK		"Folders" // This contains the folder paths that the system uses.
#define MASTER_BLOCK		"Master" // This archive contains the master images.This section controls this archive.
#define DERIVATIVE_BLOCK	"Derivative"
#define BACKUP_BLOCK		"Backup"
#define EXIFTOOL_BLOCK		"ExifTool" //External Exif Tool

class SetConfig
{
	std::string m_optionBlock;
	std::string m_option;
	std::string m_value;
	Option processGeneralOptions(std::string& optionString);
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

