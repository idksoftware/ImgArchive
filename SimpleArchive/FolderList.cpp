/*
 * FolderList.cpp
 *
 *  Created on: Oct 3, 2014
 *      Author: wzw7yn
 */
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "FolderList.h"

#include "SAUtils.h"

namespace simplearchive {

class FolderFile {

	std::string m_folderName;
	int m_nFiles;
	int m_nFolders;
	time_t m_time;
public:
	FolderFile() {
		m_nFiles = 0;
		m_nFolders = 0;
		m_time = 0;
	};
	FolderFile(std::string data) {
		//m_status = Missing;
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
	};

	FolderFile(const char *folderName, int folders = 0, int files = 0) {
		time(&m_time);
		m_folderName = folderName;
		m_nFiles = files;
		m_nFolders = folders;
	}

	virtual ~FolderFile() {
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



class FileDataContainer : public std::vector<FolderFile> {

public:
	FileDataContainer() {};
	virtual ~FileDataContainer() {};

	bool read(const char *datafile);
	bool write(const char *datafile);
	//bool add(const char *name, int crc, const char *md5);
	int find(const char *datafile);
	bool add(const char *filename);
	bool incFiles(const char *filename);
	bool incFolders(const char *filename);
};

bool FileDataContainer::read(const char *datafile) {
	char text[256];
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		std::string s = text;
		push_back(FolderFile(s));
	}
	file.close();

	return true;
}

bool FileDataContainer::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<FolderFile>::iterator i = begin(); i != end(); i++) {
		FolderFile &data = *i;
		file << data.toString() << '\n';
		//printf("%s\n", data.toString().c_str());
	}
	file.close();
	return true;
}

int FileDataContainer::find(const char *filename) {

	for (std::vector<FolderFile>::iterator i = begin(); i != end(); i++) {
		FolderFile &data = *i;
		if (data.match(filename) == true) {
			return true;
		}

	}
	return false;
}

bool FileDataContainer::add(const char *filename) {
	FolderFile folderFile(filename);

	int pos = 0;
	for (std::vector<FolderFile>::iterator i = begin(); i != end(); i++) {
		FolderFile data = *i;
		//printf("%s", data.c_str());
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			return false;
		}
		if (extStr.compare(filename) >= 0) {
			insert(begin()+(pos), folderFile);
			return true;
		}
		pos++;
	}
	push_back(folderFile);
	return false;

}

bool FileDataContainer::incFolders(const char *filename) {
	FolderFile folderFile(filename);

	for (std::vector<FolderFile>::iterator i = begin(); i != end(); i++) {
		FolderFile &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFolders();
			return true;
		}
	}
	return false;
}

bool FileDataContainer::incFiles(const char *filename) {
	FolderFile folderFile(filename);

	for (std::vector<FolderFile>::iterator i = begin(); i != end(); i++) {
		FolderFile &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFiles();
			return true;
		}
	}
	return false;
}
/*
void FileDataContainer::add(bool isYear, const char *filename) {
	FolderFile folderFile(isYear, filename);
	int pos = find(isYear, filename);
	insert(begin()+(pos), folderFile);
}
*/

FolderList::FolderList(const char *archivePath) {
	m_archivePath = archivePath;
	//m_archivePath = m_archivePath + "/.sia/metadata";
	//m_folderFile = *(new FolderFile);
}

FolderList::~FolderList() {
	// TODO Auto-generated destructor stub
}

bool FolderList::addDayFolder(const char *folderName) {
	std::string path = m_archivePath + std::string("/.sia");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
		if (SAUtils::setHidden(path.c_str()) == false) {
			throw std::exception();
		}
	}
	path = m_archivePath + std::string("/.sia/chdsk");
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
    }
    std::string fpath = path + std::string("/fdata.csv");
    FileDataContainer fileDataContainer;
    fileDataContainer.read(fpath.c_str());
    fileDataContainer.add(folderName);
    fileDataContainer.write(fpath.c_str());
    return true;
}

bool FolderList::incFolders(const char *folderName) {
	std::string path = m_archivePath;
	path += "/.sia/chdsk";
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
    }
	std::string fpath = path;
	fpath += "/fdata.csv";

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
	path += "/.sia";
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
	fpath += "/fdata.csv";
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
	path += "/.sia/chdsk";
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	// do files
	std::string fpathxml = path;
	fpathxml += "/fdata.xml";
	std::ofstream filexml(fpathxml.c_str());
	if (filexml.is_open() == false) {
		return false;
	}
	std::string fpathcsv = path;
	fpathcsv += "/fdata.csv";
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
	std::string path = m_archivePath + std::string("/.sia/chdsk");
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
	fpath = path + std::string("/fdata.xml");
	std::ofstream filexml(fpath.c_str());
	if (filexml.is_open() == false) {
		return false;
	}
	time_t timeValue = 0;
	time(&timeValue);
	filexml <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					<<	"<FolderList>\n";
	std::vector<std::string *> *filelist = SAUtils::getFiles(m_archivePath.c_str());
	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *year = *i;
		std::string filepath = m_archivePath + "/" + *year;

		char c = (*year)[0];
		if (c == '.' ) {
			continue;
		}
		//printf("%s: \n", year->c_str());
		filexml <<	"\t<YearFolder Name=\"" << *year << "\" >\n";

		std::string yearfolder = m_archivePath + '/' + *year;
		std::vector<std::string *> *dayList = SAUtils::getFiles(yearfolder.c_str());

		for (std::vector<std::string *>::iterator i = dayList->begin(); i != dayList->end(); i++) {
			std::string *name = *i;
			std::string filepath = *year + "/" + *name + "/.sia/chdsk/fdata.xml";

			char c = (*name)[0];
			if (c == '.' ) {
				continue;
			}
			//printf("folder: %s \n", name->c_str());
			filexml <<	"\t\t<DayFolder Name=\"" << *name << "\" >\n";
				filexml <<	"\t\t\t<Filelist>";
				filexml <<	filepath;
				filexml <<	"</Filelist>\n";
			filexml <<	"\t\t</DayFolder>\n";
			filedat << *name << "::" << timeValue << '\n';
		}
		filexml <<	"\t</YearFolder>\n";
	}
	filedat.close();
	filexml <<	"</FolderList>\n";
	filexml.close();
	return true;
	/*
	std::string path = targetdir + std::string("/.sia/chdsk");
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
	fpath = path + std::string("/fdata.xml");
	std::ofstream filexml(fpath.c_str());
	if (filexml.is_open() == false) {
		return false;
	}
	filexml <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
					<<	"<FileList>\n";
	int tmpcrc = 23;
	std::string targetdirStr = targetdir;
	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *name = *i;
		std::string filepath = targetdirStr + "/" + *name;
		char c = (*name)[0];
		if (c == '.' ) {
			continue;
		}
	}
	*/
}
} /* namespace simplearchive */
