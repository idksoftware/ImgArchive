/*
 * CSVDBFile.h
 *
 *  Created on: Jul 22, 2014
 *      Author: wzw7yn
 */
#ifndef CSVDBFILE_H_
#define CSVDBFILE_H_
#include <string>
#include <memory>
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

public:
	ImageInfo(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
			const char *md5, const char *uuid, int version, ExifDate &date);
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

class CSVDBFile {
	std::string m_dbpath;
	unsigned int m_data[3];
	int getMaxDirIndex(std::string &path);
public:
	CSVDBFile(const char *path);
	virtual ~CSVDBFile();
	bool insert(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
						const char *md5, const char *uuid, int version, ExifDate &date);
	const char* findPath(unsigned int idx);
	unsigned long findSize(unsigned int idx);
	int getNextIndex();
	int getNextIndex(int current);
	int getMaxIndex();
	std::auto_ptr<ImageInfo> getItemAt(int idx);
	static std::string getYear(const char *path);
};

} /* namespace simplearchive */
#endif /* CSVDBFILE_H_ */
