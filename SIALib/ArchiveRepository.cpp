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

#include "CSVDBFile.h"
#include "ArchiveRepository.h"
#include "ImageContainer.h"
#include "HistoryEvent.h"
#include "History.h"
#include "ImageHistory.h"
#include "ImageGroup.h"
#include "CVersion.h"
#include "CDate.h"
#include "SAUtils.h"
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include <vector>
#include "ImageLock.h"
#include "ImagePath.h"
#include "CSVDatabase.h"
#include "FolderList.h"
#include "HookCmd.h"
#include "IntegrityManager.h"
#include "XMLWriter.h"
#include "CLogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {
using namespace std;

ArchiveRepository::ArchiveRepository() {
	// TODO Auto-generated constructor stub

}

ArchiveRepository::~ArchiveRepository() {
	// TODO Auto-generated destructor stub
}

bool ArchiveRepository::Import(ImageGroup &imageGroup) {
	//ImageGroup imageGroup
	for (std::vector<ImageContainer *>::iterator i = imageGroup.begin(); i != imageGroup.end(); i++) {
		ImageContainer *imageContainer = *i;
		//printf("Name: %s\n", imageContainer->getName().c_str());
		if (Import(imageContainer) == false) {
			// Log error
		}
	}
	return true;
}
// filepath is the relative path from the archive root
bool ArchiveRepository::checkout(const char *filepath, const char *comment) {
	// Path to root of Archive
	std::string fullPath(m_pathToArchive);
	fullPath += "/";
	fullPath += filepath;
	std::string filename;
	std::string path;

	// This fines the filename from path NEEDS TO BE MOVED INTO UTILS
	int slashpos = fullPath.find_last_of("/");
	if (slashpos != -1) {
		filename = fullPath.substr(slashpos+1, fullPath.length() - slashpos);
		path = fullPath.substr(0, slashpos);
	} else {
		//printf("Path to Image in achive invalid \"%s\"", filepath);
	}

	// Full path to metadata directory
	std::string lockpath = path + "/.sia/metadata/" + filename;
	// Full path to lock file
	std::string imageHistoryPath = lockpath + ".hst";

	// Lock the lock file
	if ((ImageLock::lock(lockpath.c_str())) == false) {
		// testing
		//return false;
	}

	// create the image history object for the image
	//
	ImageHistory imageHistory(imageHistoryPath.c_str());
	// Add the comment
	CVersion version(fullPath.c_str());
	int idx = version.getVersion();
	version.CopyDataVersion2Old();
	char buff[128];
	sprintf_s(buff, 128, "%.4d", idx);
	History &history = History::getHistory();
	HistoryEvent he(HistoryEvent::CHECKOUT);
	history.add(filepath, buff, comment, he);
	imageHistory.add(filepath, buff, comment, he);
	return true;
}

bool ArchiveRepository::checkin(const char *filepath, const char *comment) {
	std::string fullPath = m_pathToArchive + "/" + filepath;
	std::string filename;
	std::string path;
	char buff[128];


	int slashpos = fullPath.find_last_of("/");
	if (slashpos != -1) {
		filename = fullPath.substr(slashpos+1, fullPath.length() - slashpos);
		path = fullPath.substr(0, slashpos);
	} else {
		//printf("Path to Image in achive invalid \"%s\"", filepath);
	}
	std::string hstpath = path + "/.sia/metadata/" + filename + ".hst";
	std::string lockpath = path + "/.sia/metadata/" + filename;

	if ((ImageLock::unlock(lockpath.c_str())) == false) {
		//return false;
	}
	CVersion version(fullPath.c_str());
	if (version.HasChanged() == false) {
		uncheckout(filepath, comment);
		return true;
	}
	const char *newNamePath = version.newVersion();
	// turn into log message
	//printf("New Version %s", newNamePath);
	// use the above for a log message
	int idx = version.getVersion();
	sprintf_s(buff, 128, "%.4d", idx);
	ImageHistory imageHistory(hstpath.c_str());

	History &history = History::getHistory();
	HistoryEvent he(HistoryEvent::CHECKIN);
	history.add(filepath, buff, comment, he);
	imageHistory.add(filepath, buff, comment, he);

	// use the above for a log message
	return true;
}

bool ArchiveRepository::uncheckout(const char *filepath, const char *comment) {
	std::string fullPath = m_pathToArchive + "/" + filepath;
	std::string filename;
	std::string path;
	char buff[128];


	int slashpos = fullPath.find_last_of("/");
	if (slashpos != -1) {
		filename = fullPath.substr(slashpos+1, fullPath.length() - slashpos);
		path = fullPath.substr(0, slashpos);
	} else {
		//printf("Path to Image in achive invalid \"%s\"", filepath);
	}
	std::string hstpath = path + "/.sia/metadata/" + filename + ".hst";
	std::string lockpath = path + "/.sia/metadata/" + filename;

	if ((ImageLock::unlock(lockpath.c_str())) == false) {
		//return false;
	}
	CVersion version(fullPath.c_str());

	// use the above for a log message
	int idx = version.Revert();
	if (idx < 0) {
		// log error
	}
	sprintf_s(buff, 128, "%.4d", idx);
	ImageHistory imageHistory(hstpath.c_str());
	const HistoryEvent he(HistoryEvent::UNCHECKOUT);
	//imageHistory.add(filepath, buff, comment, he);
	History &history = History::getHistory();
	imageHistory.add(filepath, buff, comment, he);
	history.add(filepath, buff, comment, he);



	// use the above for a log message
	return true;

}


bool ArchiveRepository::Import(ImageContainer *imageContainer) {

	CSVDatabase &csvDB = CSVDatabase::get();
	time_t time = imageContainer->getTime();
	ImagePath imagePath(time);
	CLogger &logger = CLogger::getLogger();
	
	
	logger.log(CLogger::SUMMARY, "Archiving images: \"%s\"", imageContainer->getName().c_str());
	// Archive image path
	std::string imageRootPath = imageContainer->getPath();
	logger.log(CLogger::INFO, "Image(s) source path: \"%s\"", imageRootPath.c_str());
	imageContainer->setImageRootPath(imagePath.getRelativePath());
	if (imageContainer->hasPictureFile()) {
		logger.log(CLogger::INFO, "Picture image: \"%s\"", imageContainer->getPictureFile());
	
	}

	if (imageContainer->hasRawFile()) {
		logger.log(CLogger::INFO, "Raw image: \"%s\"", imageContainer->getRawFile());
		imagePath.setImageName(imageContainer->getRawFile());
		std::string tmpImagePath = imagePath.getYyyymmddStr() + "/" + imageContainer->getRawFile();
		const MetadataObject *rawMetadata = imageContainer->getRawMetadata();
	
		
		if (imagePath.copyFile(imageContainer->getPath(), imageContainer->getRawFile()) == false) {
			return false;
		}
		// data
		//to = imagePath.getDataPath() + '/' + imageContainer->getRawFile();
		//if (imagePath.copyFile(imageContainer->getPath(), imageContainer->getRawFile()) == false) {
		//	return false;
		//}
		
		logger.log(CLogger::INFO, "Raw image added: \"%s\"", imageContainer->getRawFile());
		std::string filepath = imagePath.getRelativePath() + '/' + imageContainer->getRawFile();
		const HistoryEvent he(HistoryEvent::ADDED);
		processHistory(imagePath, filepath.c_str(), imageContainer->getComment().c_str(), he, 0);
	}

	return true;
}

bool ArchiveRepository::processHistory(ImagePath &imagePath, const char *filepath, const char *comment, const HistoryEvent &he, int ver) {

	
	// This fines the filename from path NEEDS TO BE MOVED INTO UTILS
	//int slashpos = fullPath.find_last_of("/");
	//if (slashpos != -1) {
	//	filename = filepath.substr(slashpos + 1, fullPath.length() - slashpos);
	//	path = fullPath.substr(0, slashpos);
	//}
	//else {
		//printf("Path to Image in achive invalid \"%s\"", filepath);
	//}
	std::string hstpath = imagePath.getShadowHistoryPath();
	/*
	std::string hstpath = path;
	hstpath += "/.sia/history";
	if (SAUtils::DirExists(hstpath.c_str()) == false) {
	if (SAUtils::mkDir(hstpath.c_str()) == false) {
	throw std::exception();
	}
	}
	*/
	// Full path to metadata directory
	hstpath += "/";
	hstpath += imagePath.getImageName();
	hstpath += ".hst";

	// create the image history object for the image
	//
	ImageHistory imageHistory(hstpath.c_str());
	// Add the comment
	char buff[128];
	sprintf_s(buff, 128, "%.4d", ver);

	History &history = History::getHistory();
	imageHistory.add(filepath, buff, comment, he);
	history.add(filepath, buff, comment, he);

	return true;
}


ArchiveRepository &ArchiveRepository::get() {
	if (m_This == 0) {
		m_This = new ArchiveRepository();
	}
	return *m_This;
}

ArchiveRepository *ArchiveRepository::m_This = 0;




} /* namespace simplearchive */

