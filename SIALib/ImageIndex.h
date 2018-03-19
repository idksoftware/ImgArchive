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
#include <memory>
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
class BasicMetadata;
class MirrorImageIndex;
class FileInfo;
class DupCache;

typedef std::unique_ptr<DupDataFile> DupDataFile_Ptr;



class ImageIndex {

	//std::unique_ptr<MirrorImageIndex> m_mirrorImageIndex;
	std::unique_ptr<DupCache> m_dupCache;
	
	std::string m_dbpath;
	unsigned char m_data[4];
	/** Adds to primary and backups */
	bool add(const char *name, unsigned long crc, const char *md5);
	/** Adds to the Image index db using the given path */
	bool add(const char *name, unsigned long crc, const char *md5, const char *rootPath);
	DupDataFile_Ptr findDupDataFile(unsigned long crc);
	DupDataFile_Ptr findDupDataFile(unsigned long crc, const char *root);
	/**
	 * This function is used to update the path to the image with the addition of the root db folder.
	 */
	bool updatePath(unsigned long crc, const char *path, int version, const char *root);
protected:
	const char *getRootPath() {
		return m_dbpath.c_str();
	}
public:
	ImageIndex();
	virtual ~ImageIndex();

	/** initalise the image index database with the primary path */
	bool init(const char *path);
	/**
	*	This function returns true if added, false if dup
	*/
	bool add(const BasicMetadata &BasicMetadata);
	bool add(const FileInfo& fileInfo);
	bool IsDup(unsigned long crc);
	bool add2DupCache(const BasicMetadata &BasicMetadata);
	bool add2DupCache(const FileInfo& fileInfo);
	bool isDupInCache(unsigned long crc);
	std::string FindDup(unsigned long crc);
	/**
	* This function is used to update the path to the image.
	*/
	bool updatePath(unsigned long crc, const char *path, int version);
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