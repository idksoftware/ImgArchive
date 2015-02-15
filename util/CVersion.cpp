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

CVersion::CVersion() {
	m_version = 0;
}

CVersion::~CVersion() {
	// TODO Auto-generated destructor stub
}

const char *CVersion::newVersion(const char *name) {
	// filename.txt -> filename[1].txt
	// filename[1].text -> filename[2].txt
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
	std::string ext = filename.substr(dotpos, filename.length() - dotpos);
	int vepos = nameonly.find_last_of(']');
	int tmp = nameonly.length();

	if (vepos == -1 || (vepos <= (nameonly.length() - 2))) {
		nameonly = nameonly + "[1]";
		m_version = 1;
		filename = nameonly + filename.substr(dotpos, filename.length() - dotpos);
		if (path.length() > 0) {
			filename = path + '/' + filename;
		}

	}
	else {
		int vspos = nameonly.find_last_of('[');
		int itmp = (nameonly.length() - vspos)-1;
		std::string numstr = nameonly.substr(vspos+1, (vepos-vspos)-1);
		m_version = atoi(numstr.c_str());
		m_version++;
		std::string nonumstr = nameonly.substr(0, vspos);
		std::stringstream ss;
		if (path.length() > 0) {
			ss << path << '/' << nonumstr << '[' << m_version << ']' << ext;
		} else {
			ss << nonumstr << '[' << m_version << ']' << ext;
		}
		filename = ss.str();

	}

	return filename.c_str();

}

std::string CVersion::nameOnly(const char *name) {
	std::string namestr = name;
	int dotpos = namestr.find_last_of(".");
	m_nameonly = namestr.substr(0, dotpos);
	int vspos = m_nameonly.find_last_of('[');
	m_nameonly = m_nameonly.substr(0, vspos);
	return m_nameonly;
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
	int vepos = nameonly.find_last_of(']');
	if (vepos == -1 || (vepos <= (nameonly.length() - 2))) {
		m_version = 0;
	} else {
		int vspos = nameonly.find_last_of('[');
		std::string numstr = nameonly.substr(vspos+1, (vepos-vspos)-1);
		m_version = atoi(numstr.c_str());
	}

	return m_version;

}

int CVersion::fileLastVersion(const char *name) {
	std::string namestr(name);

	std::string filename;
	std::string path;
	int idx = 0;

	int slashpos = namestr.find_last_of("/");
	if (slashpos != -1) {
		filename = namestr.substr(slashpos+1, namestr.length() - slashpos);
		path = namestr.substr(0, slashpos);
	} else {
		//printf("Error invalid file path \"%s\"", name);
		// raise error
	}
	std::string nameonly = nameOnly(filename.c_str());
	std::string datapath = path + "/.sia/data";
	std::vector<std::string *> *filelist = SAUtils::getFiles(datapath.c_str());
	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *name = *i;
		//printf("File \"%s\"\n", name->c_str());
		const char *tmp = name->c_str();
		std::string cname(nameOnly(tmp));
		const char *s = cname.c_str();

		if (nameonly.compare(cname.c_str()) == 0) {
			int ix = versionIndex(tmp);
			if (idx <= ix) {
				idx = ix;
				m_currentName = *name;
			}
		}

		//int slashpos = name->find_last_of("/");
		//filename = name->substr(slashpos+1, name->length() - slashpos);
		//path = name->substr(0, slashpos);
		//printf("File \"%s\"\n", filename);

	}
	return idx;
}

} /* namespace simplearchive */
