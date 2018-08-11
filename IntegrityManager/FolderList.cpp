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
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "FolderData.h"
#include "FolderList.h"
#include "CSVArgs.h"
#include "CheckDisk.h"
#include "ValidateReportingObject.h"
#include "CheckDiskJournal.h"

#include "SAUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {






FolderList::FolderList(const char *archivePath) {
	m_archivePath = archivePath;
	m_action = READING_WORKSPACE;
}

FolderList::FolderList(const char *archivePath, const char *workspacePath) {
	m_archivePath = archivePath;
	m_workspacePath = workspacePath;
	m_action = READING_WORKSPACE;
}

FolderList::~FolderList() {
	// TODO Auto-generated destructor stub
}

bool FolderList::addDayFolder(const char *folderName) {
	
	std::string path = m_archivePath + std::string("/system");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
		//if (SAUtils::setHidden(path.c_str()) == false) {
		//	throw std::exception();
		//}
	}
	
	path += std::string("/chdsk");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
    }
    std::string fpath = path + std::string("/ddata.csv");
    FileDataContainer fileDataContainer;
    fileDataContainer.read(fpath.c_str());
    fileDataContainer.add(folderName);
    fileDataContainer.write(fpath.c_str());
    return true;
}

bool FolderList::incFolders(const char *folderName) {
	std::string path = m_archivePath;
	path += "/chdsk";
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
    }
	std::string fpath = path;
	fpath += "/ddata.csv";

    if (SAUtils::FileExists(path.c_str()) == false) {
		throw std::exception();
    }
    FileDataContainer fileDataContainer;
    fileDataContainer.read(fpath.c_str());
    fileDataContainer.incFolders(folderName);
    fileDataContainer.write(fpath.c_str());
    return true;
}

bool FolderList::incFiles(const char *folderName) {
	std::string path = m_archivePath;
	
	path += "/system";
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	path += "/chdsk";
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
    }
	
	std::string fpath = path;
	fpath += "/ddata.csv";
	//if (SAUtils::FileExists(fpath.c_str()) == false) {
    //	throw std::exception();
    //}
    FileDataContainer fileDataContainer;
    fileDataContainer.read(fpath.c_str());
    fileDataContainer.incFiles(folderName);
    fileDataContainer.write(fpath.c_str());
    if (makeXML() == false) {
    	return false;
    }
    return true;
}

bool FolderList::makeXML() {
	std::string path = m_archivePath;
	path += "/system";
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	path += "/chdsk";
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	// do files
	std::string fpathxml = path;
	fpathxml += "/ddata.xml";
	std::ofstream filexml(fpathxml.c_str());
	if (filexml.is_open() == false) {
		return false;
	}
	std::string fpathcsv = path;
	fpathcsv += "/ddata.csv";
	if (SAUtils::FileExists(fpathcsv.c_str()) == false) {
	    throw std::exception();
	}
	FileDataContainer fileDataContainer;
	fileDataContainer.read(fpathcsv.c_str());

	filexml <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					<<	"<CheckDisk ordering=\"date\" >\n";
	std::string currYear;
	bool first = true;
	for (std::vector<FolderFile>::iterator i = fileDataContainer.begin(); i != fileDataContainer.end(); i++) {
		FolderFile data = *i;
			//printf("%s", data.c_str());
		//printf("%s: \n", year->c_str());

		if (currYear.compare(data.getYear()) != 0) {
			currYear = data.getYear();
			if (!first) {
				filexml <<	"\t</YearFolder>\n";
			}
			first = false;
			filexml <<	"\t<YearFolder Name=\"" << data.getYear() << "\" >\n";

		}
		std::string filepath = currYear + "/" + data.getFolderName() + "/.sia/chdsk/fdata.xml";
		//printf("folder: %s \n", name->c_str());
		filexml <<	"\t\t<DayFolder Name=\"" << data.getFolderName() << "\" "
				"Files=\"" << data.getNFiles() << "\""
				<< ">\n";
			filexml <<	"\t\t\t<Filelist>";
			filexml <<	filepath;
			filexml <<	"</Filelist>\n";
			filexml <<	"\t\t</DayFolder>\n";

	}
	filexml <<	"\t</YearFolder>\n";
	filexml <<	"</CheckDisk>\n";
	filexml.close();

	return true;
}

bool FolderList::makeList() {
	std::string path = m_archivePath;
	path += "/system";
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	path += std::string("/chdsk");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	// do files
	std::string fpath = path + std::string("/ddata.csv");
	std::ofstream filedat(fpath.c_str());
	if (filedat.is_open() == false) {
		return false;
	}
	fpath = path + std::string("/ddata.xml");
	std::ofstream filexml(fpath.c_str());
	if (filexml.is_open() == false) {
		return false;
	}
	time_t timeValue = 0;
	time(&timeValue);
	filexml <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					<<	"<FolderList>\n";
	FileList_Ptr filelist = SAUtils::getFiles_(m_archivePath.c_str());
	for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string year = *i;
		std::string filepath = m_archivePath + "/" + year;

		char c = (year)[0];
		if (c == '.' ) {
			continue;
		}
		//printf("%s: \n", year->c_str());
		filexml <<	"\t<YearFolder Name=\"" << year << "\" >\n";

		std::string yearfolder = m_archivePath + '/' + year;
		FileList_Ptr dayList = SAUtils::getFiles_(yearfolder.c_str());

		for (std::vector<std::string >::iterator i = dayList->begin(); i != dayList->end(); i++) {
			std::string name = *i;
			std::string filepath = year + "/" + name + "/.sia/chdsk/fdata.xml";

			char c = (name)[0];
			if (c == '.' ) {
				continue;
			}
			//printf("folder: %s \n", name->c_str());
			filexml <<	"\t\t<DayFolder Name=\"" << name << "\" >\n";
				filexml <<	"\t\t\t<Filelist>";
				filexml <<	filepath;
				filexml <<	"</Filelist>\n";
			filexml <<	"\t\t</DayFolder>\n";
			filedat << name << "::" << timeValue << '\n';
		}
		filexml <<	"\t</YearFolder>\n";
	}
	filedat.close();
	filexml <<	"</FolderList>\n";
	filexml.close();
	return true;
	
}


bool FolderList::validateAndRepairWorkspace() {
	ValidateAndRepairingWorkspaceObject validateAndRepairingObject(m_archivePath.c_str(), m_workspacePath.c_str());
	return validateWorkspace(validateAndRepairingObject);
}

bool FolderList::validateOnlyWorkspace() {
	ValidateReportingObject validateReportingObject;
	return validateWorkspace(validateReportingObject);
}
bool FolderList::validateWorkspace(ValidateReportingObject &validateReportingObject) {
	std::string path = m_archivePath;
	path += "/system";

	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	path += std::string("/chdsk");
	if (SAUtils::DirExists(path.c_str()) == false) {
		return false;
	}

	std::string fpath = path + std::string("/ddata.csv");
	if (SAUtils::FileExists(fpath.c_str()) == false) {
		return false;
	}
	FileDataContainer fileDataContainer;
	fileDataContainer.read(fpath.c_str());
	std::string dataPath = m_workspacePath;
	
	FileList_Ptr filelist = SAUtils::getFiles_(dataPath.c_str());
	for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string dataString = *i;
		if (dataString[0] == '.') {
			continue;
		}

		std::string year = dataString.substr(0, 4);

		if (year.compare("system") == 0) {
			continue;
		}
		//printf("Year found %s\n", year.c_str());
		std::string yearPath = dataPath += '/';
		yearPath += year;

		FileList_Ptr yearlist = SAUtils::getFiles_(yearPath.c_str());
		for (std::vector<std::string>::iterator i = yearlist->begin(); i != yearlist->end(); i++) {
			std::string dataString = *i;
			if (dataString[0] == '.') {
				continue;
			}

			if (fileDataContainer.find(dataString.c_str()) == false) {
				printf("File not found %s\n", dataString.c_str());
			}
			else {
				std::string yearDayPath = yearPath;
				yearDayPath += '/';
				yearDayPath += dataString;
				//printf("File found %s\n", dataString.c_str());
				std::string tmp = yearDayPath;
				

				std::string archivePath = m_archivePath;
					// Master
				archivePath += '/'; archivePath += year;
				archivePath += '/'; archivePath += dataString;
				archivePath += "/chdsk";
				CheckDisk checkDisk;
				if (checkDisk.check(yearDayPath.c_str(), archivePath.c_str(), dataString.c_str(), validateReportingObject) == false) {
					return false;
				}
			}
		}

	}
	validateReportingObject.save();
	m_workspaceJournalName = validateReportingObject.getJournalName();
	return true;
}







bool ShowCheckedOut::doWork(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject *visitingObject) {
	
	CheckDisk checkDisk;
	SimpleImageList imageList;
	//if (checkDisk.check(targetdir, checkFilePath, address, nullptr) == false) {
	if (checkDisk.showCheckedOut(checkFilePath, imageList) == false) {
		return false;
	}
	for (std::vector<std::string>::iterator i = imageList.begin(); i != imageList.end(); i++) {
		std::string dataString = *i;
		std::string imageAddress = address;
		imageAddress += "/";
		imageAddress += dataString;
		if (m_AddressScope.isInScope(imageAddress.c_str()) == true) {
			printf("Checked out: %s/%s\n", address, dataString.c_str()); // summary
		}
	}
	return true;
}

bool ShowUncheckedOutChanges::doWork(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject *unusedObject) {
	CheckDisk checkDisk;
	SimpleImageList imageList;
	//if (checkDisk.check(targetdir, checkFilePath, address, nullptr) == false) {
	// This will list the image that are checked out
	if (checkDisk.showCheckedOut(checkFilePath, imageList) == false) {
		return false;
	}
	// report changes in the day folder
	ValidateReportingObject& visitingObject = (ValidateReportingObject&)getVisitingObject();
	
	if (checkDisk.check(targetdir, checkFilePath, address, visitingObject) == false) {
		return false;
	}

	CheckDiskJournal &journal = visitingObject.getJournal();
	CDJournalList &list = journal.getList();
	
	for (auto i = list.begin(); i != list.end(); i++) {
		CDJournalItem data = *i;
		ReportStatus &rp = data.getStatus();
		if (rp.get() == ReportStatus::Unchanged) {
			continue;
		}
		std::string imagePath = data.getImage();
		std::string image = imagePath.substr(11, imagePath.length());
		for (auto i = imageList.begin(); i != imageList.end(); i++) {
			std::string name = *i;
			
			if (name.compare(image) == 0) {
				data.setCheckedOut();
			}
		}

		printf("Checked out: %s/%s %s\n", rp.toString(), data.getImage(), (data.getCheckedOutState()?"True":"False")); // summary
	}
	
	return true;
}



//ValidateWorkspace::~ValidateWorkspace() {
	//delete validateReportingObject;
//}

bool ValidateWorkspace::doWork(const char *targetdir, const char *checkFilePath, const char *address, VisitingObject *visitingObject) {
	CheckDisk checkDisk;
	if (checkDisk.check(targetdir, checkFilePath, address, *m_validateReportingObject) == false) {
		return false;
	}
	return true;
}

bool FolderList::showCheckedOut(const char *addressScope) {
	ShowCheckedOut showCheckedOut(m_archivePath.c_str());
	showCheckedOut.process(addressScope);
	return true;
}

bool FolderList::showUncheckedOutChanges(const char *addressScope) {
	ShowUncheckedOutChanges showUncheckedOutChanges(m_archivePath.c_str(), m_workspacePath.c_str());
	showUncheckedOutChanges.process(addressScope);
	return true;
}

bool FolderList::validateAndRepairMaster() {
	ValidateAndRepairingMasterObject validateAndRepairingObject(m_archivePath.c_str(), m_workspacePath.c_str());
	return validateMaster(validateAndRepairingObject);
}

bool FolderList::validateOnlyMaster() {
	ValidateReportingObject validateReportingObject;
	return validateMaster(validateReportingObject);
}

bool FolderList::validateMaster(ValidateReportingObject &validateReportingObject) {
	
	std::string path = m_archivePath;
	path += "/system";
		
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	path += std::string("/chdsk");
	if (SAUtils::DirExists(path.c_str()) == false) {
		throw std::exception();
	}

	std::string fpath = path + std::string("/ddata.csv");
	if (SAUtils::FileExists(fpath.c_str()) == false) {
		throw std::exception();
	}
	FileDataContainer fileDataContainer;
	fileDataContainer.read(fpath.c_str());
	std::string dataPath = m_archivePath;
	
	FileList_Ptr filelist = SAUtils::getFiles_(dataPath.c_str());
	for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string dataString = *i;
		if (dataString[0] == '.') {
			continue;
		}
			
		std::string year = dataString.substr(0, 4);
			
		if (year.compare("syst") == 0) {
			continue;
		}
//		printf("Year found %s\n", year.c_str());
		validateReportingObject.startYear(year.c_str());
		std::string yearPath = dataPath += '/';
		yearPath += year;

		
		FileList_Ptr yearlist = SAUtils::getFiles_(yearPath.c_str());
		for (std::vector<std::string>::iterator i = yearlist->begin(); i != yearlist->end(); i++) {
			std::string dataString = *i;
			if (dataString[0] == '.') {
				continue;
			}

			std::string filepath = m_archivePath + "/" + dataString;
			validateReportingObject.startDay(dataString.c_str());
			if (fileDataContainer.find(dataString.c_str()) == false) {
				printf("File not found %s\n", dataString.c_str());
			}
			else {
				std::string yearDayPath = yearPath;
				yearDayPath += '/';
				yearDayPath += dataString;
//				printf("Day found %s\n", dataString.c_str());
				std::string tmp = yearDayPath;
				// Master
				yearDayPath += "/images";
				tmp += "/chdsk";

				CheckDisk checkDisk;
				if (checkDisk.check(yearDayPath.c_str(), tmp.c_str(), dataString.c_str(), validateReportingObject) == false) {
					return false;
				}
			}
			validateReportingObject.endDay(dataString.c_str());
		}
		validateReportingObject.endYear(year.c_str());
	}
	validateReportingObject.save();
	m_MasterJournalName = validateReportingObject.getJournalName();
	return true;
}


bool FolderList::validate() {
	switch (m_action) {
	case READING_MASTER:
		return validateOnlyMaster();
	case READING_WORKSPACE:
		return validateOnlyWorkspace();
	case READING_BOTH:
	{
		bool ret = validateOnlyMaster();
		if (!ret) {
			return false;
		}
		ret = validateOnlyWorkspace();
		if (!ret) {
			return false;
		}
		
		return ret;
	}
	default:
		return false;
	}
	return false;
}

bool FolderList::validateAndRepair() {
	switch (m_action) {
	case READING_MASTER:
		return validateAndRepairMaster();
	case READING_WORKSPACE:
		return validateAndRepairWorkspace();
	case READING_BOTH:
	{
		bool ret = validateAndRepairMaster();
		if (!ret) {
			return false;
		}
		ret = validateAndRepairWorkspace();
		if (!ret) {
			return false;
		}
		return ret;
	}
	default:
		return false;
	}
	return false;
}

bool FolderList::fix() {
	return fixWorkspace(m_workspaceJournalName.c_str());
}

bool FolderList::fixWorkspace(const char *jouralFile) {
	CheckDiskJournal checkDiskJournal;

	if (checkDiskJournal.read(jouralFile) == false) {
		return false;
	}
	int size = checkDiskJournal.size();
	for (int i = 0; i < size; i++) {
		ReportStatus rs = checkDiskJournal.getReportStatusAt(i);
		std::string imagePath = checkDiskJournal.getImageAt(i);
		
		std::string year = imagePath.substr(0, 4);
		std::string archiveImagePath = m_archivePath;
		archiveImagePath += '/'; archiveImagePath += year;
		std::string yearday = imagePath.substr(0, 10);
		archiveImagePath += '/'; archiveImagePath += yearday;
		archiveImagePath += "/data";
		archiveImagePath += imagePath.substr(10, imagePath.length() - 9);

		std::string workspaceImagePath = m_workspacePath;
		workspaceImagePath += '/'; workspaceImagePath += year;
		workspaceImagePath += '/'; workspaceImagePath += imagePath;

		if (rs.get() == ReportStatus::ContentChanged) {
			if (SAUtils::copy(archiveImagePath.c_str(), workspaceImagePath.c_str())) {
				printf("Fixed - Image: %s\n", workspaceImagePath.c_str());
					
			}
			else {
				printf("Not fixed - Image: %s\n", workspaceImagePath.c_str());
			}
		}
		else if (rs.get() == ReportStatus::NameChanged) {
			std::string orginalName = checkDiskJournal.getOrginalAt(i);
			std::string yearDay = imagePath.substr(0, 10);
			std::string orginalImagePath = m_workspacePath;

			orginalImagePath += '/'; orginalImagePath += year;
			orginalImagePath += '/'; orginalImagePath += yearDay;
			orginalImagePath += '/'; orginalImagePath += orginalName;

			if (SAUtils::rename(workspaceImagePath.c_str(), orginalImagePath.c_str())) {
				printf("Not fixed - Image: %s\n", workspaceImagePath.c_str());
			}
			else {
				printf("Fixed - Image: %s\n", workspaceImagePath.c_str());
			}
		}
		else if (rs.get() == ReportStatus::Missing) {
			if (SAUtils::copy(archiveImagePath.c_str(), workspaceImagePath.c_str())) {
				printf("Not fixed - Image: %s\n", workspaceImagePath.c_str());

			}
			else {
				printf("Fixed - Image: %s\n", workspaceImagePath.c_str());
			}
		}
		else if (rs.get() == ReportStatus::Added) {
			printf("Added - Image: %s\n", workspaceImagePath.c_str());
		}
		else { // Unknown
		}
	}
	return true;
}



} /* namespace simplearchive */
