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
#include "ImageTable.h"
#include "Database.h"
#include "sqlite3.h"
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include "SQLBuilder.h"
#include "MetaType.h"


using namespace simplearchive;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

ImageTable::ImageTable(Database &db) {
	m_errcode = SQLITE_OK;
	m_database = &db;
	m_dbfile = db.getDbfile();
}

ImageTable::~ImageTable() {
	// TODO Auto-generated destructor stub
}

static int callback(void *notUsed, int argc, char **argv, char **colName) {
	return 0;
}

bool ImageTable::createFromSchema(MTTableSchema &mtSchema) {

	std::string name = mtSchema.getName().c_str();
	SQLCreatBulder createBulder(name.c_str());
	createBulder.addfield("autoIncrement", "integer", "primary key", "autoincrement");
	//for (int i = 0; i < mtSchema. i++) {
	//	mtSchema.
	//	createBulder.addfield("text");
	//}
	for (auto i = mtSchema.begin(); i != mtSchema.end(); i++) {
		MTSchema& columnInfo = *i;
		createBulder.addfield(columnInfo.getName().c_str(), columnInfo.getTypeString());
	}
	createBulder.addfield(DB_UUID, "text");
	createBulder.addfield(DB_FILENAME, "text");
	

	printf("creating table metadata");
	if (!m_database->execute(createBulder.toString().c_str())) {
		return false;
	}
	return true;
}


bool ImageTable::create() {


	SQLCreatBulder createBulder(TABLE_METADATA);
	createBulder.addfield(DB_SEQUENCEID, "integer", "primary key", "autoincrement");
	createBulder.addfield(DB_UUID, "text");
	createBulder.addfield(DB_FILENAME, "text");
	createBulder.addfield(DB_CRC, "text");
	createBulder.addfield(DB_MD5, "text");
	createBulder.addfield(DB_ORGINALNAME, "text");
	createBulder.addfield(DB_APERTURE, "text");
	createBulder.addfield(DB_CAPTUREDATE, "text");
	createBulder.addfield(DB_COLORSPACE, "text");
	createBulder.addfield(DB_COMPRESSION, "text");
	createBulder.addfield(DB_COPYRIGHT, "text");
	createBulder.addfield(DB_COPYRIGHTURL, "text");
	createBulder.addfield(DB_DATEADDED, "text");
	createBulder.addfield(DB_DATECREATE, "text");
	createBulder.addfield(DB_DATEMODIFIED, "text");
	//createBulder.addfield(DB_DEPTH, "text");
	createBulder.addfield(DB_DIGITALZOOM, "text");
	createBulder.addfield(DB_EXIFVERSION, "text");
	createBulder.addfield(DB_EXPOSUREBIAS, "text");
	createBulder.addfield(DB_EXPOSUREPROGRAM, "text");
	createBulder.addfield(DB_EXPOSURETIME, "text");
	createBulder.addfield(DB_FILEPATH, "text");
	createBulder.addfield(DB_FILESIZE, "text");
	createBulder.addfield(DB_FLASH, "text");
	createBulder.addfield(DB_FOCALLENGTH, "text");
	createBulder.addfield(DB_GPSTIMESTAMP, "text");
	createBulder.addfield(DB_HEIGHT, "text");
	createBulder.addfield(DB_ISOSPEEDRATING, "text");
	createBulder.addfield(DB_LABEL, "text");
	createBulder.addfield(DB_LATITUDE, "text");
	createBulder.addfield(DB_LIGHTSOURCE, "text");
	createBulder.addfield(DB_LONGITUDE, "text");
	createBulder.addfield(DB_MAKER, "text");
	createBulder.addfield(DB_MEDIATYPE, "text");
	createBulder.addfield(DB_METERINGMODE, "text");
	createBulder.addfield(DB_MODEL, "text");
//	createBulder.addfield(DB_PAGE, "text");
//	createBulder.addfield(DB_PRIMARYENCODING, "text");
	createBulder.addfield(DB_RATING, "text");
	createBulder.addfield(DB_RESOLUTION, "text");
	createBulder.addfield(DB_SAMPLECOLOR, "text");
	createBulder.addfield(DB_SENSINGMETHOD, "text");
	createBulder.addfield(DB_SOFTWARE, "text");
	createBulder.addfield(DB_SOURCEURL, "text");
	createBulder.addfield(DB_USAGERIGHTS, "text");
	createBulder.addfield(DB_VIEWROTATION, "text");
	createBulder.addfield(DB_WIDTH, "text");

	printf("creating table metadata");
	if (!m_database->execute(createBulder.toString().c_str())) {
		return false;
	}
	return true;
}


bool ImageTable::insert(ImageTableItem &imageTableItem) {

	SQLInsertBulder insertBulder(TABLE_METADATA);
	insertBulder.addfield(DB_FILENAME, imageTableItem.getFilename());
	insertBulder.addfield(DB_CRC, imageTableItem.getCrc());
	insertBulder.addfield(DB_FILENAME, imageTableItem.getFilename());
	insertBulder.addfield(DB_MD5, imageTableItem.getMd5());
	insertBulder.addfield(DB_ORGINALNAME, imageTableItem.getOrginalName());
	insertBulder.addfield(DB_UUID, imageTableItem.getUuid());
	insertBulder.addfield(DB_APERTURE, imageTableItem.getAperture());
	insertBulder.addfield(DB_CAPTUREDATE, imageTableItem.getCaptureDate());
	insertBulder.addfield(DB_COLORSPACE, imageTableItem.getColorSpace());
	insertBulder.addfield(DB_COMPRESSION, imageTableItem.getCompression());
	insertBulder.addfield(DB_COPYRIGHT, imageTableItem.getCopyright());
	insertBulder.addfield(DB_COPYRIGHTURL, imageTableItem.getCopyrightUrl());
	insertBulder.addfield(DB_DATEADDED, imageTableItem.getDateAdded());
	insertBulder.addfield(DB_DATECREATE, imageTableItem.getDateCreate());
	insertBulder.addfield(DB_DATEMODIFIED, imageTableItem.getDateModified());

	//insertBulder.addfield(DB_DEPTH, imageTableItem.getDepth());
	insertBulder.addfield(DB_DIGITALZOOM, imageTableItem.getDigitalZoom());
	insertBulder.addfield(DB_EXIFVERSION, imageTableItem.getExifVersion());
	insertBulder.addfield(DB_EXPOSUREBIAS, imageTableItem.getExposureBias());
	insertBulder.addfield(DB_EXPOSUREPROGRAM, imageTableItem.getExposureProgram());
	insertBulder.addfield(DB_EXPOSURETIME, imageTableItem.getExposureTime());
	insertBulder.addfield(DB_FILEPATH, imageTableItem.getFilepath());

	insertBulder.addfield(DB_FILESIZE, imageTableItem.getFileSize());
	insertBulder.addfield(DB_FLASH, imageTableItem.getFlash());
	insertBulder.addfield(DB_FOCALLENGTH, imageTableItem.getFocalLength());
	insertBulder.addfield(DB_GPSTIMESTAMP, imageTableItem.getGpsTimeStamp());
	insertBulder.addfield(DB_HEIGHT, imageTableItem.getHeight());
	insertBulder.addfield(DB_ISOSPEEDRATING, imageTableItem.getIsoSpeedRating());
	insertBulder.addfield(DB_LABEL, imageTableItem.getLabel());
	insertBulder.addfield(DB_LATITUDE, imageTableItem.getLatitude());
	insertBulder.addfield(DB_LIGHTSOURCE, imageTableItem.getLightSource());
	insertBulder.addfield(DB_LONGITUDE, imageTableItem.getLongitude());
	insertBulder.addfield(DB_MAKER, imageTableItem.getMaker());
	insertBulder.addfield(DB_MEDIATYPE, imageTableItem.getMediaType());
	insertBulder.addfield(DB_METERINGMODE, imageTableItem.getMeteringMode());
	insertBulder.addfield(DB_MODEL, imageTableItem.getModel());
//	insertBulder.addfield(DB_PAGE, imageTableItem.getPage());
//	insertBulder.addfield(DB_PRIMARYENCODING, imageTableItem.getPrimaryEncoding());
	insertBulder.addfield(DB_RATING, imageTableItem.getRating());
	insertBulder.addfield(DB_RESOLUTION, imageTableItem.getResolution());
	insertBulder.addfield(DB_SAMPLECOLOR, imageTableItem.getSampleColor());
	insertBulder.addfield(DB_SENSINGMETHOD, imageTableItem.getSensingMethod());
	insertBulder.addfield(DB_SOFTWARE, imageTableItem.getSoftware());
	insertBulder.addfield(DB_SOURCEURL, imageTableItem.getSourceUrl());
	insertBulder.addfield(DB_USAGERIGHTS, imageTableItem.getUsageRights());
	insertBulder.addfield(DB_VIEWROTATION, imageTableItem.getViewRotation());
	insertBulder.addfield(DB_WIDTH, imageTableItem.getWidth());

	//printf("%s", insertBulder.toString().c_str());


	if (!m_database->execute(insertBulder.toString().c_str())) {
		printf("Error: %s. Details: %s", m_database->getError(), m_database->getDetailedError());
		return false;
	}
	return true;

}

bool ImageTable::update(ImageTableItem &imageTableItem) {

	SQLUpdateBuilder updateBuilder(TABLE_METADATA);
	updateBuilder.addfield(DB_FILENAME, imageTableItem.getFilename());
	updateBuilder.addfield(DB_CRC, imageTableItem.getCrc());
	updateBuilder.addfield(DB_FILENAME, imageTableItem.getFilename());
	updateBuilder.addfield(DB_MD5, imageTableItem.getMd5());
	updateBuilder.addfield(DB_ORGINALNAME, imageTableItem.getOrginalName());
	updateBuilder.addfield(DB_UUID, imageTableItem.getUuid());
	updateBuilder.addfield(DB_APERTURE, imageTableItem.getAperture());
	updateBuilder.addfield(DB_CAPTUREDATE, imageTableItem.getCaptureDate());
	updateBuilder.addfield(DB_COLORSPACE, imageTableItem.getColorSpace());
	updateBuilder.addfield(DB_COMPRESSION, imageTableItem.getCompression());
	updateBuilder.addfield(DB_COPYRIGHT, imageTableItem.getCopyright());
	updateBuilder.addfield(DB_COPYRIGHTURL, imageTableItem.getCopyrightUrl());
	updateBuilder.addfield(DB_DATEADDED, imageTableItem.getDateAdded());
	updateBuilder.addfield(DB_DATECREATE, imageTableItem.getDateCreate());
	updateBuilder.addfield(DB_DATEMODIFIED, imageTableItem.getDateModified());

//	updateBuilder.addfield(DB_DEPTH, imageTableItem.getDepth());
	updateBuilder.addfield(DB_DIGITALZOOM, imageTableItem.getDigitalZoom());
	updateBuilder.addfield(DB_EXIFVERSION, imageTableItem.getExifVersion());
	updateBuilder.addfield(DB_EXPOSUREBIAS, imageTableItem.getExposureBias());
	updateBuilder.addfield(DB_EXPOSUREPROGRAM, imageTableItem.getExposureProgram());
	updateBuilder.addfield(DB_EXPOSURETIME, imageTableItem.getExposureTime());
	updateBuilder.addfield(DB_FILEPATH, imageTableItem.getFilepath());

	updateBuilder.addfield(DB_FILESIZE, imageTableItem.getFileSize());
	updateBuilder.addfield(DB_FLASH, imageTableItem.getFlash());
	updateBuilder.addfield(DB_FOCALLENGTH, imageTableItem.getFocalLength());
	updateBuilder.addfield(DB_GPSTIMESTAMP, imageTableItem.getGpsTimeStamp());
	updateBuilder.addfield(DB_HEIGHT, imageTableItem.getHeight());
	updateBuilder.addfield(DB_ISOSPEEDRATING, imageTableItem.getIsoSpeedRating());
	updateBuilder.addfield(DB_LABEL, imageTableItem.getLabel());
	updateBuilder.addfield(DB_LATITUDE, imageTableItem.getLatitude());
	updateBuilder.addfield(DB_LIGHTSOURCE, imageTableItem.getLightSource());
	updateBuilder.addfield(DB_LONGITUDE, imageTableItem.getLongitude());
	updateBuilder.addfield(DB_MAKER, imageTableItem.getMaker());
	updateBuilder.addfield(DB_MEDIATYPE, imageTableItem.getMediaType());
	updateBuilder.addfield(DB_METERINGMODE, imageTableItem.getMeteringMode());
	updateBuilder.addfield(DB_MODEL, imageTableItem.getModel());
//	updateBuilder.addfield(DB_PAGE, imageTableItem.getPage());
//	updateBuilder.addfield(DB_PRIMARYENCODING, imageTableItem.getPrimaryEncoding());
	updateBuilder.addfield(DB_RATING, imageTableItem.getRating());
	updateBuilder.addfield(DB_RESOLUTION, imageTableItem.getResolution());
	updateBuilder.addfield(DB_SAMPLECOLOR, imageTableItem.getSampleColor());
	updateBuilder.addfield(DB_SENSINGMETHOD, imageTableItem.getSensingMethod());
	updateBuilder.addfield(DB_SOFTWARE, imageTableItem.getSoftware());
	updateBuilder.addfield(DB_SOURCEURL, imageTableItem.getSourceUrl());
	updateBuilder.addfield(DB_USAGERIGHTS, imageTableItem.getUsageRights());
	updateBuilder.addfield(DB_VIEWROTATION, imageTableItem.getViewRotation());
	updateBuilder.addfield(DB_WIDTH, imageTableItem.getWidth());

	if (!m_database->execute(updateBuilder.toString().c_str())) {
		printf("Error: %s. Details: %s", m_database->getError(), m_database->getDetailedError());
		return false;
	}
	return true;

}


std::string ImageTable::writeTag(const char *tag, const std::string& value) {
	std::ostringstream xml;
	if (!value.empty()) {
		xml << "<" << tag << ">" << value << "</" << tag << ">\n";
	} else {
		xml << "<" << tag << "/>\n";
	}
	return xml.str();
}

bool ImageTable::writeXmlFile(ImageTableItem &imageTableItem, const char *path) {

	std::ofstream xmlFile;
	xmlFile.open(path);
	xmlFile <<	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	            <<	"<Metadata>\n"
	            <<	"<AssetProperties>\n"
	            << writeTag("SequenceId", imageTableItem.getFilename())
	            << writeTag("Filename", imageTableItem.getFilename())
	            << writeTag("Filepath", imageTableItem.getFilepath())
	            << writeTag("OrginalName", imageTableItem.getOrginalName())
	            << writeTag("UniqueId", imageTableItem.getUuid())
	            << writeTag("Label", imageTableItem.getLabel())
	            << writeTag("Rating", imageTableItem.getRating())
	            << writeTag("MediaType", imageTableItem.getMediaType())
	            << writeTag("Md5", imageTableItem.getMd5())
	            << writeTag("Crc", imageTableItem.getCrc())
	            << writeTag("FileSize", imageTableItem.getFileSize())
	            << writeTag("DateCreate", imageTableItem.getDateCreate())
	            << writeTag("DateModified", imageTableItem.getDateModified())
	            << writeTag("DateAdded", imageTableItem.getDateAdded())
	            << "</AssetProperties>\n"
	            << "<MediaProerties>\n"
	            << writeTag("Width", imageTableItem.getWidth())
	            << writeTag("Height", imageTableItem.getHeight())
	            << writeTag("Resolution", imageTableItem.getResolution())
	            << writeTag("Depth", imageTableItem.getDepth())
	            << writeTag("ViewRotation", imageTableItem.getViewRotation())
	            << writeTag("SampleColor", imageTableItem.getSampleColor())
	            << writeTag("Page", imageTableItem.getPage())
	            << writeTag("ColorSpace", imageTableItem.getColorSpace())
	            << writeTag("Compression", imageTableItem.getCompression())
	            << writeTag("PrimaryEncoding", imageTableItem.getPrimaryEncoding())
	            << "<MediaProerties>\n"
	            << "<CameraInformation>\n"
	            << writeTag("Maker", imageTableItem.getMaker())
	            << writeTag("Model", imageTableItem.getModel())
	            << writeTag("Software", imageTableItem.getSoftware())
	            << writeTag("SourceURL", imageTableItem.getSourceUrl())
	            << writeTag("ExifVersion", imageTableItem.getExifVersion())
	            << writeTag("CaptureDate", imageTableItem.getCaptureDate())
	            << writeTag("ExposureProgram", imageTableItem.getExposureProgram())
	            << writeTag("ISOSpeedRating", imageTableItem.getIsoSpeedRating())
	            << writeTag("ExposureBias", imageTableItem.getExposureBias())
	            << writeTag("ExposureTime", imageTableItem.getExposureTime())
	            << writeTag("Aperture", imageTableItem.getAperture())
	            << writeTag("MeteringMode", imageTableItem.getMeteringMode())
	            << writeTag("LightSource", imageTableItem.getLightSource())
	            << writeTag("Flash", imageTableItem.getFlash())
	            << writeTag("FocalLength", imageTableItem.getFocalLength())
	            << writeTag("SensingMethod", imageTableItem.getSensingMethod())
	            << writeTag("DigitalZoom", imageTableItem.getDigitalZoom())
	            << "</CameraInformation>\n"
	            << "<GPS>\n"
	            << writeTag("Latitude", imageTableItem.getLatitude())
	            << writeTag("Longitude", imageTableItem.getLongitude())
	            << writeTag("GPSTimeStamp", imageTableItem.getGpsTimeStamp())
	            << "</GPS>\n"
	            << "<CopyrightProperties>\n"
	            << writeTag("Copyright", imageTableItem.getCopyright())
	            << writeTag("UsageRights", imageTableItem.getUsageRights())
	            << writeTag("CopyrightURL", imageTableItem.getCopyrightUrl())
	            << "</Metadata>\n";


	xmlFile.close();

	return true;

}

//} /* namespace simplearchive */

/*
 std::string m_uuid;
	std::string m_md5;
	std::string m_crc;
	std::string m_filename; // This includes full path.
	std::string m_orginalName;
	std::string m_checkedOutName;
	std::string m_date;
 */
/*
bool ImageTable::insert(ImageTableItem &imageTableItem) {
	char *zErr;
	std::string sql1 = "insert into images( ";
	std::string sql2 = ") values (";
	std::string sql3 = ");";
	const std::string& uuid = imageTableItem.getUuid();
	if (!uuid.empty()) {

		sql1 += "uuid, ";

		sql2 += "'";
		sql2 += uuid; sql2 += "', '";
	}
	const std::string& md5 = imageTableItem.getMd5();
	if (!md5.empty()) {
		sql1 += "md5, ";
		sql2 += md5; sql2 += "', '";

	}
	const std::string& crc = imageTableItem.getCrc();
	if (!crc.empty()) {
		sql1 += "crc, ";
		sql2 += crc; sql2 += "', '";
	}
	const std::string& filename = imageTableItem.getFilename();
	if (!filename.empty()) {
		sql1 += "filename, ";
		sql2 += filename; sql2 += "', '";

	}
	const std::string& orginalName = imageTableItem.getOrginalName();
	if (!orginalName.empty()) {
		sql1 += "orginal_name, ";
		sql2 += orginalName; sql2 += "', '";
	}
	const std::string& checkedOutName = imageTableItem.getCheckedOutName();
	if (!checkedOutName.empty()) {
		sql1 += "checked_out_name, ";
		sql2 += checkedOutName; sql2 += "', '";
	}
	const std::string& date = imageTableItem.getDateAdded();
	if (!date.empty()) {
		sql1 += "date";
		sql2 += date; sql2 += "'";
	}
	std::string sql = sql1 + sql2 + sql3;
	printf("%s\n", sql.c_str());
	m_errcode = sqlite3_exec((sqlite3 *)m_dbfile, sql.c_str(), callback, 0, &zErr);
	if (m_errcode == SQLITE_OK) {
		return true;
	}
}
*/

//}; //Namespace