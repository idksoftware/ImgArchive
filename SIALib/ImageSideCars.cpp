/*
 * ImageGroup.cpp
 *
 *  Created on: Jul 8, 2014
 *      Author: wzw7yn
 */
#include <cstdlib>
#include "ImageSideCar.h"
#include "MetadataObject.h"

namespace simplearchive {

SideCarItem::SideCarItem(const char * sequenceId, const char * uniqueId, const char * crc,
		const char *md5, const char * filename, const char * filepath) {
	m_sequenceId = sequenceId;
	m_uniqueId = uniqueId;
	m_crc = crc;
	m_md5 = md5;
	m_filename = filename;
	m_filepath = filepath;
}

SideCarItem::~SideCarItem() {
	// TODO Auto-generated destructor stub
}

void SideCar::add(MetadataObject &metadataObject) {
	//unsigned int sequenceId = strtol(metadataObject.getSequenceId().c_str(), NULL, 16);
	//unsigned int crc = strtol(metadataObject.getCrc().c_str(), NULL, 16);

	/*
	SideCarItem sideCarItem(metadataObject.getSequenceId().c_str(), metadataObject.getUniqueId().c_str(),
			metadataObject.getCrc().c_str(), metadataObject.getMd5().c_str(),
			metadataObject.getFilename().c_str(), metadataObject.getFilepath().c_str());
*/
//	push_back(sideCarItem);
}

void SideCar::add(const char * sequenceId, const char * uniqueId, const char * crc,
		const char *md5, const char * filename, const char * filepath) {

	SideCarItem sideCarItem(sequenceId, uniqueId, crc, md5, filename, filepath);
	push_back(sideCarItem);
}

SideCar::~SideCar() {
	// TODO Auto-generated destructor stub
}



} /* namespace simplearchive */
