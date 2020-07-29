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
	ExifObject *process(const char *imageFile, const char* filename);
	//bool processLine()
	std::string getDate();
	std::string replace(std::string &commandLine, std::string &in, std::string &out);
	std::string replace(std::string &commandLine, std::string &in);
	bool findToken(std::string &str, const char *toklabel);
};

} /* namespace simplearchive */
#endif /* ExternalComand_H_ */
