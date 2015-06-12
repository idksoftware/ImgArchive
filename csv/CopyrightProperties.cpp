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

#include <cstdlib>
#include "MetadataObject.h"
#include "CopyrightProperties.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

namespace simplearchive {

class CopyrightPropertiesString : public CSVString {
public:
	CopyrightPropertiesString() {};
	virtual ~CopyrightPropertiesString() {};
	virtual const char *toString(CSVRowItem &item);

};

const char *CopyrightPropertiesString::toString(CSVRowItem &item) {
	CopyrightPropertiesItem &tmp = (CopyrightPropertiesItem &)item;
	return tmp.toString();
}

CopyrightProperties::CopyrightProperties() {
	// TODO Auto-generated constructor stub

}

CopyrightProperties::~CopyrightProperties() {
	// TODO Auto-generated destructor stub
}

// Adds the information of the side car file from a MetadataObject.
void CopyrightProperties::add(MetadataObject &metadataObject) {
	const char *idStr = metadataObject.getSequenceId().c_str();
	int seqId = atol(idStr);
	CopyrightPropertiesItem *item = new CopyrightPropertiesItem(seqId);
	item->setCopyright(metadataObject.getCopyright());
	item->setUsageRights(metadataObject.getUsageRights());
	item->setCopyrightUrl(metadataObject.getCopyrightUrl());
	push_back(item);
	return;
}

/// reads a csv side car file
bool CopyrightProperties::write(const char *path) {
	std::string filename("CopyrightProperties.csv");
	CopyrightPropertiesString cov;
	CSVRow::write(path, filename.c_str(), cov);

	return true;
}

// reads a csv side car file
bool CopyrightProperties::read(const char *datafile) {
	std::string path(datafile);
	path += "/CopyrightProperties.csv";
	if (CSVRow::read(path.c_str()) == false) {
		return false;
	}
	return true;
}

unsigned int CopyrightProperties::findImage(const char *text, int col) {
	unsigned int count = 0;
	for (std::vector<CSVRowItem *>::iterator i = this->begin(); i != this->end(); i++, count++) {
		CopyrightPropertiesItem *item = (CopyrightPropertiesItem *)*i;
		std::string field;
		switch (col) {
        	case 0: field = item->getCopyright(); break;
        	case 1: field = item->getUsageRights(); break;
        	case 2: field = item->getCopyrightUrl(); break;
        	return std::string::npos;
		}
		if (field.compare(text) == 0) {
			return count;
		}
	}
	return std::string::npos;
}
bool CopyrightProperties::load(unsigned int row, MetadataObject &mo) {
	if (this->size() < row ) {
		return false;
	}
	CopyrightPropertiesItem *item = (CopyrightPropertiesItem *)at(row);
    mo.setCopyright(item->getCopyright());
	mo.setUsageRights(item->getUsageRights());
	mo.setCopyrightUrl(item->getCopyrightUrl());

	return true;
}
bool CopyrightProperties::save(unsigned int row, MetadataObject &mo) {
	if (this->size() < row ) {
		return false;
	}
	CopyrightPropertiesItem *item = (CopyrightPropertiesItem *)at(row);
    item->setCopyright(mo.getCopyright());
	item->setUsageRights(mo.getUsageRights());
	item->setCopyrightUrl(mo.getCopyrightUrl());

	return true;
}

bool CopyrightProperties::add(const char *row) {
	std::string tmp = row;
	int pos = 0;
	int seqId = readId(tmp, &pos);
	CopyrightPropertiesItem *item = new CopyrightPropertiesItem(seqId);
	item->fromString(pos+1, tmp);
	push_back(item);
	return true;
}

CopyrightPropertiesItem::CopyrightPropertiesItem(int id) : CSVRowItem(id) {
	// TODO Auto-generated constructor stub

}

const char *CopyrightPropertiesItem::toString() {
	start();
	add(m_copyright);
	add(m_usageRights);
	add(m_copyrightURL);
	end();
	return row().c_str();
}




bool CopyrightPropertiesItem::fromString(int pos, std::string &str) {
	m_copyright = token(&pos, str);
	pos++;
	m_usageRights = token(&pos, str);
	pos++;
	m_copyrightURL = token(&pos, str);
	return true;
}

const std::string& CopyrightPropertiesItem::getCopyright() const {
	return m_copyright;
}

void CopyrightPropertiesItem::setCopyright(const std::string& copyright) {
	m_copyright = copyright;
}

const std::string& CopyrightPropertiesItem::getCopyrightUrl() const {
	return m_copyrightURL;
}

void CopyrightPropertiesItem::setCopyrightUrl(const std::string& copyrightUrl) {
	m_copyrightURL = copyrightUrl;
}

const std::string& CopyrightPropertiesItem::getSequenceId() const {
	return m_sequenceId;
}

void CopyrightPropertiesItem::setSequenceId(const std::string& sequenceId) {
	m_sequenceId = sequenceId;
}

const std::string& CopyrightPropertiesItem::getUsageRights() const {
	return m_usageRights;
}

void CopyrightPropertiesItem::setUsageRights(const std::string& usageRights) {
	m_usageRights = usageRights;
}

} /* namespace simplearchive */
