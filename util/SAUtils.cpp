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

#include "CIDKFile.h"
#include "CIDKFileFind.h"
#include "SAUtils.h"
#include <stdlib.h>
#include <istream>
#include <fstream>
#include <sys/types.h>
//#include <sys/stat.h>
//#include <dirent.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <sstream>
//#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <stdarg.h>
#else
	#include <sys/stat.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <memory>
#include "cport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

using namespace std;

const char *SIAAppException::what() const throw() {
	std::stringstream s;
	s << message << " line:" << __LINE__ << " file:" << __FILE__;
	std::string tmp = s.str();
	return tmp.c_str();
}


SAUtils::SAUtils() {
	// TODO Auto-generated constructor stub

}

SAUtils::~SAUtils() {
	// TODO Auto-generated destructor stub
}

std::string SAUtils::GetEnvironment(const std::string &key)
{
	char * var = getenv(key.c_str());
	std::string retval = "";
	if (var != NULL) {
		retval = var;
		return retval;
	}
	return retval;
}

bool SAUtils::FileExists(const char *filename)
{
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}

bool SAUtils::DirExists(const char *path) {
	struct stat info;

	if (stat(path,&info) != 0) {
		return false;
	}
	if (info.st_mode & S_IFDIR) {
		// S_ISDIR
		return true;
	}
	return false;
}

std::vector<std::string *> *SAUtils::getFiles(const char *dirpath) {
	
	//DIR *dir;
	//struct dirent *ent;
	std::vector<std::string *> *fileList = new std::vector<std::string *>();
	/*
	if ((dir = opendir(dirpath)) == NULL) {
		return false;
	}
	while ((ent = readdir(dir)) != NULL) {
		printf("%s", ent->d_name);
		fileList->push_back(new std::string(ent->d_name));
	}
	*/
	std::string dirpathstr(dirpath);
	dirpathstr = dirpathstr + "/*.*";
	CIDKFileFind fileFind(dirpathstr);
	fileFind.Open();
	if (fileFind.GotFile() == false) {
		return false;
	}
	do {
		fileList->push_back(new std::string(fileFind.GetFileName()));
	} while (fileFind.GetNext());

	return fileList;

}

FileList_Ptr SAUtils::getFiles_(const char *dirpath) {

	std::unique_ptr<std::vector<std::string>> fileList(new std::vector<std::string>);
	/*
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(dirpath)) == NULL) {
	return false;
	}
	while ((ent = readdir(dir)) != NULL) {
	printf("%s", ent->d_name);
	fileList->push_back(new std::string(ent->d_name));
	}
	*/
	std::string dirpathstr(dirpath);
	dirpathstr = dirpathstr + "/*.*";
	CIDKFileFind fileFind(dirpathstr);
	fileFind.Open();
	if (fileFind.GotFile() == false) {
		return fileList;
	}
	do {
		std::string tmp(fileFind.GetFileName());
		fileList->push_back(tmp);
	} while (fileFind.GetNext());

	return fileList;

}



bool SAUtils::IsFile(const char *path) {
	struct stat info;

	if (stat(path,&info) != 0) {
		return false;
	}
	if (!(info.st_mode & S_IFDIR)) {
		// S_ISDIR
		return true;
	}
	return false;
}

time_t SAUtils::createTime(const char *path) {
	struct stat info;

	if (stat(path, &info) != 0) {
		return 0;
	}
	return info.st_ctime;
}

time_t SAUtils::modTime(const char *path) {
	struct stat info;

	if (stat(path, &info) != 0) {
		return 0;
	}
	return info.st_mtime;
}

bool SAUtils::hasExt(const char *file) {
	std::string filestr = file;
	return hasExt(filestr);
}

bool SAUtils::hasExt(const std::string &file) {
	unsigned int i = 0;
	if ((i = (unsigned int)file.find_last_of(".")) == (unsigned int)-1) {
		return false;
	}
	if (i >= (file.length() - 1)) {
		// find . but no extention i.e "."
		return false;
	}
	return true;
}

std::string SAUtils::getExtention(const std::string &file) {

	std::string ext = file.substr(file.find_last_of(".") + 1);
	return ext;
}

std::string SAUtils::getExtention(const char *file) {
	std::string filestr = file;
	return getExtention(filestr);
}

std::string SAUtils::getFilePathNoExt(const char *file) {
	std::string filestr = file;
	return getFilePathNoExt(filestr);
}

std::string SAUtils::getFilePathNoExt(const std::string &file) {

	std::string name = file.substr(0,file.find_last_of("/"));
	return name;
}

std::string SAUtils::getFilenameNoExt(const std::string &file) {
	int sp = file.find_last_of("/");
	int ep = file.find_last_of(".");
	std::string name = file.substr(++sp);
	name = name.substr(0, name.find_last_of("."));
	return name;
}

std::string SAUtils::getFilenameNoExt(const char *file) {
	std::string filestr = file;
	return getFilenameNoExt(filestr);
}


std::string SAUtils::to_string(int i) {
	std::stringstream tmp;
	tmp << i;
	return std::string(tmp.str());

}

bool SAUtils::mkDir(const char *path) {
#ifdef _WIN32
	if (_mkdir(path) != 0) {
#else
	mode_t mode = 0777;
	if (mkdir(path, mode) != 0) {
#endif
		return false;
	}
	return true;
}

bool SAUtils::setHidden(const char *path) {
#ifdef _WIN32
	DWORD dwAttrs;
	
	dwAttrs = GetFileAttributes(path);
	if (dwAttrs == INVALID_FILE_ATTRIBUTES) {
		return false;
	}
	if (!(dwAttrs & FILE_ATTRIBUTE_HIDDEN))
	{
		SetFileAttributes(path, dwAttrs | FILE_ATTRIBUTE_HIDDEN);
	}
#endif
	return true;
}

#define BUFFER_SIZE	(1012 * 1012)
bool SAUtils::fileCompare(const char *filePath1, const char *filePath2) {
	std::ifstream file1(filePath1, std::ifstream::in | std::ifstream::binary);
	std::ifstream file2(filePath1, std::ifstream::in | std::ifstream::binary);

	if (!file1.is_open() || !file2.is_open()) {
		return false;
	}

	unsigned char *buffer1 = new unsigned char[BUFFER_SIZE]();
	unsigned char *buffer2 = new unsigned char[BUFFER_SIZE]();
	bool flag = true;
	do {


		file1.read((char *)buffer1, BUFFER_SIZE);
		file2.read((char *)buffer2, BUFFER_SIZE);
		if (file1.gcount() != file2.gcount()) {
			flag = false;
			break;
		}
		if (memcmp(buffer1, buffer2, file1.gcount()) != 0) {
			flag = false;
			break;
		}
	} while (file1.good() || file2.good());

	delete buffer1;
	delete buffer2;

	return true;
}

bool SAUtils::fileSize(const char *filePath, unsigned long *size) {
	struct stat info;

	if (stat(filePath, &info) != 0) {
		return false;
	}
	*size = info.st_size;
	return true;
}


bool SAUtils::copy(const char *from, const char *to) {
	char buf[BUFSIZ];
	size_t size;

	FILE *source = nullptr;
	fopen_p(source, from, "rb");
	if (source == nullptr) {
		return false;
	}
	FILE *dest = nullptr;
	fopen_p(dest, to, "wb");
	if (dest == nullptr) {
		return false;
	}
	while ((size = fread(buf, 1, BUFSIZ, source)) > 0) {
		fwrite(buf, 1, size, dest);
	}
	fclose(source);
	fclose(dest);

	return true;
}

bool SAUtils::verify(const char *from, const char *to) {
	return fileCompare(from, to);
}

/*
void SAUtils::sprintf(std::string &s, const std::string fmt, ...)
{
	int n, size = 100;
	bool b = false;
	va_list marker;

	while (!b)
	{
		s.resize(size);
		va_start(marker, fmt);
		n = vsnprintf((char*)s.c_str(), size, fmt.c_str(), marker);
		va_end(marker);
		if ((n>0) && ((b = (n<size)) == true)) s.resize(n); else size *= 2;
	}
}
*/

bool SAUtils::delDir(const char *path) {
#ifdef _WIN32
	if (_rmdir(path) != 0) {
#else
	mode_t mode = 0777;
	if (mkdir(path, mode) != 0) {
#endif
		return false;
	}
	return true;
}

bool SAUtils::delFile(const char *file) {
	if (::unlink(file) < 0) {
		return false;
	}
	return true;
}

bool SAUtils::makePath(const char *from, const char *to) {
	if (SAUtils::DirExists(from) == false) {
		if (SAUtils::mkDir(from) == false) {
			return false;
		}
	}
	std::string fullPath = from;
	fullPath += '/';
	fullPath += to;

	std::string curPath = from;
	bool last = false;
	std::string node;
	while (last != true) {
		unsigned int start = curPath.length();
		unsigned int end = 0;
		if ((end = fullPath.find_first_of("/", start+2)) == std::string::npos) {
			node = fullPath.substr(start + 1, (fullPath.length() - start) - 1);
			last = true;
		} else {
			node = fullPath.substr(start + 1, (end - 1) - (start));
		}
	

		curPath += '/';
		curPath += node;
		node.clear();
		if (SAUtils::DirExists(curPath.c_str()) == false) {
			if (SAUtils::mkDir(curPath.c_str()) == false) {
				return false;
			}
		}
	}
	return true;
}

bool SAUtils::makeLink(const char *file, const char *link) {
#ifdef _WIN32
	if (CreateSymbolicLink(link, file, 0x0) == false) {
		unsigned int err = GetLastError();
		printf("Error %d", err);
	}
#else
	if (symlink(file, link) < 0) {
		perror("symlink");
		return false;
	}
#endif
	return true;
}

std::string SAUtils::getFileContents(const char *filename)
{

	int count = 0;
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

std::string SAUtils::getYear(const char *path) {
	std::string fpath = path;
	return fpath.substr(0, 4);
}

std::string SAUtils::getFullRelativePath(const char *path) {
	std::string spath = getYear(path);
	spath += '/';
	spath += path;
	return spath;
}