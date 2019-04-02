#ifndef DERIVATIVEMETADATAOBJECT_H_
#define DERIVATIVEMETADATAOBJECT_H_
#include <string>
#include <vector>
#include <map>
#include "DBDefines.h"
#include "MetaType.h"
#include "FileInfo.h"

#define MetadataTableName		"Metadata"
#define MetadataTableFilename	"Metadata.csv"

/**
* @brief This is the main interface into the CSV Database. This Database is used
* to create a Comma Separated Value based database.
* The created files in the database are stored in a directory tree. This directory tree
* is defined by a year folder, day folder hierarchy much like the main archive folder hierarchy
* The aim is to limit the resulting files to a manageable size.
* In addition so the metadata records are also a manageable length, the matedata is spit up
* into sections. each section it  associated by a unique sequence number. Which will become the primary
* key to each of these table files This sequence number is generated by other the SQL database or this
* database if the SQL one is not enabled.
*/

namespace simplearchive {

	class DerivativeMetadataSchema : public MTTableSchema {
	public:
		DerivativeMetadataSchema() : MTTableSchema("DerivativeMetadata") {
			add(MTSchema(MTSchema::Integer, DB_SEQUENCEID));
			add(MTSchema(MTSchema::Integer, DB_VERSION));
			add(MTSchema(MTSchema::Integer, DB_DATABASEID));
			add(MTSchema(MTSchema::Text, DB_FILENAME));
			add(MTSchema(MTSchema::Text, DB_ORGINALNAME));
			add(MTSchema(MTSchema::Text, DB_FILEPATH));
			add(MTSchema(MTSchema::Integer, DB_CRC));
			add(MTSchema(MTSchema::Text, DB_MD5));
			add(MTSchema(MTSchema::Text, DB_UUID));


			add(MTSchema(MTSchema::Integer, DB_FILESIZE));
			add(MTSchema(MTSchema::Date, DB_DATEMODIFIED));
			add(MTSchema(MTSchema::Date, DB_DATECREATE));
			add(MTSchema(MTSchema::Date, DB_DATEADDED));

			add(MTSchema(MTSchema::Text, DB_COMMENT));
			add(MTSchema(MTSchema::Text, DB_AUTHOR));
			add(MTSchema(MTSchema::Text, DB_DESCRIPTION));
			add(MTSchema(MTSchema::Text, DB_EDITOR));
			add(MTSchema(MTSchema::Text, DB_VERSIONPATH));
		}
	};


	class DerivativeMetadata : public MTRow {
		static DerivativeMetadataSchema m_schema;
	public:
		DerivativeMetadata();
		~DerivativeMetadata();

		const char *getSequenceIDString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_SEQUENCEID_IDX)).toString().c_str();
		}
		
		const char *getVersionString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_VERSION_IDX)).toString().c_str();
		}
		const char *getDatabaseIDString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_DATABASEID_IDX)).toString().c_str();
		}
		const char *getFilenameString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_FILENAME_IDX)).toString().c_str();
		}
		const char *getOrginalNameString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_ORGINALNAME_IDX)).toString().c_str();
		}
		const char *getFilePathString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_FILEPATH_IDX)).toString().c_str();
		}
		const char *getCRCString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_CRC_IDX)).toString().c_str();
		}
		const char *getMD5String() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_MD5_IDX)).toString().c_str();
		}
		const char *getUUIDString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_UUID_IDX)).toString().c_str();
		}
		const char *getFileSizeString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_FILESIZE_IDX)).toString().c_str();
		}
		const char *getDateModifiedString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_DATEMODIFIED_IDX)).toString().c_str();
		}
		const char *getDateCreateString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_DATECREATE_IDX)).toString().c_str();
		}
		const char *getDateAddedString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_DATEADDED_IDX)).toString().c_str(); 
		}
		const char *getCommentString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_COMMENT_IDX)).toString().c_str();
		}
		const char *getAuthorString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_AUTHOR_IDX)).toString().c_str();
		}
		const char *getDescriptionString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_DESCRIPTION_IDX)).toString().c_str();
		}
		const char *getEditorString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_EDITOR_IDX)).toString().c_str();
		}
		const char *getVersionPathString() const {
			return columnAt(static_cast<int>(DerivativeMetadataIndex::DB_VERSIONPATH_IDX)).toString().c_str();
		}
		
	};

	class DerivativePartition : public MTTable {
	public:
		DerivativePartition() : MTTable(new DerivativeMetadataSchema) {};
		virtual ~DerivativePartition() {};
	};

}; // simplearchive namespace
#endif DERIVATIVEMETADATAOBJECT_H_