#pragma once

#include <string>
#include <vector>
#include <map>
#include "DBDefines.h"
#include "MetaType.h"

namespace simplearchive {
/**
	The Index Metadata Schema is the metadata used to describe the version of the image.

	This is used by the primary index to track the version of each image in the archive.
	The sequence id will depend on the version of the image? If the image is the master or original
	then the sequence id will be the master database index. However if the version, is a version other
	then the original i.e it is a derivation of the master, then it will be the index in the derivitive
	database.
*/

	
class VersionMetadataSchema : public MTTableSchema {
public:
	VersionMetadataSchema() noexcept : MTTableSchema("VersionMetadataProperties") {
		add(MTSchema(MTSchema::Integer, DB_SEQUENCEID));	// Sequence id of this version in the primary index 
		add(MTSchema(MTSchema::Integer, DB_VERSION));		// The Version number.
		add(MTSchema(MTSchema::Integer, DB_DATABASEID));	// The database ID in Master databease or the Derivitives database. 
		add(MTSchema(MTSchema::Text, DB_FILENAME));			// The file name in the form NameYYYYMMDDVVV.ext
		add(MTSchema(MTSchema::Text, DB_ORGINALNAME));		// The orginal file name
		add(MTSchema(MTSchema::Text, DB_VERSIONPATH));		// The file path in the database
		add(MTSchema(MTSchema::Integer, DB_CRC));			// CRC of the current version image file
		add(MTSchema(MTSchema::Text, DB_MD5));				// MD5 of the current version image file
		add(MTSchema(MTSchema::Text, DB_UUID));				// UUID 
		
		
		add(MTSchema(MTSchema::Integer, DB_FILESIZE));
		add(MTSchema(MTSchema::Date, DB_DATEMODIFIED));
		add(MTSchema(MTSchema::Date, DB_DATECREATE));
		add(MTSchema(MTSchema::Date, DB_DATEADDED));
		
		add(MTSchema(MTSchema::Text, DB_COMMENT));			// 
		add(MTSchema(MTSchema::Text, DB_AUTHOR));
		add(MTSchema(MTSchema::Text, DB_DESCRIPTION));
		
	}
};
class VersionMetadataObject : public MTRow {
	static VersionMetadataSchema m_tableSchema;
public:

	VersionMetadataObject() noexcept : MTRow(m_tableSchema) {};
	~VersionMetadataObject();
};



class VersionTable : public MTTable {
	
public:
	VersionTable();
	~VersionTable();
};

}; // simplearchive {
