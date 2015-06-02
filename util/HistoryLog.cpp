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

#include <iostream>
#include "HistoryLog.h"
#include "CSVArgs.h"
#include <sstream>

namespace simplearchive {

HistoryLog::HistoryLog() {
	// TODO Auto-generated constructor stub

}

HistoryLog::~HistoryLog() {
	// TODO Auto-generated destructor stub
}

bool HistoryLog::write() {
	for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
		std::cout << *i << '\n';
	}
	return true;
}

bool HistoryLog::writeXML() {
	std::cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			  << "<History ordering=\"date\" from=\"2015-03-6 12.10.45\" to=\"2015-03-6 12.10.45\">\n";

	for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
		std::cout << *i << '\n';
		CSVArgs csvArgs(':');
		if (csvArgs.process(i->c_str()) == false) {
			return false;
		}
		std::cout << "\t<Event>\n";
		std::cout << writeTag("DateTime", csvArgs.at(0), 2);
		std::cout << writeTag("ImagePage", csvArgs.at(1), 2);
		std::cout << writeTag("Value", csvArgs.at(2), 2);
		std::cout << writeTag("Comment", csvArgs.at(3), 2);
		std::cout << writeTag("Event", csvArgs.at(4), 2);
		std::cout << "\t</Event>\n";
	}
	std::cout <<	"</Catalog>\n";
	return true;
}



} /* namespace simplearchive */
