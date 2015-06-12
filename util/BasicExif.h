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


#pragma once
#include <string>
#include "MetaType.h"
#include "DBDefines.h"

class EXIFInfo;

namespace simplearchive {

/*
#define DB_SEQUENCEID	   "SequenceId"
#define DB_MD5	           "Md5"
#define DB_CRC             "Crc"
#define DB_FILENAME        "Filename"
#define DB_ORGINALNAME     "OrginalName"
#define DB_UUID            "Uuid"
#define DB_DESCRIPTION	   "Description"
#define DB_APERTURE        "Aperture"
#define DB_CAPTUREDATE     "CaptureDate"
#define DB_COLORSPACE      "ColorSpace"
#define DB_COMPRESSION     "Compression"
#define DB_COPYRIGHT       "Copyright"
#define DB_COPYRIGHTURL    "CopyrightUrl"
#define DB_DATEADDED       "DateAdded"
#define DB_DATECREATE      "DateCreate"
#define DB_DATEMODIFIED    "DateModified"
#define DB_DEPTH           "Depth"
#define DB_DIGITALZOOM     "DigitalZoom"
#define DB_EXIFVERSION     "ExifVersion"
#define DB_EXPOSUREBIAS    "ExposureBias"
#define DB_EXPOSUREPROGRAM "ExposureProgram"
#define DB_EXPOSURETIME    "ExposureTime"
#define DB_FILEPATH        "Filepath"
#define DB_FILESIZE        "FileSize"

*/



class BasicExifSchema : public MTTableSchema {
public:
	BasicExifSchema() : MTTableSchema(ROW_BASIC_EXIF) {

		add(MTSchema(MTSchema::Text, DB_FILENAME));
		add(MTSchema(MTSchema::Text, DB_ORGINALNAME));
		add(MTSchema(MTSchema::Text, DB_FILEPATH));
		add(MTSchema(MTSchema::Integer, DB_CRC));
		add(MTSchema(MTSchema::Text, DB_MD5));
		add(MTSchema(MTSchema::Text, DB_UUID));
		add(MTSchema(MTSchema::Text, DB_LABEL));
		add(MTSchema(MTSchema::Text, DB_MEDIATYPE));
		add(MTSchema(MTSchema::Integer, DB_RATING));
		add(MTSchema(MTSchema::Integer, DB_FILESIZE));
		add(MTSchema(MTSchema::Date, DB_DATEMODIFIED));
		add(MTSchema(MTSchema::Date, DB_DATECREATE));
		
		add(MTSchema(MTSchema::Text, DB_DESCRIPTION));
		add(MTSchema(MTSchema::Integer, DB_WIDTH));
		add(MTSchema(MTSchema::Integer, DB_HEIGHT));
		//add(MTSchema(MTSchema::Text, DB_RESOLUTION));
		//add(MTSchema(MTSchema::Text, DB_DEPTH));
		add(MTSchema(MTSchema::Text, DB_VIEWROTATION));
		//add(MTSchema(MTSchema::Text, DB_SAMPLECOLOR));
		//add(MTSchema(MTSchema::Text, DB_PAGE));
		//add(MTSchema(MTSchema::Text, DB_COLORSPACE));
		//add(MTSchema(MTSchema::Text, DB_COMPRESSION));
		//add(MTSchema(MTSchema::Text, DB_PRIMARYENCODING));
		add(MTSchema(MTSchema::Text, DB_MAKER));
		add(MTSchema(MTSchema::Text, DB_MODEL));
		//add(MTSchema(MTSchema::Text, DB_SOFTWARE));
		//add(MTSchema(MTSchema::Text, DB_SOURCEURL));
		add(MTSchema(MTSchema::Text, DB_EXIFVERSION));
		add(MTSchema(MTSchema::Date, DB_CAPTUREDATE));
		//add(MTSchema(MTSchema::Text, DB_EXPOSUREPROGRAM));
		add(MTSchema(MTSchema::Text, DB_ISOSPEEDRATING));
		add(MTSchema(MTSchema::Text, DB_EXPOSUREBIAS));
		add(MTSchema(MTSchema::Text, DB_EXPOSURETIME));
		add(MTSchema(MTSchema::Text, DB_APERTURE));
		add(MTSchema(MTSchema::Text, DB_METERINGMODE));
		//add(MTSchema(MTSchema::Text, DB_LIGHTSOURCE));
		//add(MTSchema(MTSchema::Text, DB_FLASH));
		add(MTSchema(MTSchema::Text, DB_FOCALLENGTH));
		//add(MTSchema(MTSchema::Text, DB_SENSINGMETHOD));
		//add(MTSchema(MTSchema::Text, DB_DIGITALZOOM));
		add(MTSchema(MTSchema::Text, DB_LATITUDE));
		add(MTSchema(MTSchema::Text, DB_LONGITUDE));
		//add(MTSchema(MTSchema::Text, DB_GPSTIMESTAMP));
		add(MTSchema(MTSchema::Text, DB_COPYRIGHT));
		add(MTSchema(MTSchema::Text, DB_KEYWORDS));
		//add(MTSchema(MTSchema::Text, DB_USAGERIGHTS));
		//add(MTSchema(MTSchema::Text, DB_COPYRIGHTURL));
	}
};



class BasicExif : public MTRow {
	
	friend class EXIFInfo;

	std::string m_name;
	std::string m_orginalName;
	std::string m_path;
	//unsigned long m_crc;
	std::string m_md5;
	std::string m_uuid;
	std::string m_mediaType;
	std::string m_label;
	int			m_rating;
	unsigned long m_size;
	ExifDateTime m_modTime;
	ExifDateTime m_createTime;

	std::string  m_make;
	std::string  m_model;
	std::string  m_software;
	//std::string  m_bitsPerSample;
	std::string  m_imageWidth;
	std::string  m_imageHeight;
	std::string  m_imageDescription;
	std::string  m_orientation;
	std::string  m_copyright;
	//std::string  m_dateTime;
	//std::string  m_dateTimeOriginal;
	std::string  m_dateTimeDigitized;
	//std::string  m_subSecTimeOriginal;
	std::string  m_exposureTime;
	std::string  m_fNumber;
	std::string  m_iSOSpeedRatings;
	//std::string  m_subjectDistance;
	std::string  m_exposureBiasValue;
	std::string  m_flash;
	std::string  m_meteringMode;
	std::string  m_focalLength;
	//std::string  m_focalLengthIn35mm;
	//std::string  m_shutterSpeedValue;
	std::string  m_latitude;
	std::string  m_longitude;
	std::string  m_altitude;

	bool m_exifFound;
	/*
	sprintf("%f deg (%f deg, %f min, %f sec %c)\n",

	exifInfo.GeoLocation.Latitude,
	exifInfo.GeoLocation.Latitude,
	exifInfo.GeoLocation.LatComponents.degrees,
	exifInfo.GeoLocation.LatComponents.degrees,
	exifInfo.GeoLocation.LatComponents.minutes,
	exifInfo.GeoLocation.LatComponents.minutes,
	exifInfo.GeoLocation.LatComponents.seconds,
	exifInfo.GeoLocation.LatComponents.seconds,
	exifInfo.GeoLocation.LatComponents.direction);

	exifInfo.GeoLocation.LatComponents.direction);
	//printf("GPS Longitude     : %f deg (%f deg, %f min, %f sec %c)\n",
	exifInfo.GeoLocation.Longitude,
	exifInfo.GeoLocation.LonComponents.degrees,
	exifInfo.GeoLocation.LonComponents.minutes,
	exifInfo.GeoLocation.LonComponents.seconds,
	exifInfo.GeoLocation.LonComponents.direction);

	//printf("GPS Altitude      : %f m\n", exifInfo.GeoLocation.Altitude);
	*/
public:
	BasicExif();
	~BasicExif();

	unsigned long getCrc() const;
	void setCrc(unsigned long crc);
	
	unsigned long getSize() const;
	void setSize(unsigned long crc);

	const std::string& getMd5() const;
	void setMd5(const std::string& md5);

	const std::string& getMediaType() const;
	void setMediaType(const std::string& mediaType);

	const std::string& getUuid() const;
	void setUuid(const std::string& uuid);

	const std::string& getName() const;
	void setName(const std::string& name);

	const std::string& getLabel() const;
	void setLabel(const std::string& name);

	const std::string& getOrginalName() const;
	void setOrginalName(const std::string& name);

	const std::string& getPath() const;
	void setPath(const std::string& path);

	const int getRating() const;
	void setRating(const int rating);

	const std::string getExt() const;
	void setCreateTime(ExifDateTime &time);
	const ExifDateTime &getCreateTime() const;
	void setModTime(ExifDateTime &time);
	const ExifDateTime &getModTime() const;

	const std::string& getAltitude() const;
	void setAltitude(const std::string& altitude);
	const std::string& getBitsPerSample() const;
	void setBitsPerSample(const std::string& bitsPerSample);
	const std::string& getCopyright() const;
	void setCopyright(const std::string& copyright);
//	const std::string& getDateTime() const;
//	void setDateTime(const std::string& dateTime);

	const ExifDateTime& getDateTimeDigitized() const;
	void setDateTimeDigitized(ExifDateTime& dateTimeDigitized);

//	const std::string& getDateTimeOriginal() const;
//	void setDateTimeOriginal(const std::string& dateTimeOriginal);
    const std::string& getExposureBiasValue() const;
	void setExposureBiasValue(const std::string& exposureBiasValue);
	const std::string& getExposureTime() const;
	void setExposureTime(const std::string& exposureTime);
	const std::string& getFlash() const;
	void setFlash(const std::string& flash);
	const std::string& getFNumber() const;
	void setFNumber(const std::string& fNumber);
	const std::string& getFocalLength() const;
	void setFocalLength(const std::string& focalLength);
	const std::string& getFocalLengthIn35mm() const;
	void setFocalLengthIn35mm(const std::string& focalLengthIn35mm);
	const std::string& getImageDescription() const;
	void setImageDescription(const std::string& imageDescription);
	const std::string& getImageHeight() const;
	void setImageHeight(const std::string& imageHeight);
	const std::string& getImageWidth() const;
	void setImageWidth(const std::string& imageWidth);
	const std::string& getISoSpeedRatings() const;
	void setISoSpeedRatings(const std::string& iSoSpeedRatings);
	const std::string& getLatitude() const;
	void setLatitude(const std::string& latitude);
	const std::string& getLongitude() const;
	void setLongitude(const std::string& longitude);
	const std::string& getMake() const;
	void setMake(const std::string& make);
	const std::string& getMeteringMode() const;
	void setMeteringMode(const std::string& meteringMode);
	const std::string& getModel() const;
	void setModel(const std::string& model);
	const std::string& getOrientation() const;
	void setOrientation(const std::string& orientation);
	const std::string& getShutterSpeedValue() const;
	void setShutterSpeedValue(const std::string& shutterSpeedValue);
	const std::string& getSoftware() const;
	void setSoftware(const std::string& software);
	const std::string& getSubjectDistance() const;
	void setSubjectDistance(const std::string& subjectDistance);
	const std::string& getSubSecTimeOriginal() const;
	void setSubSecTimeOriginal(const std::string& subSecTimeOriginal);

	void copyBasicEXIF(EXIFInfo &exifInfo);
	const bool isExifFound() const {
		return m_exifFound;
	}
};


} /* namespace simplearchive */
