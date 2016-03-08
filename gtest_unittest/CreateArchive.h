/*
 * CreateArchive.h
 *
 *  Created on: Mar 9, 2015
 *      Author: wzw7yn
 */

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
#define SHADOW_PATH "/SIAShadow"
#define TOOLS_PATH "/tools"
#define HOOKS_PATH "/hooks"
#define TEMPLATE_PATH "/template"


class CreateArchive {
	static bool makeFolder(const char *root, const char *folder);
	static bool createFile(const char *root, const char *folder, const char *filename,
														const char *array[], unsigned int size);
public:
	CreateArchive();
	virtual ~CreateArchive();
	static bool makeFolders(const char *root);

	static bool all(const char *root);
	static bool createConfigFiles(const char *root, const char *folder);
	static bool createHookFiles(const char *root, const char *folder);
	static bool createReadme(const char *root);
};

} /* namespace simplearchive */
#endif /* CREATEARCHIVE_H_ */
