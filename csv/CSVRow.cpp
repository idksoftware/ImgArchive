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

#include <fstream>
#include <cstdlib>
#include "CSVRow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

CSVRowItem::CSVRowItem(int id) {
	m_id = id;
	m_row = 0;
}

CSVRowItem::~CSVRowItem() {
	if (m_row != 0) {
		delete m_row;
	}
}



std::string CSVRowItem::token(int *pos, std::string &str) {
	int delim = str.find_first_of(DELIM, *pos);
	std::string tmp = str.substr(*pos, (delim-*pos));
	//printf("%s\n",tmp.c_str());
	*pos = delim;
	return tmp;
}

bool CSVRow::read(const char *datafile) {
	char text[1024 * 2];
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	// make this more robust??
	
	while (file.getline(text, 1024 * 2)) {
		if (strlen(text) > 0) {
			add(text);
		}
		else {
			break;
		}
	}
	file.close();
	
	return true;
}

int CSVRow::readId(std::string &row, int *pos) {
	*pos = row.find_first_of(DELIM);
	std::string sequenceId = row.substr(0,*pos);
	return atol(sequenceId.c_str());
}



bool CSVRow::write(const char *path, const char *filename, CSVString &strcov) {
	std::string fullpath(path);
	fullpath += '/';
	fullpath += filename;

	std::ofstream file(fullpath.c_str());
	if (file.is_open() == false) {
		return false;
	}
	
	for (std::vector<CSVRowItem *>::iterator i = this->begin(); i != this->end(); i++) {
		CSVRowItem *csvRow = *i;
		const char *t = strcov.toString(*csvRow);
		
		file << t << '\n';

	}
	
	file.close();
	return true;
}

} /* namespace simplearchive */
