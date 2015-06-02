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

#ifndef COPYRIGHTPROPERTIES_H_
#define COPYRIGHTPROPERTIES_H_
#include <string>
#include <vector>
#include "CSVRow.h"

namespace simplearchive {

class CopyrightPropertiesItem : public CSVRowItem {
	std::string m_sequenceId;
	std::string m_copyright;
	std::string m_usageRights;
	std::string m_copyrightURL;
public:
	CopyrightPropertiesItem(int id);
	virtual ~CopyrightPropertiesItem() {};
	const std::string& getCopyright() const;
	void setCopyright(const std::string& copyright);
	const std::string& getCopyrightUrl() const;
	void setCopyrightUrl(const std::string& copyrightUrl);
	const std::string& getSequenceId() const;
	void setSequenceId(const std::string& sequenceId);
	const std::string& getUsageRights() const;
	void setUsageRights(const std::string& usageRights);

	virtual const char *toString();
	virtual bool fromString(int pos, std::string &str);

};

class MetadataObject;
class CopyrightProperties : public CSVRow {
public:
	CopyrightProperties();
	virtual ~CopyrightProperties();

	/// Adds the information of the GPS properties from a MetadataObject.
	void add(MetadataObject &metadataObject);

	/// Adds a GPSPropertiesItem object.
	void add(CopyrightPropertiesItem &item);

	/// reads a csv GPSProperties file
	bool read(const char *datafile);

	/// writes a csv GPSProperties file
	bool write(const char *path);

	unsigned int findImage(const char *text, int col);
	bool load(unsigned int row, MetadataObject &mo);
	bool save(unsigned int row, MetadataObject &mo);
	//bool write(const char *path, const char *filename, CSVString& u) {
	//	return CSVRow::write(path, filename, u);
	//}

	virtual bool add(const char *row);
};

} /* namespace simplearchive */
#endif /* COPYRIGHTPROPERTIES_H_ */
