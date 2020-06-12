#pragma once

#include <map>
#include <mutex>
#include "DBDefines.h"
#include "MetaType.h"
#include "ConfigReader.h"
#include "ResultsPresentation.h"

namespace simplearchive {
	
class DerivativeMetadataObject;

typedef std::unique_ptr<DerivativeMetadataObject> DerivativeMetadataObject_ptr;
typedef std::unique_ptr<ConfigBlock> Config_ptr;

class DerivativeMetadataTemplateSchema : public MTTableSchema {
public:
	DerivativeMetadataTemplateSchema() noexcept : MTTableSchema(TABLE_DERIVATIVE_TEMPLATE) {

		add(MTSchema(MTSchema::Text, DB_COMMENT));
		add(MTSchema(MTSchema::Text, DB_AUTHOR));
		add(MTSchema(MTSchema::Text, DB_DESCRIPTION));
		add(MTSchema(MTSchema::Text, DB_EDITOR));
	}
};

class DerivativeMetadataTemplateRow : public MTRow {
	static DerivativeMetadataTemplateSchema m_tableSchema;

public:
	DerivativeMetadataTemplateRow() : MTRow(m_tableSchema) {};
	//MetadataTemplateRow(const MTRow& row) : MTRow(row) {};
	DerivativeMetadataTemplateRow(const MTRow& row) : MTRow(m_tableSchema) {

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
class DerivativeMetadataTemplatePartition : public MTTable {
public:
	DerivativeMetadataTemplatePartition() : MTTable(new DerivativeMetadataTemplateSchema) {};
	virtual ~DerivativeMetadataTemplatePartition() {};

};


class DerivativeMetadataTemplateResultsPresentation {
	MTRow m_row;
	std::shared_ptr<MTTableSchema> m_tableSchema;
public:
	DerivativeMetadataTemplateResultsPresentation(MTRow row) : m_row(row), m_tableSchema(new DerivativeMetadataTemplateSchema) {};
	~DerivativeMetadataTemplateResultsPresentation() = default;

	bool writeHuman();
	bool writeXML();
	bool writeCSV();
	bool writeJson();
	bool writeHtml();
};


class DerivativeMetadataTemplate {

	static Config_ptr m_templateFile;
	static std::string& getValue(const char* key);
	static DerivativeMetadataTemplateRow m_metadataTemplateRow;

	DerivativeMetadataTemplate() noexcept = default;

public:
	static DerivativeMetadataTemplate& GetInstance();
	virtual ~DerivativeMetadataTemplate();

	DerivativeMetadataTemplate(const DerivativeMetadataTemplate& src) = delete;
	DerivativeMetadataTemplate& operator=(const DerivativeMetadataTemplate& rhs) = delete;

	static bool read(const char* datafile);
	static bool write(const char* datafile);
	//static DerivativeMetadataObject_ptr getMetadataObject();
};

} /* namespace simplearchive */



