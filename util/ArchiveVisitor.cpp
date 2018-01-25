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
#include "pathcontroller.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	ArchiveVisitor::ArchiveVisitor(AVAction *folderVisitor) {
		m_folderVisitor = folderVisitor;
		m_dayPostfix = "/data";

	}

	ArchiveVisitor::ArchiveVisitor(AVAction *folderVisitor, const char *archivePath) {
		m_folderVisitor = folderVisitor;
		m_archivePath = archivePath;
		m_dayPostfix = "/data";
	}

	ArchiveVisitor::ArchiveVisitor(AVAction *folderVisitor, const char *archivePath, const char *workspacePath) {
		m_folderVisitor = folderVisitor;
		m_archivePath = archivePath;
		m_workspacePath = workspacePath;
		m_dayPostfix = "/data";
	}

	ArchiveVisitor::~ArchiveVisitor() {
		// TODO Auto-generated destructor stub
	}

	bool ArchiveVisitor::workspace(const char *rootFolder) {
		// make the root folder
		std::string path = rootFolder;

		m_folderVisitor->onStart();
		FileList_Ptr yearlist = SAUtils::getFiles_(path.c_str());
		for (std::vector<std::string>::iterator i = yearlist->begin(); i != yearlist->end(); i++) {
			std::string year = *i;
			char c = (year)[0];
			if (c == '.') {
				continue;
			}
			
			std::string yearMaster = path;
			m_folderVisitor->onYearFolder(year.c_str());
			yearMaster += '/';
			yearMaster += year;
			if (workspaceDay(yearMaster) == false) {
				return false;
			}
			/*
			FileList_Ptr daylist = SAUtils::getFiles_(yearMaster.c_str());
			for (auto ii = daylist->begin(); ii != daylist->end(); ii++) {
				std::string dayfolder = *ii;
				c = (dayfolder)[0];
				if (c == '.') {
					continue;
				}

				m_folderVisitor->onDayFolder(dayfolder.c_str());
				std::string dayFolderMaster = yearMaster;
				dayFolderMaster += '/';
				dayFolderMaster += dayfolder;
				dayFolderMaster += "/data";

				FileList_Ptr imagelist = SAUtils::getFiles_(dayFolderMaster.c_str());
				for (auto iii = imagelist->begin(); iii != imagelist->end(); i++) {
					std::string imageFile = *iii;
					c = (imageFile)[0];
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
			*/
			m_folderVisitor->onYearEnd();

		}
		m_folderVisitor->onEnd();
		return true;

	}

	bool ArchiveVisitor::workspaceDay(std::string year) {
		FileList_Ptr daylist = SAUtils::getFiles_(year.c_str());
		for (auto ii = daylist->begin(); ii != daylist->end(); ii++) {
			std::string dayfolder = *ii;
			char c = (dayfolder)[0];
			if (c == '.') {
				continue;
			}

			m_folderVisitor->onDayFolder(dayfolder.c_str());
			std::string dayFolderMaster = year;
			dayFolderMaster += '/';
			dayFolderMaster += dayfolder;
			dayFolderMaster += "/data";

			FileList_Ptr imagelist = SAUtils::getFiles_(dayFolderMaster.c_str());
			for (auto iii = imagelist->begin(); iii != imagelist->end(); iii++) {
				std::string imageFile = *iii;
				c = (imageFile)[0];
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
		return true;
	}
	
	bool ArchiveVisitor::archiveDay(std::string year) {
		FileList_Ptr daylist = SAUtils::getFiles_(year.c_str());
		for (auto ii = daylist->begin(); ii != daylist->end(); ii++) {
			std::string dayfolder = *ii;
			char c = (dayfolder)[0];
			if (PathController::validateYYMMDD(dayfolder.c_str()) == false) {
				continue; // Html or some other file/folder may be in the year folder
			}

			m_folderVisitor->onDayFolder(dayfolder.c_str());
			// Images
			std::string dayFolderMaster = year;
			dayFolderMaster += '/';
			dayFolderMaster += dayfolder;
			dayFolderMaster += m_dayPostfix;

			FileList_Ptr imageFilelist = SAUtils::getFiles_(dayFolderMaster.c_str());
			for (auto iii = imageFilelist->begin(); iii != imageFilelist->end(); iii++) {
				std::string imageFile = *iii;
				c = (imageFile)[0];
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
			dayFolderMaster = year;
			dayFolderMaster += '/';
			dayFolderMaster += dayfolder;
			dayFolderMaster += "/metadata";

			FileList_Ptr metadataFilelist = SAUtils::getFiles_(dayFolderMaster.c_str());
			for (auto im = metadataFilelist->begin(); im != metadataFilelist->end(); im++) {
				std::string imageFile = *im;
				c = (imageFile)[0];
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
		return true;
	}

	bool ArchiveVisitor::archive() {
		// make the root folder
		std::string path = m_archivePath;
		m_folderVisitor->onStart();
		FileList_Ptr yearlist = SAUtils::getFiles_(path.c_str());
		for (std::vector<std::string>::iterator i = yearlist->begin(); i != yearlist->end(); i++) {
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
			if (archiveDay(yearMaster) == false) {
				return false;
			}
			/*
			FileList_Ptr daylist = SAUtils::getFiles_(yearMaster.c_str());
			for (auto ii = daylist->begin(); ii != daylist->end(); ii++) {
				std::string dayfolder = *ii;
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
				for (auto iii = imageFilelist->begin(); iii != imageFilelist->end(); iii++) {
					std::string imageFile = *iii;
					c = (imageFile)[0];
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
				for (auto im = metadataFilelist->begin(); im != metadataFilelist->end(); im++) {
					std::string imageFile = *im;
					c = (imageFile)[0];
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
			*/
			m_folderVisitor->onYearEnd();

		}
		m_folderVisitor->onEnd();
		return true;
	}

	bool ArchiveVisitor::processDay(std::string year) {
		FileList_Ptr dayList = SAUtils::getFiles_(year.c_str());
		for (auto ii = dayList->begin(); ii != dayList->end(); ii++) {
			std::string dayfolder = *ii;
			char c = (dayfolder)[0];
			if (PathController::validateYYMMDD(dayfolder.c_str()) == false) {
				continue; // Html or some other file/folder may be in the year folder
			}

			m_folderVisitor->onDayFolder(dayfolder.c_str());
			std::string dayFolderMaster = year;
			dayFolderMaster += '/';
			dayFolderMaster += dayfolder;
			dayFolderMaster += m_dayPostfix;

			FileList_Ptr imageList = SAUtils::getFiles_(dayFolderMaster.c_str());
			for (auto iii = imageList->begin(); iii != imageList->end(); iii++) {
				std::string imageFile = *iii;
				c = (imageFile)[0];
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
		return true;
	}

	bool ArchiveVisitor::process(const char *rootFolder) {
		// make the root folder
		std::string path = rootFolder;

		m_folderVisitor->onStart();
		// read years in Master folder
		FileList_Ptr yearList = SAUtils::getFiles_(path.c_str());
		for (auto i = yearList->begin(); i != yearList->end(); i++) {
			std::string year = *i;
			char c = (year)[0];
			if (c == '.') {
				continue;
			}
			if (PathController::validateYYYY(year.c_str()) == false) {
				continue; // Html or some other file/folder may be in the year folder
			}
			// read day folders for this year in Master folder
			std::string yearMaster = path;
			try {
				m_folderVisitor->onYearFolder(year.c_str());
			}
			catch (std::exception /*e*/) {
				return false;
			}

			yearMaster += '/';
			yearMaster += year;
			if (archiveDay(yearMaster) == false) {
				return false;
			}
			/*
			FileList_Ptr dayList = SAUtils::getFiles_(yearMaster.c_str());
			for (auto ii = dayList->begin(); ii != dayList->end(); ii++) {
				std::string dayfolder = *ii;
				c = (dayfolder)[0];
				if (c == '.') {
					continue;
				}

				m_folderVisitor->onDayFolder(dayfolder.c_str());
				std::string dayFolderMaster = yearMaster;
				dayFolderMaster += '/';
				dayFolderMaster += dayfolder;
				dayFolderMaster += "/data";

				FileList_Ptr imageList = SAUtils::getFiles_(dayFolderMaster.c_str());
				for (auto iii = imageList->begin(); iii != imageList->end(); iii++) {
					std::string imageFile = *iii;
					c = (imageFile)[0];
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
		*/
		m_folderVisitor->onYearEnd();

	}
	m_folderVisitor->onEnd();
	return true;

}


} /* namespace simplearchive */
