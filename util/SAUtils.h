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

#ifdef WIN32
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 
#endif

#include <string>
#include <vector>
#include <memory>
#include "siaglobal.h"

#ifdef _WIN32
#define MAX_PATH	260
#define unlink _unlink

#endif

#define DEBUG_PRINT printf

class SIAException : public std::exception {
	std::string message;
public:
	explicit SIAException(const char *msg) : message(msg) {}
	explicit SIAException(std::string msg) : message(msg) {}
	virtual ~SIAException() throw() {};
	virtual const char *what() throw();

};



using FileList = std::vector<std::string>;
using FileList_Ptr = std::unique_ptr<FileList>;



class SAUtils {
public:
	SAUtils();
	virtual ~SAUtils();
	
	static bool FileExists(const char *filename);
	static bool DirExists(const char *path);
	static bool IsFile(const char *path);
	static time_t createTime(const char *path);
	static time_t modTime(const char *path);
	//static std::vector<std::string *> *getFiles(const char *dirpath);
	static FileList_Ptr getFiles_(const char *dirpath);
	static std::string getExtention(const std::string &file);
	static std::string getExtention(const char *file);
	static std::string getFilePathNoExt(const std::string &file);
	static std::string getFilePathNoExt(const char *file);
	static std::string getFilenameNoExt(const std::string &file);
	static std::string getFilenameNoExt(const char *file);
	static std::string getFilename(const std::string &filepath);
	static std::string getFolder(const std::string &filepath);
	static bool fileCompare(const char *file1, const char *file2);
	static bool fileSize(const char *filePath, unsigned long *size);
	static bool hasExt(const char *file);
	static bool hasExt(const std::string &file);
	static bool mkDir(const char *path);
	static bool isNumber(const std::string& s);
	static bool setHidden(const char *path);
	static bool copy(const char *from, const char *to);
	static bool rename(const char *from, const char *to);
	static bool verify(const char *from, const char *to);
	static bool delFile(const char *file);
	static bool delDir(const char *file);
	static std::string to_string(int i);
	static void chartohex2(char *buffer, unsigned char x);
	static void chartohex3(char *buffer, unsigned short x);
	static std::string sprintf(const char *fmt, ...);
	static bool makePath(const char *from, const char *to);
	static bool makePath(const char *to);
	static bool makeLink(const char *file, const char *link);
	static int getFileContents(const char *filename, std::string &contents);
	static std::string getYear(const char *path);
	static std::string getFullRelativePath(const char *path);
	static void splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);
	static bool mksymlink(const char *sourcePath, const char *destPath);
	static int stricmp(const char *a, const char *b);
	static std::string GetEnv(const std::string &key, bool all = true);
	static std::string GetPOSIXEnv(const std::string &key);
	static bool SetEnv(const std::string &key, const std::string &value, bool all = true);

};


#endif /* UTILS_H_ */
