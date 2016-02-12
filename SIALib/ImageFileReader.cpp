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

#include "BasicExifFactory.h"
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

	CAppConfig config = CAppConfig::get();
	ExternalComand externalComand(config.getTempPath());

	const char *externalCommandLine;
	if (!(externalCommandLine = config.getExternalCommandLine())) {
		//logger.log(LOG_OK, CLogger::ERROR, "Exif command line not found\n");
		
		return nullptr;
	}
	const char *exifMapPath;
	if (!(exifMapPath = config.getExifMapPath())) {
		//logger.log(LOG_OK, CLogger::ERROR, "Exif map path not found");
		
		return nullptr;
	}
	logger.log(LOG_OK, CLogger::FINE, "Raw exif command line found \"%s\"", externalCommandLine);
	if (!externalComand.init(externalCommandLine, exifMapPath)) {
		
		return nullptr;
	}
	ExifObject *exifObject = externalComand.process(path.c_str());
	if (exifObject == nullptr) {
		return nullptr;
	}
	exifObject->print();
	//DateTime = ExifDateTime(exifObject->getDateTime().c_str());
	return exifObject;
}

} /* namespace simplearchive */
