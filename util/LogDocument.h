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

#ifdef XXXXXXXXXXXXXXXXx

#ifndef LOGDOCUMENT_H_
#define LOGDOCUMENT_H_
#include <string>
#include <list>
#include <memory>

namespace simplearchive {
/**
 *
 */

class MTTableSchema;



class LogDocument : public std::list<std::string> {
	
public:
	enum class FormatType {
		XML,
		Json,
		Human,
		csv,
		Html,
		unknown
	};
protected:
	std::string writeTag(const char *tag, const std::string& value, int tab);
	std::string writeTag(const char *tag, const int value, int tab);
	std::shared_ptr<MTTableSchema> m_tableSchema;
	std::string m_title;
	std::string m_description;
	std::string m_image;

	std::string m_filename;

public:
	LogDocument(std::shared_ptr<MTTableSchema> tableSchema);
	virtual ~LogDocument();
	bool write(FormatType formatType);

	virtual bool writeHuman() = 0;
	virtual bool writeXML() = 0;
	virtual bool writeCSV() = 0;
	virtual bool writeJson() = 0;
	virtual bool writeHtml() = 0;

	void setFilename(const char* f) {
		m_filename = f;
	}

	void setTitle(const char *t) {
		m_title = t;
	}

	void setDescription(const char *d) {
		m_description = d;
	}

	void setImage(const char *i) {
		m_image = i;
	}

	static FormatType parse(const char *str);
};

} /* namespace simplearchive */
#endif /* LOGDOCUMENT_H_ */

#endif
