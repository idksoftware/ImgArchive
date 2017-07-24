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
#include <cstdlib>
#include <sstream>
#include <vector>

#include "CSVArgs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	CSVArgs::CSVArgs(char delim) {
		m_delim = delim;
	}

	CSVArgs::~CSVArgs() {
		clear();
	}

	bool CSVArgs::process(const char *dataString) {

		std::string data = dataString;
		return process(data);
	}

	bool CSVArgs::process(const std::string &data) {

		if (empty() == false) {
			clear();
		}
		//	resize(1);

		std::string field;

		std::istringstream ss(data);
		int i = 0;

		while (nextField(ss, field)) {
			push_back(field);
		}

		/*
		unsigned int commaCounter = 0;

		for (unsigned int i = 0; i < data.size(); i++) {
		char c = data[i];
		if (c == m_delim) {
		push_back("");
		commaCounter++;
		}
		else {
		at(commaCounter) += data[i];
		}

		}
		*/
		return true;
	}

	std::istream& CSVArgs::nextField(std::istringstream& s, std::string& field)
	{

		if (std::getline(s, field, m_delim)) {
			while (s && field[0] == '"' && field[field.size() - 1] != '"') {
				std::string next;
				std::getline(s, next, m_delim);
				field += m_delim + next;
			}

			if (field[0] == '"' && field[field.size() - 1] == '"') {
				field = field.substr(1, field.size() - 2);
			}
		}

		return s;
	}


	void CSVArgs::print() {
		for (auto i = begin(); i != end(); i++) {
			std::string &data = *i;
			std::cout << "Arg: " << data.c_str() << '\n';
		}
	}


} /* namespace simplearchive */
