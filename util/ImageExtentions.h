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

#ifndef IMAGEEXTENTIONS_H_
#define IMAGEEXTENTIONS_H_

#include <string>
#include <memory>
#include "ImageType.h"
#include "CSVArgs.h"

namespace simplearchive {

class CExtentionsFile;

class ExtentionItem {
	std::string m_ext;
	ImageType m_type;
	std::string m_mimeType;
	std::string m_desciption;
public:
	ExtentionItem() {
		m_ext = "";
		m_type = ImageType::Type::UNKNOWN_EXT;
	}

	ExtentionItem(const char *row) {
		std::string data = row;
		//printf("%s", data.c_str());
		
		CSVArgs csvArgs(':');
		csvArgs.process(row);
		m_ext = csvArgs.at(0);
		std::string typeStr = csvArgs.at(1);
		m_type = typeStr.c_str();
		m_mimeType = csvArgs.at(2);
		m_desciption = csvArgs.at(3);
	}

	ExtentionItem(std::string &ext,
					ImageType type,
					std::string &mimeType,
					std::string &desciption) {
		m_ext = ext;
		m_type = type;
		m_mimeType = mimeType;
		m_desciption = desciption;
	}

	const std::string& getDesciption() const {
		return m_desciption;
	}

	void setDesciption(const std::string& desciption) {
		m_desciption = desciption;
	}

	const std::string& getExt() {
		return m_ext;
	}

	void setExt(const std::string& ext) {
		m_ext = ext;
	}

	const std::string& getMimeType() const {
		return m_mimeType;
	}

	void setMimeType(const std::string& mimeType) {
		m_mimeType = mimeType;
	}

	ImageType &getType() {
		return m_type;
	}

	void setType(ImageType &type) {
		m_type = type;
	}

	std::string toString();

};
typedef std::unique_ptr<ExtentionItem> ExtentionItem_Ptr;

class ImageExtentions {

private:
	ImageExtentions();
	static ImageExtentions *m_This;
	static CExtentionsFile *m_extentionsFile;
	static std::string m_extentionsFilePath;
	static bool m_isError;
public:
	ImageType fromString();
	virtual ~ImageExtentions();
	ImageType &getType(const char *filename);
	ImageType &findType(const char *ext);
	ExtentionItem &find(const char *ext);
	static ImageExtentions &get();
	bool IsValid(const char *filename);
	bool IsValidXML(const char *filename);

	static const std::string& getExtentionsFilePath() {
		return m_extentionsFilePath;
	}

	static bool setExtentionsFilePath(const char *extentionsFilePath);

	static bool isError() {
		return m_isError;
	}
};

} /* namespace simplearchive */
#endif /* IMAGEEXTENTIONS_H_ */
