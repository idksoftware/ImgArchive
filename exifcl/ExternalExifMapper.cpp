/*
 * ExifReader.cpp
 *
 *  Created on: May 30, 2014
 *      Author: wzw7yn
 */

#include <stdio.h>
#include <memory>
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
	for (std::map<std::string, std::string>::iterator ii = exifData.begin(); ii != exifData.end(); ++ii) {
		logger.log(CLogger::FINE, "Exif data Key: \"%s\" Value: \"%s\"", ii->first.c_str(), ii->second.c_str());
		//std::cout << ii->first << " " << ii->second << '\n';
	}
	//m_exifMap->printAll();
	logger.log(CLogger::FINE, "EXIF Map");
	for (std::map<std::string, std::string>::iterator ii = m_exifMap->begin(); ii != m_exifMap->end(); ++ii) {
		logger.log(CLogger::FINE, "Exif map Key: \"%s\" Value: \"%s\"", ii->first.c_str(), ii->second.c_str());
		//std::cout << ii->first << " " << ii->second << '\n';
	}
	for (std::map<std::string, std::string>::iterator ii = exifData.begin(); ii != exifData.end(); ++ii) {
		std::map<std::string, std::string>::iterator iter = m_exifMap->find(ii->first);
		if (iter == m_exifMap->end()) {
			continue;
		}
		std::string &keyword = iter->second;
		std::string &value = ii->second;
		logger.log(CLogger::FINE, "maping:\"%s\" value:\"%s\"", keyword.c_str(), value.c_str());
		if (!keyword.compare(IMAGEWIDTH)) {
			exifObject->m_imageWidth = value;
			//} else if (!keyword.compare(IMAGELENGTH)) {
			//	exifObject->m_imageHeight = value;
		}
		else if (!keyword.compare(BITSPERSAMPLE)) {
			exifObject->m_bitsPerSample = value;
		}
		else if (!keyword.compare(COLORSPACE)) {
			exifObject->m_colorSpace = value;
			//} else if (!keyword.compare(EXIFOFFSET)) {
			//	exifObject->m_imageWidth = value;
		}
		else if (!keyword.compare(DATETIME)) {
			exifObject->m_dateTime = value;
		}
		else if (!keyword.compare(IMAGEHEIGHT)) {
			exifObject->m_imageHeight = value;
		}
		else if (!keyword.compare(SAMPLESPERPIXEL)) {
			exifObject->m_imageWidth = value;
		}
		else if (!keyword.compare(COPYRIGHT)) {
			exifObject->m_copyright = value;
		}
		else if (!keyword.compare(YRESOLUTION)) {
			exifObject->m_yResolution = value;
		}
		else if (!keyword.compare(XRESOLUTION)) {
			exifObject->m_xResolution = value;
		}
		else if (!keyword.compare(ARTIST)) {
			exifObject->m_artist = value;
		}
		else if (!keyword.compare(COMPRESSION)) {
			exifObject->m_compression = value;
		}
		else if (!keyword.compare(DEPTH)) {
			exifObject->m_depth = value;
		}
		else if (!keyword.compare(MIMETYPE)) {
			exifObject->m_mimetype = value;
		}
		else if (!keyword.compare(DIGITALZOOM)) {
			exifObject->m_digitalZoom = value;
		}
		else if (!keyword.compare(EXIFVERSION)) {
			exifObject->m_exifVersion = value;
		}
		else if (!keyword.compare(EXPOSUREPROGRAM)) {
			exifObject->m_exposureProgram = value;
		}
		else if (!keyword.compare(GPSTIMESTAMP)) {
			exifObject->m_artist = value;
		}
		else if (!keyword.compare(LIGHTSOURCE)) {
			exifObject->m_lightSource = value;
		}
		else if (!keyword.compare(PAGE)) {
			exifObject->m_page = value;
		}
		else if (!keyword.compare(PRINARYENCODING)) {
			exifObject->m_primaryEncoding = value;
		}
		else if (!keyword.compare(RESOLUTION)) {
			exifObject->m_resolution = value;
		}
		else if (!keyword.compare(SAMPLECOLOR)) {
			exifObject->m_sampleColor = value;
		}
		else if (!keyword.compare(SENSINGMETHOD)) {
			exifObject->m_sensingMethod = value;
		}
		else if (!keyword.compare(IMAGEDESCRIPTION)) {
			exifObject->m_imageDescription = value;
		}
		else if (!keyword.compare(MAKE)) {
			exifObject->m_make = value;
		}
		else if (!keyword.compare(MODEL)) {
			exifObject->m_model = value;
		}
		else if (!keyword.compare(ORIENTATION)) {
			exifObject->m_orientation = value;
		}
		else if (!keyword.compare(BITSPERSAMPLE)) {
			exifObject->m_bitsPerSample = value;
		}
		else if (!keyword.compare(SOFTWARE)) {
			exifObject->m_software = value;
		}
		else if (!keyword.compare(DATETIME)) {
			exifObject->m_dateTime = value;
		}
		else if (!keyword.compare(DATETIMEORIGINAL)) {
			exifObject->m_dateTimeOriginal = value;
		}
		else if (!keyword.compare(DATETIMEDIGITIZED)) {
			exifObject->m_dateTimeDigitized = value;
		}
		else if (!keyword.compare(SUBSECTIMEORIGINAL)) {
			exifObject->m_subSecTimeOriginal = value;
		}
		else if (!keyword.compare(COPYRIGHT)) {
			exifObject->m_copyright = value;
		}
		else if (!keyword.compare(EXPOSURETIME)) {
			exifObject->m_exposureTime = value;
		}
		else if (!keyword.compare(FNUMBER)) {
			exifObject->m_fNumber = value;
		}
		else if (!keyword.compare(ISOSPEEDRATINGS)) {
			exifObject->m_isoSpeedRatings = value;
		}
		else if (!keyword.compare(SHUTTERSPEEDVALUE)) {
			exifObject->m_shutterSpeedValue = value;
		}
		else if (!keyword.compare(EXPOSUREBIASVALUE)) {
			exifObject->m_exposureBiasValue = value;
		}
		else if (!keyword.compare(SUBJECTDISTANCE)) {
			exifObject->m_subjectDistance = value;
		}
		else if (!keyword.compare(FOCALLENGTH)) {
			exifObject->m_focalLength = value;
		}
		else if (!keyword.compare(FOCALLENGTHIN35MM)) {
			exifObject->m_focalLengthIn35mm = value;
		}
		else if (!keyword.compare(FLASH)) {
			exifObject->m_flash = value;
		}
		else if (!keyword.compare(METERINGMODE)) {
			exifObject->m_meteringMode = value;
		}
		else if (!keyword.compare(LATITUDE)) {
			exifObject->m_latitude = value;
		}
		else if (!keyword.compare(LONGITUDE)) {
			exifObject->m_longitude = value;
		}
		else if (!keyword.compare(ALTITUDE)) {
			exifObject->m_altitude = value;
		}
		else if (!keyword.compare(IMAGEWIDTH)) {
			exifObject->m_imageWidth = value;
		}
		else if (!keyword.compare(IMAGELENGTH)) {
			exifObject->m_imageHeight = value;
		}
		else if (!keyword.compare(BITSPERSAMPLE)) {
			exifObject->m_bitsPerSample = value;
		}
		else if (!keyword.compare(COLORSPACE)) {
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
		
		else if (!keyword.compare(COPYRIGHT)) {
			exifObject->m_copyright = value;
		}
		
	}
	exifData.clear();
	return exifObject;
}

} /* namespace simplearchive */
