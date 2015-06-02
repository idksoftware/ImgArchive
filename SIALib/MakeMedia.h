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

#ifndef MEDIAMAKER_H_
#define MEDIAMAKER_H_
#include "ExifDateTime.h"
#include "AppPaths.h"

namespace simplearchive {

class MakeMedia {
	bool m_all;
	int m_sizeOfMedia;
	ExifDateTime m_startDate;
	ExifDateTime m_endDate;
	std::string m_archivePath;
	std::string m_distPath;
public:
	MakeMedia() {};
	virtual ~MakeMedia();

	bool initalise(const char *archivePath, const char *distPath, unsigned long sizeOfMedia);
	bool initalise(const char *archivePath, const char *distPath, unsigned long sizeOfMedia, ExifDateTime &startDate, ExifDateTime &endDate);
	
	bool process();
	bool clearBackup();
};

} /* namespace simplearchive */
#endif /* MEDIAMAKER_H_ */
