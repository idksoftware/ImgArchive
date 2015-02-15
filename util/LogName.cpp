/*
 * LogName.cpp
 *
 *  Created on: Sep 23, 2014
 *      Author: wzw7yn
 */

#include "LogName.h"
#include "SAUtils.h"
#include "ExifDate.h"
#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <iomanip>

namespace simplearchive {

LogName::LogName() {
	// TODO Auto-generated constructor stub

}

LogName::~LogName() {
	// TODO Auto-generated destructor stub
}
int LogName::checkLogSize(const char *logPath, int maxSize) {
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
	std::vector<std::string *> *filelist = SAUtils::getFiles(logPath);
	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *filenameItem = *i;
		//printf("File \"%s\"\n", filenameItem->c_str());
		std::string tmpExt = SAUtils::getExtention(*filenameItem);
		//printf("Ext \"%s\"\n", tmpExt.c_str());
		if (tmpExt.compare(ext) != 0) {
			continue;
		}

		std::string tmpFileName = SAUtils::getFilenameNoExt(*filenameItem);

		std::string nameDate = tmpFileName.substr(0, fileMatch.length());
		//printf("nameDate \"%s\"\n", nameDate.c_str());
		if (nameDate.compare(fileMatch) != 0) {
			continue;
		}
		std::string fileNumber = tmpFileName.substr(fileMatch.length(),tmpFileName.length());
		fileNumber = fileNumber.substr(1,fileNumber.length());
		int thisVer = atoi(fileNumber.c_str());
		if (thisVer > version) {
			version = thisVer;
		}

		//printf("fileNumber \"%s\"\n", fileNumber.c_str());

		found = true;
	}

	std::string result;
	std::stringstream s;
	if (found == false) {

		s << logPath << '/' << fileMatch << "_0001." << ext;
	} else {

		s << logPath << '/' << fileMatch << '_' << std::setw(4) << std::setfill('0') << version << '.' << ext;
		int res = checkLogSize(s.str().c_str(), maxSize * 1024);
		if (res == 1) {
			// less than max size
			result = s.str();
			return result;
		}
		s.str(std::string());
		version++;
		s << logPath << '/' << fileMatch << '_' << std::setw(4) << std::setfill('0') << version << '.' << ext;
	}
	result = s.str();
	return result;


}

} /* namespace simplearchive */
