/*
 * CSVDBFile.cpp
 *
 *  Created on: Jul 22, 2014
 *      Author: wzw7yn
 */
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <streambuf>
#include "SAUtils.h"
#include "CSVDBFile.h"

namespace simplearchive {

ImageInfo::ImageInfo(int idx, const char *imagePath, unsigned long size, unsigned long crc,
						const char *md5, const char *uuid, int version, ExifDate &date)
{
	m_idx = idx;
	m_imagePath = imagePath;
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

CSVDBFile::CSVDBFile(const char *path) {
	m_dbpath = path;
}

CSVDBFile::~CSVDBFile() {

}

class IdxFileItem {
	int m_idx;
	std::string m_imagePath;
	unsigned long m_crc;
	std::string m_md5;
	std::string m_uuid;
	unsigned long m_size;
	int m_version;
	ExifDate m_dateArchived;
public:
	IdxFileItem(const char *text) {
		std::string data = text;
		int delim1 = data.find_first_of(':');
		int delim2 = data.find_first_of(":", delim1+1);
		int delim3 = data.find_first_of(":", delim2+1);
		int delim4 = data.find_first_of(":", delim3+1);
		int delim5 = data.find_first_of(":", delim4+1);
		int delim6 = data.find_first_of(":", delim5+1);
		int delim7 = data.find_first_of(":", delim6+1);
		std::string idxStr = data.substr(0,delim1);

		//m_idx = strtol(idxStr.c_str(), NULL, 16);
		m_idx = strtol(idxStr.c_str(), 0, 10);
		m_imagePath = data.substr(delim1+1, delim2-(delim1+1));
		std::string tmp = data.substr(delim2+1, delim3-(delim2+1));
		m_size = strtol(tmp.c_str(), 0, 10);
		tmp = data.substr(delim3+1, delim4-(delim3+1));
		m_crc = strtol(tmp.c_str(), 0, 10);
		m_md5 = data.substr(delim4+1, delim5-(delim4+1));
		m_uuid = data.substr(delim5+1, delim6-(delim5+1));
		tmp = data.substr(delim6+1, delim7-(delim6+1));
		m_version = strtol(tmp.c_str(), 0, 10);
		tmp = data.substr(delim7+1, data.length());
		ExifDate date(tmp.c_str());
		m_dateArchived = date;

	}
	IdxFileItem(int idx, const char *imagePath, unsigned long size, unsigned long crc,
					const char *md5, const char *uuid, int version, ExifDate &date) {
		m_idx = idx;
		m_imagePath = imagePath;
		m_crc = crc;
		m_size = size;
		m_md5 = md5;
		m_uuid = uuid;
		m_version = version;
		m_dateArchived = date;
	}

	std::auto_ptr<ImageInfo> getImageInfo() {
		// ImageInfo(int idx, const char *imagePath, unsigned long size, unsigned long crc,
		// const char *md5, const char *uuid, int version, CDate &date);
		//

		std::auto_ptr<ImageInfo> imageInfo(new ImageInfo(m_idx, m_imagePath.c_str(), m_size,
										m_crc, m_md5.c_str(), m_uuid.c_str(), m_version, m_dateArchived));
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
	//std::string toString() {
	//	std::string row(nameStr + ':' + m_imagePath);
	//}

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
	bool insert(int idx, const char *imagePath, unsigned long size,
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
	bool update(int idx, const char *imagePath, unsigned long size, unsigned long crc,
			const char *md5, const char *uuid, int version, ExifDate &date);
	IdxFileItem *find(int idx);
	int findLast();

};

bool CSVDBFile::insert(int idx, const char *imagePath, unsigned long size, unsigned long crc,
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
	if (idxFile.update(idx, imagePath, size, crc, md5, uuid, version, date) == false) {
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
	char tmppath[5];

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
	char tmppath[5];

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
	char text[256];
	m_last = -1;
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 255)) {
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
		std::string dateStr = date.toString();
		std::stringstream str;
		str << item->getIdx() << ':' << item->getImagePath() << ':'
				<< item->getSize() << ':' << item->getCrc() << ':'
				<< item->getMd5() << ':' << item->getUuid() << ':'
				<< item->getVersion() << ':' << dateStr.c_str() << '\n';

		file << str.str();
		//printf("%s", item->getImagePath().c_str());

	}
	file.close();
	return true;
}
bool IdxFile::update(int idx, const char *imagePath, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date) {
	IdxFileItem *idxItem = 0;
	if ((idxItem = find(idx)) != 0) {
		idxItem->setImagePath(imagePath);
	}
	insert(idx, imagePath, size, crc, md5, uuid, version, date);
	return true;
}


bool IdxFile::insert(int idx, const char *imagePath, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date) {

	IdxFileItem *item = new IdxFileItem(idx, imagePath, size, crc, md5, uuid, version, date);
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

const std::string& ImageInfo::getImagePath() const {
	return m_imagePath;
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
