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


class BasicExif
{
	std::string  m_make;
	std::string  m_model;
	std::string  m_software;
	std::string  m_bitsPerSample;
	std::string  m_imageWidth;
	std::string  m_imageHeight;
	std::string  m_imageDescription;
	std::string  m_orientation;
	std::string  m_copyright;
	std::string  m_dateTime;
	std::string  m_dateTimeOriginal;
	std::string  m_dateTimeDigitized;
	std::string  m_subSecTimeOriginal;
	std::string  m_exposureTime;
	std::string  m_fNumber;
	std::string  m_iSOSpeedRatings;
	std::string  m_subjectDistance;
	std::string  m_exposureBiasValue;
	std::string  m_flash;
	std::string  m_meteringMode;
	std::string  m_focalLength;
	std::string  m_focalLengthIn35mm;
	std::string  m_shutterSpeedValue;
	std::string  m_latitude;
	std::string  m_longitude;
	std::string  m_altitude;
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
	const std::string& getAltitude() const;
	void setAltitude(const std::string& altitude);
	const std::string& getBitsPerSample() const;
	void setBitsPerSample(const std::string& bitsPerSample);
	const std::string& getCopyright() const;
	void setCopyright(const std::string& copyright);
	const std::string& getDateTime() const;
	void setDateTime(const std::string& dateTime);
	const std::string& getDateTimeDigitized() const;
	void setDateTimeDigitized(const std::string& dateTimeDigitized);
	const std::string& getDateTimeOriginal() const;
	void setDateTimeOriginal(const std::string& dateTimeOriginal);
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
};

