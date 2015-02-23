/*
 * ExifObject.h
 *
 *  Created on: May 30, 2014
 *      Author: wzw7yn
 */

#ifndef EXIFOBJECT_H_
#define EXIFOBJECT_H_
#include <string>

#define COMPRESSION		"Compression"
#define DEPTH			"Depth"
#define MIMETYPE		"MIMEType"
#define DIGITALZOOM		"DigitalZoom"
#define EXIFVERSION		"ExifVersion"
#define EXPOSUREPROGRAM		"ExposureProgram"
#define GPSTIMESTAMP		"GpsTimeStamp"
#define LIGHTSOURCE		"LightSource"
#define PAGE			"Page"
#define PRINARYENCODING		"PrinaryEncoding"
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

namespace simplearchive {
/**
 * @brief This class contains the EXIF data for an image.
 */
class ExifObject {
	friend class ExternalExifMapper;
	
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
//	std::string m_artist;
	std::string m_xResolution;
	std::string m_yResolution;
	
public:

	ExifObject();
	virtual ~ExifObject();

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
	
	void print();
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
};

} /* namespace simplearchive */
#endif /* EXIFOBJECT_H_ */