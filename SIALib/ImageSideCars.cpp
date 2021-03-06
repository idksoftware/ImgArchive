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

#include <cstdlib>
#include "ImageSideCar.h"
#include "MetadataObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

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
