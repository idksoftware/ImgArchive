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

#ifndef IMAGECONTAINER_H_
#define IMAGECONTAINER_H_

#include <time.h>
#include <string>
#include <memory>
#include "ImageExtentions.h"
#include "BasicExif.h"

namespace simplearchive {

class BasicExif;
class BasicExifFactory;
class MetadataObject;

/// 
class ImageNode
{
	/// Image type found from the file extention.
	ImageType m_type;
	/// This contains the path to the Image file.
	std::string m_file;
	/// This contains a pointer to the BasicExifFactory.
	std::unique_ptr<BasicExif> m_basicExif;
	/// This contains a pointer to the MetadataObject.
	std::unique_ptr<MetadataObject> m_metadataObject;

	//void setImageID2Metadata(const BasicExif &basicExif, MetadataObject &metadataObject);
public:
	ImageNode(ImageType &type, std::unique_ptr<BasicExif>& basicExif, std::unique_ptr<MetadataObject>& metadataObject) : m_basicExif(move(basicExif)), m_metadataObject(move(metadataObject)) {
		m_type = type;
		m_file = m_basicExif->columnAt(DB_FILENAME).toString();
	}

	~ImageNode() {
		//printf("+++ ImageNode +++");
	}

	const std::string& getFile() const;
	const BasicExif &getBasicExif() {
		return *m_basicExif.get();
	}
	void setImageId(const BasicExifFactory*& imageId, const MetadataObject *metadataObject = nullptr);

	const MetadataObject& getMetadataObject() {
		return *m_metadataObject.get();
	}

	//void setMetadataObject(const MetadataObject*& metadataObject);

};

class ImageContainer {
	std::string m_Path;
	std::string m_Name;
	std::string m_MetadataFile;
	std::string m_imageRootPath;
	std::string m_comment;
	const BasicExif *m_basicExif;
	ImageNode *m_PictureNode;
	ImageNode *m_RawNode;
	time_t m_Time;
	int m_error;
	ImageContainer();
	
public:
	
	ImageContainer(const char *path, const char *imageName);
	bool add(std::unique_ptr<BasicExif>& basicExif, std::unique_ptr<MetadataObject>& metadataObject);
	bool add(const char *xmlfile) {
		return true; // to be added
	}
	virtual ~ImageContainer();

	/// Archive Name 
	const std::string& getName() const {
		return m_Name;
	}

	/// Get Picture file 
	const char *getPictureFile() const {
		if (m_PictureNode == nullptr) {
			return nullptr;
		}
		return m_PictureNode->getFile().c_str();
	}

	/// Get RAW file
	const char *getRawFile() const {
		if (m_RawNode == nullptr) {
			return nullptr;
		}
		return m_RawNode->getFile().c_str();
	}

	/// Get Picture MetadataObject 
	const MetadataObject *getPictureMetadata() const {
		if (m_PictureNode == nullptr) {
			return nullptr;
		}
		return &(m_PictureNode->getMetadataObject());
	}

	/// Get RAW MetadataObject
	const MetadataObject *getRawMetadata() const {
		if (m_RawNode == nullptr) {
			return nullptr;
		}
		return &(m_RawNode->getMetadataObject());
	}

	/// Get Picture image id 
	const BasicExif &getPictureId() const {
		//if (m_PictureNode == nullptr) {
		//	return nullptr;
		//}
		return m_PictureNode->getBasicExif();
	}

	/// Get RAW Image id
	const BasicExif &getRawId() const {
		//if (m_RawNode == nullptr) {
		//	return nullptr;
		//}
		return m_RawNode->getBasicExif();
	}
	/// Get Image path in the archive
	const std::string& getPath() const {
		return m_Path;
	}

	/// Has picture file in container
	bool hasPictureFile() const {
		if (m_PictureNode == nullptr) {
			return false;
		}
		return true;
		
	}

	/// Has RAW file in container
	bool hasRawFile() const {
		if (m_RawNode == nullptr) {
			return false;
		}
		return true;
	}

	/// Has Metadata file in container
	bool hasMetadateFile() const {
			return (m_MetadataFile.length() > 0);
	}

	/// Get archive time
	time_t getTime() const {
		return m_Time;
	}

	/// Set archive time
	void setTime(time_t time) {
		m_Time = time;
	}

	/// Get metadata file
	const std::string& getMetadataFile() const {
		return m_MetadataFile;
	}

	/// Set metadata file
	void setMetadataFile(const std::string& metadataFile) {
		m_MetadataFile = metadataFile;
	}

	const std::string& getComment() const {
		return m_comment;
	}

	void setComment(const std::string& comment) {
		m_comment = comment;
	}

	void PostProcess();

	/// Print container to stan
	void print();

	int getError() {
		return m_error;
	}

	void setImageRootPath(const std::string& path) {
		m_imageRootPath = path;
	}

	const std::string& getImageRootPath() const {
		return m_imageRootPath;
	}
};

} /* namespace simplearchive */
#endif /* IMAGECONTAINER_H_ */
