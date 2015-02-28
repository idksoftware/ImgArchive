/*
 * CUnqueName.cpp
 *
 *  Created on: May 20, 2014
 *      Author: wzw7yn
 */
#include <string>
#include <cstdlib>
#include <sstream>
#include "CUnqueName.h"

namespace simplearchive {

CUnqueName::CUnqueName() {
	m_standardType = Underscore;
	m_numberingType = Num;
	m_position = Back;
	m_countPos = 0;		// Count into the filename.
	m_pre = "(";  // pre string
	m_post = ")"; // post string
	m_dupNameCount = 0;
}

CUnqueName::~CUnqueName() {}

const char *CUnqueName::make(const char *name) {
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
	int vepos = nameonly.find_last_of(m_pre);

	if (vepos == -1 || (vepos <= (nameonly.length() - 2))) {
		nameonly = nameonly + m_pre + "1" + m_post;
		m_dupNameCount = 1;
		filename = nameonly + filename.substr(dotpos, filename.length() - dotpos);
		if (path.length() > 0) {
			filename = path + '/' + filename;
		}

	} else {
		int vspos = nameonly.find_last_of(m_pre);
		int itmp = (nameonly.length() - vspos)-1;
		std::string numstr = nameonly.substr(vspos+1, (vepos-vspos)-1);
		m_dupNameCount = atoi(numstr.c_str());
		m_dupNameCount++;
		std::string nonumstr = nameonly.substr(0, vspos);
		std::stringstream ss;
		if (path.length() > 0) {
			ss << path << '/' << nonumstr << m_pre << m_dupNameCount << m_post << ext;
		} else {
			ss << nonumstr << m_pre << m_dupNameCount << m_post << ext;
		}
		filename = ss.str();

	}
	return filename.c_str();

}
} /* namespace simplearchive */
