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

#include "BasicExif.h"

namespace simplearchive {

BasicExif::BasicExif()
{
}

const std::string& BasicExif::getAltitude() const {
	return m_altitude;
}

void BasicExif::setAltitude(const std::string& altitude) {
	m_altitude = altitude;
}

const std::string& BasicExif::getBitsPerSample() const {
	return m_bitsPerSample;
}

void BasicExif::setBitsPerSample(const std::string& bitsPerSample) {
	m_bitsPerSample = bitsPerSample;
}

const std::string& BasicExif::getCopyright() const {
	return m_copyright;
}

void BasicExif::setCopyright(const std::string& copyright) {
	m_copyright = copyright;
}

const std::string& BasicExif::getDateTime() const {
	return m_dateTime;
}

void BasicExif::setDateTime(const std::string& dateTime) {
	m_dateTime = dateTime;
}

const std::string& BasicExif::getDateTimeDigitized() const {
	return m_dateTimeDigitized;
}

void BasicExif::setDateTimeDigitized(const std::string& dateTimeDigitized) {
	m_dateTimeDigitized = dateTimeDigitized;
}

const std::string& BasicExif::getDateTimeOriginal() const {
	return m_dateTimeOriginal;
}

void BasicExif::setDateTimeOriginal(const std::string& dateTimeOriginal) {
	m_dateTimeOriginal = dateTimeOriginal;
}

const std::string& BasicExif::getExposureBiasValue() const {
	return m_exposureBiasValue;
}

void BasicExif::setExposureBiasValue(const std::string& exposureBiasValue) {
	m_exposureBiasValue = exposureBiasValue;
}

const std::string& BasicExif::getExposureTime() const {
	return m_exposureTime;
}

void BasicExif::setExposureTime(const std::string& exposureTime) {
	m_exposureTime = exposureTime;
}

const std::string& BasicExif::getFlash() const {
	return m_flash;
}

void BasicExif::setFlash(const std::string& flash) {
	m_flash = flash;
}

const std::string& BasicExif::getFNumber() const {
	return m_fNumber;
}

void BasicExif::setFNumber(const std::string& fNumber) {
	m_fNumber = fNumber;
}

const std::string& BasicExif::getFocalLength() const {
	return m_focalLength;
}

void BasicExif::setFocalLength(const std::string& focalLength) {
	m_focalLength = focalLength;
}

const std::string& BasicExif::getFocalLengthIn35mm() const {
	return m_focalLengthIn35mm;
}

void BasicExif::setFocalLengthIn35mm(const std::string& focalLengthIn35mm) {
	m_focalLengthIn35mm = focalLengthIn35mm;
}

const std::string& BasicExif::getImageDescription() const {
	return m_imageDescription;
}

void BasicExif::setImageDescription(const std::string& imageDescription) {
	m_imageDescription = imageDescription;
}

const std::string& BasicExif::getImageHeight() const {
	return m_imageHeight;
}

void BasicExif::setImageHeight(const std::string& imageHeight) {
	m_imageHeight = imageHeight;
}

const std::string& BasicExif::getImageWidth() const {
	return m_imageWidth;
}

void BasicExif::setImageWidth(const std::string& imageWidth) {
	m_imageWidth = imageWidth;
}

const std::string& BasicExif::getISoSpeedRatings() const {
	return m_iSOSpeedRatings;
}

void BasicExif::setISoSpeedRatings(const std::string& iSoSpeedRatings) {
	m_iSOSpeedRatings = iSoSpeedRatings;
}

const std::string& BasicExif::getLatitude() const {
	return m_latitude;
}

void BasicExif::setLatitude(const std::string& latitude) {
	m_latitude = latitude;
}

const std::string& BasicExif::getLongitude() const {
	return m_longitude;
}

void BasicExif::setLongitude(const std::string& longitude) {
	m_longitude = longitude;
}

const std::string& BasicExif::getMake() const {
	return m_make;
}

void BasicExif::setMake(const std::string& make) {
	m_make = make;
}

const std::string& BasicExif::getMeteringMode() const {
	return m_meteringMode;
}

void BasicExif::setMeteringMode(const std::string& meteringMode) {
	m_meteringMode = meteringMode;
}

const std::string& BasicExif::getModel() const {
	return m_model;
}

void BasicExif::setModel(const std::string& model) {
	m_model = model;
}

const std::string& BasicExif::getOrientation() const {
	return m_orientation;
}

void BasicExif::setOrientation(const std::string& orientation) {
	m_orientation = orientation;
}

const std::string& BasicExif::getShutterSpeedValue() const {
	return m_shutterSpeedValue;
}

void BasicExif::setShutterSpeedValue(const std::string& shutterSpeedValue) {
	m_shutterSpeedValue = shutterSpeedValue;
}

const std::string& BasicExif::getSoftware() const {
	return m_software;
}

void BasicExif::setSoftware(const std::string& software) {
	m_software = software;
}

const std::string& BasicExif::getSubjectDistance() const {
	return m_subjectDistance;
}

void BasicExif::setSubjectDistance(const std::string& subjectDistance) {
	m_subjectDistance = subjectDistance;
}

const std::string& BasicExif::getSubSecTimeOriginal() const {
	return m_subSecTimeOriginal;
}

void BasicExif::setSubSecTimeOriginal(const std::string& subSecTimeOriginal) {
	m_subSecTimeOriginal = subSecTimeOriginal;
}

BasicExif::~BasicExif()
{
}
} /* namespace simplearchive */