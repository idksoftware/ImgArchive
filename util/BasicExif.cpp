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
#include <sstream>
#include "BasicExif.h"
#include "SimpleExifReader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

BasicExif::BasicExif() : MTRow(new BasicExifSchema) {
	m_exifFound = false;
}
	
/*
#define BE_DESCRIPTION_IDX		0
#define BE_WIDTH_IDX			1
#define BE_HEIGHT_IDX			2
#define BE_VIEWROTATION_IDX		3
#define BE_MAKER_IDX			4
#define BE_MODEL_IDX			5
#define BE_EXIFVERSION_IDX		6
#define BE_CAPTUREDATE_IDX		7
#define BE_ISOSPEEDRATING_IDX	8
#define BE_EXPOSUREBIAS_IDX		9
#define BE_EXPOSURETIME_IDX		10
#define BE_APERTURE_IDX			11
#define BE_METERINGMODE_IDX		12
#define BE_FOCALLENGTH_IDX		13
#define BE_LATITUDE_IDX			14
#define BE_LONGITUDE_IDX		15
#define BE_COPYRIGHT_IDX		16
*/

BasicExif::~BasicExif()
{
	//printf("+++ Deleting BasicExif +++\n");
}

std::string FloatToString(float fNumber)
{
	std::stringstream os;
	os << fNumber;
	return os.str();
}

std::string IntToString(float fNumber)
{
	std::stringstream os;
	os << fNumber;
	return os.str();
}

void BasicExif::copyBasicEXIF(EXIFInfo &exifInfo) {

	columnAt(BE_MAKER_IDX) = exifInfo.Make;
	columnAt(BE_MODEL_IDX) = exifInfo.Model;
	columnAt(BE_EXIFVERSION_IDX) = exifInfo.Software;
	columnAt(BE_WIDTH_IDX) = exifInfo.ImageWidth;
	columnAt(BE_HEIGHT_IDX) = exifInfo.ImageHeight;
	columnAt(BE_DESCRIPTION_IDX) = exifInfo.ImageDescription;
	columnAt(BE_VIEWROTATION_IDX) = IntToString(exifInfo.Orientation);
	columnAt(BE_COPYRIGHT_IDX) = exifInfo.copyright;
	ExifDateTime dateTimeDigitized(exifInfo.DateTimeDigitized.c_str());
	columnAt(BE_CAPTUREDATE_IDX) = dateTimeDigitized;
	columnAt(BE_EXPOSURETIME_IDX) = FloatToString((float)exifInfo.ExposureTime);
	columnAt(BE_APERTURE_IDX) = FloatToString((float)exifInfo.FNumber);
	columnAt(BE_ISOSPEEDRATING_IDX) = IntToString(exifInfo.ISOSpeedRatings);
	columnAt(BE_EXPOSUREBIAS_IDX) = FloatToString((float)exifInfo.ExposureBiasValue);
	columnAt(BE_METERINGMODE_IDX) = IntToString(exifInfo.MeteringMode);
	columnAt(BE_FOCALLENGTH_IDX) = FloatToString((float)exifInfo.FocalLength);
	m_exifFound = true;
}

unsigned long BasicExif::getCrc() const {
	return columnAt(BE_CRC_IDX).getULong();
}

void BasicExif::setCrc(unsigned long crc) {
	columnAt(BE_CRC_IDX) = crc;
}

unsigned long BasicExif::getSize() const {
	return columnAt(BE_FILESIZE_IDX).getULong();
}

void BasicExif::setSize(unsigned long size) {
	columnAt(BE_FILESIZE_IDX) = size;
}

const std::string& BasicExif::getMd5() const {
	return columnAt(BE_MD5_IDX).getString();
}

void BasicExif::setMd5(const std::string& md5) {
	columnAt(BE_MD5_IDX).fromString(md5);
}

const std::string& BasicExif::getMediaType() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setMediaType(const std::string& mediaType) {
	columnAt(BE_MEDIATYPE_IDX) = mediaType.c_str();
}

const std::string& BasicExif::getUuid() const {
	return columnAt(BE_UUID_IDX).getString();
}

void BasicExif::setUuid(const std::string& uuid) {
	columnAt(BE_UUID_IDX) = uuid.c_str();
}

const std::string& BasicExif::getLabel() const {
	return columnAt(BE_LABEL_IDX).getString();
}

void BasicExif::setLabel(const std::string& name) {
	columnAt(BE_LABEL_IDX) = name.c_str();
}

const std::string& BasicExif::getName() const {
	return columnAt(BE_FILENAME_IDX).getString();
}

void BasicExif::setName(const std::string& name) {
	columnAt(BE_FILENAME_IDX) = name.c_str();
}

const std::string& BasicExif::getOrginalName() const {
	return columnAt(BE_ORGINALNAME_IDX).getString();
}

void BasicExif::setOrginalName(const std::string& name) {
	columnAt(BE_ORGINALNAME_IDX) = name.c_str();
}

const std::string& BasicExif::getPath() const {
	return columnAt(BE_FILEPATH_IDX).getString();
}

void BasicExif::setPath(const std::string& path) {
	columnAt(BE_FILEPATH_IDX) = path.c_str();
}

const int BasicExif::getRating() const {
	return columnAt(BE_RATING_IDX).getInt();
}
void BasicExif::setRating(const int rating) {
	columnAt(BE_RATING_IDX) = rating;
}

const std::string BasicExif::getExt() const {
	return m_path.substr(m_path.find_last_of(".") + 1);
}

void BasicExif::setCreateTime(ExifDateTime &time) {
	columnAt(BE_DATECREATE_IDX) = time;
}

const ExifDateTime &BasicExif::getCreateTime() const {
	return columnAt(BE_DATECREATE_IDX).getDate();
}

const ExifDateTime &BasicExif::getModTime() const {
	return columnAt(BE_DATEMODIFIED_IDX).getDate();
}

void BasicExif::setModTime(ExifDateTime &time) {
	columnAt(BE_DATEMODIFIED_IDX) = time;
}

const std::string& BasicExif::getAltitude() const {
	return ""; // columnAt(BE_ALTITUDE_IDX).toString();
}

void BasicExif::setAltitude(const std::string& altitude) {
	m_altitude = altitude;
}

//const std::string& BasicExif::getBitsPerSample() const {
//	return m_bitsPerSample;
//}

//void BasicExif::setBitsPerSample(const std::string& bitsPerSample) {
//	m_bitsPerSample = bitsPerSample;
//}

//const std::string& BasicExif::getCopyright() const {
//	return m_copyright;
//}

void BasicExif::setCopyright(const std::string& copyright) {
	columnAt(BE_COPYRIGHT_IDX) = copyright.c_str();
}

//const std::string& BasicExif::getDateTime() const {
//	return m_dateTime;
//}

//void BasicExif::setDateTime(const std::string& dateTime) {
//	m_dateTime = dateTime;
//}

const ExifDateTime &BasicExif::getDateTimeDigitized() const {
	return columnAt(BE_CAPTUREDATE_IDX).getDate();
}

void BasicExif::setDateTimeDigitized(ExifDateTime& dateTimeDigitized) {
	columnAt(BE_CAPTUREDATE_IDX) = dateTimeDigitized;
}

//const std::string& BasicExif::getDateTimeOriginal() const {
//	return m_dateTimeOriginal;
//}

//void BasicExif::setDateTimeOriginal(const std::string& dateTimeOriginal) {
//	m_dateTimeOriginal = dateTimeOriginal;
//}

const std::string& BasicExif::getExposureBiasValue() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setExposureBiasValue(const std::string& exposureBiasValue) {
	m_exposureBiasValue = exposureBiasValue;
}

const std::string& BasicExif::getExposureTime() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setExposureTime(const std::string& exposureTime) {
	m_exposureTime = exposureTime;
}

const std::string& BasicExif::getFlash() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setFlash(const std::string& flash) {
	m_flash = flash;
}

const std::string& BasicExif::getFNumber() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setFNumber(const std::string& fNumber) {
	m_fNumber = fNumber;
}

const std::string& BasicExif::getFocalLength() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setFocalLength(const std::string& focalLength) {
	m_focalLength = focalLength;
}

//const std::string& BasicExif::getFocalLengthIn35mm() const {
//	return m_focalLengthIn35mm;
//}

//void BasicExif::setFocalLengthIn35mm(const std::string& focalLengthIn35mm) {
//	m_focalLengthIn35mm = focalLengthIn35mm;
//}

const std::string& BasicExif::getImageDescription() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setImageDescription(const std::string& imageDescription) {
	m_imageDescription = imageDescription;
}

const std::string& BasicExif::getImageHeight() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setImageHeight(const std::string& imageHeight) {
	m_imageHeight = imageHeight;
}

const std::string& BasicExif::getImageWidth() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setImageWidth(const std::string& imageWidth) {
	m_imageWidth = imageWidth;
}

const std::string& BasicExif::getISoSpeedRatings() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setISoSpeedRatings(const std::string& iSoSpeedRatings) {
	m_iSOSpeedRatings = iSoSpeedRatings;
}

const std::string& BasicExif::getLatitude() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setLatitude(const std::string& latitude) {
	m_latitude = latitude;
}

const std::string& BasicExif::getLongitude() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setLongitude(const std::string& longitude) {
	m_longitude = longitude;
}

const std::string& BasicExif::getMake() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setMake(const std::string& make) {
	m_make = make;
}

const std::string& BasicExif::getMeteringMode() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setMeteringMode(const std::string& meteringMode) {
	m_meteringMode = meteringMode;
}

const std::string& BasicExif::getModel() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setModel(const std::string& model) {
	m_model = model;
}

const std::string& BasicExif::getOrientation() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setOrientation(const std::string& orientation) {
	m_orientation = orientation;
}

//const std::string& BasicExif::getShutterSpeedValue() const {
//	return m_shutterSpeedValue;
//}

//void BasicExif::setShutterSpeedValue(const std::string& shutterSpeedValue) {
//	m_shutterSpeedValue = shutterSpeedValue;
//}

const std::string& BasicExif::getSoftware() const {
	return columnAt(BE_MEDIATYPE_IDX).getString();
}

void BasicExif::setSoftware(const std::string& software) {
	m_software = software;
}

//const std::string& BasicExif::getSubjectDistance() const {
//	return m_subjectDistance;
//}

//void BasicExif::setSubjectDistance(const std::string& subjectDistance) {
//	m_subjectDistance = subjectDistance;
//}

//const std::string& BasicExif::getSubSecTimeOriginal() const {
//	return m_subSecTimeOriginal;
//}

//void BasicExif::setSubSecTimeOriginal(const std::string& subSecTimeOriginal) {
//	m_subSecTimeOriginal = subSecTimeOriginal;
//}


} /* namespace simplearchive */
