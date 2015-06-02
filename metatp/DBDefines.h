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

#ifndef DBDEFINES_H_
#define DBDEFINES_H_
/**
 * This file defines the key strings used by the Archiver. This allows consistent
 * key names to be used in the application.
 */

//namespace simplearchive {
#define TABLE_METADATA			"metadata"
#define TABLE_ASSETPROPERTIES	"asset_properties"
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
#define DB_DESCRIPTION_IDX      14
	/// Media Properties
#define DB_WIDTH_IDX            15
#define DB_HEIGHT_IDX           16
#define DB_RESOLUTION_IDX       17
#define DB_DEPTH_IDX            18
#define DB_VIEW_ROTATION_IDX    19
#define DB_SAMPLE_COLOR_IDX     20
#define DB_PAGE_IDX             21
#define DB_COLOR_SPACE_IDX      22
#define DB_COMPRESSION_IDX      23
#define DB_PRIMARY_ENCODING_IDX 24
	/// Camerai Information"
#define DB_MAKER_IDX            25
#define DB_MODEL_IDX            26
#define DB_SOFTWARE_IDX         27
#define DB_SOURCE_URL_IDX       28
#define DB_EXIF_VERSION_IDX     29
#define DB_CAPTURE_DATE_IDX     30
#define DB_EXPOSURE_PROGRAM_IDX 31
#define DB_ISO_SPEED_RATING_IDX 32
#define DB_EXPOSURE_BIAS_IDX    33
#define DB_EXPOSURE_TIME_IDX    34
#define DB_APERTURE_IDX         35
#define DB_METERING_MODE_IDX    36
#define DB_LIGHT_SOURCE_IDX     37
#define DB_FLASH_IDX            38
#define DB_FOCAL_LENGTH_IDX     39
#define DB_SENSING_METHOD_IDX   40
#define DB_DIGITAL_ZOOM_IDX     41
	/// GPS
#define DB_LATITUDE_IDX         42
#define DB_LONGITUDE_IDX        43
#define DB_GPS_TIMESTAMP_IDX    44
	//Copyright Properties
#define DB_COPYRIGHT_IDX        45
#define DB_USAGE_RIGHTS_IDX     46
#define DB_COPYRIGHT_URL_IDX    47

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
#endif /* DBDEFINES_H_ */
