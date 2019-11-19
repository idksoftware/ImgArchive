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

#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include "ImageExtentions.h"
#include "SAUtils.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

std::string ExtentionItem::toString() {
	std::stringstream s;
	s << m_ext << ':' << m_type.toString() << ':' << m_mimeType << ':' << m_desciption;
	return s.str();
}

class CExtentionsContainer : public std::map<std::string, ExtentionItem> {

public:

};

class CExtentionsFile {
	std::unique_ptr<CExtentionsContainer> m_extentionsContainer;

	bool insert(const char *row);
public:
	CExtentionsFile();

	virtual ~CExtentionsFile();
	bool read(const char *datafile);
	bool write(const char *datafile);
	//bool add(const char *ext, const char *name);
	ExtentionItem *find(const char *ext);

};

CExtentionsFile::CExtentionsFile() : m_extentionsContainer(std::make_unique<CExtentionsContainer>())
{
}

CExtentionsFile::~CExtentionsFile() {
	
}

bool CExtentionsFile::read(const char *datafile) {
	std::string text;
	std::ifstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}

	while (std::getline(file, text)) {
		insert(text.c_str());
	}
	file.close();

	return true;
}
bool CExtentionsFile::write(const char *datafile) {
	std::ofstream file(datafile);
	if (file.is_open() == false) {
		return false;
	}
	for (std::map<std::string, ExtentionItem>::iterator ii = m_extentionsContainer->begin(); ii != m_extentionsContainer->end(); ++ii) {
		ExtentionItem &data = ii->second;
		file << data.toString() << '\n';

	}

	file.close();
	return true;
}
/*
bool CExtentionsFile::add(const char *ext, const char *name) {

	std::string extStr = ext;
	std::string nameStr = name;
	std::string *row = new std::string(extStr + ':' + nameStr);
	if (find(ext) != -1) {
		return false; // found
	}
	//insert(ext, *row);
	return true;
}
*/
bool CExtentionsFile::insert(const char *row) {


	ExtentionItem extentionItem(row);

	std::map<std::string, ExtentionItem>::iterator it;
	std::string ext = extentionItem.getExt();
	if (m_extentionsContainer->empty() == true || (it = m_extentionsContainer->find(ext)) == m_extentionsContainer->end()) {
		m_extentionsContainer->insert(std::make_pair(extentionItem.getExt(), extentionItem));
		return true;
	}
	return false;
}



ExtentionItem *CExtentionsFile::find(const char *ext) {

	if (m_extentionsContainer->size() == 0) {
		return 0;
	}
	std::string tmp = ext;
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
	std::map<std::string, ExtentionItem>::iterator it;
	if ((it = m_extentionsContainer->find(tmp)) == m_extentionsContainer->end()) {
		for (std::map<std::string, ExtentionItem>::iterator ii = m_extentionsContainer->begin(); ii != m_extentionsContainer->end(); ++ii) {
			ExtentionItem &data = ii->second;
			//printf("%s\n", data.toString());
		}
		return 0;
	}

	return &(it->second);
}

std::string ImageExtentions::m_extentionsFilePath;

std::unique_ptr<CExtentionsFile> ImageExtentions::m_extentionsFile = 0;
bool ImageExtentions::m_once = true;
bool ImageExtentions::m_isError = false;
static ImageType defaultImageType;
static ExtentionItem defaultExtentionItem;



ImageExtentions::ImageExtentions() {

}

ImageExtentions &ImageExtentions::get() {
	static ImageExtentions INSTANCE;
	if (m_once) {
		std::string path = m_extentionsFilePath + "/ext.dat";
		if (SAUtils::FileExists(path.c_str()) == false) {
			m_isError = true;
			throw std::exception();
			
		}
		m_extentionsFile = std::make_unique<CExtentionsFile>();
		m_extentionsFile->read(path.c_str());
		m_once = false;
	}
	return INSTANCE;
}

ImageExtentions::~ImageExtentions() {
	
}

bool ImageExtentions::setExtentionsFilePath(const char *extentionsFilePath) {
	m_extentionsFilePath = extentionsFilePath;
	std::string path = m_extentionsFilePath + "/ext.dat";
	if (SAUtils::FileExists(path.c_str()) == false) {
		m_isError = true;
		return false;
	}
	return true;
}

ExtentionItem &ImageExtentions::find(const char *filename) {
	std::string ext = SAUtils::getExtention(filename);
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	ExtentionItem *item = m_extentionsFile->find(ext.c_str());
	if (item != 0) {
		return *item;
	}
	return defaultExtentionItem;
}

ImageType &ImageExtentions::findType(const char *ext) {

	std::string tmp = ext;
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
	ExtentionItem *item = m_extentionsFile->find(ext);
	if (item != 0) {
		return item->getType();
	}
	return defaultImageType;
}

ImageType &ImageExtentions::getType(const char *filename) {
	std::string ext = SAUtils::getExtention(filename);
	return findType(ext.c_str());
}

bool ImageExtentions::IsValid(const char *filename) {

	if (SAUtils::hasExt(filename) == false) {
		return false;
	}
	std::string ext = SAUtils::getExtention(filename);
	ImageType &type = findType(ext.c_str());
	if (type.getType() == ImageType::UNKNOWN_EXT) {
		return false;
	}
	return true;
}

bool ImageExtentions::IsValidXML(const char *filename) {
	if (SAUtils::hasExt(filename) == false) {
		return false;
	}
	std::string ext = SAUtils::getExtention(filename);
	std::transform(ext.begin(),ext.end(), ext.begin(), ::tolower);
	if (ext.compare("xml") == 0) {
		return true;
	}
	return false;
}


} /* namespace simplearchive */
