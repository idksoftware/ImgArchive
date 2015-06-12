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

#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include "CatalogManager.h"
#include "Catalog.h"
#include "CheckDisk.h"
#include "CSVArgs.h"
#include "CDate.h"
#include "SAUtils.h"
#include "ArchiveVisitor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

std::string CatalogManager::m_catalogPath;
std::string CatalogManager::m_shadowPath;

std::auto_ptr<CatalogManager> CatalogManager::m_this(0);

/**
 * brief This class contains the information for a folder within the catalog.
 *
 */
class FolderCatalog {

	std::string m_folderName;
	int m_nFiles;
	int m_nFolders;
	CDate m_time;
public:
	FolderCatalog() {
		m_nFiles = 0;
		m_nFolders = 0;
	};
	FolderCatalog(std::string data) {
		CSVArgs csvArgs(':');
		if (csvArgs.process(data.c_str()) == false) {
			//return false;
		}

		m_folderName = csvArgs.at(0);
		std::string nFolderStr = csvArgs.at(1);
		m_nFolders = strtol(nFolderStr.c_str(),NULL,10);
		std::string nFilesStr = csvArgs.at(2);
		m_nFiles = strtol(nFilesStr.c_str(),NULL,10);
		std::string timeStr = csvArgs.at(3);
		m_time = CDate(timeStr.c_str());
	};
	/// @brief This constructs a FolderCatalog item. This tracks the number
	/// 	   of files and folders under this named folder.
	/// @param folderName	This folder name
	/// @param folders		Number of folders in this folder
	/// @param files		Number of files in this folder
	FolderCatalog(const char *folderName, int folders = 0, int files = 0) {
		CDate tmp;
		m_time = tmp.timeNow();
		m_folderName = folderName;
		m_nFiles = files;
		m_nFolders = folders;
	}

	virtual ~FolderCatalog() {
	}

	/// @brief Returns a string object of the data in the FolderCatalog item in the form
	/// 	   of a CSV line;
	std::string toString() {
			std::stringstream s;
			s << m_folderName << ':' << m_nFolders << ':' << m_nFiles << ':' << m_time.current();
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

	CDate getTime() const {
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

/**
 * Brief This is used to contain folder data.
 */
class FolderCatalogContainer : public std::vector<FolderCatalog> {

	std::string m_currentName;
public:
	FolderCatalogContainer(const char *path) {
		m_currentName = path;
		printf("path %s: \n", path);
	};

	FolderCatalogContainer() {};
	virtual ~FolderCatalogContainer() {};

	bool read(const char *datafile);
	bool write(const char *datafile);
	//bool add(const char *name, int crc, const char *md5);
	int find(const char *datafile);
	bool add(const char *filename, int folders = 0, int files = 0);

	/// @brief Increments The file total in the object
	/// If the folder exists will increment the number of
	/// files if the folder does not exist will add the folder
	/// and increment the file total by one.
	/// @param foldername The name of the folder.
	/// @return False means the folder present, True means the
	/// folder was added.
	bool incFiles(const char *foldername);
	bool incFolders(const char *foldername);

	const std::string& getCurrentName() const {
		return m_currentName;
	}
	void print();
};

bool FolderCatalogContainer::read(const char *datafile) {
	char text[256];
	std::ifstream file(datafile);
	printf("Reading %s\n", datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		std::string s = text;
		FolderCatalog fc(s);
		printf("%s\n", fc.toString().c_str());
		push_back(fc);
	}
	file.close();
	//print();
	return true;
}

void FolderCatalogContainer::print() {
	for (std::vector<FolderCatalog>::iterator i = begin(); i != end(); i++) {
		FolderCatalog &data = *i;
		printf("%s\n", data.toString().c_str());
	}
}

bool FolderCatalogContainer::write(const char *datafile) {
	std::ofstream file(datafile);
	printf("Writing %s\n", datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<FolderCatalog>::iterator i = begin(); i != end(); i++) {
		FolderCatalog &data = *i;
		file << data.toString() << '\n';
		printf("%s\n", data.toString().c_str());
	}
	file.close();
	return true;
}

int FolderCatalogContainer::find(const char *filename) {

	for (std::vector<FolderCatalog>::iterator i = begin(); i != end(); i++) {
		FolderCatalog &data = *i;
		if (data.match(filename) == true) {
			return true;
		}

	}
	return false;
}

bool FolderCatalogContainer::add(const char *filename, int folders, int files) {
	FolderCatalog folderCatalog(filename, folders, files);

	int pos = 0;
	for (std::vector<FolderCatalog>::iterator i = begin(); i != end(); i++) {
		FolderCatalog data = *i;
		//printf("%s", data.c_str());
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			return false;
		}
		if (extStr.compare(filename) >= 0) {
			insert(begin()+(pos), folderCatalog);
			return true;
		}
		pos++;
	}
	push_back(folderCatalog);
	return true;

}

bool FolderCatalogContainer::incFolders(const char *filename) {

	for (std::vector<FolderCatalog>::iterator i = begin(); i != end(); i++) {
		FolderCatalog &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFolders();
			return false;
		}
	}
	FolderCatalogContainer::add(filename, 1);
	return true;
}

bool FolderCatalogContainer::incFiles(const char *filename) {


	for (std::vector<FolderCatalog>::iterator i = begin(); i != end(); i++) {
		FolderCatalog &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFiles();
			return false;
		}
	}
	FolderCatalogContainer::add(filename, 0, 1);
	return true;
}


/**
 * brief This class contains the information for a image within the catalog.
 *
 */
class ImageCatalog {

	std::string m_folderName;
	int m_nFiles;
	int m_nFolders;
	CDate m_time;
public:
	ImageCatalog() {
		m_nFiles = 0;
		m_nFolders = 0;
	};
	ImageCatalog(std::string data) {
		CSVArgs csvArgs(':');
		if (csvArgs.process(data.c_str()) == false) {
			//return false;
		}

		m_folderName = csvArgs.at(0);
		std::string nFolderStr = csvArgs.at(1);
		m_nFolders = strtol(nFolderStr.c_str(),NULL,10);
		std::string nFilesStr = csvArgs.at(2);
		m_nFiles = strtol(nFilesStr.c_str(),NULL,10);
		std::string timeStr = csvArgs.at(3);
		m_time = CDate(timeStr.c_str());
	};

	ImageCatalog(const char *folderName, int folders = 0, int files = 0) {
		m_time.timeNow();
		m_folderName = folderName;
		m_nFiles = files;
		m_nFolders = folders;
	}

	virtual ~ImageCatalog() {
	}

	std::string toString() {
			std::stringstream s;
			s << m_folderName << ':' << m_nFolders << ':' << m_nFiles << ':' << m_time.current();
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

	CDate getTime() const {
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

/**
 * Brief This is used to contain image data.
 */
class ImageCatalogContainer : public std::vector<FolderCatalog> {
	std::string m_currentName;
public:

	ImageCatalogContainer(const char *name) {
		m_currentName = name;
		//printf("name %s \n", name);
	};
	ImageCatalogContainer() {};
	virtual ~ImageCatalogContainer() {};

	bool read(const char *datafile);
	bool write(const char *datafile);
	//bool add(const char *name, int crc, const char *md5);
	int find(const char *datafile);
	bool add(const char *filename);
	bool incFiles(const char *filename);
	bool incFolders(const char *filename);

	const std::string& getCurrentName() const {
		return m_currentName;
	}


};

bool ImageCatalogContainer::read(const char *datafile) {
	char text[256];
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		std::string s = text;
		push_back(FolderCatalog(s));
	}
	file.close();

	return true;
}

bool ImageCatalogContainer::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<FolderCatalog>::iterator i = begin(); i != end(); i++) {
		FolderCatalog &data = *i;
		file << data.toString() << '\n';
		//printf("%s\n", data.toString().c_str());
	}
	file.close();
	return true;
}

int ImageCatalogContainer::find(const char *filename) {

	for (std::vector<FolderCatalog>::iterator i = begin(); i != end(); i++) {
		FolderCatalog &data = *i;
		if (data.match(filename) == true) {
			return true;
		}

	}
	return false;
}

bool ImageCatalogContainer::add(const char *filename) {
	FolderCatalog folderCatalog(filename);

	int pos = 0;
	for (std::vector<FolderCatalog>::iterator i = begin(); i != end(); i++) {
		FolderCatalog data = *i;
		//printf("%s", data.c_str());
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			return false;
		}
		if (extStr.compare(filename) >= 0) {
			insert(begin()+(pos), folderCatalog);
			return true;
		}
		pos++;
	}
	push_back(folderCatalog);
	return true;

}

bool ImageCatalogContainer::incFolders(const char *filename) {
	FolderCatalog folderCatalog(filename);

	for (std::vector<FolderCatalog>::iterator i = begin(); i != end(); i++) {
		FolderCatalog &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFolders();
			return false;
		}
	}
	return true;
}

bool ImageCatalogContainer::incFiles(const char *filename) {
	FolderCatalog folderCatalog(filename);

	for (std::vector<FolderCatalog>::iterator i = begin(); i != end(); i++) {
		FolderCatalog &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFiles();
			return true;
		}
	}
	return false;
}


CatalogManager::CatalogManager() {}

CatalogManager::~CatalogManager() {}

bool CatalogManager::addDayFolder(const char *folderName) {
	Catalog catalog(m_catalogPath.c_str(), m_shadowPath.c_str());
	catalog.addDayFolder(folderName);
	return true;
}

bool CatalogManager::addFile(const char *folderPath, const char *fileName) {

	std::string dayPath = folderPath;
	std::string yearPath = dayPath.substr(0,4);
	std::string path = m_catalogPath;
	std::string index = path;

	index += "/index.dat";

	FolderCatalogContainer yearCatalogContainer;
	// Read the year index example "2014"
	yearCatalogContainer.read(index.c_str());
	path += '/';
	path += yearPath;
	if (yearCatalogContainer.find(yearPath.c_str()) == false) {
		if (SAUtils::DirExists(path.c_str()) == false) {
			if (SAUtils::mkDir(path.c_str()) == false) {
				throw std::exception();
			}
		}
	}
	std::string yeardat = path;
	yeardat += ".dat";
	FolderCatalogContainer dayCatalogContainer;
	// Read the day example "2014-03-22"
	dayCatalogContainer.read(yeardat.c_str());
	ImageCatalogContainer imageCatalogContainer;
	path += '/';
	path += dayPath;
	std::string daydat= path;
	daydat += ".dat";
	// Read images example "dsc_2639.jpg"
	imageCatalogContainer.read(daydat.c_str());
	// add the image file
	if (imageCatalogContainer.add(fileName) == false) {
		// return false if the filename is included.
		// do nothing
		imageCatalogContainer.write(daydat.c_str());
		return false;
	}
	imageCatalogContainer.write(daydat.c_str());
	int foldersAdded = 0;
	if (dayCatalogContainer.incFiles(folderPath) == true) {
		// return false if the folderPath is included.
		// but update number files.
		foldersAdded++;

	}
	dayCatalogContainer.write(yeardat.c_str());
	yearCatalogContainer.incFiles(yearPath.c_str());
		// return false if the folderPath is included.
	if (foldersAdded) {
		yearCatalogContainer.incFolders(yearPath.c_str());
	}
	yearCatalogContainer.write(index.c_str());
	return true;
}


CatalogManager &CatalogManager::get() {
	if (!m_this.get()) {
		m_this.reset(new CatalogManager());
	}
	return *m_this;
}

class CatalogAction : public AVAction {
	std::string m_catalogPath;
	std::string m_currentPath;
	FolderCatalogContainer *m_yearCatalogContainer;
	FolderCatalogContainer *m_dayCatalogContainer;
	ImageCatalogContainer *m_imageCatalogContainer;
protected:
        /// On the start of each directory found, this function is run.
	virtual void onStart();
	   /// At the end of each directory found, this function is run.
	virtual void onEnd();
        /// On finding a file, this function is run.
	virtual void onYearFolder(const char *name);
    /// On finding a file, this function is run.
	virtual void onYearEnd();
        /// On finding a directory, this function is run.
	virtual void onDayFolder(const char *name);
    /// On finding a directory, this function is run.
	virtual void onDayEnd();
	   /// On finding a directory, this function is run.
	virtual void onImage(const char *path, const char *name);

        /// This function is a factory function used to create new FolderVisitor objects.
	//virtual AVAction *make() { return new CatalogAction; }
public:

	CatalogAction() {}

	/// Constructor
	CatalogAction(const char *catalogPath) {
		m_catalogPath = catalogPath;
		m_yearCatalogContainer = 0;
		m_dayCatalogContainer = 0;
		m_imageCatalogContainer = 0;
	};
        /// Distructor
	virtual ~CatalogAction() {
	}

};

void CatalogAction::onStart() {
	//printf("onStart: \n");
	m_yearCatalogContainer = new FolderCatalogContainer();

};
   /// At the end of each directory found, this function is run.
void CatalogAction::onEnd() {

	std::string path = m_catalogPath;
	path += "/index.dat";
	m_yearCatalogContainer->write(path.c_str());
	delete m_yearCatalogContainer;
	m_yearCatalogContainer = 0;

	//printf("onEnd\n");
};
	/// On finding a file, this function is run.
void CatalogAction::onYearFolder(const char *name) {
	m_currentPath = m_catalogPath;
	m_currentPath += '/';
	m_currentPath += name;
	//printf("onYearFolder %s: \n", m_currentPath.c_str());


	if (SAUtils::DirExists(m_currentPath.c_str()) == false) {
		if (SAUtils::mkDir(m_currentPath.c_str()) == false) {
			throw std::exception();
		}
	}
	m_yearCatalogContainer->add(name);
	m_dayCatalogContainer = new FolderCatalogContainer(name);

};
/// On finding a file, this function is run.
void CatalogAction::onYearEnd() {
	std::string path = m_currentPath;
	path += ".dat";
	m_dayCatalogContainer->write(path.c_str());
	delete m_dayCatalogContainer;
	m_dayCatalogContainer = 0;
	//printf("onYearEnd\n");
};
	/// On finding a directory, this function is run.
void CatalogAction::onDayFolder(const char *name) {
	m_dayCatalogContainer->add(name);
	m_imageCatalogContainer = new ImageCatalogContainer(name);
	//printf("onDayFolder %s\n", name);
};
/// On finding a directory, this function is run.
void CatalogAction::onDayEnd() {
	std::string path = m_currentPath;
	path += '/';
	path += m_imageCatalogContainer->getCurrentName();
	path += ".dat";
	//printf("onDayEnd %s\n", path.c_str());
	m_imageCatalogContainer->write(path.c_str());
	delete m_imageCatalogContainer;
	m_imageCatalogContainer = 0;
};
   /// On finding a directory, this function is run.
void CatalogAction::onImage(const char *path, const char *name) {
	//printf("\t\tonImage %s: \n", path);
	m_imageCatalogContainer->add(name);
};


bool CatalogManager::makeList() {
	ArchiveVisitor archiveVisitor(new CatalogAction(m_catalogPath.c_str()));
	archiveVisitor.process(m_shadowPath.c_str());
	return true;
}


} /* namespace simplearchive */
