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

#include "StagingManager.h"
#include <time.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include"SAUtils.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

static int m_count = 0;
StagingManager::StagingManager(const char *root, const char *name, const char *stagingFolder) {
	m_root = root;
	m_name = name;
	m_stagingFolder = stagingFolder;
	m_time = 0;
}

StagingManager::~StagingManager() {
	// TODO Auto-generated destructor stub
}

bool StagingManager::put(const char *path) {
	std::string fullpath = m_root;
	fullpath += '/';
	fullpath += path;
	//printf("%s\n",fullpath.c_str());
	std::string stage = m_stagingFolder;
	stage += '/';
	std::string number = unqueNumger();
	stage += number;
	stage += ".dat";
	if (txtFile(number, path) == false) {
		return false;
	}
	if (SAUtils::copy(fullpath.c_str(), stage.c_str()) == false) {
		return false;
	}
	if (SAUtils::verify(fullpath.c_str(), stage.c_str()) == false) {
		return false;
	}
	return true;
}

bool StagingManager::putMirror() {

	FileList_Ptr filelist = SAUtils::getFiles_(m_stagingFolder.c_str());
	if (filelist == 0) {
		return false;
	}
	for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string tmpPtr = *i;
		std::string filename = tmpPtr;
		std::string ext = SAUtils::getExtention(filename);
		std::string name = SAUtils::getFilenameNoExt(filename);
		if (ext.compare("txt") == 0) {
			std::string fullTxtPath = m_stagingFolder;
			fullTxtPath += '/';
			fullTxtPath += filename;
			std::string mirrorPath = StagingManager::readTxtFile(fullTxtPath.c_str());
			if (mirrorPath.empty() == true) {
				return false;
			}
			std::string fullMirrorPath = m_remoteMirrorPath;
			fullMirrorPath += '/';
			fullMirrorPath += mirrorPath;

			std::string fullDataPath = m_stagingFolder;
			fullDataPath += '/';
			fullDataPath += name;
			fullDataPath += ".dat";
			if (SAUtils::FileExists(fullDataPath.c_str()) == false) {
				return false;
			}
			
			if (SAUtils::makePath(m_remoteMirrorPath.c_str(), mirrorPath.c_str()) == false) {
				return false;
			}

			if (SAUtils::copy(fullDataPath.c_str(), fullMirrorPath.c_str()) == false) {
				printf("Unable to copy file: \"%s\" to \"%s\"", fullDataPath.c_str(), fullMirrorPath.c_str());
				return false;
			}
		}

	}
	return true;
}

std::string StagingManager::unqueNumger() {
	time_t timeCur = 0;
	time(&timeCur);
	if (m_time == timeCur) {
		m_count++;
	}
	else {
		m_time = timeCur;
		m_count = 0;
	}
	std::ostringstream sstream;
	sstream <<  m_time << std::setw(4) << std::hex << std::setfill('0')   << m_count;
	std::string tmp = sstream.str();
	return tmp;
}

bool StagingManager::txtFile(std::string &number, const char *path) {

	std::string filename = m_stagingFolder;
	filename += '/';
	filename += number;
	filename += ".txt";

	std::ofstream txtfile(filename.c_str());
	txtfile << number << ':' << path;
	txtfile.close();
	if (verifyTxtFile(number,path) == false) {
		return false;
	}
	return true;

}

bool StagingManager::verifyTxtFile(std::string &number, const char *path) {
	char text[1024];
	std::string filename = m_stagingFolder;
	filename += '/';
	filename += number;
	filename += ".txt";

	std::ifstream txtfile(filename.c_str());
	if (txtfile.is_open() == false) {
		return false;
	}
	std::string number1;
	std::string path1;
	while (txtfile.getline(text, 1024)) {
		std::string data = text;
		int delim1 = data.find_first_of(":");
		number1 = data.substr(0,delim1);
		path1 = data.substr(delim1+1, data.length());
	}
	txtfile.close();
	if (number1.compare(number) != 0) {
		return false;
	}
	if (path1.compare(path) != 0) {
		return false;
	}
	return true;
}

std::string StagingManager::readTxtFile(const char *txtFilePath) {
	char text[1024];
	std::ifstream txtfile(txtFilePath);
	if (txtfile.is_open() == false) {
		throw std::exception();
	}
	std::string number;
	std::string path;
	while (txtfile.getline(text, 1024)) {
		std::string data = text;
		int delim1 = data.find_first_of(":");
		number = data.substr(0,delim1);
		path = data.substr(delim1+1, data.length());
	}
	txtfile.close();
	return path;
}

const char *StagingManager::getRemoteMirrorPath() const {
	return m_remoteMirrorPath.c_str();
}

void StagingManager::setRemoteMirrorPath(const char *path) {
	m_remoteMirrorPath = path;
}


} /* namespace simplearchive */

