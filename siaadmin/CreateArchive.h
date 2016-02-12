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
#define LOG_PATH "/logs"
#define HISTORY_PATH "/history"
#define BACKUP_PATH "/backup"
#define SHADOW_PATH "/shadow"
#define TOOLS_PATH "/tools"
#define HOOKS_PATH "/hooks"

class CreateArchive {
	/**
	/* @brief Creates the folder using the the root and folder name.
	*/
	static bool makeFolder(const char *root, const char *folder);
	/**
	/* @brief Creates a file using the the folder and file name as the file path. The file is then filled
	/* with the containts of the char array.
	*/
	static bool createFile(const char *root, const char *folder, const char *filename,
														const char *array[], unsigned int size);

	static bool CreateArchive::createFile(const char *root, const char *folder, const char *filename, std::string &str);

	static std::string  makeConfigFile(const char *root, const char *workspace, const char *shadow);
public:
	CreateArchive();
	virtual ~CreateArchive();
	/**
	/* @brief Creates the configuration folders.
	*/
	static bool makeFolders(const char *root);
	/**
	/* @brief Creates the default configuration files.
	*/
	static bool createConfigFiles(const char *root, const char *folder, const char *workspace, const char *shadow);
	/**
	/* @brief Creates the default hook files.
	*/
	static bool createHookFiles(const char *root, const char *folder);
	/**
	/* @brief Creates the enviroment variable SIA_HOME the the path given.
	*/
	static bool createHomeEnvVar(const char *root, bool users);
};

} /* namespace simplearchive */
#endif /* CREATEARCHIVE_H_ */
