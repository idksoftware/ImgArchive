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

#ifndef IMAGEINDEX_H_
#define IMAGEINDEX_H_

#include <string>
#include <cstdlib>
#include "CSVArgs.h"

namespace simplearchive {


class ImageId {
	std::string m_name;
	unsigned long m_crc;
	std::string m_md5;
	std::string m_location;
public:
	ImageId() {};
	ImageId(const char *dataString) {
		
		CSVArgs csvArgs(':');
		csvArgs.process(dataString);
		
		std::string crcStr = csvArgs.at(0);
		m_crc = std::stoul(crcStr.c_str(),NULL,16);
		m_name = csvArgs.at(1);
		m_md5 = csvArgs.at(2);
		m_location = csvArgs.getOptional(3);
	}

	ImageId(const char *name, unsigned long crc, const char *md5) {
		m_name = name;
		m_crc = crc;
		m_md5 = md5;
	}

	//bool add(const char *name, unsigned long crc, const char *md5);
	
	unsigned long getCrc() const {
		return m_crc;
	}

	void setCrc(unsigned long crc) {
		this->m_crc = crc;
	}

	const std::string& getMd5() const {
		return m_md5;
	}

	void setMd5(const std::string& md5) {
		this->m_md5 = md5;
	}

	const std::string& getName() const {
		return m_name;
	}

	const std::string& getLocation() const {
		return m_location;
	}

	void setName(const std::string& name) {
		this->m_name = name;
	}
};


class DupDataFile;
class BasicExif;

class ImageIndex {
	std::string m_dbpath;
	unsigned char m_data[4];
	bool add(const char *name, unsigned long crc, const char *md5);
	DupDataFile* findDupDataFile(unsigned long crc);
public:
	ImageIndex();
	virtual ~ImageIndex();

	bool init(const char *path);
	/**
	*	This function returns true if added, false if dup
	*/
	bool add(const BasicExif &basicExif);
	bool IsDup(unsigned long crc);
	std::string FindDup(unsigned long crc);
	bool updatePath(unsigned long crc, const char *path);
	ImageId findDup(unsigned long crc) {
		std::string data = FindDup(crc);
		if (data.empty()) {
			ImageId imageId;
			return imageId;	// return empty object
		}
		ImageId imageId(data.c_str());
		return imageId;
	}
};

} /* namespace simplearchive */
#endif /* ImageIndex_H_ */