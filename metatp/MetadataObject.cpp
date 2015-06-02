/*
imageContainer * MetadataObject.cpp
 *
 *  Created on: Jul 4, 2014
 *      Author: wzw7yn
 */
#include "DBDefines.h"
#include "MetadataObject.h"

namespace simplearchive {





MetadataObject::MetadataObject() : MTRow(new MetadataSchema){

	columnAt( MD_SEQUENCE_ID_IDX).bind(m_sequenceId);
	columnAt( MD_FILENAME_IDX).bind(m_filename);
	columnAt( MD_FILEPATH_IDX).bind(m_filepath);
	columnAt( MD_ORGINAL_NAME_IDX).bind(m_orginalName);
	columnAt( MD_UNIQUE_ID_IDX).bind(m_uniqueId);
	columnAt( MD_LABEL_IDX).bind(m_label);
	columnAt( MD_RATING_IDX).bind(m_rating);
	columnAt( MD_MEDIA_TYPE_IDX).bind(m_mediaType);
	columnAt( MD_MD5_IDX).bind(m_md5);
	columnAt( MD_CRC_IDX).bind(m_crc);
	columnAt( MD_FILE_SIZE_IDX).bind(m_fileSize);
	columnAt( MD_DATE_CREATE_IDX).bind(m_dateCreate);
	columnAt( MD_DATE_MODIFIED_IDX).bind(m_dateModified);
	columnAt( MD_DATE_ADDED_IDX).bind(m_dateAdded);
	columnAt( MD_DESCRIPTION_IDX).bind(m_description);

	/// Media Properties
	columnAt( MD_WIDTH_IDX).bind(m_width);
	columnAt( MD_HEIGHT_IDX).bind(m_height);
	columnAt( MD_RESOLUTION_IDX).bind(m_resolution);
	columnAt( MD_DEPTH_IDX).bind(m_depth);
	columnAt( MD_VIEW_ROTATION_IDX).bind(m_viewRotation);
	columnAt( MD_SAMPLE_COLOR_IDX).bind(m_sampleColor);
	columnAt( MD_PAGE_IDX).bind(m_page);
	columnAt( MD_COLOR_SPACE_IDX).bind(m_colorSpace);
	columnAt( MD_COMPRESSION_IDX).bind(m_compression);
	columnAt( MD_PRIMARY_ENCODING_IDX).bind(m_primaryEncoding);
	/// Camera Information
	columnAt( MD_MAKER_IDX).bind(m_maker);
	columnAt( MD_MODEL_IDX).bind(m_model);
	columnAt( MD_SOFTWARE_IDX).bind(m_software);
	columnAt( MD_SOURCE_URL_IDX).bind(m_sourceURL);
	columnAt( MD_EXIF_VERSION_IDX).bind(m_exifVersion);
	columnAt( MD_CAPTURE_DATE_IDX).bind(m_captureDate);
	columnAt( MD_EXPOSURE_PROGRAM_IDX).bind(m_exposureProgram);
	columnAt( MD_ISO_SPEED_RATING_IDX).bind(m_isoSpeedRating);
	columnAt( MD_EXPOSURE_BIAS_IDX).bind(m_exposureBias);
	columnAt( MD_EXPOSURE_TIME_IDX).bind(m_exposureTime);
	columnAt( MD_APERTURE_IDX).bind(m_aperture);
	columnAt( MD_METERING_MODE_IDX).bind(m_meteringMode);
	columnAt( MD_LIGHT_SOURCE_IDX).bind(m_lightSource);
	columnAt( MD_FLASH_IDX).bind(m_flash);
	columnAt( MD_FOCAL_LENGTH_IDX).bind(m_focalLength);
	columnAt( MD_SENSING_METHOD_IDX).bind(m_sensingMethod);
	columnAt( MD_DIGITAL_ZOOM_IDX).bind(m_digitalZoom);
	/// GPS
	columnAt( MD_LATITUDE_IDX).bind(m_latitude);
	columnAt( MD_LONGITUDE_IDX).bind(m_longitude);
	columnAt( MD_GPS_TIMESTAMP_IDX).bind(m_gpsTimeStamp);
	//Copyright Properties
	columnAt( MD_COPYRIGHT_IDX).bind(m_copyright);
	columnAt( MD_USAGE_RIGHTS_IDX).bind(m_usageRights);
	columnAt( MD_COPYRIGHT_URL_IDX).bind(m_copyrightURL);
}

MetadataObject::~MetadataObject() {
	// TODO Auto-generated destructor stub
}

std::map<std::string, std::string> tableMap;


//const char *MetadataObject::TableLookup(const char *columnName) const {
//	MD_SEQUENCE_ID_IDX
//}


} /* namespace simplearchive */
