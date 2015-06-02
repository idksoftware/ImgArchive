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

#ifndef CREATEARCHIVE_H_
#define CREATEARCHIVE_H_

namespace simplearchive {

#define CONFIG_PATH "/config"
#define TMP_PATH "/tmp"
#define DUPS_PATH "/dups"
#define SQLITEDB_PATH "/sqldb"
#define LOG_PATH "/log"
#define HISTORY_PATH "/history"
#define BACKUP_PATH "/backup"
#define SHADOW_PATH "/shadow"
#define TOOLS_PATH "/tools"
#define HOOKS_PATH "/hooks"

class CreateArchive {
	static bool makeFolder(const char *root, const char *folder);
	static bool createFile(const char *root, const char *folder, const char *filename,
														const char *array[], unsigned int size);
public:
	CreateArchive();
	virtual ~CreateArchive();
	static bool makeFolders(const char *root);

	static bool createConfigFiles(const char *root, const char *folder);
	static bool createHookFiles(const char *root, const char *folder);
};

} /* namespace simplearchive */
#endif /* CREATEARCHIVE_H_ */
