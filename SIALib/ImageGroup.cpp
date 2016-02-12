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

#include "ImageGroup.h"
#include "ImageContainer.h"
//#include "ImageExtentions.h"
#include "SAUtils.h"
#include <vector>
#include <stdio.h>
#include "BasicExifFactory.h"
#include "BasicExif.h"
#include "CLogger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

ImageGroup::ImageGroup(const char *path) {
	m_path = path;
}

bool ImageGroup::add(ImageContainer *imageContainer)
{
	push_back(imageContainer);
	return true;
}

ImageContainer *ImageGroup::find(const char *imageFilename) {
	std::string filenameOnly = SAUtils::getFilePathNoExt(imageFilename);
	for (std::vector<ImageContainer *>::iterator i = begin(); i != end(); i++) {
		ImageContainer *imageContainer = *i;
		if (imageContainer->getName().compare(filenameOnly) == 0) {
			return imageContainer;
		}
	}
	return NULL;
}
//const BasicExifFactory *imageId
ImageContainer &ImageGroup::add(std::unique_ptr<BasicExif>& basicExif, std::unique_ptr<MetadataObject>& metadataObject)
{
	CLogger &logger = CLogger::getLogger();
	const std::string &imageFilename = basicExif->getName();
	std::string filenameOnly = SAUtils::getFilenameNoExt(imageFilename);
	ImageContainer *imageContainer = nullptr;
	if ((imageContainer = find(filenameOnly.c_str())) != nullptr) {
		// Second Instance with this name so needs to be assocated.
		logger.log(LOG_OK, CLogger::INFO, "Found name: %s, Associating: %s with %s", imageFilename.c_str(), imageFilename.c_str(), imageContainer->getName().c_str());
		imageContainer->add(basicExif, metadataObject);
	} else {
		// First Instance with this name.
		// note all images share the same path in the group.
		logger.log(LOG_OK, CLogger::INFO, "New image name: %s using %s", filenameOnly.c_str(), basicExif->getName().c_str());
		imageContainer = new ImageContainer(getPath(), filenameOnly.c_str());
		imageContainer->add(basicExif, metadataObject);
		add(imageContainer);
	}
	return *imageContainer;
}

bool ImageGroup::addMetadata(const char *filename)
{

	/*
	ImageExtentions& ie = ImageExtentions::get();
	if (ie.IsValidXML(filename) == false) {
		return false;
	}
	*/

	ImageContainer *imageContainer = nullptr;
	if ((imageContainer = find(filename)) != nullptr) {
		imageContainer->add(filename);
	}
	add(new ImageContainer(getPath(), filename));
	return true;
}

ImageGroup::~ImageGroup() {
	for (std::vector<ImageContainer *>::iterator i = begin(); i != end(); i++) {
		ImageContainer *imageContainer = *i;
		delete imageContainer;
	}
	clear();
}

ImageGroups::~ImageGroups() {
	for (std::vector<ImageGroup *>::iterator i = begin(); i != end(); i++) {
		ImageGroup *imageGroup = *i;
		delete imageGroup;
	}
	clear();
}

} /* namespace simplearchive */
