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

#ifndef CONFIGREADER_H
#define CONFIGREADER_H
#include <map>
#include <string>
#include <memory>

namespace simplearchive {
/**
 * @brief This is used as a container for the key/value pair.
 *
 * This can be used as a base for any config files that use key/value pairs
 * for runtime options.
 * Derived classes can set the key/value. the default is '='
 */
class ConfigBlock : public std::map<std::string, std::string> {
	/// @brief deliminator for the key / value pair.
	char m_delimChar;
	std::string m_name;
public:
	ConfigBlock() {
		m_delimChar = '=';
		m_name = "root";
	};
	ConfigBlock(const char *name) {
		m_delimChar = '=';
		m_name = name;
	};
	virtual ~ConfigBlock() {};

	/// @brief Prints all the values in the Config Object
	/// used for FINEging
	void printAll();

	/// @brief Maps the key word with a value to be looked up in the configuration file.
	bool value(const char *key, std::string &value);

	/// Gets the deliminator character.
	const char getDelimChar() noexcept {
		return m_delimChar;
	}
	// Set the deliminator character.
	void setDelimChar(char delimChar) noexcept {
		m_delimChar = delimChar;
	}

	/// @brief This function is called by the ConfigReader to carry out any further
	/// processing in the map.
	friend std::ostream& operator<< (std::ostream& stream, const ConfigBlock& config);

};

std::ostream& operator<< (std::ostream& stream, const ConfigBlock& config);

#define ROOT_BLOCK			"Global"
#define GENERAL_BLOCK	    "General"
#define LOGGING_BLOCK		"Logging"
#define NETWORK_BLOCK		"Network"
#define FOLDERS_BLOCK		"System Folders" // This contains the folder paths that the system uses.
#define MASTER_BLOCK		"Master Archive" // This archive contains the master images.This section controls this archive.
#define DERIVATIVE_BLOCK	"Derivative Archive"
#define BACKUP_BLOCK		"Archive Backup"
#define EXIFTOOL_BLOCK		"External Exif Tool" //External Exif Tool



class AppConfigBase : public std::map<std::string, std::shared_ptr<ConfigBlock>> {
public:
	/// @brief Prints all the values in the Config Object
	/// used for FINEging
	AppConfigBase() {
		//this->[ROOT_BLOCK] = (std::make_shared<ConfigBlock>(ROOT_BLOCK));
		insert(std::pair<std::string, std::shared_ptr<ConfigBlock>>(ROOT_BLOCK, std::make_shared<ConfigBlock>(ROOT_BLOCK)));
	}
	void printAll();
	bool setConfigBlock(const char* name, std::string &value, std::string &defaultValue, const char *configBlock);

	std::shared_ptr<ConfigBlock> getConfigBlocks(const char *name);
	friend std::ostream& operator<< (std::ostream& stream, const AppConfigBase& config);
};

std::ostream& operator<< (std::ostream& stream, const AppConfigBase& config);
/**
 * @brief This class is the file reader for the ConfigBLock Object.
 *
 * This class reads the configuration file passed looking for keyword - value pairs.
 * these are stored in a map object which is what class Config is derived. Any derived
 * classes of class Config can then read the key pairs from the map.
*/
class ConfigReader {
public:
	typedef enum {
		Error = -1,
		KeyValue,
		Comment,
		NewBlock,
		Include
	} Token;
	bool m_logging;
	int m_currentLineNumber;
protected:
	/// @brief parses a line of text
	ConfigReader::Token parse(const char *text, ConfigBlock &config);
	ConfigReader::Token parseExif(const char *text, ConfigBlock &config);
	std::string includePath(int pos, std::string line);
	int m_includeCnt;
	std::string m_path;
	std::string m_rootPath;
	/// @brief deliminator for the key / value pair.
	char m_delimChar;
	std::string m_blockName;
public:
	/// @brief Constructor for class
	ConfigReader();

	
	/// @brief Destructor for the class.
	virtual ~ConfigReader();

	/// @brief This function attempts to read a configuration file.
	/// @param    datafile	File name to read.
	/// @param    config		Config class to be filled out.
	/// @return	returns true if read correctly.
	bool read(const char *datafile, ConfigBlock &config);
	bool read(const char* path, const char* datafile, ConfigBlock& config);
	/// @brief This function attempts to read a configuration file.
	/// @param    str	string to read.
	/// @param    config		Config class to be filled out.
	/// @return	returns true if read correctly.
	bool read(const std::string &str, ConfigBlock &config);
	
	bool readExif(const std::string &str, ConfigBlock &config);
	virtual bool process() noexcept {
		return true;
	}; 
	
	void setNoLogging() noexcept {
		m_logging = false;
	}
	 
	int getCurrentLineNumber() noexcept {
		return m_currentLineNumber;
	}
};

/**
* @brief This class is the file reader for a list of ConfigBLock Objects.
*
* This class reads the configuration file passed looking for keyword - value pairs.
* these are stored in a map object which is what class Config is derived. Any derived
* classes of class Config can then read the key pairs from the map.
*/
class AppConfigReader : public ConfigReader {
	std::shared_ptr<ConfigBlock> m_current;
	
public:
	bool read(const char *datafile, AppConfigBase &config);
	//ConfigReader::Token parse(const char *text, AppConfig &config);
};

class ConfigBlockWriter {

public:
	/// @brief Constructor for class.
	ConfigBlockWriter() = default;

	/// @brief Destructor for the class.
	virtual ~ConfigBlockWriter() = default;

	bool add(const char* cmd, const char* options, ConfigBlock& config);
	bool edit(const char* cmd, const char* options, ConfigBlock& config);
	bool update(const char* cmd, const char* options, ConfigBlock& config);

	/// @brief This function attempts to read a configuration file.
	/// @param    cmd			.
	/// @param    config		Config class to be written out.
	/// @return	returns true if read correctly.
	bool remove(const char *cmd, ConfigBlock &config);
	
	/// @brief This function attempts to read a configuration file.
	/// @param    datafile	File name to write.
	/// @param    config		Config class to be written out.
	/// @return	returns true if read correctly.
	bool write(const char *datafile, ConfigBlock &config);

};

class ConfigWriter {
	ConfigBlock& m_configBlock;
public:
	ConfigWriter(ConfigBlock& c) : m_configBlock(c) {};
	virtual ~ConfigWriter() = default;

	bool update(const char* option, const char* value);
	bool remove(const char* option);
	bool write(const char* datafile);
};


class AppConfigBaseWriter {
	AppConfigBase& m_config;
public:
	/// @brief Constructor for class.
	AppConfigBaseWriter(AppConfigBase& c) : m_config(c) {};

	/// @brief Destructor for the class.
	virtual ~AppConfigBaseWriter() = default;

	bool update(const char* blockName, const char* cmd, const char* options);
	
	/// @brief This function attempts to read a configuration file.
	/// @param    cmd			.
	/// @param    config		Config class to be written out.
	/// @return	returns true if read correctly.
	bool remove(const char* cmd, ConfigBlock& config);

	/// @brief This function attempts to read a configuration file.
	/// @param    datafile	File name to write.
	/// @param    config		Config class to be written out.
	/// @return	returns true if read correctly.
	//bool load(AppConfigBase& config);
	bool write(const char* datafile, AppConfigBase& config);
};

} /* namespace simplearchive */
#endif /* CONFIGREADER_H_ */
