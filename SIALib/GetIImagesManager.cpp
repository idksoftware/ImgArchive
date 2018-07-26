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
#include "GetImagesManager.h"
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

std::string GetImagesManager::m_workspacePath;
std::string GetImagesManager::m_primaryIndexPath;

std::shared_ptr<GetImagesManager> GetImagesManager::m_this(0);

/**
 * brief This class contains the information for a folder within the catalog.
 *
 */
class GetImageFolder {

	std::string m_folderName;
	int m_nFiles;
	int m_nFolders;
	CDate m_time;
public:
	GetImageFolder() {
		m_nFiles = 0;
		m_nFolders = 0;
	};
	GetImageFolder(std::string data) {
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
	GetImageFolder(const char *folderName, int folders = 0, int files = 0) {
		CDate tmp;
		m_time = tmp.timeNow();
		m_folderName = folderName;
		m_nFiles = files;
		m_nFolders = folders;
	}

	virtual ~GetImageFolder() {
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
class GetImageFolderContainer : public std::vector<GetImageFolder> {

	std::string m_currentName;
public:
	GetImageFolderContainer(const char *path) {
		m_currentName = path;
		printf("path %s: \n", path);
	};

	GetImageFolderContainer() {};
	virtual ~GetImageFolderContainer() {};

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

bool GetImageFolderContainer::read(const char *datafile) {
	char text[256];
	std::ifstream file(datafile);
	printf("Reading %s\n", datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		std::string s = text;
		GetImageFolder fc(s);
		printf("%s\n", fc.toString().c_str());
		push_back(fc);
	}
	file.close();
	//print();
	return true;
}

void GetImageFolderContainer::print() {
	for (std::vector<GetImageFolder>::iterator i = begin(); i != end(); i++) {
		GetImageFolder &data = *i;
		printf("%s\n", data.toString().c_str());
	}
}

bool GetImageFolderContainer::write(const char *datafile) {
	std::ofstream file(datafile);
	printf("Writing %s\n", datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<GetImageFolder>::iterator i = begin(); i != end(); i++) {
		GetImageFolder &data = *i;
		file << data.toString() << '\n';
		printf("%s\n", data.toString().c_str());
	}
	file.close();
	return true;
}

int GetImageFolderContainer::find(const char *filename) {

	for (std::vector<GetImageFolder>::iterator i = begin(); i != end(); i++) {
		GetImageFolder &data = *i;
		if (data.match(filename) == true) {
			return true;
		}

	}
	return false;
}

bool GetImageFolderContainer::add(const char *filename, int folders, int files) {
	GetImageFolder folderCatalog(filename, folders, files);

	int pos = 0;
	for (std::vector<GetImageFolder>::iterator i = begin(); i != end(); i++) {
		GetImageFolder data = *i;
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

bool GetImageFolderContainer::incFolders(const char *filename) {

	for (std::vector<GetImageFolder>::iterator i = begin(); i != end(); i++) {
		GetImageFolder &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFolders();
			return false;
		}
	}
	GetImageFolderContainer::add(filename, 1);
	return true;
}

bool GetImageFolderContainer::incFiles(const char *filename) {


	for (std::vector<GetImageFolder>::iterator i = begin(); i != end(); i++) {
		GetImageFolder &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFiles();
			return false;
		}
	}
	GetImageFolderContainer::add(filename, 0, 1);
	return true;
}


/**
 * brief This class contains the information for a image within the catalog.
 *
 */
class GetImageCatalog {

	std::string m_folderName;
	int m_nFiles;
	int m_nFolders;
	CDate m_time;
public:
	GetImageCatalog() {
		m_nFiles = 0;
		m_nFolders = 0;
	};
	GetImageCatalog(std::string data) {
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

	GetImageCatalog(const char *folderName, int folders = 0, int files = 0) {
		m_time.timeNow();
		m_folderName = folderName;
		m_nFiles = files;
		m_nFolders = folders;
	}

	virtual ~GetImageCatalog() {
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
class GetImageCatalogContainer : public std::vector<GetImageCatalog> {
	std::string m_currentName;
public:

	GetImageCatalogContainer(const char *name) {
		m_currentName = name;
		//printf("name %s \n", name);
	};
	GetImageCatalogContainer() {};
	virtual ~GetImageCatalogContainer() {};

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

bool GetImageCatalogContainer::read(const char *datafile) {
	char text[256];
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		std::string s = text;
		push_back(GetImageCatalog(s));
	}
	file.close();

	return true;
}

bool GetImageCatalogContainer::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<GetImageCatalog>::iterator i = begin(); i != end(); i++) {
		GetImageCatalog &data = *i;
		file << data.toString() << '\n';
		//printf("%s\n", data.toString().c_str());
	}
	file.close();
	return true;
}

int GetImageCatalogContainer::find(const char *filename) {

	for (std::vector<GetImageCatalog>::iterator i = begin(); i != end(); i++) {
		GetImageCatalog &data = *i;
		if (data.match(filename) == true) {
			return true;
		}

	}
	return false;
}

bool GetImageCatalogContainer::add(const char *filename) {
	GetImageCatalog folderCatalog(filename);

	int pos = 0;
	for (std::vector<GetImageCatalog>::iterator i = begin(); i != end(); i++) {
		GetImageCatalog data = *i;
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

bool GetImageCatalogContainer::incFolders(const char *filename) {
	GetImageCatalog folderCatalog(filename);

	for (std::vector<GetImageCatalog>::iterator i = begin(); i != end(); i++) {
		GetImageCatalog &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFolders();
			return false;
		}
	}
	return true;
}

bool GetImageCatalogContainer::incFiles(const char *filename) {
	GetImageCatalog folderCatalog(filename);

	for (std::vector<GetImageCatalog>::iterator i = begin(); i != end(); i++) {
		GetImageCatalog &data = *i;
		std::string extStr = data.getFolderName();
		if (extStr.compare(filename) == 0) {
			data.incNFiles();
			return true;
		}
	}
	return false;
}


GetImagesManager::GetImagesManager() {}

GetImagesManager::~GetImagesManager() {}

bool GetImagesManager::addDayFolder(const char *folderName) {
	Catalog catalog(m_workspacePath.c_str(), m_primaryIndexPath.c_str());
	catalog.addDayFolder(folderName);
	return true;
}

bool GetImagesManager::addFile(const char *folderPath, const char *fileName) {

	std::string dayPath = folderPath;
	std::string yearPath = dayPath.substr(0,4);
	std::string path = m_workspacePath;
	std::string index = path;

	index += "/index.dat";

	GetImageCatalogContainer yearCatalogContainer;
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
	GetImageCatalogContainer dayCatalogContainer;
	// Read the day example "2014-03-22"
	dayCatalogContainer.read(yeardat.c_str());
	GetImageCatalogContainer imageCatalogContainer;
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


GetImagesManager &GetImagesManager::get() {
	if (!m_this.get()) {
		m_this.reset(new GetImagesManager());
	}
	return *m_this;
}

class ImageCatalogAction : public AVAction {
	std::string m_catalogPath;
	std::string m_currentPath;
	GetImageCatalogContainer *m_yearCatalogContainer;
	GetImageCatalogContainer *m_dayCatalogContainer;
	GetImageCatalogContainer *m_imageCatalogContainer;
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

	ImageCatalogAction() {}

	/// Constructor
	ImageCatalogAction(const char *catalogPath) {
		m_catalogPath = catalogPath;
		m_yearCatalogContainer = 0;
		m_dayCatalogContainer = 0;
		m_imageCatalogContainer = 0;
	};
        /// Distructor
	virtual ~ImageCatalogAction() {
	}

};

void ImageCatalogAction::onStart() {
	//printf("onStart: \n");
	m_yearCatalogContainer = new GetImageCatalogContainer();

};
   /// At the end of each directory found, this function is run.
void ImageCatalogAction::onEnd() {

	std::string path = m_catalogPath;
	path += "/index.dat";
	m_yearCatalogContainer->write(path.c_str());
	delete m_yearCatalogContainer;
	m_yearCatalogContainer = 0;

	//printf("onEnd\n");
};
	/// On finding a file, this function is run.
void ImageCatalogAction::onYearFolder(const char *name) {
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
	m_dayCatalogContainer = new GetImageCatalogContainer(name);

};
/// On finding a file, this function is run.
void ImageCatalogAction::onYearEnd() {
	std::string path = m_currentPath;
	path += ".dat";
	m_dayCatalogContainer->write(path.c_str());
	delete m_dayCatalogContainer;
	m_dayCatalogContainer = 0;
	//printf("onYearEnd\n");
};
	/// On finding a directory, this function is run.
void ImageCatalogAction::onDayFolder(const char *name) {
	m_dayCatalogContainer->add(name);
	m_imageCatalogContainer = new GetImageCatalogContainer(name);
	//printf("onDayFolder %s\n", name);
};
/// On finding a directory, this function is run.
void ImageCatalogAction::onDayEnd() {
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
void ImageCatalogAction::onImage(const char *path, const char *name) {
	//printf("\t\tonImage %s: \n", path);
	m_imageCatalogContainer->add(name);
};


bool GetImagesManager::makeList() {
	ArchiveVisitor archiveVisitor(new ImageCatalogAction(m_workspacePath.c_str()));
	archiveVisitor.process(m_primaryIndexPath.c_str());
	return true;
}


} /* namespace simplearchive */
