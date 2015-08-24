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

#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <sstream>
//#include <dirent.h>
#include "CIDKFileFind.h"
#include "LogFilename.h"
#include "SAUtils.h"
#include "ExifDate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

LogFilename::LogFilename(const char *logfilePath) {

	m_logfilePath = logfilePath;
	m_maxSize = 256;
}

LogFilename::~LogFilename() {

}

time_t toDate(const char *str) {
	std::string datestr = str;
	int s = 0;
	int e = datestr.find_first_of('-');
	std::string numstr = datestr.substr(s, e);
	int m_year = strtol(numstr.c_str(), NULL, 10);
	s = e + 1;
	e = datestr.find_first_of('-', s);
	numstr = datestr.substr(s, e - s);
	int m_month = strtol(numstr.c_str(), NULL, 10);
	s = e + 1;
	e = datestr.length();
	numstr = datestr.substr(s, e - s);
	int m_day = strtol(numstr.c_str(), NULL, 10);
	time_t m_timenum;
	time(&m_timenum);
	struct tm timeinfo;
	gmtime_p(timeinfo, &m_timenum);
	timeinfo.tm_year = m_year - 1900;
	timeinfo.tm_mon = m_month - 1;
	timeinfo.tm_mday = m_day;
	m_timenum = mktime(&timeinfo);
	return m_timenum;
}


std::string LogFilename::lastfile() {

	int m_ver = 0;
	//DIR *dir;
	//struct dirent *ent;
	std::string seachpath = m_logfilePath;
	seachpath += "/*.log";
	/*
	if ((dir = opendir(seachpath.c_str())) == NULL) {
			return false;
	}
	ExifDate dateToday;

	std::string nFilename;
	while ((ent = readdir(dir)) != NULL) {

		std::string ext = SAUtils::getExtention(ent->d_name);
		if (ext.compare("log") != 0) {
			continue;
		}
		SAUtils::getExtention(ent->d_name);
		std::string tmp = SAUtils::getFilePathNoExt(ent->d_name);
		int pos = tmp.find_first_of("+");
		std::string dateOnly = tmp.substr(0, pos);

		std::string verOnly = tmp.substr(pos+1, tmp.length() - pos);
		ExifDate exifDate(dateOnly.c_str());
		//printf("%s %d %s\n", exifDate.toString().c_str(), exifDate.getTime(), verOnly.c_str());

		if (exifDate == dateToday) {
			int nver = strtol(verOnly.c_str(), NULL, 10);
			if (nver > m_ver) {
				m_ver = nver;
			}
			nFilename = ent->d_name;
			printf("Newest = %s\n", nFilename.c_str());
		}
	}
	*/
	std::string nFilename;
	ExifDate dateToday;
	CIDKFileFind fileFind(seachpath);
	fileFind.Open();
	if (fileFind.GotFile() == false) {
		std::string empty;
		return empty;
	}
	do {
		std::string entry = fileFind.GetFileName();
		std::string tmp = SAUtils::getFilePathNoExt(entry);
		int pos = tmp.find_first_of("+");
		std::string dateOnly = tmp.substr(0, pos);

		std::string verOnly = tmp.substr(pos + 1, tmp.length() - pos);
		
		ExifDate exifDate(toDate(dateOnly.c_str()));
		//printf("%s %d %s\n", exifDate.toString().c_str(), exifDate.getTime(), verOnly.c_str());

		if (exifDate == dateToday) {
			int nver = strtol(verOnly.c_str(), NULL, 10);
			if (nver > m_ver) {
				m_ver = nver;
			}
			nFilename = entry;
			printf("Newest = %s\n", nFilename.c_str());
		}
	} while (fileFind.GetNext());
	return nFilename;
}
const std::string LogFilename::filename() {
	ExifDate exifDate;
	std::stringstream tmp;
	tmp << exifDate.getYear() << '-' << exifDate.getMonth() << '-' << exifDate.getDay();
	std::string filename = tmp.str();
	std::string lastFilename = lastfile();
	char buf[10];
	if (m_ver > 0) {
		sprintf_s(buf, 10, "%d", m_ver);
		std::string path = m_logfilePath + '/' + filename + buf;
		path = path + ".log";
		return path;
	}
	std::string path = m_logfilePath + '/' + filename + "+1.log";
	return path;
	/*
	struct stat info;

	if (stat(path.c_str(),&info) != 0) {
		return false;
	}
	if (info.st_mode & S_IFDIR) {
		// S_ISDIR

	}
	*/

}


} /* namespace simplearchive */
