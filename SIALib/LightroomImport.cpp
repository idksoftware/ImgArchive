#include "LightroomImport.h"
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "FolderData.h"
#include "FolderList.h"
#include "CSVArgs.h"
#include "CheckDisk.h"
#include "ValidateReportingObject.h"
#include "CheckDiskJournal.h"
#include "IntegrityManager.h"
#include "SAUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {





	LightroomImport::LightroomImport(const char *archivePath, const char *lightroomPath)
	{
		m_archivePath = archivePath;
		m_lightroomPath = lightroomPath;
	}


	LightroomImport::~LightroomImport()
	{
	}

	bool LightroomImport::makeList() {

		
		time_t timeValue = 0;
		time(&timeValue);
		
		ImportImageList importImageList;
		FileList_Ptr filelist = SAUtils::getFiles_(m_archivePath.c_str());
		
		for (auto i = filelist->begin(); i != filelist->end(); i++) {
			std::string year = *i;


			char c = (year)[0];
			if (c == '.') {
				continue;
			}
			//printf("%s: \n", year->c_str());
			

			std::string yearfolder = m_archivePath + "/system/chdsk/" + year;
			FileList_Ptr dayList = SAUtils::getFiles_(yearfolder.c_str());

			for (auto i = dayList->begin(); i != dayList->end(); i++) {
				std::string name = *i;
				char c = (name)[0];
				if (c == '.') {
					continue;
				}
				//for workspace std::string filepath = year + "/" + name + "/.sia/chdsk/fdata.xml";
				std::string relfilepath = year + "/" + name;
				std::string lightroomFullPath = m_lightroomPath + '/' + relfilepath;
				std::string archiveFullPath = m_archivePath + "/system/chdsk/" + relfilepath + "/fdata.csv";
				CheckDisk checkDisk;
				std::vector<std::string> list;
				checkDisk.findNewImages(archiveFullPath.c_str(), lightroomFullPath.c_str(), list);
				if (!list.empty()) {
					importImageList.onStart(lightroomFullPath.c_str());
					importImageList.onDirectory(lightroomFullPath.c_str());
					for (auto j = list.begin(); j == list.end(); j++) {
						importImageList.onDirectory((*j).c_str());
					}
				}
			}
		}
		
		return true;

	}

}