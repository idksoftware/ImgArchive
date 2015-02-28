/*
 * MakeMedia.h
 *
 *  Created on: Jan 9, 2015
 *      Author: wzw7yn
 */

#ifndef MEDIAMAKER_H_
#define MEDIAMAKER_H_
#include "ExifDateTime.h"

namespace simplearchive {

class MakeMedia {
	bool m_all;
	int m_sizeOfMedia;
	ExifDateTime m_startDate;
	ExifDateTime m_endDate;
	std::string m_archivePath;
	std::string m_distPath;
public:
	MakeMedia() {};
	virtual ~MakeMedia();

	bool initalise(const char *archivePath, const char *distPath, unsigned long sizeOfMedia);
	bool initalise(const char *archivePath, const char *distPath, unsigned long sizeOfMedia, ExifDateTime &startDate, ExifDateTime &endDate);
	
	bool process();
	bool clearBackup();
};

} /* namespace simplearchive */
#endif /* MEDIAMAKER_H_ */
