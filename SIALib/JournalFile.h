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

#ifndef CSVJOURNALFILE_H_
#define CSVJOURNALFILE_H_

#include <memory>
#include <vector>
#include "ExifDateTime.h"
namespace simplearchive {


class JournalData;
class CSVJournalFile : private std::vector<JournalData> {


public:
	CSVJournalFile();
	virtual ~CSVJournalFile();
	bool writeCSV(const char *path);
	bool writeXML(const char *path);
	bool add(int id, const char *name, unsigned int size, unsigned long crc,
							const char *md5, ExifDateTime &created, ExifDateTime &modified);
	void clearUp();
};

} /* namespace simplearchive */
#endif /* CSVJOURNALFILE_H_ */
