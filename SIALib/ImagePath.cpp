/*
 * ImagePath.cpp
 *
 *  Created on: Sep 30, 2014
 *      Author: wzw7yn
 */

#include "ImagePath.h"
#include "SAUtils.h"
#include "CDate.h"
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include "IntegrityManager.h"

namespace simplearchive {

std::string ImagePath::m_mainMetadataPath;
std::string ImagePath::m_mainSequenceNumberPath;
std::string ImagePath::m_mainDupsPath;
std::string ImagePath::m_mainHistory;
std::string ImagePath::m_pathToArchive;

bool ImagePath::settupMainArchiveFolders(const char *pathToArchive) {
	if (SAUtils::DirExists(pathToArchive) == false) {
		return false;
	}
	m_pathToArchive = pathToArchive;
	std::string m_dataFolder = m_pathToArchive;
	m_dataFolder += "/.sia";
	if (SAUtils::DirExists(m_dataFolder.c_str()) == false) {
		SAUtils::mkDir(m_dataFolder.c_str());
	}
	if (m_mainMetadataPath.empty() == true) {
		m_mainMetadataPath = m_pathToArchive + "/.sia/metadata";
		m_mainSequenceNumberPath = m_pathToArchive + "/.sia/imageid";
		m_mainDupsPath = m_pathToArchive + "/.sia/dup";
		m_mainHistory = m_pathToArchive + "/.sia/history";
		if (SAUtils::DirExists(m_mainMetadataPath.c_str()) == false) {
			SAUtils::mkDir(m_mainMetadataPath.c_str());
		}
		if (SAUtils::DirExists(m_mainSequenceNumberPath.c_str()) == false) {
			SAUtils::mkDir(m_mainSequenceNumberPath.c_str());
		}
		if (SAUtils::DirExists(m_mainDupsPath.c_str()) == false) {
			SAUtils::mkDir(m_mainDupsPath.c_str());
		}
		if (SAUtils::DirExists(m_mainHistory.c_str()) == false) {
			SAUtils::mkDir(m_mainHistory.c_str());
		}
	}
	return true;
}

ImagePath::ImagePath(time_t time, const char *pathToArchive) {

	CDate date = CDate(time);
	IntegrityManager &integrityManager = IntegrityManager::get(pathToArchive);


	int year = date.getYear();
	int month = date.getMonth();
	int day = date.getDay();
	//printf("Time: %.4d %.2d %.2d", year, month, day);
	m_pathToArchive = pathToArchive;
	m_yearStr = SAUtils::to_string(year);
	std::stringstream s;
	s << year << '-' << std::setw(2) << std::setfill('0') << month
		<< '-' << std::setw(2) << std::setfill('0') << day;
	m_yyyymmddStr = s.str();
	m_yearStrPath = m_pathToArchive + '/' + m_yearStr;
	if (SAUtils::DirExists(m_yearStrPath.c_str()) == false) {
		SAUtils::mkDir(m_yearStrPath.c_str());

	}
	m_yyyymmddStrPath = m_yearStrPath + '/' + m_yyyymmddStr;
	m_dataPath = m_yyyymmddStrPath + "/.sia/data";
	m_metadataPath = m_yyyymmddStrPath + "/.sia/metadata";
	m_dbPath = m_yyyymmddStrPath + "/.sia/db";
	if (SAUtils::DirExists(m_yyyymmddStrPath.c_str()) == false) {
		SAUtils::mkDir(m_yyyymmddStrPath.c_str());
		std::string siaFolder = m_yyyymmddStrPath;
		siaFolder += "/.sia";
		if (SAUtils::DirExists(siaFolder.c_str()) == false) {
			SAUtils::mkDir(siaFolder.c_str());
			if (SAUtils::setHidden(siaFolder.c_str()) == false) {
				throw std::exception();
			}
			SAUtils::mkDir(m_dataPath.c_str());
			SAUtils::mkDir(m_metadataPath.c_str());
			SAUtils::mkDir(m_dbPath.c_str());
			integrityManager.addDayFolder(m_yyyymmddStr.c_str());
		}
	}
	
	m_relpath = getYearStr() + '/' + getYyyymmddStr();
	m_relMetadataPath = m_relpath + "/.sia/metadata";
	/*
	if (m_mainMetadataPath.empty() == true) {
		m_mainMetadataPath = m_pathToArchive + "/.sia/metadata";
		m_mainSequenceNumberPath = m_pathToArchive + "/.sia/imageid";
		m_mainDupsPath = m_mainMetadataPath + "/dup";
		if (SAUtils::DirExists(m_mainMetadataPath.c_str()) == false) {
			SAUtils::mkDir(m_mainMetadataPath.c_str());
		}
		if (SAUtils::DirExists(m_mainSequenceNumberPath.c_str()) == false) {
			SAUtils::mkDir(m_mainSequenceNumberPath.c_str());
		}
		if (SAUtils::DirExists(m_metadataPath.c_str()) == false) {
			SAUtils::mkDir(m_metadataPath.c_str());
		}
	}
	*/

}



ImagePath::ImagePath(const char *filepath, const char *pathToArchive) {
	IntegrityManager &m_integrityManager = IntegrityManager::get(pathToArchive);
	m_pathToArchive = pathToArchive;
	std::string fullPath = m_pathToArchive + "/" + filepath;
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
}

bool ImagePath::copyFile(std::string  pathToSourceRoot, std::string file) {
	IntegrityManager &integrityManager = IntegrityManager::get();
	std::string from = pathToSourceRoot + "/" + file;
	std::string to = m_yyyymmddStrPath + '/' + file;
	integrityManager.addFile(m_yyyymmddStr.c_str(), file.c_str());
	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}
	// data
	to = m_dataPath + '/' + file;
	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}
	return true;
}

ImagePath::~ImagePath() {
	// TODO Auto-generated destructor stub
}

} /* namespace simplearchive */
