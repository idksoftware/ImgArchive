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
#include "IntegrityManager.h"
#include "SAUtils.h"
#include "CIDKCrc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	
	//FolderList::FolderList()
	//	: m_action(Action::READING_WORKSPACE)
	//{}

	FolderList::FolderList(const char* workspacePath)
		: m_workspacePath(workspacePath) //, m_action(Action::READING_WORKSPACE)
	{};

	
	std::string FolderList::makeDBPathCSV(const char* archivePath) const {
		std::string path = archivePath + std::string("/system");
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
		path += "/fdata.csv";
		return path;
	}

	std::string FolderList::makeDBPathXML(const char* archivePath) const {
		std::string path = archivePath + std::string("/system");
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
		path += "/fdata.xml";
		return path;
	}

	bool FolderList::addDayFolder(const char* archivePath, const char* folderName) {

		std::string fpath = makeDBPathCSV(archivePath);
		FileDataContainer fileDataContainer;
		fileDataContainer.read(fpath.c_str());
		fileDataContainer.add(folderName);
		fileDataContainer.write(fpath.c_str());
		return true;
	}

	bool FolderList::incFolders(const char* archivePath, const char* folderName) {
		std::string path = archivePath;
		path += "/chdsk";
		if (SAUtils::DirExists(path.c_str()) == false) {
			if (SAUtils::mkDir(path.c_str()) == false) {
				throw std::exception();
			}
		}
		std::string fpath = path;
		fpath += "/";
		fpath += folderName;
		fpath += ".csv";

		if (SAUtils::FileExists(path.c_str()) == false) {
			throw std::exception();
		}
		FileDataContainer fileDataContainer;
		fileDataContainer.read(fpath.c_str());
		fileDataContainer.incFolders(folderName);
		fileDataContainer.write(fpath.c_str());
		return true;
	}

	bool FolderList::incFiles(const char* archivePath, const char* folderName) {

		//if (SAUtils::FileExists(fpath.c_str()) == false) {
		//	throw std::exception();
		//}

		std::string fpath = makeDBPathCSV(archivePath);
		FileDataContainer fileDataContainer;
		fileDataContainer.read(fpath.c_str());
		fileDataContainer.incFiles(folderName);
		fileDataContainer.write(fpath.c_str());
		if (makeXML(archivePath) == false) {
			return false;
		}
		return true;
	}

	bool FolderList::makeXML(const char* archivePath) {

		std::string fpathcsv = makeDBPathCSV(archivePath);
		if (SAUtils::FileExists(fpathcsv.c_str()) == false) {
			throw std::exception();
		}
		// do files
		std::string fpathxml = makeDBPathXML(archivePath);

		std::ofstream filexml(fpathxml.c_str());
		if (filexml.is_open() == false) {
			return false;
		}

		FileDataContainer fileDataContainer;
		fileDataContainer.read(fpathcsv.c_str());

		filexml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<CheckDisk ordering=\"date\" >\n";
		std::string currYear;
		bool first = true;
		for (auto i = fileDataContainer.begin(); i != fileDataContainer.end(); i++) {
			FolderFile data = *i;
			//printf("%s", data.c_str());
		//printf("%s: \n", year->c_str());

			if (currYear.compare(data.getYear()) != 0) {
				currYear = data.getYear();
				if (!first) {
					filexml << "\t</YearFolder>\n";
				}
				first = false;
				filexml << "\t<YearFolder Name=\"" << data.getYear() << "\" >\n";

			}
			//for workspace std::string filepath = currYear + "/" + data.getFolderName() + "/.sia/chdsk/fdata.xml";
			std::string filepath = currYear + "/" + data.getFolderName() + "/fdata.xml";
			//printf("folder: %s \n", name->c_str());
			filexml << "\t\t<DayFolder Name=\"" << data.getFolderName() << "\" "
				"Files=\"" << data.getNFiles() << "\""
				<< ">\n";
			filexml << "\t\t\t<Filelist>";
			filexml << filepath;
			filexml << "</Filelist>\n";
			filexml << "\t\t</DayFolder>\n";

		}
		filexml << "\t</YearFolder>\n";
		filexml << "</CheckDisk>\n";
		filexml.close();

		return true;
	}

	bool FolderList::makeList(const char* archivePath) {

		std::string fpath = makeDBPathCSV(archivePath);
		std::ofstream filedat(fpath.c_str());
		if (filedat.is_open() == false) {
			return false;
		}
		fpath = makeDBPathXML(archivePath);
		std::ofstream filexml(fpath.c_str());
		if (filexml.is_open() == false) {
			return false;
		}
		time_t timeValue = 0;
		time(&timeValue);
		filexml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			<< "<FolderList>\n";
		FileList_Ptr filelist = SAUtils::getFiles_(archivePath);
		for (auto i = filelist->begin(); i != filelist->end(); i++) {
			std::string year = *i;


			char c = (year)[0];
			if (c == '.') {
				continue;
			}
			//printf("%s: \n", year->c_str());
			filexml << "\t<YearFolder Name=\"" << year << "\" >\n";

			std::string yearfolder = archivePath + '/' + year;
			FileList_Ptr dayList = SAUtils::getFiles_(yearfolder.c_str());

			for (auto i = dayList->begin(); i != dayList->end(); i++) {
				std::string name = *i;
				//for workspace std::string filepath = year + "/" + name + "/.sia/chdsk/fdata.xml";
				std::string filepath = year + "/" + name + "/fdata.xml";

				char c = (name)[0];
				if (c == '.') {
					continue;
				}
				//printf("folder: %s \n", name->c_str());
				filexml << "\t\t<DayFolder Name=\"" << name << "\" >\n";
				filexml << "\t\t\t<Filelist>";
				filexml << filepath;
				filexml << "</Filelist>\n";
				filexml << "\t\t</DayFolder>\n";
				filedat << name << "::" << timeValue << '\n';
			}
			filexml << "\t</YearFolder>\n";
		}
		filedat.close();
		filexml << "</FolderList>\n";
		filexml.close();
		return true;

	}


	bool FolderList::validateAndRepairWorkspace(const char* archivePath, IMCompletedSummary& imCompletedSummar) {
		ValidateAndRepairingWorkspaceObject validateAndRepairingObject(archivePath, m_workspacePath.c_str());
		return validateWorkspace(archivePath, validateAndRepairingObject);
	}

	bool FolderList::validateWorkspaceSummary(const char* archivePath, IMCompletedSummary& imCompletedSummar) {
		ValidateReportingObject validateReportingObject;
		bool ret = validateWorkspace(archivePath, validateReportingObject);
		CheckDiskSummaryJounal& jounal = validateReportingObject.GetCheckDiskSummaryJounal();
		imCompletedSummar.setSummary(jounal.getTotalSummary().toSummary().c_str());
		imCompletedSummar.setResult(jounal.getTotalSummary().toResult().c_str());
		return ret;
	}
	bool FolderList::validateWorkspace(const char* archivePath, ValidateReportingObject& validateReportingObject) {

		std::string fpath = makeDBPathCSV(archivePath);
		if (SAUtils::FileExists(fpath.c_str()) == false) {
			return false;
		}
		FileDataContainer fileDataContainer;
		fileDataContainer.read(fpath.c_str());
		std::string dataPath = m_workspacePath;

		FileList_Ptr filelist = SAUtils::getFiles_(dataPath.c_str());
		for (auto i = filelist->begin(); i != filelist->end(); i++) {
			std::string dataString = *i;
			if (dataString[0] == '.') {
				continue;
			}

			std::string year = dataString.substr(0, 4);

			if (year.compare("system") == 0) {
				continue;
			}
			//printf("Year found %s\n", year.c_str());
			validateReportingObject.startYear(year.c_str());
			std::string yearPath = dataPath += '/';
			yearPath += year;

			FileList_Ptr yearlist = SAUtils::getFiles_(yearPath.c_str());
			for (auto i = yearlist->begin(); i != yearlist->end(); i++) {
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
					validateReportingObject.startDay(dataString.c_str());

					std::string archivePath = archivePath;
					// Master
					archivePath += "/system/chdsk/"; archivePath += year;
					archivePath += '/'; archivePath += dataString;
					archivePath += '/';
					CheckDisk checkDisk;
					if (checkDisk.check(yearDayPath.c_str(), archivePath.c_str(), dataString.c_str(), validateReportingObject) == false) {
						return false;
					}
				}
				validateReportingObject.endDay(dataString.c_str());
			}
			validateReportingObject.endYear(year.c_str());

		}
		validateReportingObject.save();
		m_workspaceJournalName = validateReportingObject.getJournalName();
		return true;
	}







	bool ShowCheckedOut::doWork(const char* targetdir, const char* checkFilePath, const char* address, VisitingObject* visitingObject) {

		CheckDisk checkDisk;
		SimpleImageList imageList;
		//if (checkDisk.check(targetdir, checkFilePath, address, nullptr) == false) {
		if (checkDisk.showCheckedOut(checkFilePath, imageList) == false) {
			return false;
		}
		for (auto i = imageList.begin(); i != imageList.end(); i++) {
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

	bool ShowUncheckedOutChanges::doWork(const char* targetdir, const char* checkFilePath, const char* address, VisitingObject* unusedObject) {
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

		CheckDiskJournal& journal = visitingObject.getJournal();
		CDJournalList& list = journal.getList();

		for (auto i = list.begin(); i != list.end(); i++) {
			CDJournalItem data = *i;
			ReportStatus& rp = data.getStatus();
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

			printf("Checked out: %s/%s %s\n", rp.toString(), data.getImage(), (data.getCheckedOutState() ? "True" : "False")); // summary
		}

		return true;
	}



	//ValidateWorkspace::~ValidateWorkspace() {
		//delete validateReportingObject;
	//}

	bool ValidateWorkspace::doWork(const char* targetdir, const char* checkFilePath, const char* address, VisitingObject* visitingObject) {
		CheckDisk checkDisk;
		if (checkDisk.check(targetdir, checkFilePath, address, *m_validateReportingObject) == false) {
			return false;
		}
		return true;
	}

	bool FolderList::showCheckedOut(const char* archivePath, const char* addressScope) {
		ShowCheckedOut showCheckedOut(archivePath);
		showCheckedOut.process(addressScope);
		return true;
	}

	bool FolderList::showUncheckedOutChanges(const char* archivePath, const char* workspacePath, const char* addressScope) {
		ShowUncheckedOutChanges showUncheckedOutChanges(archivePath, workspacePath);
		showUncheckedOutChanges.process(addressScope);
		return true;
	}

	bool FolderList::validateAndRepairMaster(const char* archivePath, const char* workspacePath, IMCompletedSummary& imCompletedSummar) {
		ValidateAndRepairingMasterObject validateAndRepairingObject(archivePath, workspacePath);
		return validateArchive(archivePath, validateAndRepairingObject);
	}

	bool FolderList::validateArchiveSummary(const char* archivePath, IMCompletedSummary& imCompletedSummary) {
		ValidateReportingObject validateReportingObject;
		bool ret = validateArchive(archivePath, validateReportingObject);
		CheckDiskSummaryJounal& jounal = validateReportingObject.GetCheckDiskSummaryJounal();
		imCompletedSummary.setSummary(jounal.getTotalSummary().toSummary().c_str());
		imCompletedSummary.setResult(jounal.getTotalSummary().toResult().c_str());
		return ret;
	}


	bool FolderList::validateArchive(const char* archivePath, ValidateReportingObject& validateReportingObject) {

		std::string fpath = makeDBPathCSV(archivePath);
		if (SAUtils::FileExists(fpath.c_str()) == false) {
			return false; // may be an emplty archive
		}
		FileDataContainer fileDataContainer;
		fileDataContainer.read(fpath.c_str());
		std::string dataPath = archivePath;

		FileList_Ptr filelist = SAUtils::getFiles_(dataPath.c_str());
		for (auto i = filelist->begin(); i != filelist->end(); i++) {
			std::string dataString = *i;
			if (dataString[0] == '.') {
				continue;
			}

			std::string year = dataString.substr(0, 4);

			if (year.compare("system") == 0) {
				continue;
			}
			//		printf("Year found %s\n", year.c_str());
			validateReportingObject.startYear(year.c_str());
			std::string yearPath = dataPath += '/';
			yearPath += year;


			FileList_Ptr yearlist = SAUtils::getFiles_(yearPath.c_str());
			for (auto i = yearlist->begin(); i != yearlist->end(); i++) {
				std::string dataString = *i;
				if (dataString[0] == '.') {
					continue;
				}
				validateReportingObject.startDay(dataString.c_str());
				if (fileDataContainer.find(dataString.c_str()) == false) {
					printf("File not found %s\n", dataString.c_str());
				}
				else {
					// Path to images 
					std::string yearDayPath = yearPath; yearDayPath += '/';
					yearDayPath += dataString; yearDayPath += '/';
					yearDayPath += "images";
					//				printf("Day found %s\n", dataString.c_str());
									// Path to check file 
					std::string tmp = dataPath; tmp += '/';
					tmp += "system"; tmp += '/';
					tmp += "chdsk"; tmp += '/';
					tmp += year; tmp += '/';
					tmp += dataString;

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


	bool FolderList::validateArchiveOnly(ValidatedArchive validatedArchive, const char* archivePath, IMCompletedSummary& imCompletedSummary, bool repair) {
		//if (repair) return validateAndRepairMaster(archivePath, imCompletedSummary, verifyBackups);
		return validateArchiveSummary(archivePath, imCompletedSummary);
	}

	bool FolderList::validateWorkspaceOnly(const char* archivePath, IMCompletedSummary& imCompletedSummary, bool repair) {
		//if (repair) return validateAndRepairWorkspace(archivePath, imCompletedSummary);
		return validateWorkspaceSummary(archivePath, imCompletedSummary);
	}
	

	/*
	switch (m_action) {
	case Action::READING_MASTER:
		//if (repair) return validateAndRepairMaster(archivePath, imCompletedSummary, verifyBackups);
		return validateOnlyMaster(archivePath, imCompletedSummary);
	case Action::READING_WORKSPACE:
		if (repair) return validateAndRepairWorkspace(archivePath, imCompletedSummary);
		return validateOnlyWorkspace(archivePath, imCompletedSummary);
	case Action::READING_DERIVATIVE:
		if (repair) return validateAndRepairWorkspace(archivePath, imCompletedSummary);
		return validateOnlyWorkspace(archivePath, imCompletedSummary);
	case Action::READING_MAIN:
		if (repair) return validateAndRepairWorkspace(archivePath, imCompletedSummary);
		return validateOnlyWorkspace(archivePath, imCompletedSummary);
	case Action::READING_ALL:
	{
		bool ret = validateOnlyMaster(archivePath, imCompletedSummary);
		if (!ret) {
			return false;
		}
		ret = validateOnlyWorkspace(archivePath, imCompletedSummary);
		if (!ret) {
			return false;
		}
		return ret;
	}
	default:
		return false;
	}
	*/
	


bool FolderList::fix(const char* archivePath) {
	return fixWorkspace(archivePath, m_workspaceJournalName.c_str());
}

bool FolderList::fixWorkspace(const char* archivePath, const char *jouralFile) {
	CheckDiskJournal checkDiskJournal;

	if (checkDiskJournal.read(jouralFile) == false) {
		return false;
	}
	int size = checkDiskJournal.size();
	for (int i = 0; i < size; i++) {
		ReportStatus rs = checkDiskJournal.getReportStatusAt(i);
		std::string imagePath = checkDiskJournal.getImageAt(i);
		
		std::string year = imagePath.substr(0, 4);
		std::string archiveImagePath = archivePath;
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
