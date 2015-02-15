/*
 * ImageGroup.h
 *
 *  Created on: May 8, 2014
 *      Author: wzw7yn
 */

#ifndef IMAGEGROUP_H_
#define IMAGEGROUP_H_
#include <string>
#include <vector>
#include "ImageContainer.h"

namespace simplearchive {

class CImageId;
/// This is a group of images. This Images them self are groups within a Image
/// Container, i.e a jpg/RAW pair.
/// A list is normally a folder of images.
class ImageGroup : public std::vector<ImageContainer *> {
	//std::vector<ImageContainer *> *m_list;
	std::string m_path;
public:
	ImageGroup(const char *path);
	ImageContainer &add(const CImageId *imageId, const MetadataObject* metadataObject);
	bool addMetadata(const char *filename);
	virtual ~ImageGroup();
	void print() {
		//printf("Set %s\n", m_path.c_str());
		for (std::vector<ImageContainer *>::iterator i = this->begin(); i != this->end(); i++) {
			ImageContainer *data = *i;
			data->print();
		}
	}
	const char *getPath() {
		return m_path.c_str();
	}
private:
	ImageContainer *find(const char *imageFilename);
	bool add(ImageContainer *imageContainer);
	
};
/// @brief This provides a list of Image Groups.
/// This is normaly the folder structure of the imported images.
class ImageGroups : public std::vector < ImageGroup * > {
public:
	void print() {

		for (std::vector<ImageGroup *>::iterator i = this->begin(); i != this->end(); i++) {
			ImageGroup *data = *i;
			data->print();
		}
	}

};

} /* namespace simplearchive */
#endif /* IMAGEGROUP_H_ */
