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


#ifndef METADATATEMPLATE_H_
#define METADATATEMPLATE_H_

#include <map>
#include <mutex>
#include "ConfigReader.h"
#include "ResultsPresentation.h"

namespace simplearchive {



/**
 * @brief Metadata Container
 *
 * This contains the metadata Object that is created as an outcome of the substitution process.
 * This process is contained in this class using the Configuration reader class ConfigReader
 * using the configuration container. As each template is read the contents will be placed
 * into this class. At the end of the process the contents of the Config class is copied into
 * the Metadata Object class MetadataObject. This class will be used for further substitutions
 * by the Exif reading and user defined metadata information.
 *
 */

class MetadataObject;

typedef std::unique_ptr<MetadataObject> MetadataObject_ptr;
typedef std::unique_ptr<ConfigBlock> Config_ptr;
//class Config;


class MetadataTemplateSchema : public MTTableSchema {
public:
	MetadataTemplateSchema() noexcept : MTTableSchema(TABLE_METADATA_TEMPLATE) {

		// Description
		add(MTSchema(MTSchema::Text, DB_DESCRIPTION));
		add(MTSchema(MTSchema::Text, DB_LABEL));
		add(MTSchema(MTSchema::Text, DB_TITLE));
		add(MTSchema(MTSchema::Text, DB_SUBJECT));
		add(MTSchema(MTSchema::Integer, DB_RATING));
		add(MTSchema(MTSchema::Integer, DB_RATINGPERCENT));
		add(MTSchema(MTSchema::Text, DB_TAGS));// template
		add(MTSchema(MTSchema::Text, DB_KEYWORDS));// template
		// Origin
		add(MTSchema(MTSchema::Text, DB_COMMENT));// template
		add(MTSchema(MTSchema::Text, DB_AUTHOR));

		// Cammara
		add(MTSchema(MTSchema::Text, DB_COPYRIGHT));
		add(MTSchema(MTSchema::Text, DB_MAKER));
		add(MTSchema(MTSchema::Text, DB_MODEL));	

		// Location
		add(MTSchema(MTSchema::Text, DB_LATITUDE));
		add(MTSchema(MTSchema::Text, DB_LONGITUDE));
		add(MTSchema(MTSchema::Text, DB_LOCATION));
		add(MTSchema(MTSchema::Text, DB_SCENE));// template

		// IPTC
		add(MTSchema(MTSchema::Text, DB_SOURCEURL));			// template
		add(MTSchema(MTSchema::Text, DB_USAGERIGHTS));// template
		add(MTSchema(MTSchema::Text, DB_COPYRIGHTURL));// template
		add(MTSchema(MTSchema::Text, DB_HEADLINE));// template
		add(MTSchema(MTSchema::Text, DB_CATEGORY));// template
		add(MTSchema(MTSchema::Text, DB_SOURCE));// template
		add(MTSchema(MTSchema::Text, DB_INSTRUCTIONS));// template

		add(MTSchema(MTSchema::Text, DB_CREATOR));// template
		add(MTSchema(MTSchema::Text, DB_JOBTITLE));// template
		add(MTSchema(MTSchema::Text, DB_ADDRESS));// template
		add(MTSchema(MTSchema::Text, DB_CITY));// template
		add(MTSchema(MTSchema::Text, DB_STATE));// template
		add(MTSchema(MTSchema::Text, DB_POSTALCODE));// template
		add(MTSchema(MTSchema::Text, DB_COUNTRY));// template
		add(MTSchema(MTSchema::Text, DB_PHONE));// template
		add(MTSchema(MTSchema::Text, DB_EMAIL));// template
		add(MTSchema(MTSchema::Text, DB_WEBSITE));// template

	}
};

class MetadataTemplateRow : public MTRow {
	static MetadataTemplateSchema m_tableSchema;

public:
	MetadataTemplateRow() : MTRow(m_tableSchema) {};
	//MetadataTemplateRow(const MTRow& row) : MTRow(row) {};
	MetadataTemplateRow(const MTRow& row) : MTRow(m_tableSchema) {

		for (unsigned int i = 0; i < row.size(); i++) {
			MTColumn& c1 = *at(i);
			MTColumn& c2 = *row.at(i);
			c1 = c2;
		}

	}

	
};
/*
	Note table of one row
*/
class MetadataTemplatePartition : public MTTable {
public:
	MetadataTemplatePartition() : MTTable(new MetadataTemplateSchema) {};
	virtual ~MetadataTemplatePartition() {};

};


class MetadataTemplateResultsPresentation {
	MTRow m_row;
	std::shared_ptr<MTTableSchema> m_tableSchema;
public:
	MetadataTemplateResultsPresentation(MTRow row) : m_row(row), m_tableSchema(new MetadataTemplateSchema) {};
	~MetadataTemplateResultsPresentation() = default;

	bool writeHuman();
	bool writeXML(); 
	bool writeCSV();
	bool writeJson();
	bool writeHtml();
};


class MetadataTemplate {
	
	
	static Config_ptr m_templateFile;
	static std::string &getValue(const char *key);
	static MetadataTemplateRow m_metadataTemplateRow;

	MetadataTemplate() noexcept = default;
public:
	static MetadataTemplate& GetInstance();
	virtual ~MetadataTemplate();
	
	MetadataTemplate(const MetadataTemplate& src) = delete;
	MetadataTemplate& operator=(const MetadataTemplate& rhs) = delete;

	static bool read(const char* path, const char *datafile);
	static bool write(const char* datafile);
	static MetadataObject_ptr getMetadataObject();
};

} /* namespace simplearchive */
#endif /* METADATATEMPLATE_H_ */
