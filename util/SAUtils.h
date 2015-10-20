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

#ifndef UTILS_H_
#define UTILS_H_


#include <string>
#include <vector>
#include <memory>
#include "global.h"

#ifdef WIN32

#define unlink _unlink

#endif


class SIAAppException : public std::exception {
	std::string message;
public:
	explicit SIAAppException(const char *msg) : message(msg){

	}
	explicit SIAAppException(std::string msg) : message(msg) {}
	virtual ~SIAAppException() throw() {}
	virtual const char *what() const throw();

};



typedef std::vector<std::string> FileList;
typedef std::unique_ptr<FileList> FileList_Ptr;
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
	static FileList_Ptr getFiles_(const char *dirpath);
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
	static bool rename(const char *from, const char *to);
	static bool verify(const char *from, const char *to);
	static bool delFile(const char *file);
	static bool delDir(const char *file);
	static std::string to_string(int i);
	static void sprintf(std::string &s, const std::string &fmt, ...);
	static bool makePath(const char *from, const char *to);
	static bool makeLink(const char *file, const char *link);
	static std::string getFileContents(const char *filename);
	static std::string getYear(const char *path);
	static std::string getFullRelativePath(const char *path);
};


#endif /* UTILS_H_ */
