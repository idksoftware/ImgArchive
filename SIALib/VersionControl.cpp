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

#include <string.h>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <stdlib.h>

#include "CSVDBFile.h"

#include "ImageContainer.h"
#include "HistoryEvent.h"
#include "History.h"
#include "ImageHistory.h"
#include "CLogger.h"
#include "CVersion.h"
#include "CDate.h"
#include "SAUtils.h"

#include "ImageLock.h"
#include "ImagePath.h"
#include "CSVDatabase.h"
#include "FolderList.h"
#include "IntegrityManager.h"
#include "MetadataObject.h"
#include "HookCmd.h"
#include "MetadataObject.h"
#include "SummaryFile.h"
#include "VersionControl.h"

namespace simplearchive {

std::string VersionControl::m_pathToArchive;
std::string VersionControl::m_pathToShadow;

std::auto_ptr<VersionControl> VersionControl::m_this(0);
VersionControl::VersionControl() {
	// TODO Auto-generated constructor stub

}

VersionControl::~VersionControl() {
	// TODO Auto-generated destructor stub
}

VersionControl &VersionControl::get() {

	if (!m_this.get()) {
		m_this.reset(new VersionControl());
	}
	return *(m_this.get());
}

// filepath is the relative path from the archive root
bool VersionControl::checkout(const char *filepath, const char *comment) {

	CLogger &logger = CLogger::getLogger();
	ImagePath imagePath(filepath);
	std::string path = imagePath.getDataPath();
	std::string fullpath = path + '/' + imagePath.getImageName();
	std::string hstpath = imagePath.getLocalShadowHistoryPath() + '/' + imagePath.getImageName() + ".hst";
	std::string lockpath = imagePath.getLocalShadowHistoryPath() + '/' + imagePath.getImageName();

	// Lock the lock file
	if ((ImageLock::lock(lockpath.c_str())) == false) {
		logger.log(CLogger::INFO, "Cannot lock Image: \"%s\"", imagePath.getImageName().c_str());
		return false;
	}

	
	// Add the comment
	CVersion version(fullpath.c_str());
	int idx = version.getVersion();
	//version.CopyDataVersion2Old();
	setHistory(hstpath.c_str(), filepath, comment, HistoryEvent::CHECKOUT, idx);
	return true;
}

bool VersionControl::CopyNewVersion2Data(const std::string &newImagePath,const std::string &dataPath,const std::string &imageName) {
	std::string from = newImagePath + '/' + imageName;
	std::string to = dataPath + "/" + imageName;
	// data

	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}
	return true;
}

/*
bool VersionControl::CopyNewVersion2Data(const std::string &newImagePath, const std::string &dataPath, const std::string &imageName) {
	std::string from = newImagePath + '/' + imageName;
	std::string to = dataPath + "/" + imageName;
	// data

	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}
	return true;
}
*/

bool VersionControl::checkin(const char *filepath, const char *comment) {
	ImagePath imagePath(filepath);
	
	std::string path = imagePath.getDataPath();
	std::string fullpath = path + '/' + imagePath.getImageName();
	//getPathAndFilename(fullPath.c_str(), filename, path);
	
	std::string hstpath = imagePath.getLocalShadowHistoryPath() + '/' + imagePath.getImageName() + ".hst";
	std::string lockpath = imagePath.getLocalShadowHistoryPath() + '/' + imagePath.getImageName();

	if ((ImageLock::unlock(lockpath.c_str())) == false) {
		//return false;
	}
	CVersion version(fullpath.c_str());
	if (version.HasChanged() == false) {
		// check the image has changed
		uncheckout(filepath, comment);
		printf("The image has not changed\"%s\"", filepath);
		//return true;
	}
	std::string archivePath = imagePath.getImagePath() + '/' + imagePath.getImageName();
	const char *newNamePath = version.newVersion();
	CopyNewVersion2Data(imagePath.getYyyymmddStrPath(), imagePath.getDataPath(), imagePath.getImageName());
	std::string metadataPath = imagePath.getLocalShadowMetadataPath() + '/' + imagePath.getImageName() + ".xml";
	//version.newVersionMetadata(metadataPath.c_str());
	// turn into log message
	//printf("New Version %s", newNamePath);
	// use the above for a log message
	int idx = version.getVersion();
	setHistory(hstpath.c_str(), filepath, comment, HistoryEvent::CHECKIN, idx);


	// use the above for a log message
	return true;
}

bool VersionControl::uncheckout(const char *filepath, const char *comment) {
	ImagePath imagePath(filepath);

	std::string path = imagePath.getDataPath();
	std::string fullpath = path + '/' + imagePath.getImageName();
	//getPathAndFilename(fullPath.c_str(), filename, path);

	std::string hstpath = imagePath.getLocalShadowHistoryPath() + '/' + imagePath.getImageName() + ".hst";
	std::string lockpath = imagePath.getLocalShadowHistoryPath() + '/' + imagePath.getImageName();

	if ((ImageLock::unlock(lockpath.c_str())) == false) {
		//return false;
	}
	CVersion version(fullpath.c_str());

	// use the above for a log message
	int idx = version.Revert();
	if (idx < 0) {
		// log error
	}
	setHistory(hstpath.c_str(), filepath, comment, HistoryEvent::UNCHECKOUT, idx);

	// use the above for a log message
	return true;

}

int VersionControl::getNumberOfVersions(const char *imagePath) {
	std::string fullPath = m_pathToArchive + "/" + imagePath;
	CVersion version(fullPath.c_str());
	int revision = version.getVersion();
	revision++;
	return revision;
}

bool VersionControl::getImageVersion(const char *imagePath,const char *targetPath, int idx) {
	std::string fullPath = m_pathToArchive + "/" + imagePath;
	if (SAUtils::FileExists(fullPath.c_str()) == false) {
		std::string message = "Path to checkout image is invalid \"";
		message += imagePath;
		message += "\"";
		throw SIAAppException(message);
	}
	if (SAUtils::DirExists(targetPath) == false) {
		std::string message = "Path to checkout image is invalid \"";
		message += imagePath;
		message += "\"";
		throw SIAAppException(message);
	}
	CVersion version(fullPath.c_str());
	version.setVersion(idx);
	version.setToVersion(idx);
	return true;
}

bool VersionControl::uncheckout(const char *imagePath,const char *targetPath, int idx, const char *comment) {
	
	// use the above for a log message
	return true;

}


bool VersionControl::setHistory(const char *hstpath, const char *filepath, const char *comment, HistoryEvent evt, int idx) {
	ImageHistory imageHistory(hstpath);
	std::string idxStr = versonString(idx);
	imageHistory.add(filepath, idxStr.c_str(), comment, HistoryEvent::UNCHECKOUT);
	History &history = History::getHistory();
	HistoryEvent he(HistoryEvent::CHECKIN);

	history.add(filepath, idxStr.c_str(), comment, he);
	return true;
}

std::string VersionControl::versonString(int idx) {
	std::stringstream s;
	s << std::setw(4) << std::setfill('0') << std::hex << idx;
	return s.str();
}

bool VersionControl::getPathAndFilename(const char *filepath, std::string &filename, std::string &path) {
	std::string fullPath = filepath;

	if (SAUtils::FileExists(fullPath.c_str()) == false) {
		std::string message = "Path to checkout image is invalid \"";
		message += filepath;
		message += "\"";
		throw SIAAppException(message);
	}

	// This fines the filename from path NEEDS TO BE MOVED INTO UTILS
	int slashpos = fullPath.find_last_of("/");
	if (slashpos != -1) {
		filename = fullPath.substr(slashpos+1, fullPath.length() - slashpos);
		path = fullPath.substr(0, slashpos);
	} else {
		std::string message = "Path to checkout image is invalid \"";
		message += filepath;
		message += "\"";
		throw SIAAppException(message);
	}
	return true;
}

std::auto_ptr<ImageHistoryLog>  VersionControl::getHistory(const char *filepath) {
	std::string fullPath = m_pathToArchive + "/" + filepath;
	std::string filename;
	std::string path;

	getPathAndFilename(fullPath.c_str(), filename, path);
	std::string hstpath = path + "/.metadata/" + filename + ".hst";

	ImageHistory imageHistory(hstpath.c_str());
	return imageHistory.get();
}

} /* namespace simplearchive */
