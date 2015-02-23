/*
 * CSVArgs.cpp
 *
 *  Created on: Feb 17, 2015
 *      Author: wzw7yn
 */
#include <string>
#include "CSVArgs.h"

namespace simplearchive {

CSVArgs::CSVArgs(char delim) {
	m_delim = delim;
}

CSVArgs::~CSVArgs() {
	// TODO Auto-generated destructor stub
}

bool CSVArgs::process(const char *dataString) {

	std::string data = dataString;
	if (empty() == false) {
		clear();
	}
	resize(1);
	unsigned int commaCounter = 0;
	for (unsigned int i = 0; i < data.size(); i++) {
		char c = data[i];
		if (c == m_delim) {
			push_back("");
			commaCounter++;
		} else {
			at(commaCounter) += data[i];
		}

	}
	return true;
}

} /* namespace simplearchive */
