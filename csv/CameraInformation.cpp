/*
 * CameraInformation.cpp
 *
 *  Created on: Jul 9, 2014
 *      Author: wzw7yn
 */

#include "MetadataObject.h"
#include "CameraInformation.h"

namespace simplearchive {

class CameraInformationString : public CSVString {
public:
	CameraInformationString() {};
	virtual ~CameraInformationString() {};
	virtual const char *toString(CSVRowItem &item);

};

const char *CameraInformationString::toString(CSVRowItem &item) {
	CameraInformationItem &tmp = (CameraInformationItem &)item;
	return tmp.toString();
}


CameraInformationItem::CameraInformationItem(int id) : CSVRowItem(id) { }

CameraInformationItem::~CameraInformationItem() {}

const char *CameraInformationItem::toString() {
	start();
	add(m_maker);
	add(m_model);
	add(m_software);
	add(m_sourceURL);
	add(m_exifVersion);
	add(m_captureDate);
	add(m_exposureProgram);
	add(m_isoSpeedRating);
	add(m_exposureBias);
	add(m_exposureTime);
	add(m_aperture);
	add(m_meteringMode);
	add(m_lightSource);
	add(m_flash);
	add(m_focalLength);
	add(m_sensingMethod);
	add(m_digitalZoom);
	end();
	return row().c_str();
}

bool CameraInformationItem::fromString(int pos, std::string &str) {
	m_maker = token(&pos, str);
	pos++;
	m_model = token(&pos, str);
	pos++;
	m_software = token(&pos, str);
	pos++;
	m_sourceURL = token(&pos, str);
	pos++;
	m_exifVersion = token(&pos, str);
	pos++;
	m_captureDate = token(&pos, str);
	pos++;
	m_exposureProgram = token(&pos, str);
	pos++;
	m_isoSpeedRating = token(&pos, str);
	pos++;
	m_exposureBias = token(&pos, str);
	pos++;
	m_exposureTime = token(&pos, str);
	pos++;
	m_aperture = token(&pos, str);
	pos++;
	m_meteringMode = token(&pos, str);
	pos++;
	m_lightSource = token(&pos, str);
	pos++;
	m_flash = token(&pos, str);
	pos++;
	m_focalLength = token(&pos, str);
	pos++;
	m_sensingMethod = token(&pos, str);
	pos++;
	m_digitalZoom = token(&pos, str);
	return true;
}

CameraInformation::CameraInformation() {}

CameraInformation::~CameraInformation() {}

// Adds the information of the side car file from a MetadataObject.
void CameraInformation::add(MetadataObject &metadataObject) {
	const char *idStr = metadataObject.getSequenceId().c_str();
	int seqId = atol(idStr);
	CameraInformationItem *item = new CameraInformationItem(seqId);
	item->setMaker(metadataObject.getMaker());
	item->setModel(metadataObject.getModel());
	//item->setSourceUrl(metadataObject.getSourceUrl());
	item->setExifVersion(metadataObject.getExifVersion());
	item->setCaptureDate(metadataObject.getCaptureDate());
	item->setExposureProgram(metadataObject.getExposureProgram());
	item->setIsoSpeedRating(metadataObject.getIsoSpeedRating());
	item->setExposureBias(metadataObject.getExposureBias());
	item->setAperture(metadataObject.getAperture());
	item->setMeteringMode(metadataObject.getMeteringMode());
	item->setLightSource(metadataObject.getLightSource());
	item->setFlash(metadataObject.getFlash());
	item->setFocalLength(metadataObject.getFocalLength());
	item->setSensingMethod(metadataObject.getSensingMethod());
	item->setDigitalZoom(metadataObject.getDigitalZoom());
	push_back(item);
	return;
}

/// reads a csv side car file
bool CameraInformation::write(const char *path) {
	std::string filename("CameraInformation.csv");
	CameraInformationString cov;
	CSVRow::write(path, filename.c_str(), cov);

	return true;
}

bool CameraInformation::add(const char *row) {
	std::string tmp = row;
	int pos = 0;
	int seqId = readId(tmp, &pos);
	CameraInformationItem *item = new CameraInformationItem(seqId);
	item->fromString(pos+1, tmp);
	push_back(item);
	return true;
}


/// reads a csv side car file
bool CameraInformation::read(const char *datafile) {
	std::string path(datafile);
	path += "/CameraInformation.csv";
	if (CSVRow::read(path.c_str()) == false) {
		return false;
	}
	return true;
}

const std::string& CameraInformationItem::getAperture() const {
	return m_aperture;
}

void CameraInformationItem::setAperture(const std::string& aperture) {
	m_aperture = aperture;
}

const std::string& CameraInformationItem::getCaptureDate() const {
	return m_captureDate;
}

void CameraInformationItem::setCaptureDate(const std::string& captureDate) {
	m_captureDate = captureDate;
}

const std::string& CameraInformationItem::getDigitalZoom() const {
	return m_digitalZoom;
}

void CameraInformationItem::setDigitalZoom(const std::string& digitalZoom) {
	m_digitalZoom = digitalZoom;
}

const std::string& CameraInformationItem::getExifVersion() const {
	return m_exifVersion;
}

void CameraInformationItem::setExifVersion(const std::string& exifVersion) {
	m_exifVersion = exifVersion;
}

const std::string& CameraInformationItem::getExposureBias() const {
	return m_exposureBias;
}

void CameraInformationItem::setExposureBias(const std::string& exposureBias) {
	m_exposureBias = exposureBias;
}

const std::string& CameraInformationItem::getExposureProgram() const {
	return m_exposureProgram;
}

void CameraInformationItem::setExposureProgram(
		const std::string& exposureProgram) {
	m_exposureProgram = exposureProgram;
}

const std::string& CameraInformationItem::getExposureTime() const {
	return m_exposureTime;
}

void CameraInformationItem::setExposureTime(const std::string& exposureTime) {
	m_exposureTime = exposureTime;
}

const std::string& CameraInformationItem::getFlash() const {
	return m_flash;
}

void CameraInformationItem::setFlash(const std::string& flash) {
	m_flash = flash;
}

const std::string& CameraInformationItem::getFocalLength() const {
	return m_focalLength;
}

void CameraInformationItem::setFocalLength(const std::string& focalLength) {
	m_focalLength = focalLength;
}

const std::string& CameraInformationItem::getIsoSpeedRating() const {
	return m_isoSpeedRating;
}

void CameraInformationItem::setIsoSpeedRating(
		const std::string& isoSpeedRating) {
	m_isoSpeedRating = isoSpeedRating;
}

const std::string& CameraInformationItem::getLightSource() const {
	return m_lightSource;
}

void CameraInformationItem::setLightSource(const std::string& lightSource) {
	m_lightSource = lightSource;
}

const std::string& CameraInformationItem::getMaker() const {
	return m_maker;
}

void CameraInformationItem::setMaker(const std::string& maker) {
	m_maker = maker;
}

const std::string& CameraInformationItem::getMeteringMode() const {
	return m_meteringMode;
}

void CameraInformationItem::setMeteringMode(const std::string& meteringMode) {
	m_meteringMode = meteringMode;
}

const std::string& CameraInformationItem::getModel() const {
	return m_model;
}

void CameraInformationItem::setModel(const std::string& model) {
	m_model = model;
}

const std::string& CameraInformationItem::getSensingMethod() const {
	return m_sensingMethod;
}

void CameraInformationItem::setSensingMethod(const std::string& sensingMethod) {
	m_sensingMethod = sensingMethod;
}

const std::string& CameraInformationItem::getSequenceId() const {
	return m_sequenceId;
}

void CameraInformationItem::setSequenceId(const std::string& sequenceId) {
	m_sequenceId = sequenceId;
}

const std::string& CameraInformationItem::getSoftware() const {
	return m_software;
}

void CameraInformationItem::setSoftware(const std::string& software) {
	m_software = software;
}

const std::string& CameraInformationItem::getSourceUrl() const {
	return m_sourceURL;
}

} /* namespace simplearchive */
