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

#include <string>
#include <fstream>
#include <iostream>

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

void CSVArgs::print() {
	for (std::vector<std::string>::iterator i = begin(); i != end(); i++) {
		std::string &data = *i;
		std::cout << "Arg: " << data.c_str() << '\n';
	}
}

} /* namespace simplearchive */
