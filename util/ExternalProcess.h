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


#ifndef ExternalProcess_H_
#define ExternalProcess_H_

#include <string>

namespace simplearchive {
	/**
	* This enables
	*/
	class ExifObject;
	class ExternalProcess {

		std::string replaceToken(std::string &str, const char *tok, const char *repstr);
		std::string m_cmdLine;
	public:
		/// @brief Constructor for class.
		ExternalProcess();

		/// @brief Distructor for class.
		virtual ~ExternalProcess();
		bool init();
		virtual bool process(const char *imageFile);
		std::string getDate();
		std::string replace(std::string &commandLine, std::string &in, std::string &out);
	};

} /* namespace simplearchive */
#endif /* ExternalProcess_H_ */