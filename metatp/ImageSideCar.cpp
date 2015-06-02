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

#include <fstream>
#include <cstdlib>
#include <sstream>
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

bool SideCarItem::fromString(const char *line) {
	m_row = line;
	int delim1 = m_row.find_first_of(':');
	int delim2 = m_row.find_first_of(':', delim1+1);
	int delim3 = m_row.find_first_of(':', delim2+1);
	int delim4 = m_row.find_first_of(':', delim3+1);
	int delim5 = m_row.find_first_of(':', delim4+1);
	m_sequenceId = m_row.substr(0,delim1);
	m_filename = m_row.substr(delim1+1, (delim2-delim1)-1);
	m_filepath = m_row.substr(delim2+1, (delim3-delim2)-1);
	m_uniqueId = m_row.substr(delim3+1, (delim4-delim3)-1);
	m_crc = m_row.substr(delim4+1, (delim4-delim4)-1);
	m_md5 = m_row.substr(delim5+1, m_row.length());
	return true;
}

SideCarItem::~SideCarItem() {
	// TODO Auto-generated destructor stub
}

const char *SideCarItem::toString() {
	std::stringstream tmp;
	tmp << m_sequenceId << ':' << m_filename << ':'
		<< m_filepath << ':' << m_uniqueId << ':' << m_crc << ':'
		<< m_md5;
	m_row = tmp.str();
	return m_row.c_str();
}

void SideCar::add(MetadataObject &metadataObject) {
	//unsigned int sequenceId = strtol(metadataObject.getSequenceId().c_str(), NULL, 16);
	//unsigned int crc = strtol(metadataObject.getCrc().c_str(), NULL, 16);

	/* use ImageId
	SideCarItem sideCarItem(metadataObject.getSequenceId().c_str(), metadataObject.getUniqueId().c_str(),
			metadataObject.getCrc().c_str(), metadataObject.getMd5().c_str(),
			metadataObject.getFilename().c_str(), metadataObject.getFilepath().c_str());
            
	push_back(sideCarItem);
    */
}

void SideCar::add(const char * sequenceId, const char * uniqueId, const char * crc,
		const char *md5, const char * filename, const char * filepath) {

	SideCarItem sideCarItem(sequenceId, uniqueId, crc, md5, filename, filepath);
	push_back(sideCarItem);
}

SideCar::~SideCar() {
	// TODO Auto-generated destructor stub
}

bool SideCar::read(const char *datafile) {
	char text[256];
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		SideCarItem sideCarItem;
		sideCarItem.fromString(text);
		push_back(sideCarItem);
	}
	file.close();

	return true;
}
bool SideCar::write(const char *path) {
	std::string filename(m_name);
	filename += "_car.dat";
	write(path, filename.c_str());
	//printf("%s", filename.c_str());
	return true;
}
bool SideCar::write(const char *path, const char *filename) {
	std::string fullpath(path);
	fullpath += '/';
	fullpath += filename;

	std::ofstream file(fullpath.c_str());
	if (file.is_open() == false) {
		return false;
	}

	for (std::vector<SideCarItem>::iterator i = begin(); i != end(); i++) {
		SideCarItem sideCarItem = *i;
		file << sideCarItem.toString() << '\n';
		//printf("%s", sideCarItem.toString());
	}

	file.close();
	return true;
}


} /* namespace simplearchive */
