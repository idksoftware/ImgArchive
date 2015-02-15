/*
 * Mirror.h
 * @file Mirror.h
 * @brief This file contains all the Mirror functions
 *  Created on: Oct 13, 2014
 *      Author: wzw7yn
 */

#ifndef MIRROR_H_
#define MIRROR_H_
//#include <iostream>
#include <string>
#include <exception>

namespace simplearchive {

class MirrorException : public std::exception {
	std::string message;
public:
	explicit MirrorException(const char *msg) : message(msg){

	}
	explicit MirrorException(std::string msg) : message(msg) {}
	virtual ~MirrorException() throw() {}
	virtual const char *what() const throw() {
		return message.c_str();
	}
};



class MirrorItemContainer;
class StagingManager;

class Mirror {
	static Mirror *m_this;
	std::string m_confpath;
	bool readConf();
	static MirrorItemContainer *m_pContainer;
	std::string m_root;
	std::string makeFileName(const char *path);
	Mirror();
public:

	virtual ~Mirror();
	bool FileExists(const char *filename);
	bool DirExists(const char *path);
	bool IsFile(const char *path);
	bool mkDir(const char *path);
	bool copy(const char *to);
	bool syncFile(const char *to);
	bool syncDir(const char *to);
	bool mapFileVerify(const char *path);
	bool verifyMirrorName(const char *to);
	bool verifySourceName(const char *to);
	bool verifyContents(const char *to);
	bool makeCheckfile(const char *path);

	bool verifyMirrors();
	bool verifySource();

	static Mirror &get();

	static bool initalise(const char *rootPath, const char *confpath);
};

} /* namespace simplearchive */
#endif /* MIRROR_H_ */
