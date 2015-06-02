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

#ifndef STAGINGMANAGER_H_
#define STAGINGMANAGER_H_
#include <string>
namespace simplearchive {

class StagingManager {
	std::string m_name;
	std::string m_stagingFolder;
	time_t m_time;
	std::string unqueNumger();
	bool txtFile(std::string &number, const char *path);
	bool readTxtFile(std::string &number, const char *path);
	std::string m_root;
public:
	StagingManager(const char *root, const char *name, const char *stagingFolder);
	bool put(const char *path);
	virtual ~StagingManager();

};

} /* namespace simplearchive */
#endif /* STAGINGMANAGER_H_ */
