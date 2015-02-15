/*
* ImageExtentions.cpp
*
*  Created on: May 9, 2014
*      Author: wzw7yn
*/
#include "ImageId.h"
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
		//logger.log(CLogger::ERROR, "Exif command line not found\n");
		
		return nullptr;
	}
	const char *exifMapPath;
	if (!(exifMapPath = config.getExifMapPath())) {
		//logger.log(CLogger::ERROR, "Exif map path not found");
		
		return nullptr;
	}
	logger.log(CLogger::FINE, "Raw exif command line found \"%s\"", externalCommandLine);
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
