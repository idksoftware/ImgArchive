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
	
std::unique_ptr<ConfigBlock>	MetadataTemplate::m_templateFile(new ConfigBlock);
//Config *MetadataTemplate::m_templateFile = 0;

std::unique_ptr<MetadataTemplate> MetadataTemplate::m_instance;
std::once_flag MetadataTemplate::m_onceFlag;

MetadataTemplate& MetadataTemplate::GetInstance()
{
	std::call_once(m_onceFlag,
		[] {
		m_instance.reset(new MetadataTemplate);
	});
	return *m_instance.get();
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

bool MetadataTemplate::read(const char *datafile) {

	ConfigReader configReader;
	if (configReader.read(datafile, *m_templateFile) == false) {
		return false;
	}
	//templateFile.printAll();
	for (std::map<std::string, std::string>::iterator ii = m_templateFile->begin(); ii != m_templateFile->end(); ++ii) {
		std::string &value = getValue((*ii).first.c_str());
		//printf("\"%s\" opt:\"%s\"\n", (*ii).first.c_str(), (*ii).second.c_str());
		//if (value.compare("")) {
		//	*value = (*ii).second.c_str();
		//}
	}
	return true;
}
MetadataObject_ptr MetadataTemplate::getMetadataObject() {
	MetadataObject* metadataObject = new MetadataObject;
	
	for (std::map<std::string, std::string>::iterator ii = m_templateFile->begin(); ii != m_templateFile->end(); ++ii) {
		std::string &value = getValue((*ii).first.c_str());
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
	return MetadataObject_ptr(metadataObject);
}
} /* namespace simplearchive */
