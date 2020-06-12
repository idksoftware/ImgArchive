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
#include "MetadataTemplate.h"
//#include "ConfigReader.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {
	
	MetadataTemplateSchema MetadataTemplateRow::m_tableSchema;

	std::unique_ptr<ConfigBlock>	MetadataTemplate::m_templateFile(new ConfigBlock);
	MetadataTemplateRow MetadataTemplate::m_metadataTemplateRow;

//Config *MetadataTemplate::m_templateFile = 0;


MetadataTemplate& MetadataTemplate::GetInstance()
{
	static MetadataTemplate metadataTemplate;
	return metadataTemplate;
}

MetadataTemplate::~MetadataTemplate() {}

static std::string defaultStr = "";


std::string& MetadataTemplate::getValue(const char *key) {
	std::map<std::string, std::string>::iterator it;

	if ((it = m_templateFile->find(key)) == m_templateFile->end()) {
		return defaultStr;
	}
	return it->second;
}

bool MetadataTemplate::readMaster(const char* path, bool current)
{
	std::string templatePath = path;

	if (current) {

		templatePath += "/master.tpl";

		if (MetadataTemplate::readMaster(templatePath.c_str(), "master.tpl") == false) {
			return false;
		}
	}
	else {
		templatePath += "/base.tpl";

		if (MetadataTemplate::readMaster(templatePath.c_str(), "base.tpl") == false) {
			return false;
		}
	}
	return true;
}

bool MetadataTemplate::readDerivative(const char* path, bool current)
{
	return false;
}

bool MetadataTemplate::readMaster(const char* path, const char *datafile) {

	ConfigReader configReader;
	if (configReader.read(path, datafile, *m_templateFile) == false) {
		return false;
	}
	//templateFile.printAll();
	for (std::map<std::string, std::string>::iterator ii = m_templateFile->begin(); ii != m_templateFile->end(); ++ii) {
		std::string &value = getValue((*ii).first.c_str());
		//printf("\"%s\" = \"%s\"\n", (*ii).first.c_str(), (*ii).second.c_str());
		if (value.compare("")) {
			value = (*ii).second.c_str();
		}
		try {
			MTColumn& col = m_metadataTemplateRow.columnAt((*ii).first.c_str());
			col = (*ii).second.c_str();
		}
		catch (std::invalid_argument& e) {
			printf("%s", e.what());
		}
		catch (MTTypeException& e) {
			const char* tmp = e.what();
			printf("%s\n", tmp);
		}
	}

	MetadataTemplateResultsPresentation resultsPresentation(m_metadataTemplateRow);
	resultsPresentation.writeHuman();

	return true;
}

bool MetadataTemplate::readDerivative(const char* path, const char* datafile)
{
	return false;
}

bool MetadataTemplate::write(const char* datafile) {
	return true;
}

MetadataObject_ptr MetadataTemplate::getMetadataObject() {
	MetadataObject_ptr metadataObject = std::make_unique<MetadataObject>();
	
	for (std::map<std::string, std::string>::iterator ii = m_templateFile->begin(); ii != m_templateFile->end(); ++ii) {
		//std::string &value = getValue((*ii).first.c_str());
		//printf("\"%s\" opt:\"%s\"\n", (*ii).first.c_str(), (*ii).second.c_str());
		try {
			MTColumn& col = metadataObject->columnAt((*ii).first.c_str());
			col = (*ii).second.c_str();
		}
		catch (std::invalid_argument &e) {
			printf("%s", e.what());
		}
		catch (MTTypeException &e) {
			const char *tmp = e.what();
			printf("%s\n", tmp);
		}
		//if (value.compare("")) {
		//	*value = (*ii).second.c_str();
		//}
	}
	return metadataObject;
}

bool MetadataTemplateResultsPresentation::writeHuman()
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
bool MetadataTemplateResultsPresentation::writeXML()
{
	return false;
}
bool MetadataTemplateResultsPresentation::writeCSV()
{
	return false;
}
bool MetadataTemplateResultsPresentation::writeJson()
{
	return false;
}
bool MetadataTemplateResultsPresentation::writeHtml()
{
	return false;
}
} /* namespace simplearchive */
