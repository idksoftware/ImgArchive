#include "ArchiveProcessor.h"
#include "FolderData.h"
namespace simplearchive {

	ArchiveFolderVistor::ArchiveFolderVistor(const char *archivePath) {
		m_archivePath = archivePath;
	}

	ArchiveFolderVistor::ArchiveFolderVistor(const char *archivePath, const char *workspacePath) {
		m_archivePath = archivePath;
		m_workspacePath = workspacePath;
	}


	bool ShadowFolderVistor::process(const char *addressScope) {
		m_AddressScope.scope(addressScope);

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
		FileDataContainer folderDataContainer;	// Contains the folder contents note in the form of yyyy-mm-dd
		folderDataContainer.read(fpath.c_str());
		std::string dataPath = m_archivePath;
		// place holder for reporting object ValidateReportingObject validateReportingObject;
		// root year list
		FileList_Ptr yearlist = SAUtils::getFiles_(dataPath.c_str());
		for (std::vector<std::string>::iterator i = yearlist->begin(); i != yearlist->end(); i++) {
			std::string dataString = *i;
			if (dataString[0] == '.') {
				continue;
			}

			std::string year = dataString.substr(0, 4);

			if (year.compare("system") == 0) {
				continue;
			}
			printf("Year found %s\n", year.c_str());
			std::string yearPath = dataPath += '/';
			yearPath += year;


			FileList_Ptr daylist = SAUtils::getFiles_(yearPath.c_str());
			for (std::vector<std::string>::iterator i = daylist->begin(); i != daylist->end(); i++) {
				std::string dataString = *i;
				if (dataString[0] == '.') {
					continue;
				}

				std::string filepath = m_archivePath + "/" + dataString;

				if (folderDataContainer.find(dataString.c_str()) == false) {
					printf("File not found %s\n", dataString.c_str());
				}
				else {
					std::string yearDayPath = yearPath;
					yearDayPath += '/';
					yearDayPath += dataString;
					printf("File found %s\n", dataString.c_str());
					std::string tmp = yearDayPath;
					// shadow
					yearDayPath += "/data";
					tmp += "/chdsk";
					//if (checkDisk.check(yearDayPath.c_str(), tmp.c_str(), dataString.c_str(), &validateReportingObject) == false) {
					if (doWork(yearDayPath.c_str(), tmp.c_str(), dataString.c_str(), nullptr) == false) {
						return false;
					}
				}
			}

		}
		//validateReportingObject.save();
		//m_shadowJournalName = validateReportingObject.getJournalName();
		return true;
	}

	bool WorkspaceFolderVistor::process(const char *addressScope) {

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
			printf("Year found %s\n", year.c_str());
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
					printf("File found %s\n", dataString.c_str());
					std::string tmp = yearDayPath;


					std::string archivePath = m_archivePath;
					// shadow
					archivePath += '/'; archivePath += year;
					archivePath += '/'; archivePath += dataString;
					archivePath += "/chdsk";

					if (doWork(yearDayPath.c_str(), archivePath.c_str(), dataString.c_str(), nullptr) == false) {
						return false;
					}
				}
			}

		}
		//m_validateReportingObject.save();
		//m_workspaceJournalName = validateReportingObject.getJournalName();
		return true;
	}

}; 

