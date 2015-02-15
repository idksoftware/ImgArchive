/*
 * ImageContainer.cpp
 *
 *  Created on: May 8, 2014
 *      Author: wzw7yn
 */
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
#include "ImageId.h"

namespace simplearchive {



ImageContainer::ImageContainer(const char *path, const char *imageName) {
	m_PictureNode = nullptr;
	m_RawNode = nullptr;
	m_Path = path;
	m_Name = imageName;
	m_ImageId = nullptr;
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

bool ImageContainer::add(const CImageId *imageId, const MetadataObject* metadataObject) {
	CLogger &logger = CLogger::getLogger();
	

	
	if (!imageId) {
		return false;
	}
	//
	const char *imagefile = imageId->getName().c_str();
	ImageExtentions &imageExtentions = ImageExtentions::get();
	ImageType type = imageExtentions.getType(imagefile);
	switch (type.getType()) {
	// Found Picture Image
	case ImageType::PICTURE_EXT:
		logger.log(CLogger::INFO, "found pic: %s", imagefile);
		if (m_PictureNode == nullptr) {
			logger.log(CLogger::INFO, "Associating: %s with %s", imagefile, m_Name.c_str());
			m_PictureNode = new ImageNode(type, imagefile, imageId, metadataObject);
		}
		m_PictureNode->setImageId(imageId, metadataObject);
		
		if (imageId->isExifFound()) {
			const ExifDateTime &dateTime = imageId->getDateTime();
			logger.log(CLogger::INFO, "Using Exif date: %d ", ((ExifDateTime&)dateTime).toString().c_str());
			m_Time = dateTime.getTime();
		}

		/*
		else {
			struct stat info;

			if (stat(((CImageId *)imageId)->getPath().c_str(), &info) != 0) {
				return false;
			}
		}
		*/
		
		break;
	// Found RAW Image
	case ImageType::RAW_EXT:
	
		logger.log(CLogger::INFO, "found raw: %s", imagefile);
		if (m_RawNode == nullptr) {
			logger.log(CLogger::INFO, "Associating: %s with %s", imagefile, m_Name.c_str());
			m_RawNode = new ImageNode(type, imagefile, imageId, metadataObject);
		}
		m_RawNode->setImageId(imageId, metadataObject);
		
		//m_RawFile = imagefile;
		break;
	case ImageType::UNKNOWN_EXT:
	default:
		logger.log(CLogger::ERROR, "No extention found for this file type %s", imagefile);
		int m_Error;
		return false;
	}

	return true;
}

void ImageContainer::PostProcess() {
	const CImageId *imageRawId = nullptr;
	const CImageId *imagePicId = nullptr;

	if (m_RawNode != nullptr) {
		imageRawId = m_RawNode->getImageId();
	}
	if (m_PictureNode != nullptr) {
		imagePicId = m_PictureNode->getImageId();
	}
	
	if (imageRawId != nullptr) {
		m_ImageId = imageRawId;
	}
	else if (imagePicId != nullptr) {
		m_ImageId = imagePicId;
	}
	else {
		// Error
		m_error = -1;
	}
	/*
	const MetadataObject *pictureMetadata = nullptr;
	const MetadataObject *rawMetadata = nullptr;
	const CImageId *pictureImageId = nullptr;
	const CImageId *rawImageId = nullptr;

	if (m_PictureNode == nullptr) {
		//logger.log(CLogger::INFO, "Associating: %s with %s", imagefile, m_Name.c_str());
		pictureMetadata = m_PictureNode->getMetadataObject();
	}
	if (m_RawNode == nullptr) {
		//logger.log(CLogger::INFO, "Associating: %s with %s", imagefile, m_Name.c_str());
		rawMetadata = m_RawNode->getMetadataObject();
	}
	if (rawMetadata) {
		std::string date = rawMetadata->getCaptureDate();
	}
	else if (pictureMetadata) {
		std::string date = pictureMetadata->getCaptureDate();
	}
	*/


}

void ImageContainer::print() {
	CLogger &logger = CLogger::getLogger();
	
	logger.log(CLogger::FINE, "Item Name: %s\n", m_Name.c_str());
	if (hasPictureFile()) {
		logger.log(CLogger::FINE, "PictureFile: %s\n", m_PictureNode->getFile());
	}
	else
	{
		logger.log(CLogger::FINE, "No Picture File");
	}
	if (hasRawFile()) {
		logger.log(CLogger::FINE, "Has RAW File: %s\n", m_RawNode->getFile());
	}
	else {
		logger.log(CLogger::FINE, "No RAW File");
	}
}

const std::string& ImageNode::getFile() const {
	return m_file;
}



void ImageNode::setImageId(const CImageId*& imageId, const MetadataObject *metadataObject) {
	m_imageId = imageId;
	m_metadataObject = metadataObject;
	if (m_metadataObject != nullptr) {
		MetadataObject &mo = (MetadataObject &)*metadataObject;
		CImageId *id = (CImageId *)imageId;
		setImageID2Metadata(id, mo);
	}
}

void ImageNode::setImageID2Metadata(CImageId *imageIdData, MetadataObject &metadataObject) {
	/*
	unsigned long getCrc()
	unsigned long getSize()
	const std::string& getMd5()
	const std::string& getMediaType()
	const std::string& getUuid()
	const std::string& getName()
	const std::string& getPath()
	const std::string getExt()
	const ExifDateTime &getDateTime()
	const ExifDateTime& getDateTimeOriginal()
	const ExifDateTime& getDateTimeDigitized()
	const ExifDateTime &getCreateTime()
	const ExifDateTime &getModTime()
	*/
	
	imageIdData->setMetadataObject(metadataObject);
}

void ImageNode::setMetadataObject(
		const MetadataObject*& metadataObject) {
	m_metadataObject = metadataObject;
	if (m_metadataObject != nullptr && m_imageId != nullptr) {
		MetadataObject &mo = (MetadataObject &)*metadataObject;
		CImageId *id = (CImageId *)m_imageId;
		setImageID2Metadata(id, mo);
	}
}

} /* namespace simplearchive */