/*
 * ViewManager.cpp
 *
 *  Created on: Jan 23, 2015
 *      Author: wzw7yn
 */
#include <stdio.h>
#include <string>
#include "SAUtils.h"
#include "ConfigReader.h"
#include "ViewManager.h"
#include <memory>
#include "CSVDBFile.h"
#include "CLogger.h"
#include <fstream>

namespace simplearchive {

std::string ViewManager::m_confpath;
ViewManager *ViewManager::m_this = 0;
std::string ViewManager::m_archiveRoot;
std::string ViewManager::m_masterViewPath;
ViewItemContainer *ViewManager::m_pContainer = 0;

/**
	This is a View Item i.e. a View not the items in the view.
	This contains the view as read from the view.conf
*/

class ViewItem {
public:

	/// Direct or Indirect copies.
	/// Direct copies do not use a script to copy
	/// Uses a script to copy. This is used to copy to say Google Drive,
	typedef enum {
		VM_Unknown,
		VM_SnapShot,
		VM_Dynamic
	} Mode;

	/// Mirror Type
	/// Direct mirror
	/// Staging mirror
	typedef enum {
		VA_Unknown,
		VA_ReadWrite,
		VA_ReadOnly
	} Access;

	typedef enum {
		VT_Unknown,
		VT_Link,
		VT_Copy
	} Type;

	typedef enum {
		VS_Unknown,
		VS_IncludeSet,
		VS_ExcludeSet,
		VS_InclusiveSearch,
		VS_ExclusiveSearch,
		VS_ALL
	} ViewSet;

private:
	bool m_error;
	/// Name of mirror
	std::string m_name;
	/// Path to mirror or staging area
	std::string m_path;
	/// Copy script name
	std::string m_archivePath;
	/// include file name
	std::string m_includeName;
	/// Copy script name
	std::string m_scriptName;
	/// Direct/Indirect Mode
	Mode m_eMode;
	/// Direct Write/Staged Write Mode
	Type m_eType;
	Access m_eAccess;
	/// The set of images to be include in the set
	ViewSet m_eViewSet;
	std::string m_setPath;

	std::string m_set;
	std::string m_opt1;
	std::string m_opt2;
	std::string m_opt3;

	bool processAll();
	bool processSet(const char *setFile, bool include);
	bool processSearch(const char *setFile, bool include);
	void findImage();
	bool copyFile(const char*source, const char *distfile);
	bool add(std::auto_ptr<ImageInfo> imageInfo);
public:

	ViewItem(std::string &root, std::string &path) {
		m_name = "Master";
		m_path = path;
		m_archivePath = root;
	}

	ViewItem(const char *root, const char *name, const char *dataString) {
		m_archivePath = root;
		m_error = false;
		m_name = name;

		std::string data = dataString;
		int delim1 = std::string::npos;		// Mode (Direct/Indirect).
		int delim2 = std::string::npos;	//
		int delim3 = std::string::npos;
		int delim4 = std::string::npos;
		int delim5 = std::string::npos;
		int delim6 = std::string::npos;
		int delim7 = std::string::npos;

		delim1 = data.find_first_of(",");			// Mode (Direct/Indirect).
		delim2 = data.find_first_of(",", delim1+1);	//
		delim3 = data.find_first_of(",", delim2+1);
		delim4 = data.find_first_of(",", delim3+1);
		if (delim4 != ((int)std::string::npos)) {
			delim5 = data.find_first_of(",", delim4+1);
		} else if (delim5 != std::string::npos) {
			delim6 = data.find_first_of(",", delim5+1);
		} else if (delim6 != std::string::npos) {
			delim7 = data.find_first_of(",", delim6+1);
		}

		bool complete = false;

		m_eMode = VM_SnapShot;
		m_eType = VT_Unknown;
		m_eAccess = VA_ReadOnly;

		std::string type;
		std::string set = "all";
		std::string access;

		std::string mode = data.substr(0,delim1);			// pos 1
		m_path = data.substr(delim1+1, (delim2-delim1)-1);	// pos 2
		type = data.substr(delim2+1, (delim3-delim2)-1);	// pos 3

		if (delim4 == std::string::npos) {
			access = data.substr(delim3+1, data.length());													// pos 5
			complete = true;
		} else {
			access = data.substr(delim3+1, (delim4-delim3)-1);							// pos 6 and end
		}
		if (complete != true) {
			if (delim5 == std::string::npos) {
				set = data.substr(delim4+1, data.length());
				complete = true;				// pos 5
			} else {
				set = data.substr(delim4+1, (delim5-delim4)-1);							// pos 6 and end
			}
		}
		if (complete != true) {													// pos 7
			if (delim6 == std::string::npos) {
				m_opt1 = data.substr(delim5+1, data.length());
				complete = true;												// pos 4
			} else {
				m_opt1 = data.substr(delim5+1, (delim6-delim5)-1);				// pos 4 and end
			}
		}
		if (complete != true) {
			if (delim7 != std::string::npos) {
				m_opt2 = data.substr(delim7+1, data.length());												// pos 5
			} else if (complete != true) {
				m_opt2 = data.substr(delim6+1, (delim7-delim6)-1);
				m_opt3 = data.substr(delim6+1, data.length());								// pos 5 and end
			}
		}

		if (mode.compare("dynamic") == 0) {
			m_eMode = VM_Dynamic;
		} else if (mode.compare("snapshot") == 0) {
			m_eMode = VM_SnapShot;
		} else {
			m_eMode = VM_Unknown;
			m_error = true;
		}

		if (access.compare("readwrite") == 0) {
			m_eAccess = VA_ReadWrite;
		} else if (access.compare("readonly") == 0) {
			m_eAccess = VA_ReadOnly;
		} else {
			m_eAccess = VA_Unknown;
			m_error = true;
		}

		if (type.compare("link") == 0) {
			m_eType = VT_Link;
		} else if (type.compare("copy") == 0) {
			m_eType = VT_Copy;
		} else {
			m_eType = VT_Unknown;
			m_error = true;
		}
		/*
		 MS_Unknown,
		MS_IncludeSet,
		MS_ExcludeSet,
		MS_InclusiveSearch,
		MS_ExclusiveSearch,
		MS_ALL
		 */

		if (set.compare("include") == 0) {
			m_eViewSet = VS_IncludeSet;
			m_includeName = m_opt1;

		} else if (set.compare("exclude") == 0) {
			m_eViewSet = VS_ExcludeSet;
			m_includeName = m_opt1;

		} else if (set.compare("inclusive_search") == 0) {
			m_eViewSet = VS_InclusiveSearch;
			m_includeName = m_opt1;

		} else if (set.compare("exclusive_search") == 0) {
			m_eViewSet = VS_ExclusiveSearch;
			m_includeName = m_opt1;

		} else if (set.compare("all") == 0) {
			m_eViewSet = VS_ALL;
		} else {
			m_eViewSet = VS_Unknown;
			m_error = true;
		}
	}

	~ViewItem() {

	}

	bool isError() const {
		return m_error;
	}

	const char *getModeString() const {
		switch (m_eMode) {
		case VM_SnapShot: return "SnapShot";
		case VM_Dynamic: return "Dynamic";
		case VM_Unknown: return "Unknown";
		}
		return "Unknown";
	}

	Mode getMode() const {
		return m_eMode;
	}

	const std::string& getName() const {
		return m_name;
	}

	const std::string& getPath() const {
		return m_path;
	}

	const char *getAccessString() const {
		switch (m_eAccess) {
		case VA_ReadWrite: return "Read/Write";
		case VA_ReadOnly: return "Read Only";
		case VA_Unknown: return "Unknown";
		}
		return "Unknown";
	}

	Access getAccess() const {
		return m_eAccess;
	}

	const char *getTypeString() const {
		switch (m_eType) {
		case VT_Link: return "Link";
		case VT_Copy: return "Copy";
		case VT_Unknown: return "Unknown";
		}
		return "Unknown";
	}

	Type getType() const {
		return m_eType;
	}

	const char *getSetString() const {
		switch (m_eViewSet) {
		case VS_IncludeSet: return "include";
		case VS_ExcludeSet: return "exclude";
		case VS_InclusiveSearch: return "inclusive_search";
		case VS_ExclusiveSearch: return "exclusive_search";
		case VS_ALL: return "all";
		case VT_Unknown: return "unknown";
		}
		return "Unknown";
	}

	ViewSet getSet() const {
		return m_eViewSet;
	}

	void print() {
		printf("Name: \"%s\"\n", m_name.c_str());
		printf("Path: \"%s\"\n", m_path.c_str());
		printf("Mode: \"%s\"\n", getModeString());
		printf("Type: \"%s\"\n", getTypeString());
		printf("Access: \"%s\"\n", getAccessString());
		printf("set: \"%s\"\n", getSetString());
		printf("opt1: \"%s\"\n", m_opt1.c_str());
		printf("opt2: \"%s\"\n", m_opt2.c_str());
		printf("opt3: \"%s\"\n", m_opt3.c_str());
	}

	bool process();
	//bool processMaster();
	//bool add2Master(const char *relPath, const char *name);
	bool add(int id);
	bool add(const char *imagePath) {

	}

	const std::string& getIncludeName() const {
		return m_includeName;
	}
};

class ViewItemContainer : public std::vector<ViewItem> {};
class ImageSetContainer : public std::vector<std::string> {};

class ImageSetReader {
	std::string m_setFilePath;
	std::auto_ptr<ImageSetContainer> m_imageSetContainer;
public:
	ImageSetReader() : m_imageSetContainer(new ImageSetContainer) {}
	bool read(const char *setFilePath) {

		char text[1024];

		std::ifstream file(setFilePath);
		if (file.is_open() == false) {
			return false;
		}

		while (file.getline(text, 100)) {
			if (text[0] == '\0') {
				continue;
			}
			m_imageSetContainer->push_back(text);
		}
		file.close();
		return true;

	}

	const std::auto_ptr<ImageSetContainer>& getImageSetContainer() const {
		return m_imageSetContainer;
	}
};
static std::string getYear(const char *path) {
	std::string fpath = path;
	return fpath.substr(0, 4);
}

bool ViewItem::process() {
	switch (getSet()) {
	case VS_IncludeSet:
		if (processSet(getIncludeName().c_str(), true) == false) {
			return false;
		}
		return true;
	case VS_ExcludeSet:
		if (processSet(getIncludeName().c_str(), true) == false) {
			return false;
		}
		return true;
	case VS_InclusiveSearch:
		if (processSet(getIncludeName().c_str(), true) == false) {
			return false;
		}
		return true;
	case VS_ExclusiveSearch:
		if (processSet(getIncludeName().c_str(), true) == false) {
			return false;
		}
		return true;
	case VS_ALL:
		if (processAll() == false) {
			return false;
		}
		return true;
	case VT_Unknown:
		break;
	}
	return false;
	//findImage();
}

bool ViewItem::add(int id) {
	std::string indexPath = m_archivePath;
	indexPath += "/.sia/imageid";
	CSVDBFile csvDBFile(indexPath.c_str());
	std::auto_ptr<ImageInfo> ii = csvDBFile.getItemAt(id);
	switch (getSet()) {
		/*
	case VS_IncludeSet:
		if (processSet(getIncludeName().c_str(), true) == false) {
			return false;
		}
		return true;
	case VS_ExcludeSet:
		if (processSet(getIncludeName().c_str(), true) == false) {
			return false;
		}
		return true;
	case VS_InclusiveSearch:
		if (processSet(getIncludeName().c_str(), true) == false) {
			return false;
		}
		return true;
	case VS_ExclusiveSearch:
		if (processSet(getIncludeName().c_str(), true) == false) {
			return false;
		}
		return true;
		*/
	case VS_ALL:
		if (add(ii) == false) {
			return false;
		}
		return true;
	case VT_Unknown:
		break;
	}
	return false;
	//findImage();
}

bool ViewItem::add(std::auto_ptr<ImageInfo> imageInfo) {

	std::string relpath = imageInfo->getImagePath();
	std::string fullrelpath = relpath;
	fullrelpath += '/';
	fullrelpath += imageInfo->getName();
	std::string sourceFile = m_archivePath;
	sourceFile += '/';
	sourceFile += fullrelpath;
	printf("name %s\n", sourceFile.c_str());
	std::string distFile = getPath();
	distFile += '/';
	distFile += fullrelpath;
	printf("name %s\n", distFile.c_str());

	if (SAUtils::makePath(getPath().c_str(), relpath.c_str()) == false) {
		return false;
	}
	if (copyFile(sourceFile.c_str(), distFile.c_str()) == false) {
		return false;
	}

	return true;
}

bool ViewItem::processSet(const char *setFile, bool include) {

	ImageSetReader imageSetReader;
	if (imageSetReader.read(setFile) == false) {
		return false;
	}
	const std::auto_ptr<ImageSetContainer>& images = imageSetReader.getImageSetContainer();

	for (ImageSetContainer::iterator i = images->begin(); i != images->end(); i++) {
		std::string& name = *i;
		std::string year = getYear(name.c_str());
		std::string relpath = year;
		relpath += '/';
		relpath += name;

		std::string sourceFile = m_archivePath;
		sourceFile += '/';
		sourceFile += relpath;
		printf("name %s\n", sourceFile.c_str());
		std::string distFile = getPath();
		distFile += '/';
		distFile += relpath;
		printf("name %s\n", distFile.c_str());

		if (SAUtils::makePath(getPath().c_str(), relpath.c_str()) == false) {
			return false;
		}
		if (copyFile(sourceFile.c_str(), distFile.c_str()) == false) {
			return false;
		}
	}


	return true;
}

void ViewItem::findImage() {

	std::vector<std::string *> *filelist = SAUtils::getFiles(m_archivePath.c_str());

	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *name = *i;
		if ((*name)[0] == '.') {
			continue;
		}
		printf("name %s\n", name->c_str());
		std::string path = m_archivePath;
		path += '/';
		path += name->c_str();
		std::vector<std::string *> *filelist = SAUtils::getFiles(path.c_str());
		for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string *name = *i;
			if ((*name)[0] == '.') {
				continue;
			}
			printf("name %s\n", name->c_str());
			std::string filepath = path;
			filepath += '/';
			filepath += name->c_str();
			std::vector<std::string *> *filelist = SAUtils::getFiles(filepath.c_str());
			for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
				std::string *name = *i;
				if ((*name)[0] == '.') {
					continue;
				}
				printf("name %s\n", name->c_str());
			}
		}
	}
}

bool ViewItem::processSearch(const char *setFile, bool include) {
	return true;
}



bool ViewItem::processAll() {

	std::string indexPath = m_archivePath;
	indexPath += "/.sia/imageid";
	CSVDBFile csvDBFile(indexPath.c_str());
	int max = csvDBFile.getMaxIndex();
	printf("max: %d\n", max);
	int size = 0;
	int current = 0;

	for (;current <= max; current++) {
		std::auto_ptr<ImageInfo> ii = csvDBFile.getItemAt(current);
		if (add(ii) == false) {
			return false;
		}
	}
	return true;
}

bool ViewItem::copyFile(const char *source, const char *distfile) {
	Type type = getType();
	switch(type) {
	case VT_Link:
		if (SAUtils::makeLink(source, distfile) == false) {
			return false;
		}
		break;
	case VT_Copy:
		if (SAUtils::copy(source, distfile) == false) {
			return false;
		}
		break;
	}
	return true;
}



ViewManager::ViewManager() {
	m_pContainer = new ViewItemContainer;

}

ViewManager::~ViewManager() {
	if (m_pContainer) {
		delete m_pContainer;
	}
}

bool ViewManager::readConf() {
	
	std::string path = m_confpath;
	path += VIEW_CONFIG_FILE;
	
	ConfigReader configReader;
	Config config;
	if (configReader.read(path.c_str(), config) == false) {
		return false;
	}
	
	for (std::map<std::string, std::string>::iterator ii = config.begin(); ii != config.end(); ++ii) {
		ViewItem viewItem(m_archiveRoot.c_str(), (*ii).first.c_str(), (*ii).second.c_str());
		viewItem.print();
		m_pContainer->insert(m_pContainer->end(), viewItem);
	}
	return true;
}

bool ViewManager::initalise(const char *archiveRoot, const char *confpath) {
	CLogger &logger = CLogger::getLogger();
	m_confpath = confpath;
	m_archiveRoot = archiveRoot;

	if (m_this == 0) {
		m_this = new ViewManager;
	}
	if (m_this->readConf() == false) {
		logger.log(CLogger::ERROR, "Cannot read view config file: \"%s\"", confpath);
		return false;
	}
	logger.log(CLogger::FINE, "Read view config file: \"%s\"", confpath);

	for (std::vector<ViewItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		ViewItem &data = *i;
		if (SAUtils::DirExists(data.getPath().c_str()) == false) {
			logger.log(CLogger::ERROR, "View:%s cannot find path \"%s\"", data.getName().c_str(), data.getPath().c_str());

			return false;
		}
	}
	return true;
}

bool ViewManager::process() {
	CLogger &logger = CLogger::getLogger();

	for (std::vector<ViewItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		ViewItem &data = *i;
		if (SAUtils::DirExists(data.getPath().c_str()) == false) {
			logger.log(CLogger::ERROR, "View:%s cannot find path \"%s\"", data.getName().c_str(), data.getPath().c_str());

			return false;
		}
		if (data.process() == false) {
			return false;
		}
	}
	return true;
}

bool ViewManager::add(int id) {
	CLogger &logger = CLogger::getLogger();

	for (std::vector<ViewItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		ViewItem &data = *i;
		if (SAUtils::DirExists(data.getPath().c_str()) == false) {
			logger.log(CLogger::ERROR, "View:%s cannot find path \"%s\"", data.getName().c_str(), data.getPath().c_str());

			return false;
		}
		if (data.add(id) == false) {
			return false;
		}
	}
	return true;
}

/*
bool ViewManager::add2Master(const char *relPath, const char *name) {
	ViewItem viewItem(m_archiveRoot, m_masterViewPath);
	if (viewItem.add2Master(relPath, name) == false) {
		return false;
	}
	return true;
}

bool ViewManager::processMaster() {
	CLogger &logger = CLogger::getLogger();

	if (m_this == 0) {
		m_this = new ViewManager;
	}

	ViewItem viewItem(m_archiveRoot, m_masterViewPath);
	if (SAUtils::DirExists(viewItem.getPath().c_str()) == false) {
		logger.log(CLogger::ERROR, "View:%s cannot find path \"%s\"", viewItem.getName().c_str(), viewItem.getPath().c_str());
		if (SAUtils::mkDir(viewItem.getPath().c_str()) == false) {
			logger.log(CLogger::ERROR, "View:%s cannot create path \"%s\"", viewItem.getName().c_str(), viewItem.getPath().c_str());
			return false;
		}
	}
	if (viewItem.processMaster() == false) {
		return false;
	}
}
*/

} /* namespace simplearchive */
