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

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cstdint>
#include "ExifDate.h"
#include "DBDefines.h"
#include "MetaType.h"

namespace simplearchive {



class ImageStatus {
	uint64_t m_idx;
	std::string m_imagePath;
	std::string m_shortImagePath;
	std::string m_name;
	uint64_t m_crc;
	std::string m_md5;
	std::string m_uuid;
	unsigned long m_size;
	int m_version;
	ExifDate m_dateArchived;
	uint64_t m_dbidx;
public:
	
	ImageStatus(uint64_t idx, const char* imagePath, const char* name, unsigned long size, uint64_t crc,
		const char* md5, const char* uuid, int version, ExifDate& date, uint64_t dbidx);
	~ImageStatus();
	unsigned long getCrc() const;
	const ExifDate& getDate() const;
	int getIdx() const;
	const std::string& getImagePath() const;
	const std::string& getShortImagePath() const;
	const std::string& getName() const;
	const std::string& getMd5() const;
	unsigned long getSize() const;
	const std::string& getUuid() const;
	int getVersion() const;
};



class CSVStatusFile {
	std::string m_dbpath;

	unsigned int m_data[4];
	int getMaxDirIndex(std::string &path);
	bool insert(uint64_t idx, const char *imagePath, const char *name, unsigned long size, uint64_t crc,
		const char *md5, const char *uuid, int version, ExifDate &date, const char*rootPath, uint64_t dbidx);
	CSVStatusFile(const CSVStatusFile&) = delete;
	
public:
	CSVStatusFile();

	virtual ~CSVStatusFile();

	bool insert(uint64_t idx, const char *imagePath, const char *name, unsigned long size, uint64_t crc,
						const char *md5, const char *uuid, int version, ExifDate &date, uint64_t dbidx = -1);

	void setPath(const char *s);
	bool backupPaths(const char *path);
	const char* findPath(uint64_t idx);
	unsigned long findSize(unsigned int idx);
	int getNextIndex();
	uint64_t getNextIndex(uint64_t current);
	uint64_t getMaxIndex();
	std::unique_ptr<ImageStatus> getItemAt(uint64_t idx);
	static std::string getYear(const char *path);
};



} /* namespace simplearchive */
