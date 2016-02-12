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
#include <map>
#include "DBDefines.h"
#include "MetaType.h"

namespace simplearchive {



class MetadataSchema : public MTTableSchema {
public:
	MetadataSchema() : MTTableSchema("MetadataProperties") {
        	add(MTSchema(MTSchema::Integer, DB_SEQUENCEID));
        	add(MTSchema(MTSchema::Text, DB_FILENAME));
        	add(MTSchema(MTSchema::Text, DB_FILEPATH));
        	add(MTSchema(MTSchema::Text, DB_ORGINALNAME));
        	add(MTSchema(MTSchema::Text, DB_UUID));
        	add(MTSchema(MTSchema::Text, DB_LABEL));
        	add(MTSchema(MTSchema::Integer, DB_RATING));
        	add(MTSchema(MTSchema::Text, DB_MEDIATYPE));
        	add(MTSchema(MTSchema::Text, DB_MD5));
        	add(MTSchema(MTSchema::Integer, DB_CRC));
        	add(MTSchema(MTSchema::Integer, DB_FILESIZE));
        	add(MTSchema(MTSchema::Date, DB_DATECREATE));
        	add(MTSchema(MTSchema::Date, DB_DATEMODIFIED));
        	add(MTSchema(MTSchema::Date, DB_DATEADDED));
        	add(MTSchema(MTSchema::Text, DB_DESCRIPTION));
			add(MTSchema(MTSchema::Text, DB_KEYWORDS));
			add(MTSchema(MTSchema::Integer, DB_WIDTH));
			add(MTSchema(MTSchema::Integer, DB_HEIGHT));
			add(MTSchema(MTSchema::Text, DB_RESOLUTION));
			add(MTSchema(MTSchema::Text, DB_DEPTH));
			add(MTSchema(MTSchema::Text, DB_VIEWROTATION));
			add(MTSchema(MTSchema::Text, DB_SAMPLECOLOR));
			add(MTSchema(MTSchema::Text, DB_PAGE));
			add(MTSchema(MTSchema::Text, DB_COLORSPACE));
			add(MTSchema(MTSchema::Text, DB_COMPRESSION));
			add(MTSchema(MTSchema::Text, DB_PRIMARYENCODING));
			add(MTSchema(MTSchema::Text, DB_MAKER));
			add(MTSchema(MTSchema::Text, DB_MODEL));
			add(MTSchema(MTSchema::Text, DB_SOFTWARE));
			add(MTSchema(MTSchema::Text, DB_SOURCEURL));
			add(MTSchema(MTSchema::Text, DB_EXIFVERSION));
			add(MTSchema(MTSchema::Date, DB_CAPTUREDATE));
			add(MTSchema(MTSchema::Text, DB_EXPOSUREPROGRAM));
			add(MTSchema(MTSchema::Text, DB_ISOSPEEDRATING));
			add(MTSchema(MTSchema::Text, DB_EXPOSUREBIAS));
			add(MTSchema(MTSchema::Text, DB_EXPOSURETIME));
			add(MTSchema(MTSchema::Text, DB_APERTURE));
			add(MTSchema(MTSchema::Text, DB_METERINGMODE));
			add(MTSchema(MTSchema::Text, DB_LIGHTSOURCE));
			add(MTSchema(MTSchema::Text, DB_FLASH));
			add(MTSchema(MTSchema::Text, DB_FOCALLENGTH));
			add(MTSchema(MTSchema::Text, DB_SENSINGMETHOD));
			add(MTSchema(MTSchema::Text, DB_DIGITALZOOM));
			add(MTSchema(MTSchema::Text, DB_LATITUDE));
			add(MTSchema(MTSchema::Text, DB_LONGITUDE));
			add(MTSchema(MTSchema::Text, DB_GPSTIMESTAMP));
			add(MTSchema(MTSchema::Text, DB_COPYRIGHT));
			add(MTSchema(MTSchema::Text, DB_USAGERIGHTS));
			add(MTSchema(MTSchema::Text, DB_COPYRIGHTURL));
			
        }
};

/**
 * @brief Metadata Container
 *
 * This contains the metadata to be substituted during the template process.
 * As each template is read the contents will be placed into this class. This class
 * will be used for further substitutions by the Exif reading and user defined
 * keyword substitutions.
 */
class MetadataObject : public MTRow {


	friend class MetadataTemplate;
	/// Asset Properties
	/*
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
	*/




public:
	MetadataObject();
	virtual ~MetadataObject();

	//static const char *TableLookup(const char *columnName) const ;
	
	void update(const MetadataObject& mo) {
		
		if (getHeight().compare("null") == 0) {
			setHeight(mo.getHeight());
		}
		if (getWidth().compare("null") == 0) {
			setWidth(mo.getWidth());
		}
		
	}



	const std::string getAperture() const {
		return columnAt(MD_APERTURE_IDX).toString();
	}

	void setAperture(const std::string& aperture) {
		columnAt(MD_APERTURE_IDX).fromString(aperture);
	}

	const ExifDateTime& getCaptureDate() const {
		return columnAt(MD_CAPTURE_DATE_IDX).getDate();
	}

	void setCaptureDate(const std::string& captureDate) {
		columnAt(MD_CAPTURE_DATE_IDX).fromString(captureDate);;
	}

	const std::string& getColorSpace() const {
		return columnAt(MD_COLOR_SPACE_IDX).toString();

	}

	void setColorSpace(const std::string& colorSpace) {
	        columnAt(MD_COLOR_SPACE_IDX).fromString(colorSpace);;
	}

	const std::string& getCompression() const {
	        return columnAt(MD_COMPRESSION_IDX).toString();
	}

	void setCompression(const std::string& compression) {
	        columnAt(MD_COMPRESSION_IDX).fromString(compression);;
	}

	const std::string& getCopyright() const {
	        return columnAt(MD_COPYRIGHT_IDX).toString();
	}

	void setCopyright(const std::string& copyright) {
	        columnAt(MD_COPYRIGHT_IDX).fromString(copyright);;
	}

	const std::string& getCopyrightUrl() const {
	        return columnAt(MD_COPYRIGHT_URL_IDX).toString();
	}

	void setCopyrightUrl(const std::string& copyrightUrl) {
	        columnAt(MD_COPYRIGHT_URL_IDX).fromString(copyrightUrl);;
	}

	const std::string& getCrc() const {
	        return columnAt(DB_CRC_IDX).toString();
	}



	void setCrc(const std::string& crc) {
	        columnAt(DB_CRC_IDX).fromString(crc);;
	}

	const std::string& getDateAdded() const {
	        return columnAt(MD_DATE_ADDED_IDX).toString();
	}

	void setDateAdded(const std::string& dateAdded) {
	        columnAt(MD_DATE_ADDED_IDX).fromString(dateAdded);;
	}

	void setDateAdded(const ExifDateTime& dateAdded) {
		columnAt(MD_DATE_ADDED_IDX) = dateAdded;
	}

	const std::string& getDateCreate() const {
	        return columnAt(MD_DATE_CREATE_IDX).toString();
	}

	void setDateCreate(const std::string& dateCreate) {
	        columnAt(MD_DATE_CREATE_IDX).fromString(dateCreate);;
	}

	const std::string& getDateModified() const {
	        return columnAt(MD_DATE_MODIFIED_IDX).toString();
	}

	void setDateModified(const std::string& dateModified) {
	        columnAt(MD_DATE_MODIFIED_IDX).fromString(dateModified);;
	}

	const std::string& getDepth() const {
	        return columnAt(MD_DEPTH_IDX).toString();
	}

	void setDepth(const std::string& depth) {
	        columnAt(MD_DEPTH_IDX).fromString(depth);;
	}

	const std::string& getDescription() const {
	        return columnAt(MD_DESCRIPTION_IDX).toString();
	}

	void setDescription(const std::string& description) {
	        columnAt(MD_DESCRIPTION_IDX).fromString(description);;
	}

	const std::string& getDigitalZoom() const {
	        return columnAt(MD_DIGITAL_ZOOM_IDX).toString();
	}

	void setDigitalZoom(const std::string& digitalZoom) {
	        columnAt(MD_DIGITAL_ZOOM_IDX).fromString(digitalZoom);;
	}

	const std::string& getExifVersion() const {
	        return columnAt(MD_SOFTWARE_IDX).toString();
	}

	void setExifVersion(const std::string& exifVersion) {
	        columnAt(MD_SOFTWARE_IDX).fromString(exifVersion);;
	}

	const std::string& getExposureBias() const {
	        return columnAt(MD_EXPOSURE_BIAS_IDX).toString();
	}

	void setExposureBias(const std::string& exposureBias) {
	        columnAt(MD_EXPOSURE_BIAS_IDX).fromString(exposureBias);;
	}

	const std::string& getExposureProgram() const {
	        return columnAt(MD_EXPOSURE_PROGRAM_IDX).toString();
	}

	void setExposureProgram(const std::string& exposureProgram) {
	        columnAt(MD_EXPOSURE_PROGRAM_IDX).fromString(exposureProgram);;
	}

	const std::string& getExposureTime() const {
	        return columnAt(MD_EXPOSURE_TIME_IDX).toString();
	}

	void setExposureTime(const std::string& exposureTime) {
	        columnAt(MD_EXPOSURE_TIME_IDX).fromString(exposureTime);;
	}

	const std::string& getFilename() const {
		return columnAt(MD_FILENAME_IDX).toString();
	}

	void setFilename(const std::string& filename) {
	        columnAt(MD_FILENAME_IDX).fromString(filename);;
	}

	const std::string& getFilepath() const {
	        return columnAt(MD_FILEPATH_IDX).toString();
	}

	void setFilepath(const std::string& filepath) {
	        columnAt(MD_FILEPATH_IDX).fromString(filepath);;
	}

	const std::string& getFileSize() const {
	        return columnAt(MD_FILE_SIZE_IDX).toString();
	}

	void setFileSize(const std::string& fileSize) {
	        columnAt(MD_FILE_SIZE_IDX).fromString(fileSize);;
	}

	const std::string& getFlash() const {
	        return columnAt(MD_FLASH_IDX).toString();
	}

	void setFlash(const std::string& flash) {
	        columnAt(MD_FLASH_IDX).fromString(flash);;
	}

	const std::string& getFocalLength() const {
	        return columnAt(MD_FOCAL_LENGTH_IDX).toString();
	}

	void setFocalLength(const std::string& focalLength) {
	        columnAt(MD_FOCAL_LENGTH_IDX).fromString(focalLength);;
	}

	const std::string& getGpsTimeStamp() const {
	        return columnAt(MD_GPS_TIMESTAMP_IDX).toString();
	}

	void setGpsTimeStamp(const std::string& gpsTimeStamp) {
	        columnAt(MD_GPS_TIMESTAMP_IDX).fromString(gpsTimeStamp);;
	}

	const std::string& getHeight() const {
	        return columnAt(MD_HEIGHT_IDX).toString();
	}

	void setHeight(const std::string& height) {
	        columnAt(MD_HEIGHT_IDX).fromString(height);;
	}

	const std::string& getIsoSpeedRating() const {
	        return columnAt(MD_ISO_SPEED_RATING_IDX).toString();
	}

	void setIsoSpeedRating(const std::string& isoSpeedRating) {
	        columnAt(MD_ISO_SPEED_RATING_IDX).fromString(isoSpeedRating);
	}

	const std::string& getLabel() const {
	        return columnAt(MD_LABEL_IDX).toString();
	}

	void setLabel(const std::string& label) {
	        columnAt(MD_LABEL_IDX).fromString(label);;
	}

	const std::string& getLatitude() const {
	        return columnAt(MD_LATITUDE_IDX).toString();
	}

	void setLatitude(const std::string& latitude) {
	        columnAt(MD_LATITUDE_IDX).fromString(latitude);;
	}

	const std::string& getLightSource() const {
	        return columnAt(MD_LIGHT_SOURCE_IDX).toString();
	}

	void setLightSource(const std::string& lightSource) {
	        columnAt(MD_LIGHT_SOURCE_IDX).fromString(lightSource);;
	}

	const std::string& getLongitude() const {
	        return columnAt(MD_LONGITUDE_IDX).toString();
	}

	void setLongitude(const std::string& longitude) {
	        columnAt(MD_LONGITUDE_IDX).fromString(longitude);;
	}

	const std::string& getMaker() const {
	        return columnAt(MD_MAKER_IDX).toString();
	}

	void setMaker(const std::string& maker) {
	        columnAt(MD_MAKER_IDX).fromString(maker);;
	}

	const std::string& getMd5() const {
	        return columnAt(MD_MD5_IDX).toString();
	}

	void setMd5(const std::string& md5) {
	        columnAt(MD_MD5_IDX).fromString(md5);;
	}

	const std::string& getMediaType() const {
	        return columnAt(DB_CRC_IDX).toString();
	}

	void setMediaType(const std::string& mediaType) {
	        columnAt(DB_CRC_IDX).fromString(mediaType);;
	}

	const std::string& getMeteringMode() const {
	        return columnAt(MD_METERING_MODE_IDX).toString();
	}

	void setMeteringMode(const std::string& meteringMode) {
	        columnAt(MD_METERING_MODE_IDX).fromString(meteringMode);;
	}

	const std::string& getModel() const {
	        return columnAt(MD_MODEL_IDX).toString();
	}

	void setModel(const std::string& model) {
	        columnAt(MD_MODEL_IDX).fromString(model);;
	}

	const std::string& getOrginalName() const {
	        return columnAt(MD_ORGINAL_NAME_IDX).toString();
	}

	void setOrginalName(const std::string& orginalName) {
	        columnAt(MD_ORGINAL_NAME_IDX).fromString(orginalName);;
	}

	const std::string& getPage() const {
	        return columnAt(MD_PAGE_IDX).toString();
	}

	void setPage(const std::string& page) {
	        columnAt(MD_PAGE_IDX).fromString(page);;
	}

	const std::string& getPrimaryEncoding() const {
	        return columnAt(MD_PRIMARY_ENCODING_IDX).toString();
	}

	void setPrimaryEncoding(const std::string& primaryEncoding) {
	        columnAt(MD_PRIMARY_ENCODING_IDX).fromString(primaryEncoding);;
	}

	const std::string& getRating() const {
	        return columnAt(MD_RATING_IDX).toString();
	}

	void setRating(const std::string& rating) {
	        columnAt(MD_RATING_IDX).fromString(rating);;
	}

	const std::string& getResolution() const {
	        return columnAt(MD_RESOLUTION_IDX).toString();
	}

	void setResolution(const std::string& resolution) {
	        columnAt(MD_RESOLUTION_IDX).fromString(resolution);;
	}

	const std::string& getSampleColor() const {
	        return columnAt(MD_SAMPLE_COLOR_IDX).toString();
	}

	void setSampleColor(const std::string& sampleColor) {
	        columnAt(MD_SAMPLE_COLOR_IDX).fromString(sampleColor);;
	}

	const std::string& getSensingMethod() const {
	        return columnAt(MD_SENSING_METHOD_IDX).toString();
	}

	void setSensingMethod(const std::string& sensingMethod) {
	        columnAt(MD_SENSING_METHOD_IDX).fromString(sensingMethod);;
	}

	const std::string& getSequenceId() const {
	        return columnAt(MD_SEQUENCE_ID_IDX).toString();
	}

	void setSequenceId(const std::string& sequenceId) {
	        columnAt(MD_SEQUENCE_ID_IDX).fromString(sequenceId);;
	}

	void setSequenceId(const int sequenceId) {
		columnAt(MD_SEQUENCE_ID_IDX) = sequenceId;
	}
	const std::string& getSoftware() const {
	        return columnAt(MD_SOFTWARE_IDX).toString();
	}

	void setSoftware(const std::string& software) {
	        columnAt(MD_SOFTWARE_IDX).fromString(software);;
	}

	const std::string& getSourceUrl() const {
	        return columnAt(MD_SOURCE_URL_IDX).toString();
	}

	void setSourceUrl(const std::string& sourceUrl) {
		columnAt(MD_SOURCE_URL_IDX).fromString(sourceUrl);;
	}

	const std::string& getUniqueId() const {
		return columnAt(MD_UNIQUE_ID_IDX).toString();
	}

	void setUniqueId(const std::string& uniqueId) {
		columnAt(MD_UNIQUE_ID_IDX).fromString(uniqueId);;
	}

	const std::string& getUsageRights() const {
		return columnAt(MD_USAGE_RIGHTS_IDX).toString();
	}

	void setUsageRights(const std::string& usageRights) {
		columnAt(MD_USAGE_RIGHTS_IDX).fromString(usageRights);;
	}

	const std::string& getViewRotation() const {
		return columnAt(MD_VIEW_ROTATION_IDX).toString();
	}

	void setViewRotation(const std::string& viewRotation) {
		columnAt(MD_VIEW_ROTATION_IDX).fromString(viewRotation);;
	}

	const std::string& getWidth() const {
		return columnAt(MD_WIDTH_IDX).toString();

	}

	void setWidth(const std::string& width) {
		columnAt(MD_WIDTH_IDX).fromString(width);;
	}
};

} /* namespace simplearchive */
#endif /* METADATAOBJECT_H_ */
