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

#ifndef METADATAOBJECT_H_
#define METADATAOBJECT_H_

#include <string>
#include <vector>
#include "ExifDateTime.h"

#include <map>

namespace simplearchive {
/**
 * @brief Metadata Container
 *
 * This contains the metadata to be substituted during the template process.
 * As each template is read the contents will be placed into this class. This class
 * will be used for further substitutions by the Exif reading and user defined
 * keyword substitutions.
 */
	class MetadataObject {
		//std::map<std::string, std::string *> m_lookup;

	friend class MetadataTemplate;
	/// Asset Properties
	std::string m_sequenceId;
	std::string m_filename;
	std::string m_filepath;
	std::string m_orginalName;
	std::string m_uniqueId;
	std::string m_label;
	std::string m_rating;
	std::string m_mediaType;
	std::string m_md5;
	std::string m_crc;
	std::string m_fileSize;
	std::string m_dateCreate;
	std::string m_dateModified;
	std::string m_dateAdded;
	std::string m_description;
	/// Media Properties
	std::string m_width;
	std::string m_height;
	std::string m_resolution;
	std::string m_depth;
	std::string m_viewRotation;
	std::string m_sampleColor;
	std::string m_page;
	std::string m_colorSpace;
	std::string m_compression;
	std::string m_primaryEncoding;
	/// Camerai Information"
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
	/// GPS
	std::string m_latitude;
	std::string m_longitude;
	std::string m_gpsTimeStamp;
	//Copyright Properties
	std::string m_copyright;
	std::string m_usageRights;
	std::string m_copyrightURL;

	std::vector<std::string *> m_valueList;
	static const char *m_labelList[];
	bool m_doMakeValueList;

	void makeValueList();
	void makeLabelsList();

public:
	MetadataObject();
	virtual ~MetadataObject();

	const char *getLabelAt(int idx) {
		return m_labelList[idx];
	}

	const char *&getLabelList() {
		return *m_labelList;
	}

	static unsigned int getListLenght();

    std::string& getValueAt(int idx) {
		makeValueList();
		return *(m_valueList.at(idx));
	}

	std::vector<std::string *>& getAsValueList() {
		makeValueList();
		return m_valueList;
	}


	const std::string& getAperture() const {
		return m_aperture;
	}

	void setAperture(const std::string& aperture) {
		m_aperture = aperture;
	}

	const std::string& getCaptureDate() const {
		return m_captureDate;
	}

	void setCaptureDate(const std::string& captureDate) {
		m_captureDate = captureDate;
	}

	const std::string& getColorSpace() const {
		return m_colorSpace;
	}

	void setColorSpace(const std::string& colorSpace) {
		m_colorSpace = colorSpace;
	}

	const std::string& getCompression() const {
		return m_compression;
	}

	void setCompression(const std::string& compression) {
		m_compression = compression;
	}

	const std::string& getCopyright() const {
		return m_copyright;
	}

	void setCopyright(const std::string& copyright) {
		m_copyright = copyright;
	}

	const std::string& getCopyrightUrl() const {
		return m_copyrightURL;
	}

	void setCopyrightUrl(const std::string& copyrightUrl) {
		m_copyrightURL = copyrightUrl;
	}

	const std::string& getCrc() const {
		return m_crc;
	}

	void setCrc(const std::string& crc) {
		m_crc = crc;
	}

	const std::string& getDateAdded() const {
		return m_dateAdded;
	}

	void setDateAdded(const std::string& dateAdded) {
		m_dateAdded = dateAdded;
	}

	const std::string& getDateCreate() const {
		return m_dateCreate;
	}

	void setDateCreate(const std::string& dateCreate) {
		m_dateCreate = dateCreate;
	}

	const std::string& getDateModified() const {
		return m_dateModified;
	}

	void setDateModified(const std::string& dateModified) {
		m_dateModified = dateModified;
	}
	
	const std::string& getDepth() const {
		return m_depth;
	}

	void setDepth(const std::string& depth) {
		m_depth = depth;
	}

	const std::string& getDescription() const {
		return m_description;
	}

	void setDescription(const std::string& description) {
		m_description = description;
	}

	const std::string& getDigitalZoom() const {
		return m_digitalZoom;
	}

	void setDigitalZoom(const std::string& digitalZoom) {
		m_digitalZoom = digitalZoom;
	}

	const std::string& getExifVersion() const {
		return m_exifVersion;
	}

	void setExifVersion(const std::string& exifVersion) {
		m_exifVersion = exifVersion;
	}

	const std::string& getExposureBias() const {
		return m_exposureBias;
	}

	void setExposureBias(const std::string& exposureBias) {
		m_exposureBias = exposureBias;
	}

	const std::string& getExposureProgram() const {
		return m_exposureProgram;
	}

	void setExposureProgram(const std::string& exposureProgram) {
		m_exposureProgram = exposureProgram;
	}

	const std::string& getExposureTime() const {
		return m_exposureTime;
	}

	void setExposureTime(const std::string& exposureTime) {
		m_exposureTime = exposureTime;
	}

	const std::string& getFilename() const {
		return m_filename;
	}

	void setFilename(const std::string& filename) {
		m_filename = filename;
	}

	const std::string& getFilepath() const {
		return m_filepath;
	}

	void setFilepath(const std::string& filepath) {
		m_filepath = filepath;
	}

	const std::string& getFileSize() const {
		return m_fileSize;
	}

	void setFileSize(const std::string& fileSize) {
		m_fileSize = fileSize;
	}
	
	const std::string& getFlash() const {
		return m_flash;
	}

	void setFlash(const std::string& flash) {
		m_flash = flash;
	}

	const std::string& getFocalLength() const {
		return m_focalLength;
	}

	void setFocalLength(const std::string& focalLength) {
		m_focalLength = focalLength;
	}

	const std::string& getGpsTimeStamp() const {
		return m_gpsTimeStamp;
	}

	void setGpsTimeStamp(const std::string& gpsTimeStamp) {
		m_gpsTimeStamp = gpsTimeStamp;
	}

	const std::string& getHeight() const {
		return m_height;
	}

	void setHeight(const std::string& height) {
		m_height = height;
	}

	const std::string& getIsoSpeedRating() const {
		return m_isoSpeedRating;
	}

	void setIsoSpeedRating(const std::string& isoSpeedRating) {
		m_isoSpeedRating = isoSpeedRating;
	}

	const std::string& getLabel() const {
		return m_label;
	}

	void setLabel(const std::string& label) {
		m_label = label;
	}

	const std::string& getLatitude() const {
		return m_latitude;
	}

	void setLatitude(const std::string& latitude) {
		m_latitude = latitude;
	}

	const std::string& getLightSource() const {
		return m_lightSource;
	}

	void setLightSource(const std::string& lightSource) {
		m_lightSource = lightSource;
	}

	const std::string& getLongitude() const {
		return m_longitude;
	}

	void setLongitude(const std::string& longitude) {
		m_longitude = longitude;
	}

	const std::string& getMaker() const {
		return m_maker;
	}

	void setMaker(const std::string& maker) {
		m_maker = maker;
	}

	const std::string& getMd5() const {
		return m_md5;
	}

	void setMd5(const std::string& md5) {
		m_md5 = md5;
	}

	const std::string& getMediaType() const {
		return m_mediaType;
	}

	void setMediaType(const std::string& mediaType) {
		m_mediaType = mediaType;
	}

	const std::string& getMeteringMode() const {
		return m_meteringMode;
	}

	void setMeteringMode(const std::string& meteringMode) {
		m_meteringMode = meteringMode;
	}

	const std::string& getModel() const {
		return m_model;
	}

	void setModel(const std::string& model) {
		m_model = model;
	}

	const std::string& getOrginalName() const {
		return m_orginalName;
	}

	void setOrginalName(const std::string& orginalName) {
		m_orginalName = orginalName;
	}

	const std::string& getPage() const {
		return m_page;
	}

	void setPage(const std::string& page) {
		m_page = page;
	}

	const std::string& getPrinaryEncoding() const {
		return m_primaryEncoding;
	}

	void setPrimaryEncoding(const std::string& primaryEncoding) {
		m_primaryEncoding = primaryEncoding;
	}

	const std::string& getRating() const {
		return m_rating;
	}

	void setRating(const std::string& rating) {
		m_rating = rating;
	}

	const std::string& getResolution() const {
		return m_resolution;
	}

	void setResolution(const std::string& resolution) {
		m_resolution = resolution;
	}

	const std::string& getSampleColor() const {
		return m_sampleColor;
	}

	void setSampleColor(const std::string& sampleColor) {
		m_sampleColor = sampleColor;
	}

	const std::string& getSensingMethod() const {
		return m_sensingMethod;
	}

	void setSensingMethod(const std::string& sensingMethod) {
		m_sensingMethod = sensingMethod;
	}

	const std::string& getSequenceId() const {
		return m_sequenceId;
	}

	const int getSequenceNumber() const {
		return std::stoi(m_sequenceId);
	}

	void setSequenceId(const std::string& sequenceId) {
		m_sequenceId = sequenceId;
	}

	const std::string& getSoftware() const {
		return m_software;
	}

	void setSoftware(const std::string& software) {
		m_software = software;
	}

	const std::string& getSourceUrl() const {
		return m_sourceURL;
	}

	void setSourceUrl(const std::string& sourceUrl) {
		m_sourceURL = sourceUrl;
	}

	const std::string& getUniqueId() const {
		return m_uniqueId;
	}

	void setUniqueId(const std::string& uniqueId) {
		m_uniqueId = uniqueId;
	}

	const std::string& getUsageRights() const {
		return m_usageRights;
	}

	void setUsageRights(const std::string& usageRights) {
		m_usageRights = usageRights;
	}

	const std::string& getViewRotation() const {
		return m_viewRotation;
	}

	void setViewRotation(const std::string& viewRotation) {
		m_viewRotation = viewRotation;
	}

	const std::string& getWidth() const {
		return m_width;
	}

	void setWidth(const std::string& width) {
		m_width = width;
	}
};

} /* namespace simplearchive */
#endif /* METADATAOBJECT_H_ */
