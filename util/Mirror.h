/*
 * Mirror.h
 *
 *  Created on: Oct 13, 2014
 *      Author: wzw7yn
 */

#ifndef MIRROR_H_
#define MIRROR_H_
#include <string>
namespace simplearchive {

class MirrorItemContainer;
class StagingManager;

class Mirror {
	static Mirror *m_this;
	std::string m_confpath;
	bool readConf();
	MirrorItemContainer *m_pContainer;
	std::string m_root;
public:
	Mirror();
	virtual ~Mirror();
	bool FileExists(const char *filename);
	bool DirExists(const char *path);
	bool IsFile(const char *path);
	bool mkDir(const char *path);
	bool copy(const char *to);
	bool verify(const char *to);
	static Mirror &get();

	static void initalise(const char *rootPath, const char *confpath);
};

} /* namespace simplearchive */
#endif /* MIRROR_H_ */
