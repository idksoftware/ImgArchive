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
#include "FileInfo.h"

namespace simplearchive {


class MetadataSchema : public MTTableSchema {
public:
	MetadataSchema() : MTTableSchema("MetadataProperties") {

		// Assest 
		// file
        	add(MTSchema(MTSchema::Integer, DB_SEQUENCEID));	// *
        	add(MTSchema(MTSchema::Text, DB_FILENAME));
        	add(MTSchema(MTSchema::Text, DB_ORGINALNAME));
			add(MTSchema(MTSchema::Text, DB_FILEPATH));
        	add(MTSchema(MTSchema::Text, DB_UUID));
        	add(MTSchema(MTSchema::Text, DB_MEDIATYPE));
        	add(MTSchema(MTSchema::Text, DB_MD5));
        	add(MTSchema(MTSchema::Integer, DB_CRC));
        	add(MTSchema(MTSchema::Integer, DB_FILESIZE));
        	add(MTSchema(MTSchema::Date, DB_DATECREATE));
        	add(MTSchema(MTSchema::Date, DB_DATEMODIFIED));
        	add(MTSchema(MTSchema::Date, DB_DATEADDED));
			// Description
        	add(MTSchema(MTSchema::Text, DB_DESCRIPTION));
			add(MTSchema(MTSchema::Text, DB_LABEL));
			add(MTSchema(MTSchema::Text, DB_TITLE));
			add(MTSchema(MTSchema::Text, DB_SUBJECT));
			add(MTSchema(MTSchema::Integer, DB_RATING));
			add(MTSchema(MTSchema::Integer, DB_RATINGPERCENT));
		add(MTSchema(MTSchema::Text, DB_KEYWORDS));// template
		add(MTSchema(MTSchema::Integer, DB_VERSION));// template
		// Origin
		add(MTSchema(MTSchema::Text, DB_COMMENT));// template
		add(MTSchema(MTSchema::Text, DB_AUTHOR));
		add(MTSchema(MTSchema::Date, DB_CAPTUREDATE));


		// Cammara
		// Image
		add(MTSchema(MTSchema::Date, DB_SUBSETIMEORIGINAL));
		add(MTSchema(MTSchema::Text, DB_COPYRIGHT));
		
		add(MTSchema(MTSchema::Integer, DB_WIDTH));
		add(MTSchema(MTSchema::Integer, DB_HEIGHT));
		add(MTSchema(MTSchema::Integer, DB_VIEWROTATION));
		add(MTSchema(MTSchema::Integer, DB_BITSPERSAMPLE));
		add(MTSchema(MTSchema::Integer, DB_XRESOLUTION));
		add(MTSchema(MTSchema::Integer, DB_YRESOLUTION));
		add(MTSchema(MTSchema::Text, DB_RESOLUTIONUNIT));
		add(MTSchema(MTSchema::Integer, DB_COMPRESSION));
		// Camera
		add(MTSchema(MTSchema::Text, DB_MAKER));
		add(MTSchema(MTSchema::Text, DB_MODEL));
		add(MTSchema(MTSchema::Text, DB_EXPOSURETIME));
		add(MTSchema(MTSchema::Float, DB_APERTURE));
		add(MTSchema(MTSchema::Integer, DB_ISOSPEEDRATING));
		add(MTSchema(MTSchema::Float, DB_EXPOSUREBIAS));
		add(MTSchema(MTSchema::Text, DB_EXPOSUREPROGRAM));
		add(MTSchema(MTSchema::Float, DB_SUBJECTDISTANCE));
		add(MTSchema(MTSchema::Float, DB_FOCALLENGTH));
		add(MTSchema(MTSchema::Integer, DB_FOCALLENGTHIN35MM));
		add(MTSchema(MTSchema::Text, DB_FLASH));
		add(MTSchema(MTSchema::Text, DB_METERINGMODE));
		add(MTSchema(MTSchema::Float, DB_DIGITALZOOM));
		// Advanced photo
		add(MTSchema(MTSchema::Text, DB_LIGHTSOURCE));
		add(MTSchema(MTSchema::Text, DB_COLORSPACE));
		add(MTSchema(MTSchema::Text, DB_WHITEBALANCE));
		add(MTSchema(MTSchema::Text, DB_SCENECAPTURETYPE));
		add(MTSchema(MTSchema::Text, DB_CONTRAST));
		add(MTSchema(MTSchema::Text, DB_SATURATION));
		add(MTSchema(MTSchema::Text, DB_SHARPNESS));
		add(MTSchema(MTSchema::Float, DB_BRIGHTNESSVALUE));
		add(MTSchema(MTSchema::Text, DB_SUBJECTDISTANCERANGE));
		add(MTSchema(MTSchema::Float, DB_MAXAPERTURE));
		add(MTSchema(MTSchema::Float, DB_COMPRESSEDBITSPERPIXEL));
		add(MTSchema(MTSchema::Float, DB_FLASHENERGY));
		add(MTSchema(MTSchema::Text, DB_EXIFVERSION));
		add(MTSchema(MTSchema::Text, DB_SOFTWARE));
		add(MTSchema(MTSchema::Integer, DB_PHOTOMETRICINTERPRETATION));
		add(MTSchema(MTSchema::Text, DB_GPSTIMESTAMP));
	
	
	//	add(MTSchema(MTSchema::Text, DB_SENSINGMETHOD));
		
	// Location
		add(MTSchema(MTSchema::Text, DB_LATITUDE));
		add(MTSchema(MTSchema::Text, DB_LONGITUDE));
		add(MTSchema(MTSchema::Text, DB_LOCATION));
		add(MTSchema(MTSchema::Text, DB_SCENE));// template

		// IPTC
		add(MTSchema(MTSchema::Text, DB_SOURCEURL));			// template
		add(MTSchema(MTSchema::Text, DB_USAGERIGHTS));// template
		add(MTSchema(MTSchema::Text, DB_COPYRIGHTURL));// template
		add(MTSchema(MTSchema::Text, DB_HEADLINE));// template
		add(MTSchema(MTSchema::Text, DB_CATEGORY));// template
		add(MTSchema(MTSchema::Text, DB_SOURCE));// template
		add(MTSchema(MTSchema::Text, DB_INSTRUCTIONS));// template
		
		add(MTSchema(MTSchema::Text, DB_CREATOR));// template
		add(MTSchema(MTSchema::Text, DB_JOBTITLE));// template
		add(MTSchema(MTSchema::Text, DB_ADDRESS));// template
		add(MTSchema(MTSchema::Text, DB_CITY));// template
		add(MTSchema(MTSchema::Text, DB_STATE));// template
		add(MTSchema(MTSchema::Text, DB_POSTALCODE));// template
		add(MTSchema(MTSchema::Text, DB_COUNTRY));// template
		add(MTSchema(MTSchema::Text, DB_PHONE));// template
		add(MTSchema(MTSchema::Text, DB_EMAIL));// template
		add(MTSchema(MTSchema::Text, DB_WEBSITE));// template
			// new 
			
        }
};


class BasicMetadata;
/**
 * @brief Metadata Container
 *
 * This contains the metadata to be substituted during the template process.
 * As each template is read the contents will be placed into this class. This class
 * will be used for further substitutions by the Exif reading and user defined
 * keyword substitutions.
 */
class MetadataObject : public MTRow {
	static MetadataSchema m_tableSchema;
	friend class MetadataTemplate;
public:
	MetadataObject();
	MetadataObject(const MTRow &row) : MTRow(m_tableSchema) {
		
		for (unsigned int i = 0; i < row.size(); i++) {
			MTColumn& c1 = *at(i);
			MTColumn& c2 = *row.at(i);
			c1 = c2;
		}
		
	}
	virtual ~MetadataObject();

	//static const char *TableLookup(const char *columnName) const ;
	
	void update(const MetadataObject& mo) {
		
		if (getHeightString().compare("null") == 0) {
			setHeight(mo.getHeightString());
		}
		if (getWidthString().compare("null") == 0) {
			setWidth(mo.getWidthString());
		}
		
	}

	void update(const FileInfo& fileInfo, int seqNo, int version);

	const std::string getApertureString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_APERTURE_IDX)).toString();
	}

	void setAperture(const std::string& aperture) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_APERTURE_IDX)).fromString(aperture);
	}

	const std::string getCaptureDateString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_CAPTUREDATE_IDX)).toString();
	}

	const ExifDateTime& getCaptureDate() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_CAPTUREDATE_IDX)).getDate();
	}

	void setCaptureDate(const std::string& captureDate) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_CAPTUREDATE_IDX)).fromString(captureDate);;
	}

	const std::string& getColorSpaceString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_COLORSPACE_IDX)).toString();

	}

	void setColorSpace(const std::string& colorSpace) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_COLORSPACE_IDX)).fromString(colorSpace);;
	}

	const std::string& getCompressionString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_COMPRESSION_IDX)).toString();
	}

	void setCompression(const std::string& compression) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_COMPRESSION_IDX)).fromString(compression);;
	}

	const std::string& getCopyrightString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_COPYRIGHT_IDX)).toString();
	}

	void setCopyright(const std::string& copyright) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_COPYRIGHT_IDX)).fromString(copyright);;
	}

	const std::string& getCopyrightUrlString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_COPYRIGHTURL_IDX)).toString();
	}

	void setCopyrightUrl(const std::string& copyrightUrl) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_COPYRIGHTURL_IDX)).fromString(copyrightUrl);;
	}

	const std::string& getCrcString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_CRC_IDX)).toString();
	}



	void setCrc(const std::string& crc) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_CRC_IDX)).fromString(crc);;
	}

	const std::string& getVersionString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_VERSION_IDX )).toString();
	}

	void setVersion(const std::string& crc) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_VERSION_IDX)).fromString(crc);;
	}

	// MD_DATEADDED_IDX
	const std::string& getAddedDateString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_DATEADDED_IDX)).toString();
	}

	void setAddedDate(const std::string& dateAdded) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_DATEADDED_IDX)).fromString(dateAdded);;
	}

	void setAddedDate(const ExifDateTime& dateAdded) {
		const std::string s = ((ExifDateTime&)(dateAdded)).toString();
		columnAt(static_cast<int>(MetadataObjectIndex::MD_DATEADDED_IDX)).fromString(s);
	}

	const std::string& getDateCreateString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_DATECREATE_IDX)).toString();
	}

	void setDateCreate(const std::string& dateCreate) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_DATECREATE_IDX)).fromString(dateCreate);;
	}

	const std::string& getDateModifiedString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_DATEMODIFIED_IDX)).toString();
	}

	void setDateModified(const std::string& dateModified) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_DATEMODIFIED_IDX)).fromString(dateModified);;
	}


	const std::string& getDigitalZoomString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_DIGITALZOOM_IDX)).toString();
	}

	void setDigitalZoom(const std::string& digitalZoom) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_DIGITALZOOM_IDX)).fromString(digitalZoom);;
	}

	const std::string& getExifVersionString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_SOFTWARE_IDX)).toString();
	}

	void setExifVersion(const std::string& exifVersion) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_SOFTWARE_IDX)).fromString(exifVersion);;
	}

	const std::string& getExposureBiasString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_EXPOSUREBIAS_IDX)).toString();
	}

	void setExposureBias(const std::string& exposureBias) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_EXPOSUREBIAS_IDX)).fromString(exposureBias);;
	}

	const std::string& getExposureProgramString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_EXPOSUREPROGRAM_IDX)).toString();
	}

	void setExposureProgram(const std::string& exposureProgram) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_EXPOSUREPROGRAM_IDX)).fromString(exposureProgram);;
	}

	const std::string& getExposureTimeString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_EXPOSURETIME_IDX)).toString();
	}

	void setExposureTime(const std::string& exposureTime) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_EXPOSURETIME_IDX)).fromString(exposureTime);;
	}

	const std::string& getFilenameString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_FILENAME_IDX)).toString();
	}

	void setFilename(const std::string& filename) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_FILENAME_IDX)).fromString(filename);;
	}

	const std::string& getFilepathString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_FILEPATH_IDX)).toString();
	}

	void setFilepath(const std::string& filepath) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_FILEPATH_IDX)).fromString(filepath);;
	}

	const std::string& getFileSizeString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_FILESIZE_IDX)).toString();
	}

	void setFileSize(const std::string& fileSize) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_FILESIZE_IDX)).fromString(fileSize);;
	}

	const std::string& getFlashString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_FLASH_IDX)).toString();
	}

	void setFlash(const std::string& flash) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_FLASH_IDX)).fromString(flash);;
	}

	const std::string& getFocalLengthString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_FOCALLENGTH_IDX)).toString();
	}

	void setFocalLength(const std::string& focalLength) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_FOCALLENGTH_IDX)).fromString(focalLength);;
	}

	const std::string& getGpsTimeStampString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_GPSTIMESTAMP_IDX)).toString();
	}

	void setGpsTimeStamp(const std::string& gpsTimeStamp) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_GPSTIMESTAMP_IDX)).fromString(gpsTimeStamp);;
	}

	const std::string& getHeightString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_HEIGHT_IDX)).toString();
	}

	void setHeight(const std::string& height) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_HEIGHT_IDX)).fromString(height);;
	}

	const std::string& getIsoSpeedRatingString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_ISOSPEEDRATING_IDX)).toString();
	}

	void setIsoSpeedRating(const std::string& isoSpeedRating) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_ISOSPEEDRATING_IDX)).fromString(isoSpeedRating);
	}

	const std::string& getLabelString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_LABEL_IDX)).toString();
	}

	void setLabel(const std::string& label) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_LABEL_IDX)).fromString(label);;
	}

	const std::string& getLatitudeString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_LATITUDE_IDX)).toString();
	}

	void setLatitude(const std::string& latitude) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_LATITUDE_IDX)).fromString(latitude);;
	}

	const std::string& getLightSourceString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_LIGHTSOURCE_IDX)).toString();
	}

	void setLightSource(const std::string& lightSource) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_LIGHTSOURCE_IDX)).fromString(lightSource);;
	}

	const std::string& getLongitudeString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_LONGITUDE_IDX)).toString();
	}

	void setLongitude(const std::string& longitude) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_LONGITUDE_IDX)).fromString(longitude);;
	}

	const std::string& getGPSTimeStampString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_GPSTIMESTAMP_IDX)).toString();
	}

	void setGPSTimeStamp(const std::string& gpsTimeStamp) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_GPSTIMESTAMP_IDX)).fromString(gpsTimeStamp);;
	}


	const std::string& getMakerString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_MAKER_IDX)).toString();
	}

	void setMaker(const std::string& maker) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_MAKER_IDX)).fromString(maker);;
	}

	const std::string& getMd5String() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_MD5_IDX)).toString();
	}

	void setMd5(const std::string& md5) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_MD5_IDX)).fromString(md5);;
	}

	const std::string& getMediaTypeString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_CRC_IDX)).toString();
	}

	void setMediaType(const std::string& mediaType) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_CRC_IDX)).fromString(mediaType);;
	}

	const std::string& getMeteringModeString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_METERINGMODE_IDX)).toString();
	}

	void setMeteringMode(const std::string& meteringMode) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_METERINGMODE_IDX)).fromString(meteringMode);;
	}

	const std::string& getModelString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_MODEL_IDX)).toString();
	}

	void setModel(const std::string& model) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_MODEL_IDX)).fromString(model);;
	}

	const std::string& getOrginalNameString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_ORGINALNAME_IDX)).toString();
	}

	void setOrginalName(const std::string& orginalName) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_ORGINALNAME_IDX)).fromString(orginalName);;
	}

	const std::string& getRatingString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_RATING_IDX)).toString();
	}

	void setRating(const std::string& rating) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_RATING_IDX)).fromString(rating);
	}

	const std::string& getSequenceIdString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_SEQUENCEID_IDX)).toString();
	}

	void setSequenceId(const std::string& sequenceId) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_SEQUENCEID_IDX)).fromString(sequenceId);;
	}

	void setSequenceId(const int sequenceId) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_SEQUENCEID_IDX)) = sequenceId;
	}
	const std::string& getSoftwareString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_SOFTWARE_IDX)).toString();
	}

	void setSoftware(const std::string& software) {
	        columnAt(static_cast<int>(MetadataObjectIndex::MD_SOFTWARE_IDX)).fromString(software);;
	}

	const std::string& getSourceUrlString() const {
	        return columnAt(static_cast<int>(MetadataObjectIndex::MD_SOURCEURL_IDX)).toString();
	}

	void setSourceUrl(const std::string& sourceUrl) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_SOURCEURL_IDX)).fromString(sourceUrl);;
	}

	const std::string& getUniqueIdString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_UUID_IDX)).toString();
	}

	void setUniqueId(const std::string& uniqueId) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_UUID_IDX)).fromString(uniqueId);;
	}

	const std::string& getUsageRightsString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_USAGERIGHTS_IDX)).toString();
	}

	void setUsageRights(const std::string& usageRights) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_USAGERIGHTS_IDX)).fromString(usageRights);;
	}

	const std::string& getViewRotationString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_VIEWROTATION_IDX)).toString();
	}

	void setViewRotation(const std::string& viewRotation) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_VIEWROTATION_IDX)).fromString(viewRotation);;
	}

	const std::string& getWidthString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_WIDTH_IDX)).toString();

	}

	void setWidth(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_WIDTH_IDX)).fromString(width);;
	}

	const std::string& getHeadlineString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_HEADLINE_IDX)).toString();

	}

	void setHeadline(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_HEADLINE_IDX)).fromString(width);;
	}

	const std::string& getCategoryString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_CATEGORY_IDX)).toString();

	}

	void setCategory(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_CATEGORY_IDX)).fromString(width);;
	}

	const std::string& getDescriptionString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_DESCRIPTION_IDX)).toString();

	}

	void setDescription(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_DESCRIPTION_IDX)).fromString(width);;
	}

	const std::string& getKeywordsString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_KEYWORDS_IDX)).toString();

	}

	void setKeywords(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_KEYWORDS_IDX)).fromString(width);;
	}

	const std::string& getSourceString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_SOURCE_IDX)).toString();

	}

	void setSource(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_SOURCE_IDX)).fromString(width);;
	}

	const std::string& getInstructionsString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_INSTRUCTIONS_IDX)).toString();

	}

	void setInstructions(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_INSTRUCTIONS_IDX)).fromString(width);;
	}

	const std::string& getSceneString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_SCENE_IDX)).toString();

	}

	void setScene(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_SCENE_IDX)).fromString(width);;
	}

	const std::string& getSceneCaptureTypeString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_SCENECAPTURETYPE_IDX)).toString();

	}

	const std::string& getCreatorString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_CREATOR_IDX)).toString();

	}

	void setCreator(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_CREATOR_IDX)).fromString(width);;
	}

	const std::string& getJobTitleString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_JOBTITLE_IDX)).toString();

	}

	void setJobTitle(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_JOBTITLE_IDX)).fromString(width);;
	}

	const std::string& getAddressString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_ADDRESS_IDX)).toString();

	}

	void setAddress(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_ADDRESS_IDX)).fromString(width);;
	}

	const std::string& getCityString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_CITY_IDX)).toString();

	}

	void setCity(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_CITY_IDX)).fromString(width);;
	}

	const std::string& getStateString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_STATE_IDX)).toString();

	}

	void setState(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_STATE_IDX)).fromString(width);;
	}

	const std::string& getPostalCodeString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_POSTALCODE_IDX)).toString();

	}

	void setPostalCode(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_POSTALCODE_IDX)).fromString(width);;
	}

	const std::string& getCountryString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_COUNTRY_IDX)).toString();

	}

	void setCountry(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_COUNTRY_IDX)).fromString(width);;
	}

	const std::string& getPhoneString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_PHONE_IDX)).toString();

	}

	void setPhone(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_PHONE_IDX)).fromString(width);;
	}

	const std::string& getEmailString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_EMAIL_IDX)).toString();

	}

	void setEmail(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_EMAIL_IDX)).fromString(width);;
	}

	const std::string& getWebsiteString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_WEBSITE_IDX)).toString();

	}

	void setWebsite(const std::string& width) {
		columnAt(static_cast<int>(MetadataObjectIndex::MD_WEBSITE_IDX)).fromString(width);;
	}

	const std::string& getTitleString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_TITLE_IDX)).toString();
	}

	const std::string& getSubjectString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_SUBJECT_IDX)).toString();
	}

	const std::string& getRatingPercentString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_RATINGPERCENT_IDX)).toString();
	}

	const std::string& getCommentString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_COMMENT_IDX)).toString();
	}

	const std::string& getAuthorString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_AUTHOR_IDX)).toString();
	}

	const std::string& getSubsetimeOriginaLString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_SUBSETIMEORIGINAL_IDX)).toString();
	}

	const std::string& getBitsPerSampleString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_BITSPERSAMPLE_IDX)).toString();
	}

	const std::string& getXResolutionString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_XRESOLUTION_IDX)).toString();
	}

	const std::string& getYResolutionString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_YRESOLUTION_IDX)).toString();
	}

	const std::string& getResolutionUnitString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_RESOLUTIONUNIT_IDX)).toString();
	}

	const std::string& getSubjectDistanceString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_SUBJECTDISTANCE_IDX)).toString();
	}

	const std::string& getFocalLengthIn35mmString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_FOCALLENGTHIN35MM_IDX)).toString();
	}

	const std::string& getWhiteBalanceString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_WHITEBALANCE_IDX)).toString();
	}

	const std::string& getContrastString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_CONTRAST_IDX)).toString();
	}

	const std::string& getSaturationString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_SATURATION_IDX)).toString();
	}

	const std::string& getSharpnessString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_SHARPNESS_IDX)).toString();
	}

	const std::string& getBrightnessValueString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_BRIGHTNESSVALUE_IDX)).toString();}

	const std::string& getSubjectDIstanceRangeString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_SUBJECTDISTANCERANGE_IDX)).toString();
	}

	const std::string& getMaxApertureString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_MAXAPERTURE_IDX)).toString();
	}

	const std::string& getCompressedBitsPerPixelString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_COMPRESSEDBITSPERPIXEL_IDX)).toString();
	}

	const std::string& getFlashEnergyString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_FLASHENERGY_IDX)).toString();
	}

	

	const std::string& getPhotometricInterpretationString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_PHOTOMETRICINTERPRETATION_IDX)).toString();
	}

	

	const std::string& getLoctionString() const {
		return columnAt(static_cast<int>(MetadataObjectIndex::MD_LOCATION_IDX)).toString();
	}
	

	
	
};

} /* namespace simplearchive */
#endif /* METADATAOBJECT_H_ */
