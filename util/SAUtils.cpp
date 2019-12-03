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

#ifdef WIN32
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 
#endif

#include "CIDKFile.h"
#include "CIDKFileFind.h"
#include "SAUtils.h"
#include <cstdlib>
#include <istream>
#include <fstream>
#include <sys/types.h>
//#include <sys/stat.h>
//#include <dirent.h>
#include <cstdlib>
#include <cctype>
#include <stdio.h>
#include <vector>
#include <string.h>
#include <sstream>
#include <stdarg.h>  // For va_start, etc.
#include <memory>    // For std::unique_ptr
//#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <stdarg.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <errno.h>
#include "cport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

using namespace std;

const char *SIAException::what() throw() {
	std::stringstream s;
	s << message << " line:" << __LINE__ << " file:" << __FILE__;
	std::string tmp = s.str();
	message = s.str();
	return message.c_str();
}


SAUtils::SAUtils() {
	// TODO Auto-generated constructor stub

}

SAUtils::~SAUtils() {
	// TODO Auto-generated destructor stub
}

bool SAUtils::FileExists(const char *filename)
{
	struct stat buffer;
	if (stat(filename, &buffer) != 0) {
		return false;
	}
	return true;
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

/*
std::vector<std::string *> *SAUtils::getFiles(const char *dirpath) {
	
	//DIR *dir;
	//struct dirent *ent;
	std::vector<std::string *> *fileList = new std::vector<std::string *>();
	
	//if ((dir = opendir(dirpath)) == NULL) {
	//	return false;
	//}
	//while ((ent = readdir(dir)) != NULL) {
	//	printf("%s", ent->d_name);
	//	fileList->push_back(new std::string(ent->d_name));
	//}
	
	std::string dirpathstr(dirpath);
	dirpathstr = dirpathstr + "/*.*";
	CIDKFileFind fileFind(dirpathstr);
	fileFind.Open();
	if (fileFind.GotFile() == false) {
		delete fileList;
		return false;
	}
	do {
		fileList->push_back(new std::string(fileFind.GetFileName()));
	} while (fileFind.GetNext());

	return fileList;

}
*/

FileList_Ptr SAUtils::getFiles_(const char *dirpath) {

	//FileList_Ptr fileList(new std::vector<std::string>);
	FileList_Ptr fileList = std::make_unique<FileList>();
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
		fileList->emplace_back(tmp);
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

std::string SAUtils::getFilename(const std::string &filepath) {
	int sp = filepath.find_last_of("/");
	if (sp == string::npos) {
		sp = filepath.find_last_of("\\");
	}
	if (sp == string::npos) {
		return filepath;
	}
	std::string name = filepath.substr(++sp);
	return name;
}

std::string SAUtils::getFolder(const std::string &filepath) {
	int sp = filepath.find_last_of("/");
	if (sp == string::npos) {
		sp = filepath.find_last_of("\\");
	}
	if (sp == string::npos) {
		return filepath;
	}
	std::string name = filepath.substr(0, filepath.length() - (sp + 1));
	return name;
}

std::string SAUtils::getFilenameNoExt(const std::string &file) {
	int sp = file.find_last_of("/\\");
	
	std::string name;
	if (sp != -1) {
		name = file.substr(++sp);
	}
	const int ep = file.find_last_of('.');
	name = file.substr(0, ep);
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

bool SAUtils::isNumber(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
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
		if (memcmp(buffer1, buffer2, (size_t)file1.gcount()) != 0) {
			flag = false;
			break;
		}
	} while (file1.good() || file2.good());

	delete []buffer1;
	delete []buffer2;

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

bool SAUtils::rename(const char *from, const char *to) {
	int result = ::rename(from, to);
	perror("file error");
	return (result >= 0);
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
		printf("%s", strerror(errno));
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


#define TO_HEX(i) (i <= 9 ? '0' + i : 'a' - 10 + i)

void SAUtils::chartohex3(char *buffer, unsigned short x) {
	buffer[0] = TO_HEX(((x & 0x0F00) >> 8));
	buffer[1] = TO_HEX(((x & 0x00F0) >> 4));
	buffer[2] = TO_HEX( (x & 0x000F));
	buffer[3] = '\0';
}

void SAUtils::chartohex2(char *buffer, unsigned char x) {
	buffer[0] = TO_HEX(((x & 0x00F0) >> 4));
	buffer[1] = TO_HEX((x & 0x000F));
	buffer[2] = '\0';
}

// Note this MUST be a const char *format not std::string
std::string SAUtils::sprintf(const char *fmt, ...)
{
	
	
	
	int final_n, n = (strlen(fmt) * 2); // Reserve two times as much as the length of the fmt_str //
	std::string str;
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while (1) {
		//formatted.reset(new char[n]); // Wrap the plain char array into the unique_ptr
		auto formatted = std::make_unique<char[]>(n);
		strcpy(&formatted[0], fmt);
		va_start(ap, fmt);
		final_n = vsnprintf(&formatted[0], n, fmt, ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return std::string(formatted.get());
	
}


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
#ifdef _WIN32
		if ((end = fullPath.find_first_of("\\", start + 2)) == std::string::npos) {
#else
		if ((end = fullPath.find_first_of("/", start + 2)) == std::string::npos) {
#endif
//		if ((end = fullPath.find_first_of("/", start+2)) == std::string::npos) {
			node = fullPath.substr(start + 1, (fullPath.length() - start) - 1);
			last = true;
		} else {
			node = fullPath.substr(start + 1, (end - 1) - (start));
		}
	
#ifdef _WIN32
		curPath += '\\';
#else
		curPath += '/';
#endif
//		curPath += '/';
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

bool SAUtils::makePath(const char *to) {

	std::string fullPath = to;
	int idx = fullPath.find_first_of(':');
	std::string drive = fullPath.substr(0, idx + 1);
	bool last = false;
	unsigned int start = idx + 1;
	unsigned int end = 0;
	std::string curPath = drive;
	std::string node;
	while (last != true) {
#ifdef _WIN32
		if ((end = fullPath.find_first_of("\\", start + 2)) == std::string::npos) {
#else
		if ((end = fullPath.find_first_of("/", start + 2)) == std::string::npos) {
#endif
			node = fullPath.substr(start + 1, (fullPath.length() - start + 1));
			last = true;
		}
		else {
			node = fullPath.substr(start + 1, (end)-(start + 1));
		}
#ifdef _WIN32
		curPath += '\\';
#else
		curPath += '/';
#endif
		curPath += node;
		node.clear();
		if (DirExists(curPath.c_str()) == false) {
			if (mkDir(curPath.c_str()) == false) {
				return false;
			}
		}
		start = curPath.length();
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

void SAUtils::splitpath(const char *path, char *drive, char *dir, char *fname, char *ext) {
#ifdef _WIN32
	_splitpath_s(path, drive, 10, dir, 256, fname, 64, ext, 20);
#endif
}
/*
size_t driveNumberOfElements,
char * dir,
size_t dirNumberOfElements,
char * fname,
size_t nameNumberOfElements,
char * ext,
size_t extNumberOfElements
*/

int SAUtils::getFileContents(const char *filename, std::string &contents)
{

	int count = 0;
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		contents.resize((const unsigned int)in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return 0;
	}
	return errno;
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

bool SAUtils::mksymlink(const char *sourcePath, const char *destPath) {
	return true;
}

int SAUtils::stricmp(const char *a, const char *b) {
	int ca, cb;
	do {
		ca = (unsigned char)*a++;
		cb = (unsigned char)*b++;
		ca = tolower(toupper(ca));
		cb = tolower(toupper(cb));
	} while (ca == cb && ca != '\0');
	return ca - cb;
}


#ifdef _WIN32
bool SetEnv(const std::string& key, const std::string& value, bool all)
{
	HKEY   hkey;
	DWORD  dwDisposition;
	DWORD dwType, dwSize;
	LONG result;
	if (all) {
		char *regPath = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
		result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, regPath,
			0, NULL, 0, KEY_WRITE, NULL, &hkey, &dwDisposition);
	}
	else {
		char *regPath = "Environment";
		result = RegCreateKeyEx(HKEY_CURRENT_USER, regPath,
			0, NULL, 0, KEY_WRITE, NULL, &hkey, &dwDisposition);
	}

	if (result == ERROR_SUCCESS)
	{
		dwType = REG_SZ;
		dwSize = value.length() + 1;
		LONG setResult = RegSetValueEx(hkey, TEXT(key.c_str()), 0, dwType,
			(PBYTE)value.c_str(), dwSize);
		RegCloseKey(hkey);
		return setResult == ERROR_SUCCESS;
	}
	else
	{
		return false;
	}
}

#ifdef XXXXXX
bool GetEnv(const char *szSIAHome, std::string &resultStr, bool all)
{
	HKEY   hkey;
	DWORD  dwDisposition;
	DWORD dwType, dwSize;
	LONG result;
	/*
	if (all) {
		char *regPath = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
		result = RegCreateKeyEx(HKEY_LOCAL_MACHINE, regPath,
			0, NULL, 0, KEY_WRITE, NULL, &hkey, &dwDisposition);
	}
	else {
		char *regPath = "Environment";
		result = RegCreateKeyEx(HKEY_CURRENT_USER, regPath,
			0, NULL, 0, KEY_WRITE, NULL, &hkey, &dwDisposition);
	}
	*/
	char *regPath = "Environment";
	char path[MAX_PATH + 1];
	DWORD size = MAX_PATH;
	result = RegQueryValueEx(HKEY_CURRENT_USER, regPath, nullptr, nullptr, (LPBYTE)path, &size);
	resultStr = path;
	return true;
}
#endif

std::string SAUtils::GetPOSIXEnv(const std::string &key)
{
	char *var = nullptr;
#ifdef _WIN32
	size_t pReturnValue = 0;
	char buffer[2 * 1024];
	size_t numberOfElements = 2 * 1024;
	int errno = getenv_s(&pReturnValue, buffer, numberOfElements, key.c_str());
	if (errno == 0) {
		var = buffer;
	}

#else
	var = getenv(key.c_str());
#endif
	std::string retval;
	if (var != nullptr) {
		retval = var;
		return retval;
	}
	return retval;
}


std::string SAUtils::GetEnv(const std::string& value, bool all) {
	HKEY hKey = 0;
	char buf[MAX_PATH];
	DWORD dwType = 0;
	DWORD dwBufSize = MAX_PATH;
	std::string res;
	if (all) {
		const char* subkey = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, subkey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS) {
			return false;
		}
	}
	else {
		const char* subkey = "Environment";
		if (RegOpenKeyEx(HKEY_CURRENT_USER, subkey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS) {
			return false;
		}
	}
	if (RegQueryValueEx(hKey, value.c_str(), NULL, NULL, (BYTE*)buf, &dwBufSize) != ERROR_SUCCESS)
	//if (RegQueryValueEx(hKey, "SIA_HOME", NULL, NULL, (BYTE*)buf, &dwBufSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return res;
	}
	res = buf;
	RegCloseKey(hKey);
	return res;
}



#else

std::string SAUtils::GetEnv(const std::string &key, bool all)
{
	char * var = getenv(key.c_str());
	std::string retval;
	if (var != NULL) {
		retval = var;
		return retval;
	}
	return retval;
}

bool SAUtils::SetEnv(const std::string &key, const std::string &value, bool all)
{
	if (setenv(key.c_str(), value.c_str(), 1) != 0) {
		return false;
	}
	return true;
}

std::string SAUtils::GetPOSIXEnv(const std::string &key)
{
	return GetEnv(key, true);
}
#endif


