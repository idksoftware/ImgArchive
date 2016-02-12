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

#include "LogName.h"
#include "SAUtils.h"
#include "CDate.h"
#include "ExifDate.h"
#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <iomanip>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

LogName::LogName() {
	// TODO Auto-generated constructor stub

}

LogName::~LogName() {
	// TODO Auto-generated destructor stub
}
int LogName::checkLogSize(const char *logPath, unsigned long maxSize) {
	unsigned long size = 0;
	if (SAUtils::fileSize(logPath, &size) == false) {
		return -1;
	}
	if (size > maxSize) {
		return 0;
	}
	return 1;
}
std::string LogName::fileTemplate(const char *preName) {
	//CDate& date = CDate::timeNow();
	ExifDate date;
	date.now();
	int year = date.getYear();
	int month = date.getMonth();
	int day = date.getDay();
	//printf("Time: %d %d %d", year, month, day);
	std::string fileMatch;
	if (preName != 0) {
		fileMatch += preName;
	}
	std::stringstream s;
	s << year << std::setw(2) << std::setfill('0') << month
			<< std::setw(2) << std::setfill('0') << day;
	fileMatch += s.str();
	return fileMatch;
}
std::string LogName::makeName(const char *logPath, const char *preName, const char *ext, int maxSize) {

	std::string fileMatch = fileTemplate(preName);
	bool found = false;
	int version = 0;
	FileList_Ptr filelist = SAUtils::getFiles_(logPath);
	if (filelist == nullptr || filelist->empty() == false) {

		for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string filenameItem = *i;
			//printf("File \"%s\"\n", filenameItem->c_str());
			std::string tmpExt = SAUtils::getExtention(filenameItem);
			//printf("Ext \"%s\"\n", tmpExt.c_str());
			if (tmpExt.compare(ext) != 0) {
				continue;
			}

			std::string tmpFileName = SAUtils::getFilenameNoExt(filenameItem);

			std::string nameDate = tmpFileName.substr(0, fileMatch.length());
			//printf("nameDate \"%s\"\n", nameDate.c_str());
			if (nameDate.compare(fileMatch) != 0) {
				continue;
			}
			std::string fileNumber = tmpFileName.substr(fileMatch.length(), tmpFileName.length());
			fileNumber = fileNumber.substr(1, fileNumber.length());
			int thisVer = atoi(fileNumber.c_str());
			if (thisVer > version) {
				version = thisVer;
			}

			//printf("fileNumber \"%s\"\n", fileNumber.c_str());

			found = true;
		}
	}
	
	std::string result;
	std::stringstream s;
	if (found == false) {

		s << fileMatch << "_0001." << ext;
	} else {

		s << fileMatch << '_' << std::setw(4) << std::setfill('0') << version << '.' << ext;
		if (maxSize != ALWAYS_CREATE) { // always create new filename.
			int res = checkLogSize(s.str().c_str(), maxSize * 1024);
			if (res == 1) {
				// less than max size
				result = s.str();
				return result;
			}
		}
		s.str(std::string());
		version++;
		s << fileMatch << '_' << std::setw(4) << std::setfill('0') << version << '.' << ext;
	}

	m_filename = s.str();
	result = logPath;
	result += '/'; result += m_filename;
	return result;


}

// Date string from filename
std::string LogName::dateString(CDate &date) {

	int year = date.getYear();
	int month = date.getMonth();
	int day = date.getDay();
	//printf("Time: %d %d %d", year, month, day);
	std::string dateString;

	std::stringstream s;
	s << year << std::setw(2) << std::setfill('0') << month
		<< std::setw(2) << std::setfill('0') << day;
	dateString += s.str();
	return dateString;
}

// todays date string
std::string LogName::dateStringToday() {
	CDate date = CDate::timeNow();
	return dateString(date);
}

} /* namespace simplearchive */
