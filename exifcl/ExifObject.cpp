/*
 * ExifObject.cpp
 *
 *  Created on: May 30, 2014
 *      Author: wzw7yn
 */

#include <stdio.h>
#include "ExifObject.h"
#include "CLogger.h"

namespace simplearchive {

ExifObject::ExifObject() {
	// TODO Auto-generated constructor stub

}

ExifObject::~ExifObject() {
	// TODO Auto-generated destructor stub
}

const std::string& ExifObject::getAltitude() const {
	return m_altitude;
}

void ExifObject::setAltitude(const std::string& altitude) {
	m_altitude = altitude;
}

void ExifObject::setArtist(const std::string& artist) {
	m_artist = artist;
}

const std::string& ExifObject::getBitsPerSample() const {
	return m_bitsPerSample;
}

void ExifObject::setBitsPerSample(const std::string& bitsPerSample) {
	m_bitsPerSample = bitsPerSample;
}

void ExifObject::setColorSpace(const std::string& colorSpace) {
	m_colorSpace = colorSpace;
}

void ExifObject::setCopyright(const std::string& copyright) {
	m_copyright = copyright;
}

void ExifObject::setDateTime(const std::string& dateTime) {
	m_dateTime = dateTime;
}

const std::string& ExifObject::getDateTimeDigitized() const {
	return m_dateTimeDigitized;
}

void ExifObject::setDateTimeDigitized(const std::string& dateTimeDigitized) {
	m_dateTimeDigitized = dateTimeDigitized;
}

const std::string& ExifObject::getDateTimeOriginal() const {
	return m_dateTimeOriginal;
}

void ExifObject::setDateTimeOriginal(const std::string& dateTimeOriginal) {
	m_dateTimeOriginal = dateTimeOriginal;
}

const std::string& ExifObject::getExposureBiasValue() const {
	return m_exposureBiasValue;
}

void ExifObject::setExposureBiasValue(const std::string& exposureBiasValue) {
	m_exposureBiasValue = exposureBiasValue;
}

const std::string& ExifObject::getExposureTime() const {
	return m_exposureTime;
}

void ExifObject::setExposureTime(const std::string& exposureTime) {
	m_exposureTime = exposureTime;
}

const std::string& ExifObject::getFlash() const {
	return m_flash;
}

void ExifObject::setFlash(const std::string& flash) {
	m_flash = flash;
}

const std::string& ExifObject::getFNumber() const {
	return m_fNumber;
}

void ExifObject::setFNumber(const std::string& fNumber) {
	m_fNumber = fNumber;
}

const std::string& ExifObject::getFocalLength() const {
	return m_focalLength;
}

void ExifObject::setFocalLength(const std::string& focalLength) {
	m_focalLength = focalLength;
}

const std::string& ExifObject::getFocalLengthIn35mm() const {
	return m_focalLengthIn35mm;
}

void ExifObject::setFocalLengthIn35mm(const std::string& focalLengthIn35mm) {
	m_focalLengthIn35mm = focalLengthIn35mm;
}

const std::string& ExifObject::getImageDescription() const {
	return m_imageDescription;
}

void ExifObject::setImageDescription(const std::string& imageDescription) {
	m_imageDescription = imageDescription;
}

void ExifObject::setImageHeight(const std::string& imageHeight) {
	m_imageHeight = imageHeight;
}

void ExifObject::setImageWidth(const std::string& imageWidth) {
	m_imageWidth = imageWidth;
}

const std::string& ExifObject::getIsoSpeedRatings() const {
	return m_isoSpeedRatings;
}

void ExifObject::setIsoSpeedRatings(const std::string& isoSpeedRatings) {
	m_isoSpeedRatings = isoSpeedRatings;
}

const std::string& ExifObject::getLatitude() const {
	return m_latitude;
}

void ExifObject::setLatitude(const std::string& latitude) {
	m_latitude = latitude;
}

const std::string& ExifObject::getLongitude() const {
	return m_longitude;
}

void ExifObject::setLongitude(const std::string& longitude) {
	m_longitude = longitude;
}

const std::string& ExifObject::getMake() const {
	return m_make;
}

void ExifObject::setMake(const std::string& make) {
	m_make = make;
}

const std::string& ExifObject::getMeteringMode() const {
	return m_meteringMode;
}

void ExifObject::setMeteringMode(const std::string& meteringMode) {
	m_meteringMode = meteringMode;
}

const std::string& ExifObject::getModel() const {
	return m_model;
}

void ExifObject::setModel(const std::string& model) {
	m_model = model;
}

const std::string& ExifObject::getOrientation() const {
	return m_orientation;
}

void ExifObject::setOrientation(const std::string& orientation) {
	m_orientation = orientation;
}

const std::string& ExifObject::getShutterSpeedValue() const {
	return m_shutterSpeedValue;
}

void ExifObject::setShutterSpeedValue(const std::string& shutterSpeedValue) {
	m_shutterSpeedValue = shutterSpeedValue;
}

const std::string& ExifObject::getSoftware() const {
	return m_software;
}

void ExifObject::setSoftware(const std::string& software) {
	m_software = software;
}

const std::string& ExifObject::getSubjectDistance() const {
	return m_subjectDistance;
}

void ExifObject::setSubjectDistance(const std::string& subjectDistance) {
	m_subjectDistance = subjectDistance;
}

const std::string& ExifObject::getSubSecTimeOriginal() const {
	return m_subSecTimeOriginal;
}

void ExifObject::setSubSecTimeOriginal(const std::string& subSecTimeOriginal) {
	m_subSecTimeOriginal = subSecTimeOriginal;
}

void ExifObject::setXResolution(const std::string& xResolution) {
	m_xResolution = xResolution;
}

void ExifObject::setYResolution(const std::string& yResolution) {
	m_yResolution = yResolution;
}

void ExifObject::print() {
	CLogger &logger = CLogger::getLogger();
	logger.log(CLogger::FINE, "Using Exif Color Space: \"%s\"", m_colorSpace.c_str());
	logger.log(CLogger::FINE, "Using Exif Image Width: \"%s\"", m_imageWidth.c_str());
	logger.log(CLogger::FINE, "Using Exif Image Height: \"%s\"", m_imageHeight.c_str());
	logger.log(CLogger::FINE, "Using Exif Date Time: \"%s\"", m_dateTime.c_str());
	logger.log(CLogger::FINE, "Using Exif Copyright: \"%s\"", m_copyright.c_str());
	logger.log(CLogger::FINE, "Using Exif Artist: \"%s\"", m_artist.c_str());
	logger.log(CLogger::FINE, "Using Exif X Resolution: \"%s\"", m_xResolution.c_str());
	logger.log(CLogger::FINE, "Using Exif Y Resolution: \"%s\"", m_yResolution.c_str());
}

} /* namespace simplearchive */
