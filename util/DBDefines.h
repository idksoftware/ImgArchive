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
**	Filename	: DBDefines.h
**	Author		: I.Ferguson
**	Version		: 1.000
**	Date		: 26-05-2015
**
** #$$@@$$# */

#ifndef DBDEFINES_H_
#define DBDEFINES_H_
/**
 * This file defines the key strings used by the Archiver. This allows consistent
 * key names to be used in the application.
 */

//namespace simplearchive {

#define TABLE_ASSET_PROPERTIES		"AssetProperties"
#define TABLE_CAMERA_INFORMATION	"CameraInformation"
#define TABLE_MEDIA_PROPERTIES		"MediaProperties"
#define TABLE_COPYRIGHT_PROPERTIES	"CopyrightProperties"
#define TABLE_GPS_PROPERTIES		"GPSProperties"
#define ROW_BASIC_EXIF				"BasicExif"
#define ROW_EXTERNAL_EXIF			"ExternalExif"

#define BE_FILENAME_IDX			0
#define BE_ORGINALNAME_IDX		1
#define BE_FILEPATH_IDX			2
#define BE_CRC_IDX			3
#define BE_MD5_IDX			4
#define BE_UUID_IDX			5
#define BE_LABEL_IDX			6
#define BE_MEDIATYPE_IDX		7
#define BE_RATING_IDX			8
#define BE_FILESIZE_IDX			9
#define BE_DATEMODIFIED_IDX		10
#define BE_DATECREATE_IDX		11

#define BE_DESCRIPTION_IDX		12
#define BE_WIDTH_IDX			13
#define BE_HEIGHT_IDX			14
#define BE_VIEWROTATION_IDX		15
#define BE_MAKER_IDX			16
#define BE_MODEL_IDX			17
#define BE_EXIFVERSION_IDX		18
#define BE_CAPTUREDATE_IDX		19
#define BE_ISOSPEEDRATING_IDX	20
#define BE_EXPOSUREBIAS_IDX		21
#define BE_EXPOSURETIME_IDX		22
#define BE_APERTURE_IDX			23
#define BE_METERINGMODE_IDX		24
#define BE_FOCALLENGTH_IDX		25
#define BE_LATITUDE_IDX			26
#define BE_LONGITUDE_IDX		27
#define BE_COPYRIGHT_IDX		28
#define BE_KEYWORDS_IDX			29

#define EE_DESCRIPTION_IDX      0
/// Media Properties
#define EE_WIDTH_IDX            1
#define EE_HEIGHT_IDX           2
#define EE_RESOLUTION_IDX       3
#define EE_DEPTH_IDX            4
#define EE_VIEW_ROTATION_IDX    5
#define EE_SAMPLE_COLOR_IDX     6
#define EE_PAGE_IDX             7
#define EE_COLOR_SPACE_IDX      8
#define EE_COMPRESSION_IDX      9
#define EE_PRIMARY_ENCODING_IDX 10
/// Camerai Information"
#define EE_MAKER_IDX            11
#define EE_MODEL_IDX            12
#define EE_SOFTWARE_IDX         13
#define EE_SOURCE_URL_IDX       14
#define EE_EXIF_VERSION_IDX     15
#define EE_CAPTURE_DATE_IDX     16
#define EE_EXPOSURE_PROGRAM_IDX 17
#define EE_ISO_SPEED_RATING_IDX 18
#define EE_EXPOSURE_BIAS_IDX    19
#define EE_EXPOSURE_TIME_IDX    20
#define EE_APERTURE_IDX         21
#define EE_METERING_MODE_IDX    22
#define EE_LIGHT_SOURCE_IDX     23
#define EE_FLASH_IDX            24
#define EE_FOCAL_LENGTH_IDX     25
#define EE_SENSING_METHOD_IDX   26
#define EE_DIGITAL_ZOOM_IDX     27
/// GPS
#define EE_LATITUDE_IDX         28
#define EE_LONGITUDE_IDX        29
#define EE_GPS_TIMESTAMP_IDX    30
//Copyright Properties
#define EE_COPYRIGHT_IDX        31
#define EE_USAGE_RIGHTS_IDX     32
#define EE_COPYRIGHT_URL_IDX    33
#define EE_MEDIATYPE_IDX		34


/// Metadata Defines
	/// Asset Properties
#define MD_SEQUENCE_ID_IDX      0
#define MD_FILENAME_IDX         1
#define MD_FILEPATH_IDX         2
#define MD_ORGINAL_NAME_IDX     3
#define MD_UNIQUE_ID_IDX        4
#define MD_LABEL_IDX            5
#define MD_RATING_IDX           6
#define MD_MEDIA_TYPE_IDX       7
#define MD_MD5_IDX              8
#define MD_CRC_IDX              9
#define MD_FILE_SIZE_IDX        10
#define MD_DATE_CREATE_IDX      11
#define MD_DATE_MODIFIED_IDX    12
#define MD_DATE_ADDED_IDX       13
	/// Media Properties
#define MD_WIDTH_IDX            14
#define MD_HEIGHT_IDX           15
#define MD_RESOLUTION_IDX       16
#define MD_DEPTH_IDX            17
#define MD_VIEW_ROTATION_IDX    18
#define MD_SAMPLE_COLOR_IDX     19
#define MD_PAGE_IDX             20
#define MD_COLOR_SPACE_IDX      21
#define MD_COMPRESSION_IDX      22
#define MD_PRIMARY_ENCODING_IDX 23
	/// Camerai Information"
#define MD_MAKER_IDX            24
#define MD_MODEL_IDX            25
#define MD_SOFTWARE_IDX         26
#define MD_SOURCE_URL_IDX       27
#define MD_EXIF_VERSION_IDX     28
#define MD_CAPTURE_DATE_IDX     29
#define MD_EXPOSURE_PROGRAM_IDX 30
#define MD_ISO_SPEED_RATING_IDX 31
#define MD_EXPOSURE_BIAS_IDX    32
#define MD_EXPOSURE_TIME_IDX    33
#define MD_APERTURE_IDX         34
#define MD_METERING_MODE_IDX    35
#define MD_LIGHT_SOURCE_IDX     36
#define MD_FLASH_IDX            37
#define MD_FOCAL_LENGTH_IDX     38
#define MD_SENSING_METHOD_IDX   39
#define MD_DIGITAL_ZOOM_IDX     40
	/// GPS
#define MD_LATITUDE_IDX         41
#define MD_LONGITUDE_IDX        42
#define MD_GPS_TIMESTAMP_IDX    43
	//Copyright Properties
#define MD_COPYRIGHT_IDX        44
#define MD_USAGE_RIGHTS_IDX     45
#define MD_COPYRIGHT_URL_IDX    46
	// Image Properties
#define MD_HEADLINE_IDX			47
#define MD_CATEGORY_IDX			48
#define MD_DESCRIPTION_IDX		49
#define MD_KEYWORDS_IDX			50
#define MD_SOURCE_IDX			51
#define MD_INSTRUCTIONS_IDX		52
#define MD_SCENE_IDX			53
#define MD_CREATOR_IDX			54
#define MD_JOBTITLE_IDX			55
#define MD_ADDRESS_IDX			56
#define MD_CITY_IDX				57
#define MD_STATE_IDX			58
#define MD_POSTALCODE_IDX		59
#define MD_COUNTRY_IDX			60
#define MD_PHONE_IDX			62
#define MD_EMAIL_IDX			63
#define MD_WEBSITE_IDX			64

// CSV Database Defines
	/// Asset Properties
#define DB_SEQUENCE_ID_IDX      0
#define DB_FILENAME_IDX         1
#define DB_FILEPATH_IDX         2
#define DB_ORGINAL_NAME_IDX     3
#define DB_UNIQUE_ID_IDX        4
#define DB_LABEL_IDX            5
#define DB_RATING_IDX           6
#define DB_MEDIA_TYPE_IDX       7
#define DB_MD5_IDX              8
#define DB_CRC_IDX              9
#define DB_FILE_SIZE_IDX        10
#define DB_DATE_CREATE_IDX      11
#define DB_DATE_MODIFIED_IDX    12
#define DB_DATE_ADDED_IDX       13
	/// Media Properties
#define DB_SEQUENCE_ID_IDX      0
#define DB_WIDTH_IDX            1
#define DB_HEIGHT_IDX           2
#define DB_RESOLUTION_IDX       3
#define DB_DEPTH_IDX            4
#define DB_VIEW_ROTATION_IDX    5
#define DB_SAMPLE_COLOR_IDX     6
#define DB_PAGE_IDX             7
#define DB_COLOR_SPACE_IDX      8
#define DB_COMPRESSION_IDX      9
#define DB_PRIMARY_ENCODING_IDX 10
	/// Camerai Information"
#define DB_SEQUENCE_ID_IDX      0
#define DB_MAKER_IDX            1
#define DB_MODEL_IDX            2
#define DB_SOFTWARE_IDX         3
#define DB_SOURCE_URL_IDX       4
#define DB_EXIF_VERSION_IDX     5
#define DB_CAPTURE_DATE_IDX     6
#define DB_EXPOSURE_PROGRAM_IDX 7
#define DB_ISO_SPEED_RATING_IDX 8
#define DB_EXPOSURE_BIAS_IDX    9
#define DB_EXPOSURE_TIME_IDX    10
#define DB_APERTURE_IDX         11
#define DB_METERING_MODE_IDX    12
#define DB_LIGHT_SOURCE_IDX     13
#define DB_FLASH_IDX            14
#define DB_FOCAL_LENGTH_IDX     15
#define DB_SENSING_METHOD_IDX   16
#define DB_DIGITAL_ZOOM_IDX     17
	/// GPS
#define DB_SEQUENCE_ID_IDX      0
#define DB_LATITUDE_IDX         1
#define DB_LONGITUDE_IDX        2
#define DB_GPS_TIMESTAMP_IDX    3
	//Copyright Properties
#define DB_SEQUENCE_ID_IDX      0
#define DB_COPYRIGHT_IDX        1
#define DB_USAGE_RIGHTS_IDX     2
#define DB_COPYRIGHT_URL_IDX    3
	// Image Properties
#define DB_HEADLINE_IDX			0
#define DB_CATEGORY_IDX			1
#define DB_DESCRIPTION_IDX		2
#define DB_KEYWORDS_IDX			3
#define DB_SOURCE_IDX			4
#define DB_INSTRUCTIONS_IDX		5
#define DB_SCENE_IDX			6
#define DB_CREATOR_IDX			7
#define DB_JOBTITLE_IDX			8
#define DB_ADDRESS_IDX			9
#define DB_CITY_IDX				10
#define DB_STATE_IDX			11
#define DB_POSTALCODE_IDX		12
#define DB_COUNTRY_IDX			13
#define DB_PHONE_IDX			14
#define DB_EMAIL_IDX			15
#define DB_WEBSITE_IDX			16


//namespace simplearchive {
#define DB_SEQUENCEID	   "SequenceId"
#define DB_MD5	           "Md5"
#define DB_CRC             "Crc"
#define DB_FILENAME        "Filename"
#define DB_ORGINALNAME     "OrginalName"
#define DB_UUID            "Uuid"
#define DB_DESCRIPTION	   "Description"
#define DB_APERTURE        "Aperture"
#define DB_CAPTUREDATE     "CaptureDate"
#define DB_COLORSPACE      "ColorSpace"
#define DB_COMPRESSION     "Compression"
#define DB_COPYRIGHT       "Copyright"
#define DB_COPYRIGHTURL    "CopyrightUrl"
#define DB_DATEADDED       "DateAdded"
#define DB_DATECREATE      "DateCreate"
#define DB_DATEMODIFIED    "DateModified"
#define DB_DEPTH           "Depth"
#define DB_DIGITALZOOM     "DigitalZoom"
#define DB_EXIFVERSION     "ExifVersion"
#define DB_EXPOSUREBIAS    "ExposureBias"
#define DB_EXPOSUREPROGRAM "ExposureProgram"
#define DB_EXPOSURETIME    "ExposureTime"
#define DB_FILEPATH        "Filepath"
#define DB_FILESIZE        "FileSize"
#define DB_FLASH           "Flash"
#define DB_FOCALLENGTH     "FocalLength"
#define DB_GPSTIMESTAMP    "GpsTimeStamp"
#define DB_HEIGHT          "Height"
#define DB_ISOSPEEDRATING  "IsoSpeedRating"
#define DB_LABEL           "Label"
#define DB_LATITUDE        "Latitude"
#define DB_LIGHTSOURCE     "LightSource"
#define DB_LONGITUDE       "Longitude"
#define DB_MAKER           "Maker"
#define DB_MEDIATYPE       "MediaType"
#define DB_METERINGMODE    "MeteringMode"
#define DB_MODEL           "Model"
#define DB_PAGE            "Page"
#define DB_PRIMARYENCODING "PrimaryEncoding"
#define DB_RATING          "Rating"
#define DB_RESOLUTION      "Resolution"
#define DB_SAMPLECOLOR     "SampleColor"
#define DB_SENSINGMETHOD   "SensingMethod"
#define DB_SOFTWARE        "Software"
#define DB_SOURCEURL       "SourceUrl"
#define DB_USAGERIGHTS     "UsageRights"
#define DB_VIEWROTATION    "ViewRotation"
#define DB_WIDTH           "Width"
#define	DB_KEYWORDS        "Keywords"
#define	DB_Tags            "Tags"
#define DB_HEADLINE        "Headline"
#define DB_CATEGORY        "Category"
#define DB_DESCRIPTION     "Description"
#define DB_KEYWORDS        "Keywords"
#define DB_SOURCE          "Source"
#define DB_INSTRUCTIONS    "Instructions"
#define DB_SCENE           "Scene"
#define DB_CREATOR         "Creator"
#define DB_JOBTITLE        "JobTitle"
#define DB_ADDRESS         "Address"
#define DB_CITY            "City"
#define DB_STATE           "State"
#define DB_POSTALCODE      "PostalCode"
#define DB_COUNTRY         "Country"
#define DB_PHONE           "Phone"
#define DB_EMAIL           "Email"
#define DB_WEBSITE         "Website"
/*
keywords are used to find things via search engines; tags are used to find things within your own Archive.
So, you would use keywords to help label your article so when people search on Google, they hopefully use
the words in your archive(or in your archive keyword Metadata).
*/
#endif /* DBDEFINES_H_ */
