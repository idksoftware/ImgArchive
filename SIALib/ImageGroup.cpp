/*
 * ImageGroup.cpp
 *
 *  Created on: May 8, 2014
 *      Author: wzw7yn
 */

#include "ImageGroup.h"
#include "ImageContainer.h"
//#include "ImageExtentions.h"
#include "SAUtils.h"
#include <vector>
#include <stdio.h>
#include "ImageId.h"
#include "CLogger.h"

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
//const CImageId *imageId
ImageContainer &ImageGroup::add(const CImageId *imageId, const MetadataObject *metadataObject = nullptr)
{
	CLogger &logger = CLogger::getLogger();
	const std::string &imageFilename = imageId->getName();
	std::string filenameOnly = SAUtils::getFilenameNoExt(imageFilename);
	ImageContainer *imageContainer = nullptr;
	if ((imageContainer = find(filenameOnly.c_str())) != nullptr) {
		// Second Instance with this name so needs to be assocated.
		logger.log(CLogger::INFO, "Found name: %s, Associating: %s with %s", imageFilename.c_str(), imageFilename.c_str(), imageContainer->getName().c_str());
		imageContainer->add(imageId, metadataObject);
	} else {
		// First Instance with this name.
		// note all images share the same path in the group.
		logger.log(CLogger::INFO, "New image name: %s using %s", filenameOnly.c_str(), imageId->getName().c_str());
		imageContainer = new ImageContainer(getPath(), filenameOnly.c_str());
		imageContainer->add(imageId, metadataObject);
		add(imageContainer);
	}
	return *imageContainer;
}

bool ImageGroup::addMetadata(const char *filename)
{

	/*
	ImageExtentions ie = ImageExtentions::get();
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
	// TODO Auto-generated destructor stub
}

} /* namespace simplearchive */
