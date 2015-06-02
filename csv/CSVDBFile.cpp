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

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <streambuf>
#include "SAUtils.h"
#include "CSVDBFile.h"
#include "CSVArgs.h"

namespace simplearchive {

ImageInfo::ImageInfo(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
						const char *md5, const char *uuid, int version, ExifDate &date)
{
	m_idx = idx;
	m_shortImagePath = imagePath;
	m_imagePath = CSVDBFile::getYear(imagePath);
	m_imagePath += '/';
	m_imagePath += imagePath;
	m_name = name;
	m_crc = crc;
	m_size = size;
	m_md5 = md5;
	m_uuid = uuid;
	m_version = version;
	m_dateArchived = date;
}

ImageInfo::~ImageInfo() {
	//printf("ImageInfo deleted\n");
}

std::string CSVDBFile::getYear(const char *path) {
	std::string fpath = path;
	return fpath.substr(0, 4);
}

CSVDBFile::CSVDBFile(const char *path) {
	m_dbpath = path;
}

CSVDBFile::~CSVDBFile() {

}

class IdxFileItem {
	int m_idx;
	std::string m_imagePath;
	std::string m_name;
	unsigned long m_crc;
	std::string m_md5;
	std::string m_uuid;
	unsigned long m_size;
	int m_version;
	ExifDate m_dateArchived;
public:
	IdxFileItem(const char *text) {
		CSVArgs csvArgs(':');
		csvArgs.process(text);
		std::string idxStr = csvArgs.at(0);

		//m_idx = strtol(idxStr.c_str(), NULL, 16);
		m_idx = strtol(csvArgs.at(0).c_str(), 0, 10);
		m_imagePath = csvArgs.at(1);
		m_name = csvArgs.at(2);
		m_size = strtol(csvArgs.at(3).c_str(), 0, 10);
		m_crc = strtol(csvArgs.at(4).c_str(), 0, 10);
		m_md5 = csvArgs.at(5);
		m_uuid = csvArgs.at(6);
		m_version = strtol(csvArgs.at(7).c_str(), 0, 10);
		std::string dateStr = csvArgs.at(8);
		ExifDate date(csvArgs.at(8).c_str());
		m_dateArchived = date;

	}
	IdxFileItem(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
					const char *md5, const char *uuid, int version, ExifDate &date) {
		m_idx = idx;
		m_imagePath = imagePath;
		m_name = name;
		m_crc = crc;
		m_size = size;
		m_md5 = md5;
		m_uuid = uuid;
		m_version = version;
		m_dateArchived = date;
	}

	std::auto_ptr<ImageInfo> getImageInfo() {

		std::auto_ptr<ImageInfo> imageInfo (new ImageInfo(m_idx, m_imagePath.c_str(),m_name.c_str(), m_crc,
										m_size, m_md5.c_str(), m_uuid.c_str(), m_version, m_dateArchived));
		return imageInfo;
	}

	int getIdx() const {
		return m_idx;
	}

	void setIdx(int idx) {
		m_idx = idx;
	}

	const std::string& getImagePath() const {
		return m_imagePath;
	}

	void setImagePath(const char *imagePath) {
		m_imagePath = imagePath;
	}

	const std::string& getName() const {
		return m_name;
	}

	void setName(const std::string& name) {
		m_name = name;
	}

	unsigned long getSize() const {
		return m_size;
	}

	unsigned long getCrc() const {
		return m_crc;
	}

	const std::string& getMd5() const {
		return m_md5;
	}

	const std::string& getUuid() const {
		return m_uuid;
	}

	int getVersion() const {
		return m_version;
	}

	void setVersion(int version) {
		m_version = version;
	}

	ExifDate& getDateArchived() {
		return m_dateArchived;
	}


};

class IdxFile {
	bool compare(std::string c1, std::string c2);
	void sorted();
	bool insert(int idx, const char *imagePath, const char *name, unsigned long size,
			unsigned long crc, const char *md5, const char *uuid, int version, ExifDate &date);
	IdxFileItem *list[256];
	int m_last;
public:
	IdxFile() {
		m_last = 0;
		for (int i = 0; i < 256; i++) {
			list[i] = 0;
		}
	};
	virtual ~IdxFile() {};
	bool read(const char *datafile);
	bool write(const char *datafile);
	bool update(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
			const char *md5, const char *uuid, int version, ExifDate &date);
	IdxFileItem *find(int idx);
	int findLast();

};

bool CSVDBFile::insert(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date) {

	char tmppath[10];
	//unsigned int idx = indx - 1;
	m_data[0] = (unsigned int)idx & 0xFF;
	m_data[1] = (unsigned int)(idx >> 8) & 0xFFF;
	m_data[2] = (unsigned int)(idx >> (12 + 8)) & 0xFFF;

	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		return false;
	}
	//printf("%x: %x %x %x\n",idx,  m_data[2], m_data[1], m_data[0]);

	sprintf(tmppath, "%.3x", m_data[2]);
	std::string path = m_dbpath + '/' + tmppath;
	//printf("%s\n",path.c_str());
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	sprintf(tmppath, "%.3x.csv", m_data[1]);
	path = path + '/' + tmppath;
	IdxFile idxFile;
	//printf("%s\n",path.c_str());
	if (SAUtils::FileExists(path.c_str()) == true) {
		if (idxFile.read(path.c_str()) == false) {
			throw std::exception();
		}

	}

	std::string imagepath = imagePath;
	int pos = imagepath.find_first_of('/');
	if (pos == std::string::npos) {
		return false;
	}
	imagepath = imagepath.substr(5, imagepath.length() - 5);
	if (idxFile.update(idx, imagepath.c_str(), name, size, crc, md5, uuid, version, date) == false) {
		return false;
	}
	if (idxFile.write(path.c_str()) == false) {
		throw std::exception();
	}
	return true;
}

int CSVDBFile::getMaxIndex() {
	int m_data[4];
	char tmppath[10];
	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		return false;
	}
	// Folders
	m_data[3] = getMaxDirIndex(m_dbpath);
	sprintf(tmppath, "%.3x", m_data[3]);
	std::string path = m_dbpath + '/' + tmppath;
	if (SAUtils::DirExists(path.c_str()) == false) {
		// if empty and getMaxIndex returned zero no sequence numbers. so create
		if (m_data[3] == 0) {
			SAUtils::mkDir(path.c_str());
		}
		else {
			throw std::exception();
		}
	}
	// files
	//printf("%s\n", path.c_str());
	m_data[2] = getMaxDirIndex(path);
	sprintf(tmppath, "%.3x.csv", m_data[2]);
	path = path + '/' + tmppath;
	if (SAUtils::FileExists(path.c_str()) == false) {
		if (m_data[2] != 0) {
			throw std::exception();
		}
	}
	int last = -1;
	IdxFile idxFile;
	//printf("%s\n",path.c_str());
	if (idxFile.read(path.c_str()) == false) {
		last = -1;
	} else {
		last = idxFile.findLast();
	}

	return last;

}

int CSVDBFile::getNextIndex(int current) {
	char tmppath[5];
	m_data[0] = (unsigned int)current & 0xFF;
	m_data[1] = (unsigned int)(current >> 8) & 0xFFF;
	m_data[2] = (unsigned int)(current >> (12 + 8)) & 0xFFF;
	sprintf(tmppath, "%.3x", m_data[3]);
	std::string path = m_dbpath + '/' + tmppath;
	if (SAUtils::DirExists(path.c_str()) == false) {
		// if empty and getMaxIndex returned zero no sequence numbers. so create
		if (m_data[3] == 0) {
			SAUtils::mkDir(path.c_str());
		}
		else {
			throw std::exception();
		}
	}
	// files
	//printf("%s\n", path.c_str());
	m_data[2] = getMaxDirIndex(path);
	sprintf(tmppath, "%.3x.csv", m_data[2]);
	path = path + '/' + tmppath;
	if (SAUtils::FileExists(path.c_str()) == false) {
		if (m_data[2] != 0) {
			throw std::exception();
		}
	}
	int last = -1;
	IdxFile idxFile;
	//printf("%s\n",path.c_str());
	if (idxFile.read(path.c_str()) == false) {
		last = -1;
	} else {
		last = idxFile.findLast();
	}
	++last;
	return last;
}

int CSVDBFile::getNextIndex() {
	int last = getMaxIndex();
	last++;
	return last;
}
int CSVDBFile::getMaxDirIndex(std::string &path) {

	int max = 0;

	std::vector<std::string *> *filelist = SAUtils::getFiles(path.c_str());
	for (std::vector<std::string *>::iterator i = filelist->begin(); i != filelist->end(); i++) {
		std::string *name = *i;
		if (!(name->compare(".")) || !(name->compare(".."))) {
			continue;
		}
		//printf("%s", name->c_str());
		int idx = strtol(name->c_str(), NULL, 16);
		if (idx > max) {
			max = idx;
		}
	}

	return max;
}

std::auto_ptr<ImageInfo> CSVDBFile::getItemAt(int idx) {
	char tmppath[10];

	m_data[0] = (unsigned int)idx & 0xFF;
	m_data[1] = (unsigned int)(idx >> 8) & 0xFFF;
	m_data[2] = (unsigned int)(idx >> (12 + 8)) & 0xFFF;

	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		throw std::exception();
	}
	//printf("%x: %x %x %x\n",idx,  m_data[2], m_data[1], m_data[0]);

	sprintf(tmppath, "%.3x", m_data[2]);
	std::string path = m_dbpath + '/' + tmppath;
	//printf("%s\n",path.c_str());
	if (SAUtils::DirExists(path.c_str()) == false) {
		throw std::exception();
	}
	sprintf(tmppath, "%.3x.csv", m_data[1]);
	path = path + '/' + tmppath;
	IdxFile idxFile;
	//printf("%s\n",path.c_str());
	if (SAUtils::FileExists(path.c_str()) == false) {
		throw std::exception();
	}
	if (idxFile.read(path.c_str()) == false) {
		throw std::exception();
	}
	IdxFileItem *item = 0;

	if ((item = idxFile.find(idx)) == 0) {
		throw std::exception();
	}
	std::auto_ptr<ImageInfo> ii = item->getImageInfo();
	return ii;


}

unsigned long CSVDBFile::findSize(unsigned int idx) {
	char tmppath[10];

	m_data[0] = (unsigned int)idx & 0xFF;
	m_data[1] = (unsigned int)(idx >> 8) & 0xFFF;
	m_data[2] = (unsigned int)(idx >> (12 + 8)) & 0xFFF;

	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		return 0;
	}
	//printf("%x: %x %x %x\n",idx,  m_data[2], m_data[1], m_data[0]);

	sprintf(tmppath, "%.3x", m_data[2]);
	std::string path = m_dbpath + '/' + tmppath;
	//printf("%s\n",path.c_str());
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			return 0;
		}
	}
	sprintf(tmppath, "%.3x.csv", m_data[1]);
	path = path + '/' + tmppath;
	IdxFile idxFile;
	//printf("%s\n",path.c_str());
	if (SAUtils::FileExists(path.c_str()) == true) {
		if (idxFile.read(path.c_str()) == false) {
			return 0;
		}
		const IdxFileItem *item = 0;

		if ((item = idxFile.find(idx)) == 0) {
			return 0;
		}
		//printf("%d ", item->getIdx());
		return item->getSize();
	}
	return -1;
}

const char* CSVDBFile::findPath(unsigned int idx) {
	char tmppath[10];

	m_data[0] = (unsigned int)idx & 0xFF;
	m_data[1] = (unsigned int)(idx >> 8) & 0xFFF;
	m_data[2] = (unsigned int)(idx >> (12 + 8)) & 0xFFF;

	if (SAUtils::DirExists(m_dbpath.c_str()) == false) {
		return 0;
	}
	//printf("%x: %x %x %x\n",idx,  m_data[2], m_data[1], m_data[0]);

	sprintf(tmppath, "%.3x", m_data[2]);
	std::string path = m_dbpath + '/' + tmppath;
	//printf("%s\n",path.c_str());
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			return 0;
		}
	}
	sprintf(tmppath, "%.3x.csv", m_data[1]);
	path = path + '/' + tmppath;
	IdxFile idxFile;
	//printf("%s\n",path.c_str());
	if (SAUtils::FileExists(path.c_str()) == true) {
		if (idxFile.read(path.c_str()) == false) {
			return 0;
		}
		const IdxFileItem *item = 0;

		if ((item = idxFile.find(idx)) == 0) {
			return 0;
		}
		//printf("%d ", item->getIdx());
		return item->getImagePath().c_str();
	}
	return 0;
}

bool IdxFile::read(const char *datafile) {
	char text[2 * 1012];
	m_last = -1;
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 2 * 1012)) {
		IdxFileItem *item = new IdxFileItem(text);
		int fullidx = item->getIdx();
		int fileidx = (unsigned int)fullidx & 0xFF;

		if (fileidx < 0 || fileidx > 255) {
			return false;
		}
		this->list[fileidx] = item;
		if (m_last < fileidx) {
			m_last = fileidx;
		}
	}
	file.close();
	return true;
}
bool IdxFile::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (int i = 0; i < 256; i++) {
		IdxFileItem *item = list[i];
		if (item == 0) {
			continue;
		}

		//file << std::hex  << item->getIdx() << ':' << item->getImagePath() + '\n';
		ExifDate &date = item->getDateArchived();
		std::string dateStr = date.toFileString();
		std::stringstream str;
		str << item->getIdx() << ':' << item->getImagePath() << ':' << item->getName() << ':'
				<< item->getSize() << ':' << item->getCrc() << ':'
				<< item->getMd5() << ':' << item->getUuid() << ':'
				<< item->getVersion() << ':' << dateStr.c_str() << '\n';

		file << str.str();
		//printf("%s", item->getImagePath().c_str());

	}
	file.close();
	return true;
}
bool IdxFile::update(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date) {
	IdxFileItem *idxItem = 0;
	if ((idxItem = find(idx)) != 0) {
		idxItem->setImagePath(imagePath);
	}
	insert(idx, imagePath, name, size, crc, md5, uuid, version, date);
	return true;
}


bool IdxFile::insert(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date) {

	IdxFileItem *item = new IdxFileItem(idx, imagePath, name, size, crc, md5, uuid, version, date);
	int fullidx = item->getIdx();
	int posIdx = (unsigned int)fullidx & 0xFF;
	if (posIdx < 0 || posIdx > 255) {
		return false;
	}
	if (m_last < posIdx) {
		m_last = posIdx;
	}
	this->list[posIdx] = item;
	return true;
}


IdxFileItem *IdxFile::find(int idx) {
	int fullidx = idx;
	int posIdx = (unsigned int)fullidx & 0xFF;
	if (posIdx < 0 || posIdx > 255) {
			return 0;
	}

	return this->list[posIdx];
}

int IdxFile::findLast() {
	if (m_last == -1) {
		return -1;
	}
	IdxFileItem *item = list[m_last];
	return item->getIdx();
}



unsigned long ImageInfo::getCrc() const {
	return m_crc;
}

const ExifDate& ImageInfo::getDate() const {
	return m_dateArchived;
}

int ImageInfo::getIdx() const {
	return m_idx;
}

const std::string& ImageInfo::getShortImagePath() const {
	return m_shortImagePath;
}

const std::string& ImageInfo::getImagePath() const {
	return m_imagePath;
}
const std::string& ImageInfo::getName() const {
	return m_name;
}
const std::string& ImageInfo::getMd5() const {
	return m_md5;
}

unsigned long ImageInfo::getSize() const {
	return m_size;
}

const std::string& ImageInfo::getUuid() const {
	return m_uuid;
}

int ImageInfo::getVersion() const {
	return m_version;
}
} /* namespace simplearchive */
