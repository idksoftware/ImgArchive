/*
 * VersionControl.cpp
 *
 *  Created on: Feb 24, 2015
 *      Author: wzw7yn
 */

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
	std::string fullPath = m_pathToArchive + "/" + filepath;
	std::string filename;
	std::string path;

	getPathAndFilename(fullPath.c_str(), filename, path);
	// Full path to metadata directory
	std::string lockpath = path + "/.metadata/" + filename;
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
	setHistory(imageHistoryPath.c_str(), filepath, comment, HistoryEvent::CHECKOUT, idx);
	return true;
}

bool VersionControl::checkin(const char *filepath, const char *comment) {
	std::string fullPath = m_pathToArchive + "/" + filepath;
	std::string filename;
	std::string path;

	getPathAndFilename(fullPath.c_str(), filename, path);

	std::string hstpath = path + "/.metadata/" + filename + ".hst";
	std::string lockpath = path + "/.metadata/" + filename;

	if ((ImageLock::unlock(lockpath.c_str())) == false) {
		//return false;
	}
	CVersion version(fullPath.c_str());
	if (version.HasChanged() == false) {
		// check the image has changed
		uncheckout(filepath, comment);
		printf("The image has not changed\"%s\"", filepath);
		//return true;
	}
	int idx = version.getVersion();
	setHistory(hstpath.c_str(), filepath, comment, HistoryEvent::CHECKIN, idx);


	// use the above for a log message
	return true;
}

bool VersionControl::uncheckout(const char *filepath, const char *comment) {
	std::string fullPath = m_pathToArchive + "/" + filepath;
	std::string filename;
	std::string path;

	getPathAndFilename(filepath, filename, path);

	std::string hstpath = path + "/.metadata/" + filename + ".hst";
	std::string lockpath = path + "/.metadata/" + filename;

	if ((ImageLock::unlock(lockpath.c_str())) == false) {
		//return false;
	}
	CVersion version(fullPath.c_str());

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
}

bool VersionControl::uncheckout(const char *imagePath,const char *targetPath, int idx, const char *comment) {

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

void VersionControl::getPathAndFilename(const char *filepath, std::string &filename, std::string &path) {
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
}

} /* namespace simplearchive */
