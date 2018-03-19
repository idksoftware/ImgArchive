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

#include "BasicMetadataFactory.h"
#include "CIDKCrc.h"
#include "md5.h"
#include <string>
#include <memory>
#include <fstream>
#include "SimpleExifReader.h"
#include "CLogger.h"
#include "IDKUuid.h"
#include <iostream>
#include "ExternalComand.h"
#include "ExifObject.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "ImageFileReader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

ImageFileReader::ImageFileReader()
{
}


ImageFileReader::~ImageFileReader()
{
}

ExifObject *ImageFileReader::externalExifTool(std::string &path) {
	CLogger &logger = CLogger::getLogger();

	AppConfig config = AppConfig::get();
	
	ExternalComand externalComand(config.getTempPath());
	if (config.getExternalExifTool() == nullptr) {
		//logger.log(LOG_OK, CLogger::ERROR, "Exif command line not found\n");

		return nullptr;
	}
	std::string externalExifTool = config.getExternalExifTool();
	std::string ExifToolPath = config.getToolsPath();
	ExifToolPath += '/';
	ExifToolPath += externalExifTool;

	if (SAUtils::FileExists(ExifToolPath.c_str()) == false) {
		return nullptr;
	}

	if (config.getExternalCommandLine() == nullptr) {
		logger.log(LOG_OK, CLogger::Level::ERR, "Exif command line not found\n");
		return nullptr;
	}
	std::string externalCommandLine = ExifToolPath;
	externalCommandLine += ' ';
	externalCommandLine += config.getExternalCommandLine();
	
	const char *exifMapPath;
	if (!(exifMapPath = config.getExifMapPath())) {
		logger.log(LOG_OK, CLogger::Level::ERR, "Exif map path not found");
		return nullptr;
	}
	std::string exifMap = exifMapPath;
	const char *exifMapFile;
	if (!(exifMapFile = config.getExifMapFile())) {
		logger.log(LOG_OK, CLogger::Level::ERR, "Exif map path not found");
		return nullptr;
	}
	exifMap += '/';
	exifMap += exifMapFile;

	logger.log(LOG_OK, CLogger::Level::FINE, "Raw Exif command line found \"%s\"", externalCommandLine.c_str());
	if (!externalComand.init(externalCommandLine.c_str(), exifMapPath)) {
		
		return nullptr;
	}
	ExifObject *exifObject = externalComand.process(path.c_str());
	if (exifObject == nullptr) {
		logger.log(LOG_OK, CLogger::Level::WARNING, "Failed to read map file for \"%s\"", externalCommandLine.c_str());
		return nullptr;
	}
	exifObject->print();
	//DateTime = ExifDateTime(exifObject->getDateTime().c_str());
	return exifObject;
}

} /* namespace simplearchive */
