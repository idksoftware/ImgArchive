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
#include "Catalog.h"
#include "SAUtils.h"
#include "CSVArgs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

class CatalogFolder {

	std::string m_folderName;
	int m_nFiles;
	int m_nFolders;
	time_t m_time;
public:
	CatalogFolder() {
		m_nFiles = 0;
		m_nFolders = 0;
		m_time = 0;
	};
	CatalogFolder(std::string &data) {
		CSVArgs csvArgs(':');
		csvArgs.process(data.c_str());
		m_folderName = csvArgs.at(0);
		std::string nFolderStr = csvArgs.at(1);
		m_nFolders = strtol(nFolderStr.c_str(),NULL,10);
		std::string nFilesStr = csvArgs.at(2);
		m_nFiles = strtol(nFilesStr.c_str(),NULL,10);
		std::string timeStr = csvArgs.at(3);
		m_time = strtol(timeStr.c_str(),NULL,10);

		//m_status = Missing;
		/*
		int delim1 = data.find_first_of(":");
		int delim2 = data.find_first_of(":", delim1+1);
		int delim3 = data.find_first_of(":", delim2+1);
		m_folderName = data.substr(0,delim1);
		std::string nFolderStr = data.substr(delim1+1, delim2-(delim1+1));
		m_nFolders = strtol(nFolderStr.c_str(),NULL,10);
		std::string nFilesStr = data.substr(delim2+1, delim3-(delim2+1));
		m_nFiles = strtol(nFilesStr.c_str(),NULL,10);
		std::string timeStr = data.substr(delim3+1, data.length());
		m_time = strtol(timeStr.c_str(),NULL,10);
		*/
	};

	CatalogFolder(const char *folderName, int folders = 0, int files = 0) {
		time(&m_time);
		m_folderName = folderName;
		m_nFiles = files;
		m_nFolders = folders;
	}

	virtual ~CatalogFolder() {
	}

	std::string toString() {
			std::stringstream s;
			s << m_folderName << ':' << m_nFolders << ':' << m_nFiles << ':' << m_time;
			return s.str();
		}

	const std::string& getFolderName() const {
		return m_folderName;
	}

	std::string getYear() const {
		return m_folderName.substr(0,4);
	}

	void setFolderName(const std::string& folderName) {
		m_folderName = folderName;
	}

	time_t getTime() const {
		return m_time;
	}

	void setTime(time_t time) {
		m_time = time;
	}

	bool match(const char *filename) {
		if (m_folderName.compare(filename) == 0) {
			return true;
		}
		return false;
	}

	int getNFiles() const {
		return m_nFiles;
	}

	void setNFiles(int nFiles) {
		m_nFiles = nFiles;
	}

	void incNFiles() {
		m_nFiles++;
	}

	int getNFolders() const {
		return m_nFolders;
	}

	void setNFolders(int nFolders) {
		m_nFolders = nFolders;
	}

	void incNFolders() {
		m_nFolders++;
	}
};



class CatalogContainer : public std::vector<CatalogFolder> {

public:
	CatalogContainer() {};
	virtual ~CatalogContainer() {};

	bool read(const char *datafile);
	bool write(const char *datafile);
	//bool add(const char *name, int crc, const char *md5);
	int find(const char *datafile);
	bool add(const char *filename);
	bool incFiles(const char *filename);
	bool incFolders(const char *filename);
};

bool CatalogContainer::read(const char *datafile) {
	char text[256];
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		std::string s = text;
		push_back(CatalogFolder(s));
	}
	file.close();

	return true;
}

bool CatalogContainer::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<CatalogFolder>::iterator i = begin(); i != end(); i++) {
		CatalogFolder &data = *i;
		file << data.toString() << '\n';
		printf("%s\n", data.toString().c_str());
	}
	file.close();
	return true;
}

int CatalogContainer::find(const char *filename) {

	for (std::vector<CatalogFolder>::iterator i = begin(); i != end(); i++) {
		CatalogFolder &data = *i;
		if (data.match(filename) == true) {
			return true;
		}

	}
	return false;
}

bool CatalogContainer::add(const char *filename) {
	CatalogFolder catalogFolder(filename);

	int pos = 0;
	for (std::vector<CatalogFolder>::iterator i = begin(); i != end(); i++) {
		CatalogFolder data = *i;
		//printf("%s", data.c_str());
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			return false;
		}
		if (extStr.compare(filename) >= 0) {
			insert(begin()+(pos), catalogFolder);
			return true;
		}
		pos++;
	}
	push_back(catalogFolder);
	return false;

}

bool CatalogContainer::incFolders(const char *filename) {
	CatalogFolder catalogFolder(filename);

	for (std::vector<CatalogFolder>::iterator i = begin(); i != end(); i++) {
		CatalogFolder &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFolders();
			return true;
		}
	}
	return false;
}

bool CatalogContainer::incFiles(const char *filename) {
	CatalogFolder catalogFolder(filename);

	for (std::vector<CatalogFolder>::iterator i = begin(); i != end(); i++) {
		CatalogFolder &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFiles();
			return true;
		}
	}
	return false;
}
/*
void CatalogContainer::add(bool isYear, const char *filename) {
	CatalogFolder CatalogFolder(isYear, filename);
	int pos = find(isYear, filename);
	insert(begin()+(pos), CatalogFolder);
}
*/

Catalog::Catalog(const char *catalogPath, const char *archivePath) {
	m_catalogPath = catalogPath;
	m_archivePath = archivePath;
	//m_archivePath = m_archivePath + "/.metadata";
	//m_folderFile = *(new FolderFile);
}

Catalog::~Catalog() {
	// TODO Auto-generated destructor stub
}

bool Catalog::addDayFolder(const char *folderName) {
	std::string path = m_archivePath + std::string(CATALOG_FOLDER);
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
    }
    std::string fpath = path + std::string("/fdata.csv");
    CatalogContainer catalogContainer;
    catalogContainer.read(fpath.c_str());
    catalogContainer.add(folderName);
    catalogContainer.write(fpath.c_str());
    return true;
}

bool Catalog::incFolders(const char *folderName) {
	std::string path = m_archivePath + std::string(CATALOG_FOLDER);
	if (SAUtils::DirExists(path.c_str()) == false) {
		throw std::exception();
    }
    std::string fpath = path + std::string("/fdata.csv");
    if (SAUtils::FileExists(path.c_str()) == false) {
    	throw std::exception();
    }
    CatalogContainer catalogContainer;
    catalogContainer.read(fpath.c_str());
    catalogContainer.incFolders(folderName);
    catalogContainer.write(fpath.c_str());
    return true;
}

bool Catalog::incFiles(const char *folderName) {
	std::string path = m_archivePath + std::string(CATALOG_FOLDER);
	if (SAUtils::DirExists(path.c_str()) == false) {
		throw std::exception();
    }
    std::string fpath = path + std::string("/fdata.csv");
    if (SAUtils::FileExists(path.c_str()) == false) {
    	throw std::exception();
    }
    CatalogContainer catalogContainer;
    catalogContainer.read(fpath.c_str());
    catalogContainer.incFiles(folderName);
    catalogContainer.write(fpath.c_str());
    if (makeXML() == false) {
    	return false;
    }
    return true;
}

bool Catalog::makeXML() {
	std::string path = m_archivePath + std::string(CATALOG_FOLDER);
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	// do files
	std::string fpathxml = path + std::string("/fdata.xml");
	std::ofstream filexml(fpathxml.c_str());
	if (filexml.is_open() == false) {
		return false;
	}
	std::string fpathcsv = path + std::string("/fdata.csv");
	if (SAUtils::FileExists(fpathcsv.c_str()) == false) {
	    throw std::exception();
	}
	CatalogContainer catalogContainer;
	catalogContainer.read(fpathcsv.c_str());

	filexml <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					<<	"<Catalog ordering=\"date\" >\n";
	std::string currYear;
	bool first = true;
	for (std::vector<CatalogFolder>::iterator i = catalogContainer.begin(); i != catalogContainer.end(); i++) {
		CatalogFolder data = *i;
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
		std::string filepath = currYear + "/" + data.getFolderName() + "/.chkdsk/fdata.xml";
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
	filexml <<	"</Catalog>\n";
	filexml.close();

	return true;
}

bool Catalog::makeList() {
	std::string path = m_archivePath + std::string(CATALOG_FOLDER);
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	// do files
	std::string fpath = path + std::string("/fdata.csv");
	std::ofstream filedat(fpath.c_str());
	if (filedat.is_open() == false) {
		return false;
	}
	fpath = path + std::string("/index.xml");
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

		for (std::vector<std::string>::iterator i = dayList->begin(); i != dayList->end(); i++) {
			std::string name = *i;

			char c = (name)[0];
			if (c == '.' ) {
				continue;
			}
			std::string filepath = year + '/' + name;
			std::string linkpath = name + "/index.xml";
			if (makeFolder(filepath, name) == false) {
				return false;
			}
			//printf("folder: %s \n", name->c_str());
			filexml <<	"\t\t<DayFolder Name=\"" << name << "\" >\n";
				filexml <<	"\t\t\t<Filelist>";
				filexml <<	linkpath;
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
bool Catalog::makeFolder(std::string &targetFolder, std::string &filepath) {
	std::string path = m_archivePath + std::string(CATALOG_FOLDER);
	path += '/';
	path += filepath;
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	// do files
	std::string fpath = path + std::string("/fdata.csv");
	std::ofstream filedat(fpath.c_str());
	if (filedat.is_open() == false) {
		return false;
	}
	fpath = path + std::string("/index.xml");
	std::ofstream filexml(fpath.c_str());
	if (filexml.is_open() == false) {
		return false;
	}
	time_t timeValue = 0;
	time(&timeValue);
	std::string filefolder = m_archivePath + '/' + targetFolder;
	filexml <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					<<	"<FolderList Name=\"" <<  filepath << "\">\n";


	FileList_Ptr dayList = SAUtils::getFiles_(filefolder.c_str());

	for (std::vector<std::string>::iterator i = dayList->begin(); i != dayList->end(); i++) {
		std::string name = *i;
		std::string filepath = name;
		std::string linkpath = name + "/index.xml";
		char c = (name)[0];
		if (c == '.' ) {
			continue;
		}
		//printf("folder: %s \n", name->c_str());
		filexml <<	"\t<DayFolder Name=\"" << name << "\" >\n";
			filexml <<	"\t\t<Image>";
			filexml <<	linkpath;
			filexml <<	"</Image>\n";
		filexml <<	"\t</DayFolder>\n";
		filedat << name << "::" << timeValue << '\n';
	}


	filedat.close();
	filexml <<	"</FolderList>\n";
	filexml.close();
	return true;
}

} /* namespace simplearchive */
