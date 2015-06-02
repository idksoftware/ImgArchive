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

#ifndef CATALOG_H_
#define CATALOG_H_

#include <string>

namespace simplearchive {

#define CATALOG_FOLDER	"/.catalog"


/**
 * @brief This class manages the image folder list in the archive.
 * It records changes to the image folder and maintains an XML list
 * of folders.
 */

class Catalog {
	std::string m_catalogPath;
	std::string m_archivePath;
	bool makeXML();
public:
	Catalog(const char *catalogPath, const char *archivePath);
	virtual ~Catalog();

	bool addDayFolder(const char *folderName);
	bool incFolders(const char *folderName);
	bool incFiles(const char *folderName);
	bool makeList();
	bool makeFolder(std::string &fileFolder, std::string &filepath);
};

} /* namespace simplearchive */
#endif /* CATALOG_H_ */
