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
#include "LogDocument.h"
#include "CSVArgs.h"
#include <sstream>
#include "MetaType.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	LogDocument::LogDocument(std::shared_ptr<MTTableSchema> tableSchema) : m_tableSchema(tableSchema) {
	// TODO Auto-generated constructor stub

}

LogDocument::~LogDocument() {
	// TODO Auto-generated destructor stub
}

bool LogDocument::write(FormatType formatType) {
	switch (formatType) {
	case FormatType::XML:
		return writeXML();
	case FormatType::Json:
		return writeJson();
	case FormatType::Human:
		return writeHuman();
	case FormatType::csv:
		return writeCSV();
	case FormatType::Html:
		return writeHtml();
		
	case FormatType::unknown:
	default:
		break;
	}
	return false;
}

bool iequals(const std::string& a, const std::string& b)
{
	unsigned int sz = a.size();
	if (b.size() != sz)
		return false;
	for (unsigned int i = 0; i < sz; ++i)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}

LogDocument::FormatType LogDocument::parse(const char *str) {
	std::string typeStr = str;
	
	if (iequals(typeStr,"XML")) {
		return FormatType::XML;
	}
	else if (iequals(typeStr, "Json")) {
		return FormatType::Json;
	}
	else if (iequals(typeStr, "csv")) {
		return FormatType::csv;
	}
	else if (iequals(typeStr, "human")) {
		return FormatType::Human;
	}
	else if (iequals(typeStr, "html")) {
		return FormatType::Html;
	}
	return FormatType::unknown;
}

std::string LogDocument::writeTag(const char *tag, const std::string& value, int tab) {
	std::ostringstream xml;
	for (int i = 0; i < tab; i++) {
		xml << '\t';
	}
	if (!value.empty() && (value.compare("null") != 0)) {
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	} else {
		xml << "<" << tag << "/>\n";
	}
	return xml.str();
}

std::string LogDocument::writeTag(const char *tag, const int value, int tab) {
	std::ostringstream xml;
	for (int i = 0; i < tab; i++) {
		xml << '\t';
	}
	xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	return xml.str();
}

} /* namespace simplearchive */
