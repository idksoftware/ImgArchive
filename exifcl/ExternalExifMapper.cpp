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
//#include "MetadataProperties.h"
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
	m_exifMap = new ConfigBlock;

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

/*
bool ExternalExifMapper::init(const std::string &str) {
	ConfigReader configReader;


	if (!configReader.read(str, *m_exifMap)) {
		return false;
	}
	return true;
}
*/

ExifObject *ExternalExifMapper::create(const char *exiffile) {
	// test
	
	ConfigReader configReader;
	ConfigBlock exifData;
	exifData.setDelimChar(':');
	CLogger &logger = CLogger::getLogger();
	logger.log(LOG_OK, CLogger::Level::FINE, "Reading external Exif data file: \"%s\"", exiffile);
	if (!configReader.read(exiffile, exifData)) {
		logger.log(LOG_OK, CLogger::Level::ERR, "Failed reading external Exif data file: \"%s\"", exiffile);
		return 0;
	}
	return create(exifData);
}

ExifObject *ExternalExifMapper::create(const std::string &str) {
	// test

	ConfigReader configReader;
	ConfigBlock exifData;
	exifData.setDelimChar(':');
	CLogger &logger = CLogger::getLogger();

	if (!configReader.readExif(str, exifData)) {
		logger.log(LOG_OK, CLogger::Level::ERR, "Failed reading external Exif data");
		return 0;
	}
	return create(exifData);
}


ExifObject *ExternalExifMapper::create(ConfigBlock &exifData) {
	ExifObject *exifObject = new ExifObject;
	
	CLogger &logger = CLogger::getLogger();
	logger.log(LOG_OK, CLogger::Level::FINE, "==== Processing EXIF Data from external reader =====");
	logger.log(LOG_OK, CLogger::Level::FINE, "Display the EXIF data generated from tool");

	for (auto ii = exifData.begin(); ii != exifData.end(); ++ii) {
		logger.log(LOG_OK, CLogger::Level::TRACE, "File item: \"%s\" Value: \"%s\"", ii->first.c_str(), ii->second.c_str());
	}
	

	int valuesFound = 0;
	logger.log(LOG_OK, CLogger::Level::FINE, "Display EXIF Map");

	for (auto ii = m_exifMap->begin(); ii != m_exifMap->end(); ++ii) {
		logger.log(LOG_OK, CLogger::Level::FINE, "\"%s\" Value: \"%s\"", ii->first.c_str(), ii->second.c_str());
		valuesFound++;
	}
	logger.log(LOG_OK, CLogger::Level::FINE, "Completed %d values found", valuesFound);

	logger.log(LOG_OK, CLogger::Level::FINE, "Processing EXIF Map");
	valuesFound = 0;
	for (auto ii = m_exifMap->begin(); ii != m_exifMap->end(); ++ii) {

		auto iter = exifData.find(ii->second);
		if (iter == exifData.end()) {
			continue;
		}

		logger.log(LOG_OK, CLogger::Level::TRACE, "Found keyword:\"%s\" value:\"%s\"", ii->first.c_str(), ii->second.c_str());
		std::string keyword = ii->first;
		std::string value = iter->second;
		if (value.empty()) {
			continue; // Do not add empty string
		}
		logger.log(LOG_OK, CLogger::Level::FINE, "Keyword:\"%s\" = \"%s\"", keyword.c_str(), value.c_str());

		try {
			MTColumn &col = exifObject->columnAt(keyword.c_str());
			if (col.fromString(value) == false) {
				logger.log(LOG_OK, CLogger::Level::WARNING, "Invalid value. keyword:\"%s\" value:\"%s\"", keyword.c_str(), value.c_str());
			}
			else {
				valuesFound++;
			}
		}
		catch (std::exception &e) {
			logger.log(LOG_OK, CLogger::Level::WARNING, "invalid keyword:\"%s\" error: %s", keyword.c_str(), e.what());
		}

	}
	logger.log(LOG_OK, CLogger::Level::FINE, "Completed mapping valuse %d values found", valuesFound);

		
	logger.log(LOG_OK, CLogger::Level::FINE, "Final metadata");
	ExifObjectSchema& mos = (ExifObjectSchema&)exifObject->getSchema();
	for (std::vector<MTSchema>::iterator i = mos.begin(); i != mos.end(); i++) {
		MTSchema& columnInfo = *i;
		
		logger.log(LOG_OK, CLogger::Level::FINE, "%-20s %s", columnInfo.getName().c_str(), exifObject->columnAt(columnInfo.getName().c_str()).toString().c_str());
	}
	
	logger.log(LOG_OK, CLogger::Level::FINE, "==== Completed processing EXIF Data from external reader =====");
	exifData.clear();
	return exifObject;
}

} /* namespace simplearchive */
