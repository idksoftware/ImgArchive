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

#include "BasicMetadataFactory.h"
#include "CIDKCrc.h"
#include "md5.h"
#include <string>
#include <fstream>
#include "SimpleExifReader.h"
#include "CLogger.h"
#include "IDKUuid.h"
#include <iostream>
#include "ExternalComand.h"
#include "ExifObject.h"
#include "AppConfig.h"
#include "SAUtils.h"
#include "ExifDateTime.h"
#include "ImageExtentions.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

	
	BasicMetadataFactory::BasicMetadataFactory()
	{
		m_exifFound = false;
	}


	BasicMetadataFactory::~BasicMetadataFactory()
	{
	}

#ifdef NOT_USED
	void BasicMetadataFactory::copyBasicMetadata(EXIFInfo &exifInfo) {

		/*
		Make = exifInfo.Make;
		Model = exifInfo.Model;
		Software = exifInfo.Software;
		BitsPerSample = exifInfo.BitsPerSample;
		ImageWidth = exifInfo.ImageWidth;
		ImageHeight = exifInfo.ImageHeight;
		ImageDescription = exifInfo.ImageDescription;
		Orientation = exifInfo.Orientation;
		Copyright = exifInfo.copyright;
		DateTime = ExifDateTime(exifInfo.DateTime.c_str());
		DateTimeOriginal = ExifDateTime(exifInfo.DateTimeOriginal.c_str());
		DateTimeDigitized = ExifDateTime(exifInfo.DateTimeDigitized.c_str());
		SubSecTimeOriginal = exifInfo.SubSecTimeOriginal;
		ExposureTime = exifInfo.ExposureTime;
		FNumber = exifInfo.FNumber;
		ISOSpeedRatings = exifInfo.ISOSpeedRatings;
		SubjectDistance = exifInfo.SubjectDistance;
		ExposureBiasValue = exifInfo.ExposureBiasValue;
		Flash = exifInfo.Flash;
		MeteringMode = exifInfo.MeteringMode;
		FocalLength = exifInfo.FocalLength;
		FocalLengthIn35mm = exifInfo.FocalLengthIn35mm;
		ShutterSpeedValue = exifInfo.ShutterSpeedValue;

		m_BasicMetadata->copyBasicMetadata(exifInfo);
		*/
		/*
		LatitudeStr
		LongitudeStr
		AltitudeStr
		*/
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
	}
#endif

	void BasicMetadataFactory::copy(BasicMetadata &basicMetadata, JpegEXIF &jpegEXIF)
	{
		basicMetadata.copyJpegExif(jpegEXIF);
		basicMetadata.print();
	}

	

	static std::string getFileContents(const char *filename)
	{
		
		std::ifstream in(filename, std::ios::in | std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize((const unsigned int)in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return(contents);
		}
		throw(errno);
	}

	static std::string splitName(std::string pathStr) {
		unsigned int pos = pathStr.find_last_of("/");
		if (pos == std::string::npos) {
			pos = pathStr.find_last_of("\\");
			if (pos == std::string::npos) {
				return pathStr;
			}
		}
		std::string filename = pathStr.substr(pos + 1, pathStr.length() - (pos + 1));
		return filename;
	}

	

	BasicMetadata_Ptr BasicMetadataFactory::make(std::string &p) {
		CLogger &logger = CLogger::getLogger();

		
		BasicMetadata_Ptr basicMetadata(new BasicMetadata);
		/*
		SIADBLite::SIADatabase &database = SIADBLite::SIADatabase::get();
		DBLite::SharedDBLiteTable bmTable = database.getTable(ROW_BASIC_EXIF);
		if (!bmTable) {
			return nullptr;
		}
		DBLite::SharedDBLiteRow basicMetadata = bmTable->makeRow();
		if (!basicMetadata) {
			return nullptr;
		}
		*/
		CIDKCrc Crc;
		
		// Replace with FileInfo.

		basicMetadata->setPath(p);
		std::string name = splitName(p);
		basicMetadata->setOrginalName(name);
		basicMetadata->setName(name);
		basicMetadata->setLabel(name);
		logger.log(LOG_OK, CLogger::Level::INFO, "Image: %s", p.c_str());
		std::string buf = getFileContents(p.c_str());
		MD5 md5Obj(buf);
		std::string md5 = md5Obj.hexdigest();
		basicMetadata->setMd5(md5);
		logger.log(LOG_OK, CLogger::Level::INFO, "MD5 of image: %s is %s", p.c_str(), md5.c_str());
		CIDKUuid uuidObj;
		std::string uuid = uuidObj.GetUuid();
		basicMetadata->setUuid(uuid);
		logger.log(LOG_OK, CLogger::Level::INFO, "UUID of image: %s is %s", p.c_str(), uuid.c_str());
		int size = buf.length();
		basicMetadata->setSize(size);

		logger.log(LOG_OK, CLogger::Level::INFO, "Size of image: %s is %d", p.c_str(), size);
		unsigned long crc = Crc.crc((unsigned char *)buf.c_str(), size);
		basicMetadata->setCrc(crc);
		logger.log(LOG_OK, CLogger::Level::INFO, "CRC from image: %s is %x", p.c_str(), crc);
		
		basicMetadata->setRating(0);

		ImageExtentions& ie = ImageExtentions::get();
		ExtentionItem ei = ie.find(name.c_str());
		basicMetadata->setMediaType(ei.getMimeType());
		ExifDateTime createTime;
		createTime = SAUtils::createTime(p.c_str());
		basicMetadata->setCreateTime(createTime);
		logger.log(LOG_OK, CLogger::Level::INFO, "Create time of image: %s", createTime.toLogString().c_str());
		ExifDateTime modTime;
		modTime = SAUtils::modTime(p.c_str());
		basicMetadata->setModTime(modTime);
		ExifDateTime addTime;
		basicMetadata->setAddTime(addTime);
		basicMetadata->setVersion(0);
		logger.log(LOG_OK, CLogger::Level::INFO, "Internal Exif reader, reading Exif from image: %s", p.c_str());
		EXIFInfo exifInfo;
		JpegExif_Ptr jpegExifPtr = exifInfo.parseFrom(buf);
		switch (jpegExifPtr->error) {
		case PARSE_EXIF_SUCCESS:
			logger.log(LOG_OK, CLogger::Level::INFO, "Completed reading Exif from file: %s", p.c_str());
			basicMetadata->copyJpegExif(*jpegExifPtr);
			break;
		case PARSE_EXIF_ERROR_NO_JPEG:
			logger.log(LOG_OK, CLogger::Level::WARNING, "No JPEG markers found in buffer, possibly a RAW file: %s", p.c_str());
			// No EXIF header found in JPEG file.
			break;
		case PARSE_EXIF_ERROR_NO_EXIF:
			logger.log(LOG_OK, CLogger::Level::WARNING, "No JPEG markers found in buffer, possibly JPEG file with no EXIF or a RAW file: %s", p.c_str());
			break;
		case PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN:
			logger.log(LOG_OK, CLogger::Level::WARNING, "Byte alignment specified in EXIF file was unknown in file: %s", p.c_str());
			break;
		case PARSE_EXIF_ERROR_CORRUPT:
			logger.log(LOG_OK, CLogger::Level::WARNING, "EXIF header was found, but data was corrupted in file: %s", p.c_str());
			break;
		default:
			logger.log(LOG_OK, CLogger::Level::WARNING, "Exif invalid: %s", p.c_str());
			break;
		}
		
		//basicMetadata->print();
		
		return basicMetadata;
	}

	/*
	ExifObject *BasicMetadataFactory::externalExifTool(std::string &path) {
		CLogger &logger = CLogger::getLogger();
		
		AppConfig config = AppConfig::get();
		ExternalComand externalComand(config.getTempPath());
		
		const char *externalCommandLine;
		if (!(externalCommandLine = config.getExternalCommandLine())) {
			//logger.log(LOG_OK, CLogger::ERROR, "Exif command line not found\n");
			m_exifFound = false;
			return false;
		}
		const char *exifMapPath;
		if (!(exifMapPath = config.getExifMapPath())) {
			//logger.log(LOG_OK, CLogger::ERROR, "Exif map path not found");
			m_exifFound = false;
			return false;
		}
		logger.log(LOG_OK, CLogger::FINE, "Exif command line found \"%s\"", externalCommandLine);
		if (!externalComand.init(externalCommandLine,exifMapPath)) {
			m_exifFound = false;
			return false;
		}
		ExifObject *exifObject = externalComand.process(path.c_str());
		if (exifObject == nullptr) {
			return nullptr;
		}
		exifObject->print();
		DateTime = ExifDateTime(exifObject->getDateTime().c_str());
		return exifObject;
	}
	*/
}


/*
#include <iostream>
#include "ExternalComand.h"
#include "ExifObject.h"

using namespace simplearchive;

int main() {
ExternalComand externalComand;
externalComand.init();
ExifObject *exifObject = externalComand.process("GB-wp1.jpg");
exifObject->print();
delete exifObject;
return 0;
}

*/
