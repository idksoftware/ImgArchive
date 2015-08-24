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

#ifndef INTEGRITYMANAGER_H_
#define INTEGRITYMANAGER_H_
#include <string>

namespace simplearchive {

class IntegrityManager {
	static IntegrityManager *m_this;
	std::string m_archivePath;
	IntegrityManager();
public:
	
	virtual ~IntegrityManager();
	bool addDayFolder(const char *folderName);
	bool addFile(const char *folderPath, const char *fileName);
	bool makeList();
	bool validate();
	bool repair();
	static IntegrityManager &get(const char *archivePath);
	static IntegrityManager &get();
	void setArchivePath(const char* archivePath);
};

} /* namespace simplearchive */
#endif /* INTEGRITYMANAGER_H_ */
