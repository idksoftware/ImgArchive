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

#include "Version.h"
#include "SAUtils.h"
#include "pathcontroller.h"
#include "ImageEncode.h"
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <sstream>
#ifndef _WIN32
#include <unistd.h>
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {


const char *Version::makeVersion() {

	int dotpos = m_imagefilename.find_last_of(".");
	m_ext = m_imagefilename.substr(dotpos, m_imagefilename.length() - dotpos);
	
	std::stringstream ss;
	ss << m_nameonly << '[' << m_version << ']' << m_ext;
	m_versionName = ss.str();

	return m_versionName.c_str();
}

const char *Version::makeEncodedVersion() {

	int dotpos = m_imagefilename.find_last_of(".");
	m_ext = m_imagefilename.substr(dotpos+1, m_imagefilename.length() - (dotpos + 1));

	std::stringstream ss;
	ss << m_nameonly << '[' << m_version << ']' << '.' << m_ext;
	m_versionName = ss.str();

	ImageEncode imageEncode(m_sourcePath.c_str(), m_nameonly.c_str(), m_version, m_ext.c_str());
	m_encodedVersion = imageEncode.getEncodedString();

	return m_encodedVersion.c_str();
}

bool Version::checkin(const char *database, const char *workspace) {
	PathController workspacePath(workspace);
	if (workspacePath.isValid() == false) {
		return false;
	}
	PathController databasePath(database);
	if (databasePath.isValid() == false) {
		return false;
	}
	//std::string image = makeVersion();
	ImageEncode imageEncode(m_sourcePath.c_str(), m_nameonly.c_str(), m_version, m_ext.c_str());
	std::string encoded = imageEncode.getEncodedString();
	databasePath.setImage(encoded);
	if (SAUtils::copy(workspacePath.getFullPath().c_str(), databasePath.getFullPath().c_str()) == false) {
		return false;
	}
	return true;
}


int Version::Revert() {
	if (CopyDataVersion2Current() == false) {
		return -1;
	}
	if (RemoveTemp() == false) {
		return -1;
	}
	return m_version;
}

bool Version::HasChanged() {
	std::string datapath = m_imagePath + "/.sia/data";
	std::string imageFilePath = m_imagePath + "/" + m_imagefilename;
	std::string imageDataPath = datapath + "/" + m_imagefilename;

	if (SAUtils::fileCompare(imageFilePath.c_str(), imageDataPath.c_str()) == true) {
		return true;
	}
	return false;
}

bool Version::RemoveTemp() {
	std::string datapath = m_imagePath + "/.sia/data/";
	std::string tmpVersion = datapath + m_imagefilename + ".tmp";
	if (SAUtils::FileExists(tmpVersion.c_str()) == false) {
		return true;
	}
	if (::unlink(tmpVersion.c_str()) < 0) {
		return false;
	}
	return true;
}


/*
bool Version::CopyDataVersion2Old() {
	if (m_version == 0) {
		std::string datapath = m_imagePath;

		std::string from = datapath + "/" + m_imagefilename;
		std::string to = m_versionPath;

		// data
		if (SAUtils::FileExists(to.c_str()) == true) {
			if (SAUtils::fileCompare(from.c_str(), to.c_str()) == true) {
				return true;
			}
			return false;
		}
		if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
			return false;
		}
	}
	return true;
}
*/

bool Version::CopyDataVersion2Temp() {
	std::string datapath = m_imagePath + "/.sia/data";
	std::string from = datapath + "/" + m_imagefilename;
	std::string to = from + ".tmp";
	// data

	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}

/*
	from = path + "/" + filename;
	to = datapath + "/" + filename;
	// data

	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}
*/
	return true;
}

bool Version::CopyDataVersion2Current() {
	std::string datapath = m_imagePath + "/.sia/data";
	std::string from = datapath + "/" + m_imagefilename;
	std::string to = m_imagePath + "/" + m_imagefilename;
	// data

	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}
	return true;
}

std::string Version::nameOnly(const char *name) {
	std::string namestr = name;

	int dotpos = namestr.find_last_of(".");
	std::string nameonly = namestr.substr(0, dotpos);
	int vspos = nameonly.find_last_of('[');
	nameonly = nameonly.substr(0, vspos);
	return nameonly;
}

int Version::versionIndex(const char *name) {
	std::string namestr(name);
	std::string filename;
	std::string path;


	int slashpos = namestr.find_last_of("/");
	if (slashpos != -1) {
		filename = namestr.substr(slashpos+1, namestr.length() - slashpos);
		path = namestr.substr(0, slashpos);
	} else {
		filename = namestr;
	}
	int dotpos = filename.find_last_of(".");
	std::string nameonly = filename.substr(0, dotpos);
	unsigned int vepos = nameonly.find_last_of(']');
	if (vepos == -1 || (vepos <= (nameonly.length() - 2))) {
		m_version = 0;
	} else {
		int vspos = nameonly.find_last_of('[');
		std::string numstr = nameonly.substr(vspos+1, (vepos-vspos)-1);
		m_version = atoi(numstr.c_str());
	}

	return m_version;

}

bool Version::splitpath(const char *name) {
	std::string namestr(name);

	int slashpos = namestr.find_last_of("/");
	if (slashpos != -1) {
		m_imagefilename = namestr.substr(slashpos+1, namestr.length() - slashpos);
		m_imagePath = namestr.substr(0, slashpos);
	} else {
		//printf("Error invalid file path \"%s\"", name);
		return false;
		// raise error
	}
	return true;
}





bool Version::setToVersion(int idx) {
	return setToVersion(m_imagePath.c_str(), idx);
}

bool Version::setToVersion(const char *name, int idx) {


	if (splitpath(name) == false) {
		return false;
	}

	m_nameonly = nameOnly(m_imagefilename.c_str());
	std::string datapath = m_imagePath + "/.data";
	m_versionFolder = datapath;
	m_versionFolder += '/';
	m_versionFolder += m_imagefilename;
	m_versionFolder += VERSION_EXT;
	if (SAUtils::DirExists(m_versionFolder.c_str()) == true) {
		FileList_Ptr filelist = SAUtils::getFiles_(m_versionFolder.c_str());
		for (std::vector<std::string>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string filenameItem = *i;

			const char *tmp = filenameItem.c_str();
			std::string cname(nameOnly(tmp));
			const char *s = cname.c_str();

			if (m_nameonly.compare(cname.c_str()) == 0) {
				int ix = versionIndex(tmp);
				if (idx == ix) {
					printf("File: \"%s\"\n", filenameItem.c_str());
					m_version = ix;
					m_versionName = tmp;
				}
			}

		//int slashpos = name->find_last_of("/");
		//filename = name->substr(slashpos+1, name->length() - slashpos);
		//path = name->substr(0, slashpos);
		//printf("File \"%s\"\n", filename);

		}
		m_versionPath = m_versionFolder + "/" + m_versionName;
	} else {
		m_version = 0;
		m_versionPath = name;
		std::string namestr(m_versionPath);
		int slashpos = namestr.find_last_of("/");
		if (slashpos != -1) {
			m_versionName = namestr.substr(slashpos+1, namestr.length() - slashpos);
			//m_imagePath = namestr.substr(0, slashpos);
		} else {
			return false;
		}
	}

	printf("Index: %d\n", m_version);
	return true;
}

} /* namespace simplearchive */
