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
#include <memory>
#include "MetadataProperties.h"
#include "ExternalExifMapper.h"
#include "ExifObject.h"
#include "ConfigReader.h"
#include "CLogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {
struct MapItem {
	MapItem(const char *name);
};
ExternalExifMapper::ExternalExifMapper() {
	m_exifMap = new Config;

}

ExternalExifMapper::~ExternalExifMapper() {
	if (m_exifMap == 0) {
		m_exifMap->clear();
		delete m_exifMap;
		m_exifMap = 0;
	}
}

bool ExternalExifMapper::init(const char *exifMapFile) {
	ConfigReader configReader;
	
	
	if (!configReader.read(exifMapFile, *m_exifMap)) {
		return false;
	}
	return true;
}

ExifObject *ExternalExifMapper::create(const char *exiffile) {
	// test
	exiffile = "c:/temp/test.exf";
	ConfigReader configReader;
	Config exifData;
	exifData.setDelimChar(':');
	CLogger &logger = CLogger::getLogger();
	logger.log(CLogger::FINE, "Reading external Exif data file: \"%s\"", exiffile);
	if (!configReader.read(exiffile, exifData)) {
		logger.log(CLogger::ERROR, "Failed reading external Exif data file: \"%s\"", exiffile);
		return 0;
	}
	return create(exifData);
}

ExifObject *ExternalExifMapper::create(Config &exifData) {
	ExifObject *exifObject = new ExifObject;
	//exifData.printAll();
	CLogger &logger = CLogger::getLogger();
	logger.log(CLogger::FINE, "EXIF Data");

//	printf("Print the EXIF File generated\n");
	for (std::map<std::string, std::string>::iterator ii = exifData.begin(); ii != exifData.end(); ++ii) {
		logger.log(CLogger::FINE, "Exif data Key: \"%s\" Value: \"%s\"", ii->first.c_str(), ii->second.c_str());
//		printf("Exif data Key: \"%s\" Value: \"%s\"\n", ii->first.c_str(), ii->second.c_str());
		//std::cout << ii->first << " " << ii->second << '\n';
	}
	
	//m_exifMap->printAll();
	logger.log(CLogger::FINE, "EXIF Map");
//	printf("Print the EXIF map\n");
	for (std::map<std::string, std::string>::iterator ii = m_exifMap->begin(); ii != m_exifMap->end(); ++ii) {
		logger.log(CLogger::FINE, "Exif map Key: \"%s\" Value: \"%s\"", ii->first.c_str(), ii->second.c_str());
//		printf("Exif map Key: \"%s\" Value: \"%s\"\n", ii->first.c_str(), ii->second.c_str());
		//std::cout << ii->first << " " << ii->second << '\n';
	}
	for (std::map<std::string, std::string>::iterator ii = exifData.begin(); ii != exifData.end(); ++ii) {
		std::map<std::string, std::string>::iterator iter = m_exifMap->find(ii->first);
		if (iter == m_exifMap->end()) {
			continue;
		}
			
//		printf("Found\n");
		std::string &keyword = iter->second;
		std::string &value = ii->second;
		logger.log(CLogger::FINE, "maping:\"%s\" value:\"%s\"", keyword.c_str(), value.c_str());
//		printf("maping:\"%s\" value:\"%s\"\n", keyword.c_str(), value.c_str());

		try {
			MTColumn &col = exifObject->columnAt(keyword.c_str());
			if (col.fromString(value) == false) {
				logger.log(CLogger::WARNING, "Invalid value. keyword:\"%s\" value:\"%s\"", keyword.c_str(), value.c_str());
			}
		}
		catch (std::exception /*&e*/) {
			logger.log(CLogger::WARNING, "invalid keyword:\"%s\"", keyword.c_str());
		}
		
	}
	/* Updated 
	if (exifObject->m_dateTime.empty()) {
		exifObject->m_dateTime = exifObject->m_dateTimeOriginal;
	}
	*/
	exifData.clear();
	return exifObject;
}

} /* namespace simplearchive */
