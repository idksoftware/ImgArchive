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
		if (!keyword.compare(MD_SAMPLESPERPIXEL)) {
			//exifObject->m = value;
		}
		else if (!keyword.compare(MD_BITSPERSAMPLE)) {
			exifObject->m_bitsPerSample = value;
		}
		else if (!keyword.compare(MD_COLORSPACE)) {
			exifObject->m_colorSpace = value;
			//} else if (!keyword.compare(MD_EXIFOFFSET)) {
			//	exifObject->m_imageWidth = value;
		}
		else if (!keyword.compare(MD_DATECREATE)) {
			exifObject->m_dateTimeDigitized = value;
		}
		else if (!keyword.compare(MD_DATETIME)) {
			exifObject->m_dateTime = value;
		}
		else if (!keyword.compare(MD_IMAGEHEIGHT)) {
			exifObject->m_imageHeight = value;
		}
		else if (!keyword.compare(MD_IMAGEWIDTH)) {
			exifObject->m_imageWidth = value;
		}
		
		else if (!keyword.compare(MD_COPYRIGHT)) {
			exifObject->m_copyright = value;
		}
		else if (!keyword.compare(MD_YRESOLUTION)) {
			exifObject->m_yResolution = value;
		}
		else if (!keyword.compare(MD_XRESOLUTION)) {
			exifObject->m_xResolution = value;
		}
		else if (!keyword.compare(MD_ARTIST)) {
			exifObject->m_artist = value;
		}
		else if (!keyword.compare(MD_COMPRESSION)) {
			exifObject->m_compression = value;
		}
		else if (!keyword.compare(MD_DEPTH)) {
			exifObject->m_depth = value;
		}
		else if (!keyword.compare(MD_MIMETYPE)) {
			exifObject->m_mimetype = value;
		}
		else if (!keyword.compare(MD_DIGITALZOOM)) {
			exifObject->m_digitalZoom = value;
		}
		else if (!keyword.compare(MD_EXIFVERSION)) {
			exifObject->m_exifVersion = value;
		}
		else if (!keyword.compare(MD_EXPOSUREPROGRAM)) {
			exifObject->m_exposureProgram = value;
		}
		else if (!keyword.compare(MD_GPSTIMESTAMP)) {
			exifObject->m_artist = value;
		}
		else if (!keyword.compare(MD_LIGHTSOURCE)) {
			exifObject->m_lightSource = value;
		}
		else if (!keyword.compare(MD_PAGE)) {
			exifObject->m_page = value;
		}
		else if (!keyword.compare(MD_PRIMARYENCODING)) {
			exifObject->m_primaryEncoding = value;
		}
		else if (!keyword.compare(MD_RESOLUTION)) {
			exifObject->m_resolution = value;
		}
		else if (!keyword.compare(MD_SAMPLECOLOR)) {
			exifObject->m_sampleColor = value;
		}
		else if (!keyword.compare(MD_SENSINGMETHOD)) {
			exifObject->m_sensingMethod = value;
		}
		else if (!keyword.compare(MD_DESCRIPTION)) {
			exifObject->m_imageDescription = value;
		}
		else if (!keyword.compare(MD_MAKER)) {
			exifObject->m_make = value;
		}
		else if (!keyword.compare(MD_MODEL)) {
			exifObject->m_model = value;
		}
		else if (!keyword.compare(MD_ORIENTATION)) {
			exifObject->m_orientation = value;
		}
		else if (!keyword.compare(MD_BITSPERSAMPLE)) {
			exifObject->m_bitsPerSample = value;
		}
		else if (!keyword.compare(MD_SOFTWARE)) {
			exifObject->m_software = value;
		}
		
		else if (!keyword.compare(MD_DATETIMEORIGINAL)) {
			exifObject->m_dateTimeOriginal = value;
		}
		
		else if (!keyword.compare(MD_COPYRIGHT)) {
			exifObject->m_copyright = value;
		}
		else if (!keyword.compare(MD_EXPOSURETIME)) {
			exifObject->m_exposureTime = value;
		}
		else if (!keyword.compare(MD_FNUMBER)) {
			exifObject->m_fNumber = value;
		}
		else if (!keyword.compare(MD_ISOSPEEDRATING)) {
			exifObject->m_isoSpeedRatings = value;
		}
		else if (!keyword.compare(MD_EXPOSURETIME)) {
			exifObject->m_shutterSpeedValue = value;
		}
		else if (!keyword.compare(MD_EXPOSUREBIAS)) {
			exifObject->m_exposureBiasValue = value;
		}
		else if (!keyword.compare(MD_FOCUSDISTANCE)) {
			exifObject->m_subjectDistance = value;
		}
		else if (!keyword.compare(MD_FOCALLENGTH)) {
			exifObject->m_focalLength = value;
		}
		else if (!keyword.compare(MD_FOCALLENGTHIN35MM)) {
			exifObject->m_focalLengthIn35mm = value;
		}
		else if (!keyword.compare(MD_FLASH)) {
			exifObject->m_flash = value;
		}
		else if (!keyword.compare(MD_METERINGMODE)) {
			exifObject->m_meteringMode = value;
		}
		else if (!keyword.compare(MD_LATITUDE)) {
			exifObject->m_latitude = value;
		}
		else if (!keyword.compare(MD_LONGITUDE)) {
			exifObject->m_longitude = value;
		}
		else if (!keyword.compare(MD_ALTITUDE)) {
			exifObject->m_altitude = value;
		}
		else if (!keyword.compare(MD_IMAGEWIDTH)) {
			exifObject->m_imageWidth = value;
		}
		
		else if (!keyword.compare(MD_BITSPERSAMPLE)) {
			exifObject->m_bitsPerSample = value;
		}
		else if (!keyword.compare(MD_COLORSPACE)) {
			exifObject->m_colorSpace = value;
		}
		//else if (!keyword.compare(EXIFOFFSET)) {
		//	exifObject->m_artist = value;
		//}
		//else if (!keyword.compare(EXIFIMAGEWIDTH)) {
		//	exifObject->m_artist = value;
		//}
		
		//else if (!keyword.compare(EXIFIMAGEHEIGHT)) {
		//	exifObject->m_artist = value;
		//}
		
		else if (!keyword.compare(MD_COPYRIGHT)) {
			exifObject->m_copyright = value;
		}
		
	}
	if (exifObject->m_dateTime.empty()) {
		exifObject->m_dateTime = exifObject->m_dateTimeOriginal;
	}
	exifData.clear();
	return exifObject;
}

} /* namespace simplearchive */
