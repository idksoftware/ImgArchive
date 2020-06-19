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
#include "History.h"
//#include "ImageGroup.h"
#include "Version.h"
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
#include "CheckDiskJournal.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {
using namespace std;

ArchiveRepository::ArchiveRepository() noexcept {}

ArchiveRepository::~ArchiveRepository() {
	// TODO Auto-generated destructor stub
}

/*
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
*/
// filepath is the relative path from the archive root
bool ArchiveRepository::checkout(const char *filepath, const char *comment) {
	// Path to root of Archive
	std::string fullPath(m_pathToArchive);
	fullPath += "/";
	fullPath += filepath;
	std::string filename;
	std::string path;

	// This fines the filename from path NEEDS TO BE MOVED INTO UTILS
	size_t slashpos = fullPath.find_last_of("/");
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
	//ImageHistory& imageHistory = ImageHistory::getImageHistory();
	// Add the comment
//	Version version(fullPath.c_str());
//	int idx = version.getVersion();
//  version.CopyDataVersion2Old();

	int idx = 0;
	
	std::string buff = SAUtils::sprintf("%.4d", idx);
	History &history = History::getHistory();
	/*
	ImageHistory &history = ImageHistory::getImageHistory(filepath);
	HistoryEvent he(HistoryEvent::CHECKOUT);
	history.add(filepath, buff.c_str(), comment, he);
	//imageHistory.add(filepath, buff.c_str(), comment, he);
	*/
	return true;
}

bool ArchiveRepository::checkin(const char *filepath, const char *comment) {
	std::string fullPath = m_pathToArchive + "/" + filepath;
	std::string filename;
	std::string path;
	std::string buff;


	size_t slashpos = fullPath.find_last_of("/");
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
// to put in
//	Version version(fullPath.c_str());
//	if (version.HasChanged() == false) {
//		uncheckout(filepath, comment);
//		return true;
//	}
//	const char *newNamePath = version.newVersion();
	// turn into log message
	//printf("New Version %s", newNamePath);
	// use the above for a log message
//	int idx = version.getVersion();
	int idx = 0;
	buff = SAUtils::sprintf("%.4d", idx);
	History &history = History::getHistory();
//	history.checkinImage(filepath, buff.c_str(), comment);
//ImageHistory imageHistory(hstpath.c_str());
	/*
	ImageHistory &history = ImageHistory::getImageHistory(filepath);
	HistoryEvent he(HistoryEvent::CHECKIN);
	history.add(filepath, buff.c_str(), comment, he);
//	imageHistory.add(filepath, buff, comment, he);
*/
	// use the above for a log message
	return true;
}

bool ArchiveRepository::uncheckout(const char *filepath, const char *comment) {
	std::string fullPath = m_pathToArchive + "/" + filepath;
	std::string filename;
	std::string path;
	std::string buff;


	size_t slashpos = fullPath.find_last_of("/");
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
// To be put in
//	Version version(fullPath.c_str());
//
	// use the above for a log message
//	int idx = version.Revert();
//	if (idx < 0) {
//		// log error
//	}
	int idx = 0;
	buff = SAUtils::sprintf("%.4d", idx);
	//ImageHistory imageHistory(hstpath.c_str());
	const HistoryEvent he(HistoryEvent::Event::UNCHECKOUT);
	//imageHistory.add(filepath, buff, comment, he);
	History &history = History::getHistory();
	//history.checkoutImage(filepath, buff.c_str(), comment);
	/*
	ImageHistory &history = ImageHistory::getImageHistory(filepath);
	//imageHistory.add(filepath, buff, comment, he);
	history.add(filepath, buff.c_str(), comment, he);
	*/


	// use the above for a log message
	return true;

}



ArchiveRepository &ArchiveRepository::get() {
	static ArchiveRepository archiveRepository;
	return archiveRepository;
}




} /* namespace simplearchive */

