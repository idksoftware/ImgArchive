/*
 * CVersion.cpp
 *
 *  Created on: May 20, 2014
 *      Author: wzw7yn
 */

#include "CVersion.h"
#include "SAUtils.h"
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <sstream>

namespace simplearchive {

CVersion::CVersion(const char *name) {
	m_version = fileLastVersion(name);
}

CVersion::~CVersion() {
	// TODO Auto-generated destructor stub
}

const char *CVersion::newVersion() {


	int dotpos = m_imagefilename.find_last_of(".");

	std::string ext = m_imagefilename.substr(dotpos, m_imagefilename.length() - dotpos);

	if (SAUtils::DirExists(m_versionFolder.c_str()) == false) {

		if (SAUtils::mkDir(m_versionFolder.c_str()) == false ) {
			return 0;
		}
		m_version = 1;
		m_versionName = m_nameonly + "[1]" + ext;
		m_versionPath = m_versionFolder + "/" + m_versionName;

	}
	else {
		m_version++;
		std::stringstream ss;
		ss << m_nameonly << '[' << m_version << ']' << ext;
		m_versionName = ss.str();
		m_versionPath = m_versionFolder + "/" + m_versionName;

	}
	if (CopyDataVersion2Old() == false) {
		return 0;
	}
	if (CopyNewVersion2Data() == false) {
		return 0;
	}
	if (RemoveTemp() == false) {
		return 0;
	}
	return m_versionPath.c_str();

}

int CVersion::Revert() {
	if (CopyDataVersion2Current() == false) {
		return -1;
	}
	if (RemoveTemp() == false) {
		return -1;
	}
	return m_version;
}

bool CVersion::HasChanged() {
	std::string datapath = m_imagePath + "/.sia/data";
	std::string imageFilePath = m_imagePath + "/" + m_imagefilename;
	std::string imageDataPath = datapath + "/" + m_imagefilename;

	if (SAUtils::fileCompare(imageFilePath.c_str(), imageDataPath.c_str()) == true) {
		return true;
	}
	return false;
}

bool CVersion::RemoveTemp() {
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

bool CVersion::CopyNewVersion2Data() {
		std::string datapath = m_imagePath + "/.sia/data";
		std::string from = m_imagePath + "/" + m_imagefilename;
		std::string to = datapath + "/" + m_imagefilename;
		// data

		if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
			return false;
		}
		return true;
}

bool CVersion::CopyDataVersion2Old() {
	std::string datapath = m_imagePath + "/.data";
	std::string from = datapath + "/" + m_imagefilename;
	std::string to = m_versionPath;
	// data

	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}
	return true;
}

bool CVersion::CopyDataVersion2Temp() {
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

bool CVersion::CopyDataVersion2Current() {
	std::string datapath = m_imagePath + "/.sia/data";
	std::string from = datapath + "/" + m_imagefilename;
	std::string to = m_imagePath + "/" + m_imagefilename;
	// data

	if (SAUtils::copy(from.c_str(), to.c_str()) == false) {
		return false;
	}
	return true;
}

std::string CVersion::nameOnly(const char *name) {
	std::string namestr = name;

	int dotpos = namestr.find_last_of(".");
	std::string nameonly = namestr.substr(0, dotpos);
	int vspos = nameonly.find_last_of('[');
	nameonly = nameonly.substr(0, vspos);
	return nameonly;
}

int CVersion::versionIndex(const char *name) {
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

bool CVersion::splitpath(const char *name) {
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

int CVersion::fileLastVersion(const char *name) {


	if (splitpath(name) == false) {
		return false;
	}

	m_nameonly = nameOnly(m_imagefilename.c_str());
	std::string datapath = m_imagePath + "/.sia/data";
	m_versionFolder = datapath;
	m_versionFolder += '/';
	m_versionFolder += m_imagefilename;
	m_versionFolder += VERSION_EXT;
	if (SAUtils::DirExists(m_versionFolder.c_str()) == true) {
		std::vector<std::string *> *filelist = SAUtils::getFiles(m_versionFolder.c_str());
		for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
			std::string *filenameItem = *i;
			//printf("File \"%s\"\n", filenameItem->c_str());
			const char *tmp = filenameItem->c_str();
			std::string cname(nameOnly(tmp));
			const char *s = cname.c_str();

			if (m_nameonly.compare(cname.c_str()) == 0) {
				int ix = versionIndex(tmp);
				if (m_version <= ix) {
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
			return -1;
		}
	}

	//printf("Index: %d\n", m_version);
	return m_version;
}

} /* namespace simplearchive */
