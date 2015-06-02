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

#include "DBDefines.h"
#include "MetadataObject.h"

namespace simplearchive {

const char *MetadataObject::m_labelList[] = {
	DB_SEQUENCEID,
	DB_FILENAME,
	DB_FILEPATH,
	DB_ORGINALNAME,
	DB_UUID,
	DB_LABEL,
	DB_RATING,
	DB_MEDIATYPE,
	DB_MD5,
	DB_CRC,
	DB_FILESIZE,
	DB_DATECREATE,
	DB_DATEMODIFIED,
	DB_DATEADDED,
	DB_DESCRIPTION,

	/// Media Properties
	DB_WIDTH,
	DB_HEIGHT,
	DB_RESOLUTION,
	DB_DEPTH,
	DB_VIEWROTATION,
	DB_SAMPLECOLOR,
	DB_PAGE,
	DB_COLORSPACE,
	DB_COMPRESSION,
	DB_PRIMARYENCODING,
	/// Camera Information
	DB_MAKER,
	DB_MODEL,
	DB_SOFTWARE,
	DB_SOURCEURL,
	DB_EXIFVERSION,
	DB_CAPTUREDATE,
	DB_EXPOSUREPROGRAM,
	DB_ISOSPEEDRATING,
	DB_EXPOSUREBIAS,
	DB_EXPOSURETIME,
	DB_APERTURE,
	DB_METERINGMODE,
	DB_LIGHTSOURCE,
	DB_FLASH,
	DB_FOCALLENGTH,
	DB_SENSINGMETHOD,
	DB_DIGITALZOOM,
	/// GPS
	DB_LATITUDE,
	DB_LONGITUDE,
	DB_GPSTIMESTAMP,
	//Copyright Properties
	DB_COPYRIGHT,
	DB_USAGERIGHTS,
	DB_COPYRIGHTURL
};

unsigned int MetadataObject::getListLenght() {
	int s = sizeof(m_labelList);
	return (sizeof(m_labelList)/sizeof(char *));
}


MetadataObject::MetadataObject() {
	m_doMakeValueList = true;

	/*
	m_lookup.insert(std::make_pair( DB_SEQUENCEID, &m_sequenceId));
	m_lookup.insert(std::make_pair( DB_FILENAME, &m_filename));
	m_lookup.insert(std::make_pair( DB_FILEPATH, &m_filepath));
	m_lookup.insert(std::make_pair( DB_ORGINALNAME, &m_orginalName));
	m_lookup.insert(std::make_pair( DB_UUID, &m_uniqueId));
	m_lookup.insert(std::make_pair( DB_LABEL, &m_label));
	m_lookup.insert(std::make_pair( DB_RATING, &m_rating));
	m_lookup.insert(std::make_pair( DB_MEDIATYPE, &m_mediaType));
	m_lookup.insert(std::make_pair( DB_MD5, &m_md5));
	m_lookup.insert(std::make_pair( DB_CRC, &m_crc));
	m_lookup.insert(std::make_pair( DB_FILESIZE, &m_fileSize));
	m_lookup.insert(std::make_pair( DB_DATECREATE, &m_dateCreate));
	m_lookup.insert(std::make_pair( DB_DATEMODIFIED, &m_dateModified));
	m_lookup.insert(std::make_pair( DB_DATEADDED, &m_dateAdded));
	m_lookup.insert(std::make_pair( DB_DESCRIPTION, &m_description));

	/// Media Properties
	m_lookup.insert(std::make_pair( DB_WIDTH, &m_width));
	m_lookup.insert(std::make_pair( DB_HEIGHT, &m_height));
	m_lookup.insert(std::make_pair( DB_RESOLUTION, &m_resolution));
	m_lookup.insert(std::make_pair( DB_DEPTH, &m_depth));
	m_lookup.insert(std::make_pair( DB_VIEWROTATION, &m_viewRotation));
	m_lookup.insert(std::make_pair( DB_SAMPLECOLOR, &m_sampleColor));
	m_lookup.insert(std::make_pair( DB_PAGE, &m_page));
	m_lookup.insert(std::make_pair( DB_COLORSPACE, &m_colorSpace));
	m_lookup.insert(std::make_pair( DB_COMPRESSION, &m_compression));
	m_lookup.insert(std::make_pair( DB_PRINARYENCODING, &m_prinaryEncoding));
	/// Camera Information
	m_lookup.insert(std::make_pair( DB_MAKER, &m_maker));
	m_lookup.insert(std::make_pair( DB_MODEL, &m_model));
	m_lookup.insert(std::make_pair( DB_SOFTWARE, &m_software));
	m_lookup.insert(std::make_pair( DB_SOURCEURL, &m_sourceURL));
	m_lookup.insert(std::make_pair( DB_EXIFVERSION, &m_exifVersion));
	m_lookup.insert(std::make_pair( DB_CAPTUREDATE, &m_captureDate));
	m_lookup.insert(std::make_pair( DB_EXPOSUREPROGRAM, &m_exposureProgram));
	m_lookup.insert(std::make_pair( DB_ISOSPEEDRATING, &m_isoSpeedRating));
	m_lookup.insert(std::make_pair( DB_EXPOSUREBIAS, &m_exposureBias));
	m_lookup.insert(std::make_pair( DB_EXPOSURETIME, &m_exposureTime));
	m_lookup.insert(std::make_pair( DB_APERTURE, &m_aperture));
	m_lookup.insert(std::make_pair( DB_METERINGMODE, &m_meteringMode));
	m_lookup.insert(std::make_pair( DB_LIGHTSOURCE, &m_lightSource));
	m_lookup.insert(std::make_pair( DB_FLASH, &m_flash));
	m_lookup.insert(std::make_pair( DB_FOCALLENGTH, &m_focalLength));
	m_lookup.insert(std::make_pair( DB_SENSINGMETHOD, &m_sensingMethod));
	m_lookup.insert(std::make_pair( DB_DIGITALZOOM, &m_digitalZoom));
	/// GPS
	m_lookup.insert(std::make_pair( DB_LATITUDE, &m_latitude));
	m_lookup.insert(std::make_pair( DB_LONGITUDE, &m_longitude));
	m_lookup.insert(std::make_pair( DB_GPSTIMESTAMP, &m_gpsTimeStamp));
	//Copyright Properties
	m_lookup.insert(std::make_pair( DB_COPYRIGHT, &m_copyright));
	m_lookup.insert(std::make_pair( DB_USAGERIGHTS, &m_usageRights));
	m_lookup.insert(std::make_pair( DB_COPYRIGHTURL, &m_copyrightURL));
*/
}

MetadataObject::~MetadataObject() {
	
}


void MetadataObject::makeValueList() {
	if (m_doMakeValueList == false) {
		return;
	}
	m_valueList.push_back(&m_sequenceId);
	m_valueList.push_back(& m_filename);
	m_valueList.push_back(& m_filepath);
	m_valueList.push_back(& m_orginalName);
	m_valueList.push_back(& m_uniqueId);
	m_valueList.push_back(& m_label);
	m_valueList.push_back(& m_rating);
	m_valueList.push_back(& m_mediaType);
	m_valueList.push_back(& m_md5);
	m_valueList.push_back(& m_crc);
	m_valueList.push_back(& m_fileSize);
	m_valueList.push_back(& m_dateCreate);
	m_valueList.push_back(& m_dateModified);
	m_valueList.push_back(& m_dateAdded);
	m_valueList.push_back(& m_description);
	/// Media Properties
	m_valueList.push_back(& m_width);
	m_valueList.push_back(& m_height);
	m_valueList.push_back(& m_resolution);
	m_valueList.push_back(& m_depth);
	m_valueList.push_back(& m_viewRotation);
	m_valueList.push_back(& m_sampleColor);
	m_valueList.push_back(& m_page);
	m_valueList.push_back(& m_colorSpace);
	m_valueList.push_back(& m_compression);
	m_valueList.push_back(& m_primaryEncoding);
	/// Camerai Information"
	m_valueList.push_back(& m_maker);
	m_valueList.push_back(& m_model);
	m_valueList.push_back(& m_software);
	m_valueList.push_back(& m_sourceURL);
	m_valueList.push_back(& m_exifVersion);
	m_valueList.push_back(& m_captureDate);
	m_valueList.push_back(& m_exposureProgram);
	m_valueList.push_back(& m_isoSpeedRating);
	m_valueList.push_back(& m_exposureBias);
	m_valueList.push_back(& m_exposureTime);
	m_valueList.push_back(& m_aperture);
	m_valueList.push_back(& m_meteringMode);
	m_valueList.push_back(& m_lightSource);
	m_valueList.push_back(& m_flash);
	m_valueList.push_back(& m_focalLength);
	m_valueList.push_back(& m_sensingMethod);
	m_valueList.push_back(& m_digitalZoom);
	/// GPS
	m_valueList.push_back(& m_latitude);
	m_valueList.push_back(& m_longitude);
	m_valueList.push_back(& m_gpsTimeStamp);
	//Copyright Properties
	m_valueList.push_back(& m_copyright);
	m_valueList.push_back(& m_usageRights);
	m_valueList.push_back(& m_copyrightURL);
	m_doMakeValueList = false;

}
} /* namespace simplearchive */
