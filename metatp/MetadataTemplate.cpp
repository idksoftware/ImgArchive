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
#include "ConfigReader.h"

namespace simplearchive {

MetadataTemplate::MetadataTemplate() {
	m_metadataObject = new MetadataObject;

	m_lookup.insert(std::make_pair( DB_SEQUENCEID, &(m_metadataObject->m_sequenceId)));
	//m_lookup.insert(std::make_pair( DB_FILENAME, &(m_metadataObject->m_filename)));
	//m_lookup.insert(std::make_pair( DB_FILEPATH, &(m_metadataObject->m_filepath)));
	//m_lookup.insert(std::make_pair( DB_ORGINALNAME, &(m_metadataObject->m_orginalName)));
	//m_lookup.insert(std::make_pair( DB_UUID, &(m_metadataObject->m_uniqueId)));
	m_lookup.insert(std::make_pair( DB_LABEL, &(m_metadataObject->m_label)));
	m_lookup.insert(std::make_pair( DB_RATING, &(m_metadataObject->m_rating)));
	//m_lookup.insert(std::make_pair( DB_MEDIATYPE, &(m_metadataObject->m_mediaType)));
	//m_lookup.insert(std::make_pair( DB_MD5, &(m_metadataObject->m_md5)));
	//m_lookup.insert(std::make_pair( DB_CRC, &(m_metadataObject->m_crc)));
	//m_lookup.insert(std::make_pair( DB_FILESIZE, &(m_metadataObject->m_fileSize)));
	/*
	m_lookup.insert(std::make_pair( DB_DATECREATE, &(m_metadataObject->m_dateCreate)));
	m_lookup.insert(std::make_pair( DB_DATEMODIFIED, &(m_metadataObject->m_dateModified)));
	m_lookup.insert(std::make_pair( DB_DATEADDED, &(m_metadataObject->m_dateAdded)));
	m_lookup.insert(std::make_pair( DB_DESCRIPTION, &(m_metadataObject->m_description)));
    */
	/// Media Properties
	m_lookup.insert(std::make_pair( DB_WIDTH, &(m_metadataObject->m_width)));
	m_lookup.insert(std::make_pair( DB_HEIGHT, &(m_metadataObject->m_height)));
	m_lookup.insert(std::make_pair( DB_RESOLUTION, &(m_metadataObject->m_resolution)));
	m_lookup.insert(std::make_pair( DB_DEPTH, &(m_metadataObject->m_depth)));
	m_lookup.insert(std::make_pair( DB_VIEWROTATION, &(m_metadataObject->m_viewRotation)));
	m_lookup.insert(std::make_pair( DB_SAMPLECOLOR, &(m_metadataObject->m_sampleColor)));
	m_lookup.insert(std::make_pair( DB_PAGE, &(m_metadataObject->m_page)));
	m_lookup.insert(std::make_pair( DB_COLORSPACE, &(m_metadataObject->m_colorSpace)));
	m_lookup.insert(std::make_pair( DB_COMPRESSION, &(m_metadataObject->m_compression)));
	m_lookup.insert(std::make_pair( DB_PRIMARYENCODING, &(m_metadataObject->m_primaryEncoding)));
	/// Camera Information
	m_lookup.insert(std::make_pair( DB_MAKER, &(m_metadataObject->m_maker)));
	m_lookup.insert(std::make_pair( DB_MODEL, &(m_metadataObject->m_model)));
	m_lookup.insert(std::make_pair( DB_SOFTWARE, &(m_metadataObject->m_software)));
	m_lookup.insert(std::make_pair( DB_SOURCEURL, &(m_metadataObject->m_sourceURL)));
	m_lookup.insert(std::make_pair( DB_EXIFVERSION, &(m_metadataObject->m_exifVersion)));
	m_lookup.insert(std::make_pair( DB_CAPTUREDATE, &(m_metadataObject->m_captureDate)));
	m_lookup.insert(std::make_pair( DB_EXPOSUREPROGRAM, &(m_metadataObject->m_exposureProgram)));
	m_lookup.insert(std::make_pair( DB_ISOSPEEDRATING, &(m_metadataObject->m_isoSpeedRating)));
	m_lookup.insert(std::make_pair( DB_EXPOSUREBIAS, &(m_metadataObject->m_exposureBias)));
	m_lookup.insert(std::make_pair( DB_EXPOSURETIME, &(m_metadataObject->m_exposureTime)));
	m_lookup.insert(std::make_pair( DB_APERTURE, &(m_metadataObject->m_aperture)));
	m_lookup.insert(std::make_pair( DB_METERINGMODE, &(m_metadataObject->m_meteringMode)));
	m_lookup.insert(std::make_pair( DB_LIGHTSOURCE, &(m_metadataObject->m_lightSource)));
	m_lookup.insert(std::make_pair( DB_FLASH, &(m_metadataObject->m_flash)));
	m_lookup.insert(std::make_pair( DB_FOCALLENGTH, &(m_metadataObject->m_focalLength)));
	m_lookup.insert(std::make_pair( DB_SENSINGMETHOD, &(m_metadataObject->m_sensingMethod)));
	m_lookup.insert(std::make_pair( DB_DIGITALZOOM, &(m_metadataObject->m_digitalZoom)));
	/// GPS
	m_lookup.insert(std::make_pair( DB_LATITUDE, &(m_metadataObject->m_latitude)));
	m_lookup.insert(std::make_pair( DB_LONGITUDE, &(m_metadataObject->m_longitude)));
	m_lookup.insert(std::make_pair( DB_GPSTIMESTAMP, &(m_metadataObject->m_gpsTimeStamp)));
	//Copyright Properties
	m_lookup.insert(std::make_pair( DB_COPYRIGHT, &(m_metadataObject->m_copyright)));
	m_lookup.insert(std::make_pair( DB_USAGERIGHTS, &(m_metadataObject->m_usageRights)));
	m_lookup.insert(std::make_pair( DB_COPYRIGHTURL, &(m_metadataObject->m_copyrightURL)));

}

MetadataTemplate::~MetadataTemplate() {
	// TODO Auto-generated destructor stub
}

std::string *MetadataTemplate::getValue(const char *key) {
	std::map<std::string, std::string *>::iterator it;

	if ((it = m_lookup.find(key)) == m_lookup.end()) {
		return 0;
	}
	return it->second;
}

bool MetadataTemplate::read(const char *datafile) {
	Config templateFile;
	ConfigReader configReader;
	if (configReader.read(datafile, templateFile) == false) {
		return false;
	}
	//templateFile.printAll();
	for (std::map<std::string, std::string>::iterator ii = templateFile.begin(); ii != templateFile.end(); ++ii) {
		std::string *value = getValue((*ii).first.c_str());
		//printf("\"%s\" opt:\"%s\"\n", (*ii).first.c_str(), (*ii).second.c_str());
		if (value != 0) {
			*value = (*ii).second.c_str();
		}
	}
	return true;
}

} /* namespace simplearchive */
