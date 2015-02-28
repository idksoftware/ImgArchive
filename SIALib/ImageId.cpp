#include "ImageId.h"
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

namespace simplearchive {

	CImageId::CImageId()
	{
		m_exifFound = false;
	}


	CImageId::~CImageId()
	{
	}

	void CImageId::copyBasicEXIF(EXIFInfo &exifInfo) {

		
		Make = exifInfo.Make;
		Model = exifInfo.Model;
		Software = exifInfo.Software;
		BitsPerSample = exifInfo.BitsPerSample;
		ImageWidth = exifInfo.ImageWidth;
		ImageHeight = exifInfo.ImageHeight;
		ImageDescription = exifInfo.ImageDescription;
		Orientation = exifInfo.Orientation;
		Copyright = exifInfo.Copyright;
		DateTime = ExifDateTime(exifInfo.DateTime.c_str());
		DateTimeOriginal = ExifDateTime(exifInfo.DateTimeOriginal.c_str());
		DateTimeDigitized = ExifDateTime(exifInfo.DateTimeDigitized.c_str());
		SubSecTimeOriginal = exifInfo.SubSecTimeOriginal;
		ExposureTime = (unsigned)(1.0 / exifInfo.ExposureTime);
		FNumber = exifInfo.FNumber;
		ISOSpeedRatings = exifInfo.ISOSpeedRatings;
		SubjectDistance = exifInfo.SubjectDistance;
		ExposureBiasValue = exifInfo.ExposureBiasValue;
		Flash = exifInfo.Flash;
		MeteringMode = exifInfo.MeteringMode;
		FocalLength = exifInfo.FocalLength;
		
		//FocalLengthIn35mm = exifInfo.FocalLengthIn35mm;
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
	
	void CImageId::setMetadataObject(MetadataObject &metadataObject) {

		metadataObject.setFilename(m_name);
		metadataObject.setOrginalName(m_name);
		metadataObject.setFilepath(m_path);
		metadataObject.setCrc(m_crc);
		metadataObject.setMediaType(m_mediaType);
		metadataObject.setMd5(m_md5);
		metadataObject.setUniqueId(m_uuid);
		metadataObject.setFileSize(m_size);
		metadataObject.setDateModified(m_modTime);
		metadataObject.setDateCreate(m_createTime);


		metadataObject.setMaker(Make);
		metadataObject.setModel(Model);
		metadataObject.setSoftware(Software);
		//metadataObject.setB BitsPerSample);
		metadataObject.setWidth(ImageWidth);
		metadataObject.setHeight(ImageHeight);
		metadataObject.setDescription(ImageDescription);
		metadataObject.getViewRotation(Orientation);
		metadataObject.setCopyright(Copyright);
		//metadataObject. DateTime);
		//metadataObject.set DateTimeOriginal);
		//metadataObject DateTimeDigitized);
		//metadataObject SubSecTimeOriginal);
		metadataObject.setExposureTime(ExposureTime);
		//metadataObject.setAperture(FNumber);
		metadataObject.setIsoSpeedRating(ISOSpeedRatings);
		//metadataObject. SubjectDistance);
		metadataObject.setExposureBias(ExposureBiasValue);
		metadataObject.setFlash((Flash)?"Flash used":"No flash");
		metadataObject.setMeteringMode(MeteringMode);
		metadataObject.setFocalLength(FocalLength);

	}

	static std::string getFileContents(const char *filename)
	{
		
		int count = 0;
		std::ifstream in(filename, std::ios::in | std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
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

	

	CImageId::CImageId(std::string &path) {
		CLogger &logger = CLogger::getLogger();
		
		CIDKCrc Crc;
		m_path = path;
		m_name = splitName(m_path);
		logger.log(CLogger::INFO, "Image: %s", m_path.c_str());
		std::string buf = getFileContents(path.c_str());
		MD5 md5(buf);
		m_md5 =	md5.hexdigest();
		logger.log(CLogger::INFO, "MD5 of image: %s is %s", path.c_str(), m_md5.c_str());
		CIDKUuid uuid;
		m_uuid = uuid.GetUuid();
		logger.log(CLogger::INFO, "UUID of image: %s is %s", path.c_str(), m_uuid.c_str());
		m_size = buf.length();
		logger.log(CLogger::INFO, "Size of image: %s is %d", path.c_str(), m_size);
		m_crc = Crc.crc((unsigned char *)buf.c_str(), m_size);
		logger.log(CLogger::INFO, "CRC from image: %s is %x", path.c_str(), m_crc);
		
		m_createTime = SAUtils::createTime(path.c_str());
		logger.log(CLogger::INFO, "Create time of image: %s", m_createTime.toLogString().c_str());
		m_modTime = SAUtils::modTime(path.c_str());
		
		logger.log(CLogger::INFO, "Internal Exif reader, reading Exif from image: %s", path.c_str());
		EXIFInfo exifInfo;
		switch (exifInfo.parseFrom(buf)) {
		case PARSE_EXIF_SUCCESS:
			logger.log(CLogger::INFO, "Completed reading Exif from file: %s", path.c_str());
			m_exifFound = true;
			copyBasicEXIF(exifInfo);
			break;
		case PARSE_EXIF_ERROR_NO_JPEG:
			logger.log(CLogger::FATAL, "No JPEG markers found in buffer, possibly invalid JPEG file: %s", path.c_str());
			// No EXIF header found in JPEG file.
			m_exifFound = false;
			break;
		case PARSE_EXIF_ERROR_NO_EXIF:
			logger.log(CLogger::FATAL, "No JPEG markers found in buffer, possibly invalid JPEG file: %s", path.c_str());
			m_exifFound = false;
			break;
		case PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN:
			logger.log(CLogger::FATAL, "Byte alignment specified in EXIF file was unknown in file: %s", path.c_str());
			
			m_exifFound = false;
			break;
		case PARSE_EXIF_ERROR_CORRUPT:
			logger.log(CLogger::FATAL, "EXIF header was found, but data was corrupted in file: %s", path.c_str());
			m_exifFound = false;
			break;
		default:
			logger.log(CLogger::FATAL, "Exif invalid: %s", path.c_str());
			m_exifFound = false;
			break;
		}
		
	}


	/*
	ExifObject *CImageId::externalExifTool(std::string &path) {
		CLogger &logger = CLogger::getLogger();
		
		CAppConfig config = CAppConfig::get();
		ExternalComand externalComand(config.getTempPath());
		
		const char *externalCommandLine;
		if (!(externalCommandLine = config.getExternalCommandLine())) {
			//logger.log(CLogger::ERROR, "Exif command line not found\n");
			m_exifFound = false;
			return false;
		}
		const char *exifMapPath;
		if (!(exifMapPath = config.getExifMapPath())) {
			//logger.log(CLogger::ERROR, "Exif map path not found");
			m_exifFound = false;
			return false;
		}
		logger.log(CLogger::FINE, "Exif command line found \"%s\"", externalCommandLine);
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