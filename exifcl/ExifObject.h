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

#ifndef EXIFOBJECT_H_
#define EXIFOBJECT_H_
#include <string>
#include "MetaType.h"
#include "DBDefines.h"

/*
#define COMPRESSION		"Compression"
#define DEPTH			"Depth"
#define MIMETYPE		"MIMEType"
#define DIGITALZOOM		"DigitalZoom"
#define EXIFVERSION		"ExifVersion"
#define EXPOSUREPROGRAM		"ExposureProgram"
#define GPSTIMESTAMP		"GpsTimeStamp"
#define LIGHTSOURCE		"LightSource"
#define PAGE			"Page"
#define PRINARYENCODING		"PrimaryEncoding"
#define RESOLUTION		"Resolution"
#define SAMPLECOLOR		"SampleColor"
#define SENSINGMETHOD		"SensingMethod"


#define IMAGEDESCRIPTION	"ImageDescription"		//< Image description
#define MAKE			"Make"				//< Camera manufacturer's name
#define MODEL			"Model"				//< Camera model
#define ORIENTATION		"Orientation"			//< Image orientation, start of data corresponds to
								// 0: unspecified in EXIF data
								// 1: upper left of image
								// 3: lower right of image
								// 6: upper right of image
								// 8: lower left of image
								// 9: undefined
#define BITSPERSAMPLE		"BitsPerSample"			//< Number of bits per component
#define SOFTWARE		"Software"			//< Software used
#define DATETIME		"DateTime"			//< File change date and time
#define DATETIMEORIGINAL	"DateTimeOriginal"		//< Original file date and time (may not exist)
#define DATETIMEDIGITIZED	"DateTimeDigitized"		//< Digitization date and time (may not exist)
#define SUBSECTIMEORIGINAL	"SubSecTimeOriginal"		//< Sub-second time that original picture was taken
#define COPYRIGHT		"Copyright"			//< File copyright information
#define EXPOSURETIME		"ExposureTime"			//< Exposure time in seconds
#define FNUMBER			"FNumber"			//< F/stop
#define ISOSPEEDRATINGS		"IsoSpeedRatings"		//< ISO speed
#define SHUTTERSPEEDVALUE	"ShutterSpeedValue"		//< Shutter speed (reciprocal of exposure time)
#define EXPOSUREBIASVALUE	"ExposureBiasValue"		//< Exposure bias value in EV
#define SUBJECTDISTANCE		"SubjectDistance"		//< Distance to focus point in meters
#define FOCALLENGTH		"FocalLength"			//< Focal length of lens in millimeters
#define FOCALLENGTHIN35MM	"FocalLengthIn35mm"		//< Focal length in 35mm film
#define FLASH			"Flash"				//< 0 = no flash, 1 = flash used
#define METERINGMODE		"MeteringMode"			//< Metering mode
								// 1: average
								// 2: center weighted average
								// 3: spot
								// 4: multi-spot
								// 5: multi-segment

#define LATITUDE		"Latitude"			//< Image latitude expressed as decimal
#define LONGITUDE		"Longitude"
#define ALTITUDE		"Altitude"			//< Altitude in meters, relative to sea level

#define IMAGEWIDTH		"ImageWidth"
#define IMAGELENGTH		"ImageLength"

#define COLORSPACE		"ColorSpace"
#define EXIFOFFSET		"ExifOffset"
//#define EXIFIMAGEWIDTH		"ExifImageWidth"
#define DATETIME		"DateTime"
#define IMAGEHEIGHT		"ImageHeight"
#define SAMPLESPERPIXEL		"SamplesPerPixel"
#define COPYRIGHT		"Copyright"
#define YRESOLUTION		"YResolution"
#define XRESOLUTION		"XResolution"
#define ARTIST			"Artist"
*/

namespace simplearchive {

	class ExifObjectSchema : public MTTableSchema {
	public:
		ExifObjectSchema() noexcept : MTTableSchema(ROW_EXTERNAL_EXIF) {
/*
			add(MTSchema(MTSchema::Text, DB_ORGINALNAME));
			add(MTSchema(MTSchema::Text, DB_LABEL));
			add(MTSchema(MTSchema::Integer, DB_RATING));
			add(MTSchema(MTSchema::Text, DB_MEDIATYPE));
			add(MTSchema(MTSchema::Date, DB_DATECREATE));
			add(MTSchema(MTSchema::Text, DB_DESCRIPTION));
			add(MTSchema(MTSchema::Integer, DB_WIDTH));
			add(MTSchema(MTSchema::Integer, DB_HEIGHT));
			add(MTSchema(MTSchema::Text, DB_RESOLUTION));
//			add(MTSchema(MTSchema::Text, DB_DEPTH));
			add(MTSchema(MTSchema::Text, DB_VIEWROTATION));
			add(MTSchema(MTSchema::Text, DB_SAMPLECOLOR));
//			add(MTSchema(MTSchema::Text, DB_PAGE));
			add(MTSchema(MTSchema::Text, DB_COLORSPACE));
			add(MTSchema(MTSchema::Text, DB_COMPRESSION));
//			add(MTSchema(MTSchema::Text, DB_PRIMARYENCODING));
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
			add(MTSchema(MTSchema::Text, DB_MEDIATYPE));
			*/
			// Assest 
			// file
		
			add(MTSchema(MTSchema::Text, DB_MEDIATYPE));
			
			//add(MTSchema(MTSchema::Date, DB_DATECREATE));
			//add(MTSchema(MTSchema::Date, DB_DATEMODIFIED));
			//add(MTSchema(MTSchema::Date, DB_DATEADDED));
			// Description
			add(MTSchema(MTSchema::Text, DB_DESCRIPTION));
			add(MTSchema(MTSchema::Text, DB_LABEL));
			add(MTSchema(MTSchema::Text, DB_TITLE));
			add(MTSchema(MTSchema::Text, DB_SUBJECT));
			add(MTSchema(MTSchema::Integer, DB_RATING));
			add(MTSchema(MTSchema::Integer, DB_RATINGPERCENT));
			add(MTSchema(MTSchema::Text, DB_TAGS));// template
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


/**
 * @brief This class contains the EXIF data for an image.
 */
class ExifObject : public MTRow {
	static ExifObjectSchema m_exifObjectSchema;
	friend class ExternalExifMapper;
	/*
	std::string m_compression;
	std::string m_depth;
	std::string m_mimetype;	
	std::string m_digitalZoom;
	std::string m_exifVersion;
	std::string m_exposureProgram;
	std::string m_artist;
	std::string m_lightSource;
	std::string m_page;
	std::string m_primaryEncoding;
	std::string m_resolution;
	std::string m_sampleColor;
	std::string m_sensingMethod;
	std::string m_imageDescription;     // Image description
	std::string m_make;                 // Camera manufacturer's name
	std::string m_model;                // Camera model
	std::string m_orientation;       // Image orientation, start of data corresponds to
	// 0: unspecified in EXIF data
	// 1: upper left of image
	// 3: lower right of image
	// 6: upper right of image
	// 8: lower left of image
	// 9: undefined
	std::string m_bitsPerSample;     // Number of bits per component
	std::string m_software;             // Software used
	std::string m_dateTime;             // File change date and time
	std::string m_dateTimeOriginal;     // Original file date and time (may not exist)
	std::string m_dateTimeDigitized;    // Digitization date and time (may not exist)
	std::string m_subSecTimeOriginal;   // Sub-second time that original picture was taken
	std::string m_copyright;            // File copyright information
	std::string m_exposureTime;              // Exposure time in seconds
	std::string m_fNumber;                   // F/stop
	std::string m_isoSpeedRatings;   // ISO speed
	std::string m_shutterSpeedValue;         // Shutter speed (reciprocal of exposure time)
	std::string m_exposureBiasValue;         // Exposure bias value in EV
	std::string m_subjectDistance;           // Distance to focus point in meters
	std::string m_focalLength;               // Focal length of lens in millimeters
	std::string m_focalLengthIn35mm; // Focal length in 35mm film
	std::string m_flash;                       // 0 = no flash, 1 = flash used
	std::string m_meteringMode;      // Metering mode
	// 1: average
	// 2: center weighted average
	// 3: spot
	// 4: multi-spot
	// 5: multi-segment
	std::string m_imageWidth;              // Image width reported in EXIF data
	std::string m_imageHeight;             // Image height reported in EXIF data
	std::string m_colorSpace;
	std::string m_latitude;                  // Image latitude expressed as decimal
	std::string m_longitude;                 // Image longitude expressed as decimal
	std::string m_altitude;                  // Altitude in meters, relative to sea level
	std::string m_xResolution;
	std::string m_yResolution;
	*/
public:

	ExifObject();
	virtual ~ExifObject();
	/*
	const std::string& getArtist() const {
		return m_artist;
	}

	const std::string& getColorSpace() const {
		return m_colorSpace;
	}

	const std::string& getCopyright() const {
		return m_copyright;
	}

	const std::string& getDateTime() const {
		return m_dateTime;
	}

	const std::string& getImageHeight() const {
		return m_imageHeight;
	}

	const std::string& getImageWidth() const {
		return m_imageWidth;
	}

	const std::string& getXResolution() const {
		return m_xResolution;
	}

	const std::string& getYResolution() const {
		return m_yResolution;
	}

	const std::string& getCompression() const {
		return m_compression;
	}

	const std::string& getDepth() const {
		return 	m_depth;
	}

	const std::string& getDigitalZoom() const {
		return 	m_digitalZoom;
	}

	const std::string& getExifVersion() const {
		return 	m_exifVersion;
	}
	*/
	void print();
	/*
	const std::string& getAltitude() const;
	void setAltitude(const std::string& altitude);
	void setArtist(const std::string& artist);
	const std::string& getBitsPerSample() const;
	void setBitsPerSample(const std::string& bitsPerSample);
	void setColorSpace(const std::string& colorSpace);
	void setCopyright(const std::string& copyright);
	void setDateTime(const std::string& dateTime);
	const std::string& getDateTimeDigitized() const;
	void setDateTimeDigitized(const std::string& dateTimeDigitized);
	const std::string& getDateTimeOriginal() const;
	void setDateTimeOriginal(const std::string& dateTimeOriginal);
	const std::string& getExposureBiasValue() const;
	void setExposureBiasValue(const std::string& exposureBiasValue);
	void setExposureProgram(const std::string& exposureBiasValue);
	const std::string& ExifObject::getExposureProgram() const;
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
	void setImageHeight(const std::string& imageHeight);
	void setImageWidth(const std::string& imageWidth);
	const std::string& getIsoSpeedRatings() const;
	void setIsoSpeedRatings(const std::string& isoSpeedRatings);
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
	void setXResolution(const std::string& xResolution);
	void setYResolution(const std::string& yResolution);
	*/
};

} /* namespace simplearchive */
#endif /* EXIFOBJECT_H_ */
