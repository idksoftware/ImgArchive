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

#include "ImageIndex.h"
#include "SAUtils.h"
#include <exception>
//#include <stdio.h>
#include <vector>
#include <fstream>
#include <string>
#include <streambuf>
#include <algorithm>
#include "CIDKCrc.h"
#include "md5.h"
#include "BasicExifFactory.h"
#include "CLogger.h"

namespace simplearchive {

class DataContainer : public std::vector<std::string> {

public:

};

class DupDataFile {
	DataContainer *m_dataContainer;
	bool compare(std::string c1, std::string c2);
	void sorted();
	void insert(unsigned long crc, std::string);
public:
	DupDataFile();
	virtual ~DupDataFile();
	bool read(const char *datafile);
	bool write(const char *datafile);
	bool add(const char *name, unsigned long crc, const char *md5);
	int find(unsigned long crc);
	std::string findData(unsigned long crc);
	bool IsEmpty() {
		return m_dataContainer->empty();
	}

};

DupDataFile::DupDataFile() {
	m_dataContainer = new DataContainer();
}

DupDataFile::~DupDataFile() {

}

bool DupDataFile::read(const char *datafile) {
	char text[256];
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (file.getline(text, 100)) {
		m_dataContainer->push_back(*(new std::string(text)));
	}
	file.close();

	return true;
}
bool DupDataFile::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::vector<std::string>::iterator i = m_dataContainer->begin(); i != m_dataContainer->end(); i++) {
		std::string name = *i;
		file << name + '\n';
		//printf("%s", name.c_str());
	}
	file.close();
	return true;
}
bool DupDataFile::add(const char *name, unsigned long crc, const char *md5) {
	//char c_crc[9];
	std::string crcStr; // = c_crc;
	SAUtils::sprintf(crcStr, "%.8x", crc);
	//std::string crcStr = c_crc;
	std::string nameStr = name;
	std::string md5Str = md5;
	std::string *row = new std::string(crcStr + ':' + nameStr + ':' + md5Str);
	if (find(crc) != -1) {
		return false; // found
	}
	insert(crc, *row);
	return true;
}

void DupDataFile::insert(unsigned long crc, std::string row) {
	bool found = false;
	int pos = 0;
	for (std::vector<std::string>::iterator i = m_dataContainer->begin(); i != m_dataContainer->end(); i++) {
		std::string data = *i;
		//printf("%s", data.c_str());
		int delim1 = data.find_first_of(':');
		std::string crcStr = data.substr(0,delim1);
		int newcrc = std::stoul(crcStr.c_str(), NULL, 16);
		if (newcrc > crc) {
			found = true;
			break;
		}
		pos++;
	}
	if (!found) {
		m_dataContainer->push_back(row);
	} else {
		m_dataContainer->insert(m_dataContainer->begin()+(pos), row);
	}

}

void DupDataFile::sorted() {
	//std::sort(m_dataContainer->begin(), m_dataContainer->end(), compare);
}
bool DupDataFile::compare(std::string c1, std::string c2) {
	return true;
}

std::string DupDataFile::findData(unsigned long crc) {
	int pos = 0;
	std::string data;
	if (m_dataContainer->size() == 0) {
		return data;
	}
	for (std::vector<std::string>::iterator i = m_dataContainer->begin(); i != m_dataContainer->end(); i++) {

		data = *i;
		//printf("%s", data.c_str());
		int delim1 = data.find_first_of(':');
		std::string crcStr = data.substr(0, delim1);
		unsigned long newcrc = std::stoul(crcStr.c_str(), NULL, 16);
		if (newcrc == crc) {
			/* crc only
			DupData dupData(data.c_str());
			if (dupData.getMd5().compare(md5) == 0) {
			return pos;
			}
			*/
			return data;
		}
		pos++;
	}

	return data;
}

int DupDataFile::find(unsigned long crc) {
	int pos = 0;
	if (m_dataContainer->size() == 0) {
		return -1;
	}
	for (std::vector<std::string>::iterator i = m_dataContainer->begin(); i != m_dataContainer->end(); i++, pos++) {

		std::string data = *i;
		//printf("%s", data.c_str());
		int delim1 = data.find_first_of(':');
		std::string crcStr = data.substr(0,delim1);
		unsigned long newcrc = std::stoul(crcStr.c_str(), NULL, 16);
		if (newcrc == crc) {
			/* crc only
			DupData dupData(data.c_str());
			if (dupData.getMd5().compare(md5) == 0) {
				return pos;
			}
			*/
			return pos;
		}
		pos++;
	}

	return ((int)-1);
}

ImageIndex::ImageIndex() {}

ImageIndex::~ImageIndex() {
	// TODO Auto-generated destructor stub
}

bool ImageIndex::init(const char *path) {
	if (SAUtils::DirExists(path) == false) {
		CLogger &logger = CLogger::getLogger();
		logger.log(CLogger::FATAL, "De-duplication database path not found: %s", path);
		// "De-duplication database path not found"
		return false;
	}
	m_dbpath = path;
	return true;
}

std::string get_file_contents(const char *filename)
{

	char buff[2048];
	int count = 0;
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}


bool ImageIndex::add(const BasicExif &basicExif) {
	
	std::string pathStr = basicExif.getPath();
	
#ifdef _WIN32
#define SEP "\\"
#else
#define SEP "/"
#endif
	unsigned long c = basicExif.getCrc();

	std::string filename = pathStr.substr(pathStr.find_last_of("/") + 1);
	return add(filename.c_str(), basicExif.getCrc(), basicExif.getMd5().c_str());
}

bool ImageIndex::add(const char *name, unsigned long crc, const char *md5) {
	char tmppath[3];

	m_data[0] = (unsigned char)crc & 0xFF;
	m_data[1] = (unsigned char)(crc >> 8) & 0xFF;
	m_data[2] = (unsigned char)(crc >> 2 * 8) & 0xFF;
	m_data[3] = (unsigned char)(crc >> 3 * 8) & 0xFF;

	//printf("%x %x %x %x : ", m_data[3], m_data[2], m_data[1], m_data[0]);
	sprintf(tmppath, "%.2x", m_data[3]);
	std::string path = m_dbpath + '/' + tmppath;
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	sprintf(tmppath, "%.2x", m_data[2]);
	path = path + '/' + tmppath;
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}

	DupDataFile dupDataFile;
	sprintf(tmppath, "%.2x", m_data[1]);
	path = path + '/' + tmppath;
	if (SAUtils::FileExists(path.c_str()) == true) {
		if (dupDataFile.read(path.c_str()) == false) {
			throw std::exception();
		}

	}
	if (dupDataFile.add(name, crc, md5) == false) {
		return false;
	}
	if (dupDataFile.write(path.c_str()) == false) {
		throw std::exception();
	}
	return true;
}

DupDataFile *ImageIndex::findDupDataFile(unsigned long crc) {
	std::string tmppath;

	m_data[0] = (unsigned char)crc & 0xFF;
	m_data[1] = (unsigned char)(crc >> 8) & 0xFF;
	m_data[2] = (unsigned char)(crc >> 2 * 8) & 0xFF;
	m_data[3] = (unsigned char)(crc >> 3 * 8) & 0xFF;

	//printf("%x %x %x %x : ", m_data[3], m_data[2], m_data[1], m_data[0]);
	SAUtils::sprintf(tmppath, "%.2x", m_data[3]);
	std::string path = m_dbpath + '/' + tmppath;
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}
	SAUtils::sprintf(tmppath, "%.2x", m_data[2]);
	path = path + '/' + tmppath;
	if (SAUtils::DirExists(path.c_str()) == false) {
		if (SAUtils::mkDir(path.c_str()) == false) {
			throw std::exception();
		}
	}

	DupDataFile *dupDataFile = new DupDataFile;
	SAUtils::sprintf(tmppath, "%.2x", m_data[1]);
	path = path + '/' + tmppath;
	if (SAUtils::FileExists(path.c_str()) == true) {
		if (dupDataFile->read(path.c_str()) == false) {
			throw std::exception();
		}

	}
	return dupDataFile;
}

bool ImageIndex::IsDup(unsigned long crc) {
	// this is a possable memory leek
	DupDataFile *dupDataFile = findDupDataFile(crc);
	if (dupDataFile->IsEmpty() == true) {
		delete dupDataFile;
		return false;
	}
	if (dupDataFile->find(crc) < 0) {
		return false;	// not found
	}
	return true;
}

bool ImageIndex::getData(unsigned long crc) {
	// this is a possable memory leek
	DupDataFile *dupDataFile = findDupDataFile(crc);
	if (dupDataFile->IsEmpty() == true) {
		delete dupDataFile;
		return false;
	}
	std::string data = dupDataFile->findData(crc);
	return true;
}

} /* namespace simplearchive */
