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

#include <stdio.h>
#include <iomanip>
#include "DBDefines.h"
#include "MetadataObject.h"
#include "MasterMetadataTemplate.h"
//#include "ConfigReader.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {
	
	

bool MasterMetadataTemplateResultsPresentation::writeHuman()
{
	/*
	std::ofstream file;
	if (!m_filename.empty()) {
		file.open(m_filename.c_str());
		if (file.is_open() == false) {
			return false;
		}
		file << "\n---------------------------------------------------\n";
		file << "Image: " << m_title << '\n';
		file << "Path : " << m_description << '\n';
		file << "=====================================================\n";
		file << "Date Time             version     Event      Comment\n\n";
		for (std::list<std::string>::iterator i = begin(); i != end(); i++) {
			//std::cout << *i << '\n';
			CSVArgs csvArgs(',');
			if (csvArgs.process(i->c_str()) == false) {
				return false;
			}

			file << csvArgs.at(0) << "    ";
			file << csvArgs.at(1) << "      ";
			file << csvArgs.at(4) << "  ";
			file << csvArgs.at(3) << "\n\n";
		}
	}
	else {
	*/
	//std::cout << "\n---------------------------------------------------\n";
	//std::cout << "Image: " << m_title << '\n';
	//std::cout << "Path : " << m_description << '\n';
	//std::cout << "=====================================================\n";
	//std::cout << "Date Time             version     Event      Comment\n\n";

	std::cout << "\n";
	std::cout << "Master Metadata Template\n";
	std::cout << "========================\n";
	std::cout << "\n";
	for (auto i = m_row.begin(); i != m_row.end(); i++) {
		SharedMTColumn column = *i;
		std::shared_ptr<MTSchema> mtSchema = column->getMTSchemaItem();
		std::cout << std::setw(20) << mtSchema->getName();
		std::cout << ": ";
		std::cout << column->toString();
		std::cout << "\n";
	}
	std::cout << '\n';
	
	//}
	return true;
}
bool MasterMetadataTemplateResultsPresentation::writeXML()
{
	return false;
}
bool MasterMetadataTemplateResultsPresentation::writeCSV()
{
	return false;
}
bool MasterMetadataTemplateResultsPresentation::writeJson()
{
	return false;
}
bool MasterMetadataTemplateResultsPresentation::writeHtml()
{
	return false;
}
} /* namespace simplearchive */
