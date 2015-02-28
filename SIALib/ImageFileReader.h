/*
* ImageExtentions.h
*
*  Created on: May 9, 2014
*      Author: wzw7yn
*/

#ifndef IMAGEFILEREADER_H_
#define IMAGEFILEREADER_H_

#include <string>
namespace simplearchive {
class ExifObject;
class ImageFileReader
{
	bool m_exifFound;
public:
	ImageFileReader();
	~ImageFileReader();
	static ExifObject *externalExifTool(std::string &path);
};

} /* namespace simplearchive */
#endif /* IMAGEFILEREADER_H_ */