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

#include "MetaType.h"
#include "DBDefines.h"

class JpegEXIF;

namespace simplearchive {

class BasicMetadataSchema : public MTTableSchema {
public:
	BasicMetadataSchema() : MTTableSchema(ROW_BASIC_EXIF) {
		
		

		add(MTSchema(MTSchema::Text, DB_FILENAME));
		add(MTSchema(MTSchema::Text, DB_ORGINALNAME));
		add(MTSchema(MTSchema::Text, DB_FILEPATH));
		add(MTSchema(MTSchema::Integer, DB_CRC));
		add(MTSchema(MTSchema::Text, DB_MD5));
		add(MTSchema(MTSchema::Text, DB_UUID));
		
		add(MTSchema(MTSchema::Text, DB_MEDIATYPE));
		add(MTSchema(MTSchema::Integer, DB_FILESIZE));
		add(MTSchema(MTSchema::Date, DB_DATEMODIFIED));
		add(MTSchema(MTSchema::Date, DB_DATECREATE));
		add(MTSchema(MTSchema::Date, DB_DATEADDED));

		add(MTSchema(MTSchema::Text, DB_LABEL));
		add(MTSchema(MTSchema::Text, DB_TITLE));
		add(MTSchema(MTSchema::Text, DB_SUBJECT));
		add(MTSchema(MTSchema::Integer, DB_RATING));
		add(MTSchema(MTSchema::Integer, DB_RATINGPERCENT));
		add(MTSchema(MTSchema::Text, DB_DESCRIPTION));
		add(MTSchema(MTSchema::Text, DB_KEYWORDS));
		add(MTSchema(MTSchema::Integer, DB_VERSION));
		
		add(MTSchema(MTSchema::Text, DB_COMMENT));
		add(MTSchema(MTSchema::Text, DB_AUTHOR));
		add(MTSchema(MTSchema::Date, DB_CAPTUREDATE));
		add(MTSchema(MTSchema::Date, DB_SUBSETIMEORIGINAL));
		add(MTSchema(MTSchema::Text, DB_COPYRIGHT));

		add(MTSchema(MTSchema::Integer, DB_WIDTH));
		add(MTSchema(MTSchema::Integer, DB_HEIGHT));
		add(MTSchema(MTSchema::Integer, DB_VIEWROTATION));
		add(MTSchema(MTSchema::Integer, DB_XRESOLUTION));
		add(MTSchema(MTSchema::Integer, DB_YRESOLUTION));
		add(MTSchema(MTSchema::Text, DB_RESOLUTIONUNIT));
		
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

		add(MTSchema(MTSchema::Text, DB_LIGHTSOURCE));
		add(MTSchema(MTSchema::Text, DB_COLORSPACE));
		add(MTSchema(MTSchema::Text, DB_WHITESBALANCE));
		add(MTSchema(MTSchema::Text, DB_SCENECAPTURETYPE));
		add(MTSchema(MTSchema::Text, DB_CONTRAST));
		add(MTSchema(MTSchema::Text, DB_SATURATION));
		add(MTSchema(MTSchema::Text, DB_SHARPNESS));
		add(MTSchema(MTSchema::Float, DB_BRIGHTNESSVALUE));
		add(MTSchema(MTSchema::Text, DB_SUBJECTDISTANCERANGE));
		add(MTSchema(MTSchema::Float, DB_MAXAPERTURE));
		add(MTSchema(MTSchema::Integer, DB_COMPRESSION));
		add(MTSchema(MTSchema::Integer, DB_BITSPERSAMPLE));
		add(MTSchema(MTSchema::Float, DB_COMPRESSEDBITSPERPIXEL));
		add(MTSchema(MTSchema::Float, DB_FLASHENERGY));
		add(MTSchema(MTSchema::Text, DB_EXIFVERSION));
		add(MTSchema(MTSchema::Text, DB_SOFTWARE));
		add(MTSchema(MTSchema::Integer, DB_PHOTOMETRICINTERPRETATION));
		
		add(MTSchema(MTSchema::Text, DB_LATITUDE));
		add(MTSchema(MTSchema::Text, DB_LONGITUDE));
	}
};

/*
// Description
std::string title;							// XP Title
std::string subject;						// XP Subject
unsigned short rating;						// Short Rating tag used by Windows
unsigned short ratingPercent;				// Rating tag used by Windows, value in percent
std::string ImageDescription;				// Image description
std::string keywords;						// XP Keywords Byte Keywords tag used by Windows, encoded in UCS2
// Origin
std::string comment;						// XP Comment Byte Comment tag used by Windows, encoded in UCS2
std::string author;							// XP Author Byte Author tag used by Windows, encoded in UCS2
std::string DateTime;						// File change date and time
std::string DateTimeOriginal;				// Original file date and time (may not exist)
std::string DateTimeDigitized;				// Digitization date and time (may not exist)
std::string SubSecTimeOriginal;				// Sub-second time that original picture was taken
std::string copyright;						// File copyright information
// Image
unsigned ImageWidth;						// Image width reported in EXIF data
unsigned ImageHeight;						// Image height reported in EXIF data
unsigned short Orientation;					// Image orientation, start of data corresponds to
unsigned short BitsPerSample;				// Number of bits per component
double xResolution;							// XResolution
double yResolution;							// YResolution
unsigned short resolutionUnit;				// The unit for measuring <XResolution> and <YResolution>. The same unit is used for both <XResolution> and <YResolution>.
// If the image resolution is unknown, 2 (inches) is designated.
unsigned compression;						// Compression
// Camera
std::string Make;							// Camera manufacturer's name
std::string Model;							// Camera model
Rational ExposureTime;						// Exposure time in seconds
Rational FNumber;							// F/stop
unsigned short ISOSpeedRatings;				// ISO speed
Rational ExposureBiasValue;					// Exposure bias value in EV
unsigned short exposureProgram;				// The class of the program used by the camera to set exposure when the picture is taken.
double SubjectDistance;						// Distance to focus point in meters
double FocalLength;							// Focal length of lens in millimeters
unsigned short FocalLengthIn35mm;			// Focal length in 35mm film
char Flash;									// 0 = no flash, 1 = flash used
unsigned short meteringMode;				// Metering mode
double DigitalZoomRatio;					// This tag indicates the digital zoom ratio when the image was shot.If the numerator of the recorded value is 0, this indicates that digital zoom was not used.
// Advanced photo
unsigned short lightSource;					// The kind of light source.
unsigned short colorSpace;					// The color space information tag (ColorSpace) is always recorded as the color space specifier
unsigned short  whiteBalance;				// Short This tag indicates the white balance mode set when the image was shot.
unsigned short SceneCaptureType;			// This tag indicates the type of scene that was shot.It can also be used to record the mode in which the image was shot.Note that this differs from the <SceneType> tag.
unsigned short Contrast;					// This tag indicates the direction of contrast processing applied by the camera when the image was shot.
unsigned short Saturation;					// This tag indicates the direction of saturation processing applied by the camera when the image was shot.
unsigned short Sharpness;					// This tag indicates the direction of sharpness processing applied by the camera when the image was shot.
Rational brightnessValue;					// The value of brightness. The unit is the APEX(Additive System of Photographic Exposure) setting. Ordinarily it is given in the range of - 99.99 to 99.99.Note
// that if the numerator of the recorded value is hex FFFFFFFF, Unknown shall be indicated.
unsigned short SubjectDistanceRange;		// This tag indicates the distance to the subject.
Rational maxApertureValue;					// The smallest F number of the lens. The unit is the APEX value. Ordinarily it is given in the range of 00.00 to 99.99,
// but it is not limited to this range.
Rational compressedBitsPerPixel;			// Specific to compressed data; states the compressed bits per pixel. The compression mode used for a compressed image is indicated in unit bits per pixel.
double flashEnergy;							// Amount of flash energy(BCPS).
std::string exifVersion;					// The version of this standard supported. Nonexistence of this field is taken to mean nonconformance to the standard.
std::string Software;						// Software used. This tag records the name and version of the software or firmware of the camera or image input device used to generate the image.
// The detailed format is not specified, but it is recommended that the example shown below be followed. When the field is left blank, it is
// treated as unknown.
unsigned short photometricInterpretation;	// The pixel composition. In JPEG compressed data a JPEG marker is used instead of this tag.
// GPS
*/

class BasicMetadata : public MTRow {
	
	static BasicMetadataSchema m_BasicMetadataSchema;

// file	
	std::string m_name;							// Current File name
	std::string m_orginalName;					// Orginal File name
	std::string m_path;							// Relative path to the image within the archive 
	unsigned long m_crc;						// A cyclic redundancy check (CRC) is an error-detecting code commonly used in digital networks and storage devices to detect
												// accidental changes to raw data
	std::string m_md5;							// The MD5 algorithm is a widely used hash function producing a 128-bit hash value. Although MD5 was initially designed to be
												// used as a cryptographic hash function, it has been found to suffer from extensive vulnerabilities. It can still be used as
												// a checksum to verify data integrity, but only against unintentional corruption, which is its use in this case.
	std::string m_uuid;							// A universally unique identifier (UUID) is a 128-bit number used to identify information in computer systems. The term
												// globally unique identifier (GUID) is also used.
												// When generated according to the standard methods, UUIDs are for practical purposes unique, without depending for their
												// uniqueness on a central registration authority or coordination between the parties generating them, unlike most other numbering schemes.
												//While the probability that a UUID will be duplicated is not zero, it is so close to zero as to be negligible.

	std::string m_mediaType;					// Define the Type of image media. 
	unsigned long m_size;						// Size of the image in bytes.
	ExifDateTime m_modTime;						// Last time the image was modified
	ExifDateTime m_createTime;					// Time the image was created
// Description
	std::string m_label;						// A user defined label for the image 
	std::string title;							// XP Title
	std::string subject;						// XP Subject
	unsigned short m_rating;					// Short Rating tag used by Windows
	unsigned short ratingPercent;				// Rating tag used by Windows, value in percent
	std::string ImageDescription;				// Image description
	std::string keywords;						// XP Keywords Byte Keywords tag used by Windows, encoded in UCS2
	unsigned m_version;							// Current version. If Zero '0' then the image is the master, else is a derivitive.
// Origin
	std::string m_comment;						// XP Comment Byte Comment tag used by Windows, encoded in UCS2
	std::string m_author;						// XP Author Byte Author tag used by Windows, encoded in UCS2				
	
	//ExifDateTime m_dateTimeOriginal;			// Original file date and time (may not exist)
	ExifDateTime m_dateTimeDigitized;			// Digitization date and time (may not exist)
	std::string m_subSecTimeOriginal;			// Sub-second time that original picture was taken
	std::string m_copyright;					// File copyright information

// Image
	unsigned m_imageWidth;						// Image width reported in EXIF data
	unsigned m_imageHeight;						// Image height reported in EXIF data
	unsigned short m_orientation;				// Image orientation, start of data corresponds to
	unsigned short BitsPerSample;				// Number of bits per component
	int xResolution;							// XResolution
	int yResolution;							// YResolution
	unsigned short resolutionUnit;				// The unit for measuring <XResolution> and <YResolution>. The same unit is used for both <XResolution> and <YResolution>.
												// If the image resolution is unknown, 2 (inches) is designated.
	unsigned compression;						// Compression

// Camera	
	std::string m_make;							// Camera manufacturer's name
	std::string m_model;						// Camera model
	double m_exposureTime;						// Exposure time in seconds
	double m_fNumber;							// F/stop
	unsigned short m_isoSpeedRatings;			// ISO speed
	double m_exposureBiasValue;					// Exposure bias value in EV
	unsigned short m_exposureProgram;			// The class of the program used by the camera to set exposure when the picture is taken.
	double m_subjectDistance;					// Distance to focus point in meters
	double m_focalLength;						// Focal length of lens in millimeters
	unsigned short m_focalLengthIn35mm;			// Focal length in 35mm film
	char m_flash;								// 0 = no flash, 1 = flash used
	unsigned short m_meteringMode;				// Metering mode
	double m_digitalZoomRatio;					// This tag indicates the digital zoom ratio when the image was shot.If the numerator of the recorded value is 0, this indicates that digital zoom was not used.

// Advanced photo 	
	unsigned short m_lightSource;				// The kind of light source.
	unsigned short m_colorSpace;				// The color space information tag (ColorSpace) is always recorded as the color space specifier
	unsigned short  m_whiteBalance;				// Short This tag indicates the white balance mode set when the image was shot.
	unsigned short m_sceneCaptureType;			// This tag indicates the type of scene that was shot.It can also be used to record the mode in which the image was shot.Note that this differs from the <SceneType> tag.
	unsigned short m_contrast;					// This tag indicates the direction of contrast processing applied by the camera when the image was shot.
	unsigned short m_saturation;				// This tag indicates the direction of saturation processing applied by the camera when the image was shot.
	unsigned short m_sharpness;					// This tag indicates the direction of sharpness processing applied by the camera when the image was shot.
	double m_brightnessValue;					// The value of brightness. The unit is the APEX(Additive System of Photographic Exposure) setting. Ordinarily it is given in the range of - 99.99 to 99.99.Note
												// that if the numerator of the recorded value is hex FFFFFFFF, Unknown shall be indicated.
	unsigned short m_subjectDistanceRange;		// This tag indicates the distance to the subject.
	double m_maxApertureValue;					// The smallest F number of the lens. The unit is the APEX value. Ordinarily it is given in the range of 00.00 to 99.99,
												// but it is not limited to this range.
	double m_compressedBitsPerPixel;			// Specific to compressed data; states the compressed bits per pixel. The compression mode used for a compressed image is indicated in unit bits per pixel.
	double m_flashEnergy;						// Amount of flash energy(BCPS).
	std::string m_exifVersion;					// The version of this standard supported. Nonexistence of this field is taken to mean nonconformance to the standard.
	std::string m_software;						// Software used. This tag records the name and version of the software or firmware of the camera or image input device used to generate the image.
												// The detailed format is not specified, but it is recommended that the example shown below be followed. When the field is left blank, it is
												// treated as unknown.
	unsigned short m_photometricInterpretation;	// The pixel composition. In JPEG compressed data a JPEG marker is used instead of this tag.

// gps
	std::string  m_latitude;
	std::string  m_longitude;
	std::string  m_altitude;
	
	bool m_exifFound;
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
	BasicMetadata();
	~BasicMetadata();

	unsigned long getCrc() const;
	void setCrc(unsigned long crc);
	
	unsigned long getSize() const;
	void setSize(unsigned long crc);

	const std::string& getMd5() const;
	void setMd5(const std::string& md5);

	const std::string& getMediaType() const;
	void setMediaType(const std::string& mediaType);

	const std::string& getUuid() const;
	void setUuid(const std::string& uuid);

	const std::string& getName() const;
	void setName(const std::string& name);

	const std::string& getLabel() const;
	void setLabel(const std::string& name);

	const std::string& getOrginalName() const;
	void setOrginalName(const std::string& name);

	const std::string& getPath() const;
	void setPath(const std::string& path);

	const int getRating() const;
	void setRating(const int rating);

	const std::string getExt() const;
	void setCreateTime(ExifDateTime &time);
	const ExifDateTime &getCreateTime() const;
	void setModTime(ExifDateTime &time);
	const ExifDateTime &getModTime() const;
	void setAddTime(ExifDateTime &time);
	const ExifDateTime &getAddTime() const;
	void setVersion(int v);
	const int getVersion() const;

	const std::string& getAltitude() const;
	void setAltitude(const std::string& altitude);
	const std::string& getBitsPerSample() const;
	void setBitsPerSample(const std::string& bitsPerSample);
	const std::string& getCopyright() const;
	void setCopyright(const std::string& copyright);
//	const std::string& getDateTime() const;
//	void setDateTime(const std::string& dateTime);

	const ExifDateTime& getDateTimeDigitized() const;
	bool getDateTimeDigitizedNull() const;
	void setDateTimeDigitized(ExifDateTime& dateTimeDigitized);

//	const std::string& getDateTimeOriginal() const;
//	void setDateTimeOriginal(const std::string& dateTimeOriginal);
    const std::string& getExposureBiasValue() const;
	void setExposureBiasValue(const std::string& exposureBiasValue);
	const std::string& getExposureProgram() const;
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

	const std::string& getXResolution() const;
	const std::string& getYResolution() const;
	const std::string& getResolutionUnit() const;

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
	const std::string& getDigitalZoomRatio() const;
	const std::string& getLightSource() const;
	const std::string& getColorSpace() const;
	const std::string& getWhiteBalance() const;
	const std::string& getSceneCaptureType() const;
	const std::string& getContrast() const;
	const std::string& getSaturation() const;
	const std::string& getSharpness() const;
	const std::string& getBrightnessValue() const;
	const std::string& getSubjectDistanceRange() const;
	const std::string& getMAXApertureValue() const;
	const std::string& getCompression() const;
	const std::string& getCompressedBitsPerPixel() const;
	const std::string& getFlashEnergy() const;
	const std::string& getExifVersion() const;
	const std::string& getPhotometricInterpretation() const;
	
	void copyJpegExif(JpegEXIF &exifInfo);
	const bool isExifFound() const {
		return m_exifFound;
	}
	//void print();

	void print_();
};


} /* namespace simplearchive */
