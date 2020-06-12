#pragma once
#include <string>
#include <vector>

class OutputType {
public:
	enum class Type {
		plain,
		xml,
		json,
		html,
		unknown
	};
private:
	Type m_type;
public:
	OutputType() = default;
	virtual ~OutputType() = default;
	bool parse(const char* str);
	Type get();
};

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

	bool iequals(const std::string& a, const std::string& b);
	bool isNumber(const std::string& str);
	std::vector<std::string> split(const std::string& str, char delim);
	bool validateIP(std::string ip);


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

