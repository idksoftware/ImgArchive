#pragma once
#include <string>

enum class Option;

class ParseOptions
{
public:
	enum class Error {
		Ok,
		Invalid_argument,
		Path_not_found,
		Unknown
	};
protected:
	std::string m_optionBlock;
	std::string m_option;
	std::string m_value;
	
	Error m_error;
	bool processArgs(const char* ov);
	bool checkPath(const char* path);
	bool checkIPAddress(const char* ip);
	bool checkBool();
	bool checkPath();
	bool checkNumber();
	bool checkIPAddress();
public:
	ParseOptions() = default;
	virtual ~ParseOptions() = default;

	std::string& getOption() {
		return m_option;
	}
	std::string& getValue() {
		return m_value;
	}
	std::string& getOptionBlock() {
		return m_optionBlock;
	}

	Error getError() {
		return m_error;
	}

	const char* errorString() {
		switch (m_error) {
		case Error::Ok:
			return "Ok";
		case Error::Invalid_argument:
			return "Invalid argument";
		case Error::Path_not_found:
			return "Path not found";
		case Error::Unknown:
			return "Unknown";
		default:
			break;
		}
		return "Unknown";
	}
};

class SetSettings : public ParseOptions {
	Option processSettingsOptions(std::string& optionString);
public:
	SetSettings() = default;
	virtual ~SetSettings() = default;
	bool parseSettingsOptions(const char* optionString);
};


class SetConfig
{
public:
	enum class Error {
		Ok,
		Invalid_argument,
		Path_not_found,
		Unknown
	};
private:
	std::string m_optionBlock;
	std::string m_option;
	std::string m_value;
	Option processGeneralOptions(std::string& optionString);
	Option processLoggingOptions(std::string& optionString);
	Option processFolderOptions(std::string& optionString);
	Option processExifToolOptions(std::string& optionString);
	Option processMasterOptions(std::string& optionString);
	Option processDerivativeOptions(std::string& optionString);
	Option processBackupOptions(std::string& optionString);
	Option processNetworkOptions(std::string& optionString);
	Error m_error;
	bool processArgs(const char* ov);
	bool checkPath(const char* path);
	bool checkIPAddress(const char* ip);
	bool checkBool();
	bool checkPath();
	bool checkNumber();
	bool checkIPAddress();
public:
	SetConfig() = default;
	virtual ~SetConfig() = default;
	bool parseGeneralOptions(const char* optionString);
	bool parseLoggingOptions(const char* optionString);
	bool parseFolderOptions(const char* optionString);
	bool parseExifToolOptions(const char* optionString);
	bool parseMasterOptions(const char* optionString);
	bool parseDerivativeOptions(const char* optionString);
	bool parseBackupOptions(const char* optionString);
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

	Error getError() {
		return m_error;
	}

	const char* errorString() {
		switch (m_error) {
		case Error::Ok:
			return "Ok";
		case Error::Invalid_argument:
			return "Invalid argument";
		case Error::Path_not_found:
			return "Path not found";
		case Error::Unknown:
			return "Unknown";
		default:
			break;
		}
		return "Unknown";
	}
};

