/*
 * ExternalComand.h
 *
 *  Created on: May 28, 2014
 *      Author: wzw7yn
 */

#ifndef ExternalComand_H_
#define ExternalComand_H_

#include <string>

namespace simplearchive {
class CLogger;
class ExifObject;
class ExternalComand {
	std::string m_commandLine;
	std::string m_exifMapPath;
	std::string m_tempPath;
	std::string replaceToken(std::string &str, const char *tok, const char *repstr);
	const CLogger *m_logger;
public:
	/// @brief Constructor for class.
	ExternalComand(const char *tempPath);

	/// @brief Distructor for class.
	virtual ~ExternalComand();
	bool init(const char *externalCommandLine, const char *exifMapFile);
	ExifObject *process(const char *imageFile);
	std::string getDate();
	std::string replace(std::string &commandLine, std::string &in, std::string &out);
};

} /* namespace simplearchive */
#endif /* ExternalComand_H_ */
