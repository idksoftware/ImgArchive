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
#include "ArchiveVisitor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	ArchiveVisitor::ArchiveVisitor(AVAction *folderVisitor) {
		m_folderVisitor = folderVisitor;

	}

	ArchiveVisitor::ArchiveVisitor(AVAction *folderVisitor, const char *archivePath) {
		m_folderVisitor = folderVisitor;
		m_archivePath = archivePath;
	}

	ArchiveVisitor::ArchiveVisitor(AVAction *folderVisitor, const char *archivePath, const char *workspacePath) {
		m_folderVisitor = folderVisitor;
		m_archivePath = archivePath;
		m_workspacePath = workspacePath;
	}

	ArchiveVisitor::~ArchiveVisitor() {
		// TODO Auto-generated destructor stub
	}

	bool ArchiveVisitor::workspace(const char *rootFolder) {
		// make the root folder
		std::string path = rootFolder;

		m_folderVisitor->onStart();
		FileList_Ptr filelist = SAUtils::getFiles_(path.c_str());
		for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string year = *i;
			char c = (year)[0];
			if (c == '.') {
				continue;
			}
			//if (!isdigit(atoi(year->c_str()))) {
			//	continue;
			//}
			// read day folders for this year in Master folder
			std::string yearMaster = path;
			m_folderVisitor->onYearFolder(year.c_str());
			yearMaster += '/';
			yearMaster += year;
			FileList_Ptr filelist = SAUtils::getFiles_(yearMaster.c_str());
			for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
				std::string dayfolder = *i;
				char c = (dayfolder)[0];
				if (c == '.') {
					continue;
				}

				m_folderVisitor->onDayFolder(dayfolder.c_str());
				std::string dayFolderMaster = yearMaster;
				dayFolderMaster += '/';
				dayFolderMaster += dayfolder;
				dayFolderMaster += "/data";

				FileList_Ptr filelist = SAUtils::getFiles_(dayFolderMaster.c_str());
				for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
					std::string imageFile = *i;
					char c = (imageFile)[0];
					if (c == '.') {
						continue;
					}
					if (SAUtils::getExtention(imageFile).compare("ver") == 0) {
						continue;
					}
					//printf("\t\tImage %s: \n", imageFile->c_str());
					std::string imagePath = dayFolderMaster;
					m_folderVisitor->onImage(dayFolderMaster.c_str(), imageFile.c_str());
				}
				m_folderVisitor->onDayEnd();
			}
			m_folderVisitor->onYearEnd();

		}
		m_folderVisitor->onEnd();
		return true;

	}
	
	bool ArchiveVisitor::archive() {
		// make the root folder
		std::string path = m_archivePath;
		m_folderVisitor->onStart();
		FileList_Ptr filelist = SAUtils::getFiles_(path.c_str());
		for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string year = *i;
			char c = (year)[0];
			if (c == '.') {
				continue;
			}
			//if (!isdigit(atoi(year->c_str()))) {
			//	continue;
			//}
			// read day folders for this year in Master folder
			std::string yearMaster = path;
			if (year.compare("system") == 0) {
				continue;
			}
			m_folderVisitor->onYearFolder(year.c_str());
			
			yearMaster += '/';
			yearMaster += year;
			FileList_Ptr filelist = SAUtils::getFiles_(yearMaster.c_str());
			for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
				std::string dayfolder = *i;
				char c = (dayfolder)[0];
				if (c == '.') {
					continue;
				}

				m_folderVisitor->onDayFolder(dayfolder.c_str());
				// Images
				std::string dayFolderMaster = yearMaster;
				dayFolderMaster += '/';
				dayFolderMaster += dayfolder;
				dayFolderMaster += "/data";

				FileList_Ptr imageFilelist = SAUtils::getFiles_(dayFolderMaster.c_str());
				for (std::vector<std::string>::iterator i = imageFilelist->begin(); i != imageFilelist->end(); i++) {
					std::string imageFile = *i;
					char c = (imageFile)[0];
					if (c == '.') {
						continue;
					}
					if (SAUtils::getExtention(imageFile).compare("ver") == 0) {
						continue;
					}
					//printf("\t\tImage %s: \n", imageFile->c_str());
					std::string imagePath = dayFolderMaster;
					m_folderVisitor->onImage(dayFolderMaster.c_str(), imageFile.c_str());
				}
				/// Metadata
				dayFolderMaster = yearMaster;
				dayFolderMaster += '/';
				dayFolderMaster += dayfolder;
				dayFolderMaster += "/metadata";

				FileList_Ptr metadataFilelist = SAUtils::getFiles_(dayFolderMaster.c_str());
				for (std::vector<std::string>::iterator i = metadataFilelist->begin(); i != metadataFilelist->end(); i++) {
					std::string imageFile = *i;
					char c = (imageFile)[0];
					if (c == '.') {
						continue;
					}
					//if (SAUtils::getExtention(imageFile).compare("ver") == 0) {
					//	continue;
					//}
					//printf("\t\tImage %s: \n", imageFile->c_str());
					std::string imagePath = dayFolderMaster;
					m_folderVisitor->onMetadata(dayFolderMaster.c_str(), imageFile.c_str());
				}
				m_folderVisitor->onDayEnd();
			}
			m_folderVisitor->onYearEnd();

		}
		m_folderVisitor->onEnd();
		return true;
	}

	bool ArchiveVisitor::process(const char *rootFolder) {
		// make the root folder
		std::string path = rootFolder;

		m_folderVisitor->onStart();
		// read years in Master folder
		FileList_Ptr filelist = SAUtils::getFiles_(path.c_str());
		for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string year = *i;
			char c = (year)[0];
			if (c == '.') {
				continue;
			}
			//if (!isdigit(atoi(year->c_str()))) {
			//	continue;
			//}
			// read day folders for this year in Master folder
			std::string yearMaster = path;
			m_folderVisitor->onYearFolder(year.c_str());
			yearMaster += '/';
			yearMaster += year;
			FileList_Ptr filelist = SAUtils::getFiles_(yearMaster.c_str());
			for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
				std::string dayfolder = *i;
				char c = (dayfolder)[0];
				if (c == '.') {
					continue;
				}

				m_folderVisitor->onDayFolder(dayfolder.c_str());
				std::string dayFolderMaster = yearMaster;
				dayFolderMaster += '/';
				dayFolderMaster += dayfolder;
				dayFolderMaster += "/data";

				FileList_Ptr filelist = SAUtils::getFiles_(dayFolderMaster.c_str());
				for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
					std::string imageFile = *i;
					char c = (imageFile)[0];
					if (c == '.') {
						continue;
					}
					if (SAUtils::getExtention(imageFile).compare("ver") == 0) {
						continue;
					}
				//printf("\t\tImage %s: \n", imageFile->c_str());
				std::string imagePath = dayFolderMaster;
				m_folderVisitor->onImage(dayFolderMaster.c_str(), imageFile.c_str());
			}
			m_folderVisitor->onDayEnd();
		}
		m_folderVisitor->onYearEnd();

	}
	m_folderVisitor->onEnd();
	return true;

}


} /* namespace simplearchive */
