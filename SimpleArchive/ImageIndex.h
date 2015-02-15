/*
 * ImageIndex.h
 *
 *  Created on: May 15, 2014
 *      Author: wzw7yn
 */

#ifndef IMAGEINDEX_H_
#define IMAGEINDEX_H_

#include <string>
#include <cstdlib>

namespace simplearchive {


class ImageId {
	std::string m_name;
	unsigned long m_crc;
	std::string m_md5;
public:
	ImageId();
	ImageId(const char *dataString) {
		std::string m_data = dataString;
		int delim1 = m_data.find_first_of(':');
		int delim2 = m_data.find_first_of(delim1, ':');
		std::string crcStr = m_data.substr(0,delim1);
		m_crc = std::stoul(crcStr.c_str(),NULL,16);
		std::string name = m_data.substr(delim1, delim2);
		std::string md5 = m_data.substr(delim2, m_data.length());
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

	void setName(const std::string& name) {
		this->m_name = name;
	}
};
class DupDataFile;
class CImageId;
class ImageIndex {
	std::string m_dbpath;
	unsigned char m_data[4];
	bool add(const char *name, unsigned long crc, const char *md5);
	DupDataFile *findDupDataFile(unsigned long crc);
public:
	ImageIndex();
	virtual ~ImageIndex();

	bool init(const char *path);
	/**
	*	This function returns true if added, false if dup
	*/
	bool add(CImageId *imageId);
	bool IsDup(unsigned long crc);
	bool getData(unsigned long crc);
};

} /* namespace simplearchive */
#endif /* ImageIndex_H_ */