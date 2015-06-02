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

#ifndef CONFIGREADER_H_
#define CONFIGREADER_H_
#include <map>
#include <string>

namespace simplearchive {
/**
 * @brief This is used as a container for the key/value pair.
 *
 * This can be used as a base for any config files that use key/value pairs
 * for runtime options.
 * Derived classes can set the key/value. the default is '='
 */
class Config : public std::map<std::string, std::string> {
	/// @brief deliminator for the key / value pair.
	char m_delimChar;
	
public:
	Config() {
		m_delimChar = '=';
	};
	virtual ~Config() {};

	/// @brief Prints all the values in the Config Object
	/// used for FINEging
	void printAll();

	/// @brief Maps the key word with a value to be looked up in the configuration file.
	bool value(const char *key, std::string &value);

	/// Gets the deliminator character.
	const char getDelimChar() {
		return m_delimChar;
	}
	// Set the deliminator character.
	void setDelimChar(char delimChar) {
		m_delimChar = delimChar;
	}

	/// @brief This function is called by the ConfigReader to carry out any further
	/// processing in the map.


};

/**
 * @brief This class is the file reader for the Config Object.
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
		Include
	} Token;
	bool m_logging;
private:
	/// @brief parses a line of text
	ConfigReader::Token parse(const char *text, Config &config);
	std::string includePath(int pos, std::string line);
	int m_includeCnt;
	std::string m_path;
	/// @brief deliminator for the key / value pair.
	char m_delimChar;
public:
	/// @brief Constructor for class
	ConfigReader();

	
	/// @brief Destructor for the class.
	virtual ~ConfigReader();

	/// @brief This function attempts to read a configuration file.
	/// @param    datafile	File name to read.
	/// @param    config		Config class to be filled out.
	/// @return	returns true if read correctly.
	bool read(const char *datafile, Config &config);

	virtual bool process() {
		return true;
	}; 
	
	void setNoLogging() {
		m_logging = false;
	}
	 
};

class ConfigWriter {
public:
	/// @brief Constructor for class.
	ConfigWriter();

	/// @brief Destructor for the class.
	virtual ~ConfigWriter();

	bool add(const char *cmd, const char *options, Config &config);
	bool edit(const char *cmd, const char *options, Config &config);
	bool remove(const char *cmd, Config &config);
	/// @brief This function attempts to read a configuration file.
	/// @param    datafile	File name to write.
	/// @param    config		Config class to be written out.
	/// @return	returns true if read correctly.
	bool write(const char *datafile, Config &config);

};

} /* namespace simplearchive */
#endif /* CONFIGREADER_H_ */
