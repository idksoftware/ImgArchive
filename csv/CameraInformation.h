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

#ifndef CAMERAINFORMATION_H_
#define CAMERAINFORMATION_H_
#include <string>
#include <vector>
#include <cstdlib>
#include "CSVRow.h"

namespace simplearchive {
class MetadataObject;
class CameraInformationItem : public CSVRowItem {
	std::string m_sequenceId;
	std::string m_maker;
	std::string m_model;
	std::string m_software;
	std::string m_sourceURL;
	std::string m_exifVersion;
	std::string m_captureDate;
	std::string m_exposureProgram;
	std::string m_isoSpeedRating;
	std::string m_exposureBias;
	std::string m_exposureTime;
	std::string m_aperture;
	std::string m_meteringMode;
	std::string m_lightSource;
	std::string m_flash;
	std::string m_focalLength;
	std::string m_sensingMethod;
	std::string m_digitalZoom;
public:
	CameraInformationItem(int id);
	virtual ~CameraInformationItem();
	const std::string& getAperture() const;
	void setAperture(const std::string& aperture);
	const std::string& getCaptureDate() const;
	void setCaptureDate(const std::string& captureDate);
	const std::string& getDigitalZoom() const;
	void setDigitalZoom(const std::string& digitalZoom);
	const std::string& getExifVersion() const;
	void setExifVersion(const std::string& exifVersion);
	const std::string& getExposureBias() const;
	void setExposureBias(const std::string& exposureBias);
	const std::string& getExposureProgram() const;
	void setExposureProgram(const std::string& exposureProgram);
	const std::string& getExposureTime() const;
	void setExposureTime(const std::string& exposureTime);
	const std::string& getFlash() const;
	void setFlash(const std::string& flash);
	const std::string& getFocalLength() const;
	void setFocalLength(const std::string& focalLength);
	const std::string& getIsoSpeedRating() const;
	void setIsoSpeedRating(const std::string& isoSpeedRating);
	const std::string& getLightSource() const;
	void setLightSource(const std::string& lightSource);
	const std::string& getMaker() const;
	void setMaker(const std::string& maker);
	const std::string& getMeteringMode() const;
	void setMeteringMode(const std::string& meteringMode);
	const std::string& getModel() const;
	void setModel(const std::string& model);
	const std::string& getSensingMethod() const;
	void setSensingMethod(const std::string& sensingMethod);
	const std::string& getSequenceId() const;
	void setSequenceId(const std::string& sequenceId);
	const std::string& getSoftware() const;
	void setSoftware(const std::string& software);
	const std::string& getSourceUrl() const;
	void setSourceUrl(const std::string& sourceUrl);

	virtual const char *toString();
	virtual bool fromString(int pos, std::string &str);
};

class MetadataObject;
class CameraInformation : public CSVRow {
public:
	CameraInformation();
	virtual ~CameraInformation();

	/// Adds the information of the GPS properties from a MetadataObject.
	void add(MetadataObject &metadataObject);

	/// Adds a GPSPropertiesItem object.
	void add(CameraInformationItem &item);

	/// reads a csv GPSProperties file
	bool read(const char *datafile);

	/// writes a csv GPSProperties file
	bool write(const char *path);

	unsigned int findImage(const char *text, int col);
	bool load(unsigned int row, MetadataObject &mo);
	bool save(unsigned int row, MetadataObject &mo);
	//bool write(const char *path, const char *filename, CSVString& u) {
	//	return CSVRow::write(path, filename, u);
	//}

	virtual bool add(const char *row);
};

} /* namespace simplearchive */
#endif /* CAMERAINFORMATION_H_ */
