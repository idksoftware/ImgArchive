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
#include <cstdlib>
#include <memory>
#include "ExifDateTime.h"
#include "MetadataObject.h"
#include "BasicMetadata.h"
class EXIFInfo;


namespace simplearchive {

	typedef std::unique_ptr<BasicMetadata> BasicMetadata_Ptr;

//	class FileInfo;
	
	class ExifObject;
	/// This class contains the primary image identification infomation.
	/// This will be put into the Metadata Object
	
	class BasicMetadataFactory
	{
		bool m_exifFound; //< Set true if exif found in this image file

		/*
		// File Properties
		std::string m_name; 
		std::string m_path;
		unsigned long m_crc;
		std::string m_mediaType;
		std::string m_md5;
		std::string m_uuid;
		unsigned long m_size;
		ExifDateTime m_modTime;
		ExifDateTime m_createTime;

		std::string split(std::string pathStr);
		// Basic exif properties
		char ByteAlign;                   // 0 = Motorola byte alignment, 1 = Intel 
		std::string ImageDescription;     // Image description
		std::string Make;                 // Camera manufacturer's name
		std::string Model;                // Camera model
		unsigned short Orientation;       // Image orientation, start of data corresponds to
		// 0: unspecified in EXIF data
		// 1: upper left of image
		// 3: lower right of image
		// 6: upper right of image
		// 8: lower left of image
		// 9: undefined
		unsigned short BitsPerSample;     // Number of bits per component
		std::string Software;             // Software used
		ExifDateTime DateTime;             // File change date and time
		ExifDateTime DateTimeOriginal;     // Original file date and time (may not exist)
		ExifDateTime DateTimeDigitized;    // Digitization date and time (may not exist)
		std::string SubSecTimeOriginal;   // Sub-second time that original picture was taken
		std::string Copyright;            // File copyright information
		double ExposureTime;              // Exposure time in seconds
		double FNumber;                   // F/stop
		unsigned short ISOSpeedRatings;   // ISO speed
		double ShutterSpeedValue;         // Shutter speed (reciprocal of exposure time)
		double ExposureBiasValue;         // Exposure bias value in EV
		double SubjectDistance;           // Distance to focus point in meters
		double FocalLength;               // Focal length of lens in millimeters
		unsigned short FocalLengthIn35mm; // Focal length in 35mm film
		char Flash;                       // 0 = no flash, 1 = flash used
		unsigned short MeteringMode;      // Metering mode
		// 1: average
		// 2: center weighted average
		// 3: spot
		// 4: multi-spot
		// 5: multi-segment
		unsigned ImageWidth;              // Image width reported in EXIF data
		unsigned ImageHeight;             // Image height reported in EXIF data
		struct Geolocation_t {            // GPS information embedded in file
			double Latitude;                  // Image latitude expressed as decimal
			double Longitude;                 // Image longitude expressed as decimal
			double Altitude;                  // Altitude in meters, relative to sea level
			char AltitudeRef;                 // 0 = above sea level, -1 = below sea level
			struct Coord_t {
				double degrees;
				double minutes;
				double seconds;
				char direction;
			} LatComponents, LonComponents;   // Latitude, Longitude expressed in deg/min/sec 
		} GeoLocation;
		std::string LatitudeStr;
		std::string LongitudeStr;
		std::string AltitudeStr;
		*/
		//BasicMetadata_Ptr m_BasicMetadata;
	public:
		BasicMetadataFactory();
		~BasicMetadataFactory();
		//BasicMetadataFactory(std::string &path) {
		//	m_path = path;
		//}
		BasicMetadataFactory(const char *dataString) {
			// Note UUID missing
			/*
			std::string m_data = dataString;
			int delim1 = m_data.find_first_of(':');
			int delim2 = m_data.find_first_of(delim1, ':');
			std::string crcStr = m_data.substr(0, delim1);
			m_crc = std::stoul(crcStr.c_str(), NULL, 16);
			std::string name = m_data.substr(delim1, delim2);
			std::string md5 = m_data.substr(delim2, m_data.length());
			*/
		}

		BasicMetadataFactory(const char *path, unsigned long crc, const char *md5) {
			/*
			m_path = path;
			m_name = split(m_path);
			m_crc = crc;
			m_md5 = md5;
			*/
		}

		static BasicMetadata_Ptr make(std::string &path);

		//BasicMetadata &getBasicMetadata() {
		//	return *m_BasicMetadata;
		//}
	

	const bool isExifFound() const {
		return m_exifFound;
	}
	static void copy(BasicMetadata &BasicMetadata, JpegEXIF &jpegEXIF);
	//void copyBasicMetadata(EXIFInfo &exifInfo);
	ExifObject *externalExifTool(std::string &path);
	void setMetadataObject(MetadataObject &metadataObject);
};

}
