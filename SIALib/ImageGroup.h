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

#ifndef IMAGEGROUP_H_
#define IMAGEGROUP_H_
#include <string>
#include <vector>
#include "ImageContainer.h"

namespace simplearchive {
class BasicMetadata;
class BasicMetadataFactory;
/// This is a group of images. This Images them self are groups within a Image
/// Container, i.e a jpg/RAW pair.
/// A list is normally a folder of images.
class ImageGroup : public std::vector<ImageContainer *> {
	std::string m_path;
public:
	ImageGroup(const char *path);
	ImageContainer &add(std::unique_ptr<BasicMetadata>& BasicMetadata, std::unique_ptr<MetadataObject>& metadataObject);
	bool addMetadata(const char *filename);
	virtual ~ImageGroup();
	void print() {
		for (auto i = this->begin(); i != this->end(); i++) {
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

		for (auto i = this->begin(); i != this->end(); i++) {
			ImageGroup *data = *i;
			data->print();
		}
	}
	virtual ~ImageGroups();
};

} /* namespace simplearchive */
#endif /* IMAGEGROUP_H_ */
