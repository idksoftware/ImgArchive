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

#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include "CSVDatabase.h"
#include "MetadataUpdate.h"
#include "MetadataDocument.h"
#include "CSVArgs.h"
#include "DBDefines.h"

namespace simplearchive {


std::string MetadataUpdate::m_archivePath;

class MetadataUpdateItem {
	const char *m_cmdLabel;
	unsigned int m_idx;
public:
	MetadataUpdateItem(const char *cmdLabel, int idx) {
		m_cmdLabel = cmdLabel;
		m_idx = idx;
	}
	const char *getName() {
		return m_cmdLabel;
	}
	unsigned int idx() {
		return m_idx;
	}
};

MetadataUpdateItem commandList[] = {
        MetadataUpdateItem(DB_SEQUENCEID, DB_SEQUENCE_ID_IDX),

        MetadataUpdateItem(DB_FILENAME, DB_FILENAME_IDX),

        MetadataUpdateItem(DB_FILEPATH, DB_FILEPATH_IDX),

        MetadataUpdateItem(DB_ORGINALNAME, DB_ORGINAL_NAME_IDX),

        MetadataUpdateItem(DB_UUID, DB_UNIQUE_ID_IDX),
        MetadataUpdateItem(DB_LABEL, DB_LABEL_IDX),
        MetadataUpdateItem(DB_RATING, DB_RATING_IDX),

        MetadataUpdateItem(DB_MEDIATYPE, DB_MEDIA_TYPE_IDX),
        MetadataUpdateItem(DB_MD5, DB_MD5_IDX),
        MetadataUpdateItem(DB_CRC, DB_CRC_IDX),
        MetadataUpdateItem(DB_FILESIZE, DB_FILE_SIZE_IDX),
        MetadataUpdateItem(DB_DATECREATE, DB_DATE_CREATE_IDX),
        MetadataUpdateItem(DB_DATEMODIFIED, DB_DATE_MODIFIED_IDX),
        MetadataUpdateItem(DB_DATEADDED, DB_DATE_ADDED_IDX),
        MetadataUpdateItem(DB_DESCRIPTION, DB_DESCRIPTION_IDX),
	/// Media Properties
        MetadataUpdateItem(DB_WIDTH, DB_WIDTH_IDX),
        MetadataUpdateItem(DB_HEIGHT, DB_HEIGHT_IDX),
        MetadataUpdateItem(DB_RESOLUTION, DB_RESOLUTION_IDX),
        MetadataUpdateItem(DB_DEPTH, DB_DEPTH_IDX),
        MetadataUpdateItem(DB_VIEWROTATION, DB_VIEW_ROTATION_IDX),
        MetadataUpdateItem(DB_SAMPLECOLOR, DB_SAMPLE_COLOR_IDX),
        MetadataUpdateItem(DB_PAGE, DB_PAGE_IDX),
        MetadataUpdateItem(DB_COLORSPACE, DB_COLOR_SPACE_IDX),
        MetadataUpdateItem(DB_COMPRESSION, DB_COMPRESSION_IDX),
        MetadataUpdateItem(DB_PRIMARYENCODING, DB_PRIMARY_ENCODING_IDX),

	/// Camerai Information"
        MetadataUpdateItem(DB_MAKER, DB_MAKER_IDX),
        MetadataUpdateItem(DB_MODEL, DB_MODEL_IDX),
        MetadataUpdateItem(DB_SOFTWARE, DB_SOFTWARE_IDX),
        MetadataUpdateItem(DB_SOURCEURL, DB_SOURCE_URL_IDX),
        MetadataUpdateItem(DB_EXIFVERSION, DB_EXIF_VERSION_IDX),
        MetadataUpdateItem(DB_CAPTUREDATE, DB_CAPTURE_DATE_IDX),
        MetadataUpdateItem(DB_EXPOSUREPROGRAM, DB_EXPOSURE_PROGRAM_IDX),
        MetadataUpdateItem(DB_ISOSPEEDRATING, DB_ISO_SPEED_RATING_IDX),
        MetadataUpdateItem(DB_EXPOSUREBIAS, DB_EXPOSURE_BIAS_IDX),
        MetadataUpdateItem(DB_EXPOSURETIME, DB_EXPOSURE_TIME_IDX),
        MetadataUpdateItem(DB_APERTURE, DB_APERTURE_IDX),
        MetadataUpdateItem(DB_METERINGMODE, DB_METERING_MODE_IDX),
        MetadataUpdateItem(DB_LIGHTSOURCE, DB_LIGHT_SOURCE_IDX),
        MetadataUpdateItem(DB_FLASH, DB_FLASH_IDX),
        MetadataUpdateItem(DB_FOCALLENGTH, DB_FOCAL_LENGTH_IDX),
        MetadataUpdateItem(DB_SENSINGMETHOD, DB_SENSING_METHOD_IDX),
        MetadataUpdateItem(DB_DIGITALZOOM, DB_DIGITAL_ZOOM_IDX ),
	/// GPS
        MetadataUpdateItem(DB_LATITUDE, DB_LATITUDE_IDX),
        MetadataUpdateItem(DB_LONGITUDE, DB_LONGITUDE_IDX),
        MetadataUpdateItem(DB_GPSTIMESTAMP, DB_GPS_TIMESTAMP_IDX),
	//Copyright Properties
        MetadataUpdateItem(DB_COPYRIGHT, DB_COPYRIGHT_IDX),
        MetadataUpdateItem(DB_USAGERIGHTS, DB_USAGE_RIGHTS_IDX),
        MetadataUpdateItem(DB_COPYRIGHTURL, DB_COPYRIGHT_URL_IDX)
};


MetadataUpdate::MetadataUpdate() {
	// TODO Auto-generated constructor stub

}

MetadataUpdate::~MetadataUpdate() {
	// TODO Auto-generated destructor stub
}

bool MetadataUpdate::process(const char *imagePath, const char *imageName, const char *cmd) {
	std::string cmdString = cmd;
	if (cmdString[0] != '[' || cmdString[cmdString.length()-1] != ']') {
		return false;
	}

	std::string tmp = imagePath;
	std::string fullImagePath = tmp.substr(0, 4);
	fullImagePath += '/';
	fullImagePath += imagePath;

	std::string fullPath = m_archivePath;
	fullPath += '/';
	fullPath += fullImagePath;
	fullPath += "/.metadata";
	CSVDatabase &db = CSVDatabase::get();
	std::auto_ptr<MetadataObject> mo = db.get(imageName, fullPath.c_str());
	if (!mo.get()) {
		return false;
	}
	cmdString = cmdString.substr(1, cmdString.length()-2);
	CSVArgs csvArgs(';');
	csvArgs.process(cmdString.c_str());
	for (std::vector<std::string>::iterator i = csvArgs.begin(); i != csvArgs.end(); i++) {
		std::string &data = *i;
		//std::cout << "Arg: " << data.c_str() << '\n';
		if (command(data, *mo) == false) {
			return false;
		}
	}
	db.put(imageName, fullPath.c_str(), *mo);
	return true;
}

int MetadataUpdate::command(std::string &cmdarg, MetadataObject &mo) {
	unsigned int idx = cmdarg.find_first_of('=');
	if (idx == std::string::npos) {
		return false;
	}
	std::string cmd = cmdarg.substr(0, idx);
	std::string arg = cmdarg.substr(idx +1, cmdarg.length() - (idx + 1));
	if (arg[0] != '"' || arg[arg.length()-1] != '"') {
		return false;
	}
	arg = arg.substr(1, arg.length()-2);
	std::cout << "Cmd: " << cmd.c_str() << " Arg: " << arg.c_str() << '\n';
	MetadataUpdateItem *updateItem = 0;;
	for (int i = 0; i < sizeof(commandList)/sizeof(MetadataUpdateItem); i++) {
		if (cmd.compare(commandList[i].getName()) == 0) {
			updateItem = &(commandList[i]);
			break;
		}
	}
	unsigned int item = updateItem->idx();
	std::string &moValue = mo.getValueAt(item);
	moValue = arg;
	return true;
}

std::auto_ptr<MetadataDocument> MetadataUpdate::get(const char *imagePath, const char *imageName) {
	std::string tmp = imagePath;
	std::string fullImagePath = tmp.substr(0, 4);
	fullImagePath += '/';
	fullImagePath += imagePath;

	std::string fullPath = m_archivePath;
	fullPath += '/';
	fullPath += fullImagePath;
	fullPath += "/.metadata";
	CSVDatabase &db = CSVDatabase::get();
	std::auto_ptr<MetadataObject> mo = db.get(imageName, fullPath.c_str());
	if (!mo.get()) {
		//return false;
	}
	std::auto_ptr<MetadataDocument> metadataDocument(new MetadataDocument);
	metadataDocument->setImageName(imageName);
	metadataDocument->setImagePath(imagePath);
	loadMetadataDocument(*metadataDocument, *mo);
	return metadataDocument;
}

bool MetadataUpdate::loadMetadataDocument(MetadataDocument &metadataDocument, MetadataObject &metadataObject) {
	unsigned len = metadataObject.getListLenght();

	for (unsigned int i = 0; i < len; i++) {
		std::string line = metadataObject.getLabelAt(i);
		line += ':';
		line += metadataObject.getValueAt(i);
		metadataDocument.push_back(line);
	}
	return true;
}
} /* namespace simplearchive */
