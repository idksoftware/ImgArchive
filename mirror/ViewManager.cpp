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
#include <string>
#include "SAUtils.h"
#include "ConfigReader.h"
#include "ViewManager.h"
#include <memory>
#include "CSVDBFile.h"
#include "CLogger.h"
#include <fstream>
#include "CSVArgs.h"
#include "AppPaths.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

std::string ViewManager::m_confpath;

std::string ViewManager::m_archiveRoot;
//std::string ViewManager::m_masterViewPath;
ViewItemContainer *ViewManager::m_pContainer = 0;



std::unique_ptr<ViewManager> ViewManager::m_instance;
std::once_flag ViewManager::m_onceFlag;

ViewManager& ViewManager::GetInstance()
{
	// Add safe guard against not calling initalise()  
	return *m_instance.get();
}


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
	bool add(std::unique_ptr<ImageInfo>& imageInfo);
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
		//bool complete = false;

		m_eMode = VM_SnapShot;
		m_eType = VT_Unknown;
		m_eAccess = VA_ReadOnly;

		std::string type;
		std::string set = "all";
		std::string access;

		CSVArgs csvArgs(',');
		csvArgs.process(dataString);

		std::string mode = csvArgs.at(0);
		m_path = csvArgs.at(1);
		type = csvArgs.at(2);
		//int size = csvArgs.size();
		if (csvArgs.size() >= 4) {
			access = csvArgs.at(3);
		}
		if (csvArgs.size() >= 5) {
			set = csvArgs.at(4);
		}
		if (csvArgs.size() >= 6) {
			m_opt1 = csvArgs.at(5);
		}
		if (csvArgs.size() >= 7) {
			m_opt1 = csvArgs.at(6);
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
		return false;
	}

	const std::string& getIncludeName() const {
		return m_includeName;
	}
};

class ViewItemContainer : public std::vector<ViewItem> {};
class ImageSetContainer : public std::vector<std::string> {};

class ImageSetReader {
	std::string m_setFilePath;
	std::shared_ptr<ImageSetContainer> m_imageSetContainer;
public:
	ImageSetReader() : m_imageSetContainer(std::make_shared<ImageSetContainer>()) {}
	bool read(const char *setFilePath) {

		std::string text;

		std::ifstream file(setFilePath);
		if (file.is_open() == false) {
			return false;
		}

		for (std::getline(file, text); file.good(); std::getline(file, text))) {
			if (text[0] == '\0') {
				continue;
			}
			m_imageSetContainer->push_back(text);
		}
		file.close();
		return true;

	}

	const std::shared_ptr<ImageSetContainer>& getImageSetContainer() const {
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
	indexPath += IMAGEID_FOLDER;
	CSVDBFile csvDBFile;
	csvDBFile.setPath(indexPath.c_str());
	std::unique_ptr<ImageInfo> ii = csvDBFile.getItemAt(id);
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
	default:
		return false;
	}
	return false;
	//findImage();
}

bool ViewItem::add(std::unique_ptr<ImageInfo>& imageInfo) {

	std::string relpath = imageInfo->getImagePath();
	// source
	std::string fullrelpath = relpath;
	
	fullrelpath += "/data/";
	fullrelpath += imageInfo->getName();
	std::string sourceFile = m_archivePath;
	sourceFile += '/';
	sourceFile += fullrelpath;
	printf("name %s\n", sourceFile.c_str());
	// destination
	fullrelpath = relpath;
	fullrelpath += '/';
	fullrelpath += imageInfo->getName();
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
	const std::shared_ptr<ImageSetContainer>& images = imageSetReader.getImageSetContainer();

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

	FileList_Ptr filelist = SAUtils::getFiles_(m_archivePath.c_str());

	for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string name = *i;
		if ((name)[0] == '.') {
			continue;
		}
		printf("name %s\n", name.c_str());
		std::string path = m_archivePath;
		path += '/';
		path += name.c_str();
		FileList_Ptr filelist = SAUtils::getFiles_(path.c_str());
		for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string name = *i;
			if ((name)[0] == '.') {
				continue;
			}
			printf("name %s\n", name.c_str());
			std::string filepath = path;
			filepath += '/';
			filepath += name.c_str();
			FileList_Ptr filelist = SAUtils::getFiles_(filepath.c_str());
			for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
				std::string name = *i;
				if ((name)[0] == '.') {
					continue;
				}
				printf("name %s\n", name.c_str());
			}
		}
	}
}

bool ViewItem::processSearch(const char *setFile, bool include) {
	return true;
}



bool ViewItem::processAll() {

	std::string indexPath = m_archivePath;
	indexPath += IMAGEID_FOLDER;
	CSVDBFile csvDBFile;
	csvDBFile.setPath(indexPath.c_str());
	int max = csvDBFile.getMaxIndex();
	printf("max: %d\n", max);
	//int size = 0;
	int current = 0;

	for (;current <= max; current++) {
		std::unique_ptr<ImageInfo> ii = csvDBFile.getItemAt(current);
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
	default:
		return false;
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
	ConfigBlock config;
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
	
	std::call_once(m_onceFlag,
		[] {
		m_instance.reset(new ViewManager);
	});

	ViewManager *viewManager = m_instance.get();

	if (viewManager->readConf() == false) {
		logger.log(LOG_OK, CLogger::Level::ERR, "Cannot read view config file: \"%s\"", confpath);
		return false;
	}
	logger.log(LOG_OK, CLogger::Level::FINE, "Reading view config file: \"%s\"", confpath);

	for (std::vector<ViewItem>::iterator i = m_pContainer->begin(); i != m_pContainer->end(); i++) {
		ViewItem &data = *i;
		if (SAUtils::DirExists(data.getPath().c_str()) == false) {
			logger.log(LOG_OK, CLogger::Level::ERR, "View:%s cannot find path \"%s\"", data.getName().c_str(), data.getPath().c_str());

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
			logger.log(LOG_OK, CLogger::Level::ERR, "View:%s cannot find path \"%s\"", data.getName().c_str(), data.getPath().c_str());

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
			logger.log(LOG_OK, CLogger::Level::ERR, "View:%s cannot find path \"%s\"", data.getName().c_str(), data.getPath().c_str());

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
		logger.log(LOG_OK, CLogger::ERROR, "View:%s cannot find path \"%s\"", viewItem.getName().c_str(), viewItem.getPath().c_str());
		if (SAUtils::mkDir(viewItem.getPath().c_str()) == false) {
			logger.log(LOG_OK, CLogger::ERROR, "View:%s cannot create path \"%s\"", viewItem.getName().c_str(), viewItem.getPath().c_str());
			return false;
		}
	}
	if (viewItem.processMaster() == false) {
		return false;
	}
}
*/
bool ViewManager::remove(const char *name) {
	ConfigBlock config;
	ConfigReader configReader;
	configReader.read(m_confpath.c_str(), config);

	ConfigBlockWriter configWriter;
	if (configWriter.remove(name, config) == false) {
		return false;
	}
	//configWriter.write(m_confpath.c_str(), config);
	return true;
}

std::string ViewManager::makeOptionString(const char *locationPath, bool mode, bool type, bool access, EViewSet viewSet, bool metadata, const char *setpath) {
	std::string line;
	std::string valueStr;
	std::string typeStr;
	std::string accessStr;
	std::string viewSetStr;
	std::string metadataStr;



	if (mode) {
		valueStr = "dynamic";
	}
	else {
		valueStr = "snapshot";
	}

	line = valueStr;
	line += ',';
	line += locationPath;
	line += ',';

	if (type) {
		valueStr = "link";
	}
	else {
		valueStr = "copy";
	}

	line += valueStr;
	line += ',';

	if (access) {
		valueStr = "readwrite";
	}
	else {
		valueStr = "readonly";
	}

	line += valueStr;
	line += ',';

	switch (viewSet) {
	case ViewManager::VS_IncludeSet:
		valueStr = "include";
		break;
	case ViewManager::VS_ExcludeSet:
		valueStr = "exclude";
		break;
	case ViewManager::VS_InclusiveSearch:
		valueStr = "inclusive_search";
		break;
	case ViewManager::VS_ExclusiveSearch:
		valueStr = "exclusive_search";
		break;
	case ViewManager::VS_ALL:
		valueStr = "all";
		break;
	case ViewManager::VS_Unknown:
		valueStr = "unknown";
		break;
	}

	line += valueStr;
	line += ',';
	line += setpath;
	line += ',';

	if (metadata) {
		valueStr = "metadata";
	}
	else {
		valueStr = "nometadata";
	}

	line += valueStr;
	return line;
}

bool ViewManager::edit(const char *name, const char *locationPath, bool mode, bool type, bool access, EViewSet viewSet, bool metadata, const char *setpath) {

	std::string line = makeOptionString(locationPath, mode, type, access, viewSet, metadata, setpath);
	//std::cout << line << '\n';
	ConfigBlock config;
	ConfigReader configReader;
	configReader.read(m_confpath.c_str(), config);
	ConfigBlockWriter configWriter;
	configWriter.edit(name, line.c_str(), config);
	configWriter.write(m_confpath.c_str(), config);

	return true;
}

bool ViewManager::create(const char *name, const char *locationPath, bool mode, bool type, bool access, EViewSet viewSet, bool metadata, const char *setpath) {

	std::string line = makeOptionString(locationPath, mode, type, access, viewSet, metadata, setpath);
	//std::cout << line << '\n';
	ConfigBlock config;
	ConfigReader configReader;
	configReader.read(m_confpath.c_str(), config);
	ConfigBlockWriter configWriter;
	configWriter.add(name, line.c_str(), config);
	configWriter.write(m_confpath.c_str(), config);
	return true;
}
} /* namespace simplearchive */
