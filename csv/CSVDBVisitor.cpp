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

#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <cstdlib>

#include "SAUtils.h"
//#include "CSVArgs.h"
#include "CSVDBVisitor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

CSVDBVisitor::CSVDBVisitor(CSVDBAction *folderVisitor) {
	m_folderVisitor = folderVisitor;

}

CSVDBVisitor::~CSVDBVisitor() {
	// TODO Auto-generated destructor stub
}

bool CSVDBVisitor::process(const char *rootFolder) {
	// make the root folder
	std::string path = rootFolder;

	m_folderVisitor->onStart();
	// read years in Master folder
	FileList_Ptr filelist = SAUtils::getFiles_(path.c_str());
	for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string year = *i;
		char c = (year)[0];
		if (c == '.' ) {
			continue;
		}
		// read day folders for this year in Master folder
		std::string yearMaster = path;
		m_folderVisitor->onYearFolder(year.c_str());
		yearMaster += '/';
		yearMaster += year;
		FileList_Ptr filelist = SAUtils::getFiles_(yearMaster.c_str());
		for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string dayfolder = *i;
			char c = (dayfolder)[0];
			if (c == '.' ) {
				continue;
			}

			m_folderVisitor->onDayFolder(dayfolder.c_str());
			std::string dayFolderMaster = yearMaster;
			dayFolderMaster += '/';
			dayFolderMaster += dayfolder;

			FileList_Ptr filelist = SAUtils::getFiles_(dayFolderMaster.c_str());
			for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
				std::string imageFile = *i;
				char c = (imageFile)[0];
				if (c == '.' ) {
					continue;
				}
				//printf("\t\tImage %s: \n", imageFile->c_str());
				std::string imagePath = dayFolderMaster;
				m_folderVisitor->onDBFile(dayFolderMaster.c_str(), imageFile.c_str());
			}
			m_folderVisitor->onDayEnd();
		}
		m_folderVisitor->onYearEnd();

	}
	m_folderVisitor->onEnd();
	return true;

}


} /* namespace simplearchive */
