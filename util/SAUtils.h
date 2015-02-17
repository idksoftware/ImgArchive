/*
 * Utils.h
 *
 *  Created on: May 7, 2014
 *      Author: wzw7yn
 */

#ifndef UTILS_H_
#define UTILS_H_


#include <string>
#include <vector>

#ifdef WIN32

#define unlink _unlink

#endif


class SAUtils {
public:
	SAUtils();
	virtual ~SAUtils();
	static std::string GetEnvironment(const std::string &key);
	static bool FileExists(const char *filename);
	static bool DirExists(const char *path);
	static bool IsFile(const char *path);
	static time_t createTime(const char *path);
	static time_t modTime(const char *path);
	static std::vector<std::string *> *getFiles(const char *dirpath);
	static std::string getExtention(const std::string &file);
	static std::string getExtention(const char *file);
	static std::string getFilePathNoExt(const std::string &file);
	static std::string getFilePathNoExt(const char *file);
	static std::string getFilenameNoExt(const std::string &file);
	static std::string getFilenameNoExt(const char *file);
	static bool fileCompare(const char *file1, const char *file2);
	static bool fileSize(const char *filePath, unsigned long *size);
	static bool hasExt(const char *file);
	static bool hasExt(const std::string &file);
	static bool mkDir(const char *path);
	static bool setHidden(const char *path);
	static bool copy(const char *from, const char *to);
	static bool verify(const char *from, const char *to);
	static bool delFile(const char *file);
	static bool delDir(const char *file);
	static std::string to_string(int i);
	static void sprintf(std::string &s, const std::string fmt, ...);
	static bool makePath(const char *from, const char *to);
	static bool SAUtils::makeLink(const char *file, const char *link);
};


#endif /* UTILS_H_ */
