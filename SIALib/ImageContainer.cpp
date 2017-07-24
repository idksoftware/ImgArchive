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

#include <time.h>
#ifdef LINUX
//#include <ctime.h>
#endif
#include "MetadataObject.h"
#include "ImageContainer.h"
#include "ImageExtentions.h"
#include "SAUtils.h"
#include "CDate.h"
#include "CLogger.h"
#include "BasicMetadataFactory.h"
#include "BasicMetadata.h"

namespace simplearchive {


#ifdef _DEBUG
#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif


ImageContainer::ImageContainer(const char *path, const char *imageName) {
	m_PictureNode = nullptr;
	m_RawNode = nullptr;
	m_Path = path;
	m_Name = imageName;
	m_BasicMetadata = nullptr;
	m_Time = -1;
	m_error = 0;
}


ImageContainer::ImageContainer() {
	m_PictureNode = nullptr;
	m_RawNode = nullptr;
}


ImageContainer::~ImageContainer() {
	if (m_PictureNode) {
		delete m_PictureNode;
	}
	if (m_RawNode) {
		delete m_RawNode;
	}
}



bool ImageContainer::add(std::unique_ptr<BasicMetadata> &BasicMetadata, std::unique_ptr<MetadataObject>& metadataObject) {
	CLogger &logger = CLogger::getLogger();

	//
	const char *imagefile = BasicMetadata->getName().c_str();
	ImageExtentions &imageExtentions = ImageExtentions::get();
	ImageType type = imageExtentions.getType(imagefile);
	switch (type.getType()) {
	// Found Picture Image
	case ImageType::PICTURE_EXT:
		logger.log(LOG_OK, CLogger::Level::INFO, "found pic: %s", imagefile);
		if (m_PictureNode == nullptr) {
			/* not sure needed
			if (BasicMetadata->isExifFound()) {
				const ExifDateTime &dateTime = BasicMetadata->getDateTimeDigitized();
				logger.log(LOG_OK, CLogger::INFO, "Using Exif date: %d ", ((ExifDateTime&)dateTime).toString().c_str());
				m_Time = dateTime.getTime();
			}
			*/
			logger.log(LOG_OK, CLogger::Level::SUMMARY, "Associating: %s with %s", imagefile, m_Name.c_str());
			m_PictureNode = new ImageNode(type, BasicMetadata, metadataObject);
		}
		//m_PictureNode->setImageId(BasicMetadata, metadataObject);
		
		

		/*
		else {
			struct stat info;

			if (stat(((BasicMetadataFactory *)imageId)->getPath().c_str(), &info) != 0) {
				return false;
			}
		}
		*/
		
		break;
	// Found RAW Image
	case ImageType::RAW_EXT:
	
		logger.log(LOG_OK, CLogger::Level::INFO, "found raw: %s", imagefile);
		if (m_RawNode == nullptr) {
			logger.log(LOG_OK, CLogger::Level::SUMMARY, "Associating: %s with %s", imagefile, m_Name.c_str());
			m_RawNode = new ImageNode(type, BasicMetadata, metadataObject);
		}
		//m_RawNode->setImageId(imageId, metadataObject);
		
		//m_RawFile = imagefile;
		break;
	case ImageType::UNKNOWN_EXT:
	default:
		logger.log(LOG_OK, CLogger::Level::ERR, "No extention found for this file type %s", imagefile);
		m_error = -1;
		return false;
	}

	return true;
}

void ImageContainer::PostProcess() {
	
	
	if (m_PictureNode != nullptr) {
		m_BasicMetadata = &(m_PictureNode->getBasicMetadata());
	} else if (m_RawNode != nullptr) {
		m_BasicMetadata = &(m_RawNode->getBasicMetadata());
	}
	else {
		// Error
		m_error = -1;
	}
	if ((m_PictureNode != nullptr) && (m_RawNode != nullptr)) {
		const MetadataObject& picMO = m_PictureNode->getMetadataObject();
		m_RawNode->setMetadataObject(picMO);
	
	

		/*

		<Width>6016</Width>

		<Height>4000</Height>

		<Resolution/>

		<Depth/>

		<ViewRotation>6</ViewRotation>

		<SampleColor/>

		<Page/>

		<ColorSpace/>

		<Compression/>

		<PrimaryEncoding/>

		</MediaProerties>


		-<CameraInformation>

		<Maker>NIKON CORPORATION</Maker>

		<Model>NIKON D3200</Model>

		<Software/>

		<SourceURL/>

		<ExifVersion/>

		<CaptureDate>2012.04.25.15.56.07</CaptureDate>

		<ExposureProgram/>

		<ISOSpeedRating>400</ISOSpeedRating>

		<ExposureBias>0</ExposureBias>

		<ExposureTime>0.0008</ExposureTime>

		<Aperture>9</Aperture>

		<MeteringMode>3</MeteringMode>

		<LightSource/>

		<Flash/>

		<FocalLength>65</FocalLength>
		*/

	}
}

void ImageContainer::print() {
	CLogger &logger = CLogger::getLogger();
	
	logger.log(LOG_OK, CLogger::Level::FINE, "Item Name: %s\n", m_Name.c_str());
	if (hasPictureFile()) {
		logger.log(LOG_OK, CLogger::Level::FINE, "PictureFile: %s\n", m_PictureNode->getFile());
	}
	else
	{
		logger.log(LOG_OK, CLogger::Level::FINE, "No Picture File");
	}
	if (hasRawFile()) {
		logger.log(LOG_OK, CLogger::Level::FINE, "Has RAW File: %s\n", m_RawNode->getFile());
	}
	else {
		logger.log(LOG_OK, CLogger::Level::FINE, "No RAW File");
	}
}

const std::string& ImageNode::getFile() const {
	return m_file;
}

void ImageNode::setMetadataObject(const MetadataObject& metadataObject) {
	m_metadataObject->update(metadataObject);
}
/*
void ImageNode::setImageId(const BasicMetadataFactory*& imageId, const MetadataObject *metadataObject) {
	m_imageId = imageId;
	m_metadataObject = metadataObject;
	if (m_metadataObject != nullptr) {
		MetadataObject &mo = (MetadataObject &)*metadataObject;
		BasicMetadataFactory *id = (BasicMetadataFactory *)imageId;
		setImageID2Metadata(id, mo);
	}
}
*/

/*
void ImageNode::setImageID2Metadata(BasicMetadataFactory *imageIdData, MetadataObject &metadataObject) {
	
	
	imageIdData->setMetadataObject(metadataObject);
}
*/

/*
void ImageNode::setMetadataObject(
		const MetadataObject*& metadataObject) {
	m_metadataObject = metadataObject;
	if (m_metadataObject != nullptr) {
		MetadataObject &mo = (MetadataObject &)*metadataObject;
		//BasicMetadataFactory *id = (BasicMetadataFactory *)m_BasicMetadata;
		//setImageID2Metadata(m_BasicMetadata, mo);
	}
}
*/

} /* namespace simplearchive */
