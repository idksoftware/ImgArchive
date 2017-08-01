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

#ifndef CSVDBFILE_H_
#define CSVDBFILE_H_
#include <string>
#include <memory>
#include <vector>
#include "ExifDate.h"


namespace simplearchive {

/*
 * @brief This class manages the CSV database files.
 *
 * The CSV Database if file based. as the database expands the records in
 * the files will also expand. At some point the file containing the records
 * will become un-manageably large.
 * This class splits the files in to smaller parts
 */
class ImageInfo {
	int m_idx;
	std::string m_imagePath;
	std::string m_shortImagePath;
	std::string m_name;
	unsigned long m_crc;
	std::string m_md5;
	std::string m_uuid;
	unsigned long m_size;
	int m_version;
	ExifDate m_dateArchived;
	int m_dbidx;
public:
	ImageInfo(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date, int m_dbidx);
	~ImageInfo();
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


//class MirrorIdxDB;
class CSVDBFile {
	std::string m_dbpath;
//	std::unique_ptr<MirrorIdxDB> m_mirrorIdxDB;
	unsigned int m_data[4];
	int getMaxDirIndex(std::string &path);
	bool insert(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date, const char*rootPath, int dbidx);
	CSVDBFile(const CSVDBFile &) = delete;
	
public:
	CSVDBFile();

	virtual ~CSVDBFile();

	bool insert(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
						const char *md5, const char *uuid, int version, ExifDate &date, int dbidx = -1);

	void setPath(const char *s);
	bool backupPaths(const char *path);
	const char* findPath(unsigned int idx);
	unsigned long findSize(unsigned int idx);
	int getNextIndex();
	int getNextIndex(int current);
	int getMaxIndex();
	std::unique_ptr<ImageInfo> getItemAt(int idx);
	static std::string getYear(const char *path);
};

} /* namespace simplearchive */
#endif /* CSVDBFILE_H_ */
