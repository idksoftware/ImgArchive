#include "DBDefines.h"
#include "MetaType.h"
#include "CSVDBFile.h"

#pragma once

class TableRow;

namespace simplearchive {

	class SQLInsertException : public std::exception {
		std::string message;

	public:
		explicit SQLInsertException(const char *msg) : message(msg) {

		}
		explicit SQLInsertException(std::string msg) : message(msg) {}
		virtual ~SQLInsertException() throw() {}
		virtual const char *what() const throw();

	};

	class TableIndexSchema : public MTTableSchema {
	public:
		TableIndexSchema(const char *tablename) : MTTableSchema(tablename) {
			add(MTSchema(MTSchema::Integer, DB_SEQUENCEID));// template
			add(MTSchema(MTSchema::Text, DB_SHORTIMAGEPATH));
			add(MTSchema(MTSchema::Text, DB_FILENAME));
			add(MTSchema(MTSchema::Integer, DB_FILESIZE));
			add(MTSchema(MTSchema::Integer, DB_CRC));
			add(MTSchema(MTSchema::Text, DB_MD5));
			add(MTSchema(MTSchema::Text, DB_UUID));
			add(MTSchema(MTSchema::Integer, DB_VERSION));// template
			add(MTSchema(MTSchema::Date, DB_DATEADDED));
			add(MTSchema(MTSchema::Integer, DB_DATABASEID));

		}
	};

	class PrimaryIndexSchema : public TableIndexSchema {
	public:
		PrimaryIndexSchema() : TableIndexSchema(TABLE_PRIMARY_INDEX) {};
	};

	class DerivativeIndexSchema : public TableIndexSchema {
	public:
		DerivativeIndexSchema() : TableIndexSchema(TABLE_DERIVATIVE_INDEX) {};
	};

	class MasterIndexSchema : public TableIndexSchema {
	public:
		MasterIndexSchema() : TableIndexSchema(TABLE_MASTER_INDEX) {};
	};

	class IndexRow : public MTRow {
		
	public:
		IndexRow(MTTableSchema &tableSchema);
		IndexRow(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
			const char *md5, const char *uuid, int version, ExifDate &date, int m_dbidx, MTTableSchema &tableSchema);
		IndexRow(const MTRow &row, MTTableSchema &tableSchema) : MTRow(tableSchema) {

			for (unsigned int i = 0; i < row.size(); i++) {
				MTColumn& c1 = *at(i);
				MTColumn& c2 = *row.at(i);
				c1 = c2;
			}
		}

		bool insert(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
			const char *md5, const char *uuid, int version, ExifDate &date, int dbidx = -1);
	};


	class PrimaryIndexRow : public IndexRow {
		static PrimaryIndexSchema m_tableSchema;
	public:
		PrimaryIndexRow();
		PrimaryIndexRow(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
			const char *md5, const char *uuid, int version, ExifDate &date, int m_dbidx);
		PrimaryIndexRow(const MTRow &row);

	};


	class DerivativeIndexRow : public IndexRow {
		static DerivativeIndexSchema m_tableSchema;
	public:
		DerivativeIndexRow();
		DerivativeIndexRow(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
			const char *md5, const char *uuid, int version, ExifDate &date, int m_dbidx);
		DerivativeIndexRow(const MTRow &row);
	};

	class MasterIndexRow : public IndexRow {
		static MasterIndexSchema m_tableSchema;
	public:
		MasterIndexRow();
		MasterIndexRow(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
			const char *md5, const char *uuid, int version, ExifDate &date, int m_dbidx);
		MasterIndexRow(const MTRow &row);


	};

	class IndexTable : public CSVDBFile {
		IndexRow &m_row;
	public:
		IndexTable(IndexRow &row) : m_row(row) {};
		bool insert(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
			const char *md5, const char *uuid, int version, ExifDate &date, int dbidx = -1);
	};

	class PrimaryIndexTable : public IndexTable {
		static PrimaryIndexRow m_indexRow;
	public:
		PrimaryIndexTable() : IndexTable(m_indexRow) {};
		
	};

	class MasterIndexTable : public IndexTable {
		static MasterIndexRow m_indexRow;
	public:
		MasterIndexTable() : IndexTable(m_indexRow) {};
	};

	class DerivativeIndexTable : public IndexTable {
		static DerivativeIndexRow m_indexRow;
	public:
		DerivativeIndexTable() : IndexTable(m_indexRow) {};
	};
	
	class MTTableSchema;
	class MetadataObject;
	class DerivativeMetadata;
	class ImageHistoryRow;
	class SystemHistoryRow;
	class PrimaryIndexRow;
	class MasterIndexRow;
	class DerivativeIndexRow;

	class MTRow;

	class SQLiteDB
	{
		
		static bool m_enabled;
		static bool makeSQLfile(const char *file);
	public:
		SQLiteDB();
		~SQLiteDB();
		static bool createRow(::TableRow &sqlRow, MTTableSchema &mtRow);
		static bool insertRow(::TableRow &sqlRow, MTRow &mtRow);
		static void enableSQLiteDB(bool b) {
			m_enabled = b;
		}
		static bool init(const char *file, bool createIfNotFound);
		static bool open(const char *dbpath, const char *username, const char *password);
		/// This will create a fresh DB
		static bool CreateDB();
		static const char *error();
		
		static bool add(PrimaryIndexRow &primaryIndexRow);
		static bool add(MasterIndexRow &masterIndexRow);
		static bool add(DerivativeIndexRow &derivativeIndexRow);
		static bool add(MetadataObject &metadataObject);
		static bool add(ImageHistoryRow &imageHistoryRow);
		static bool add(SystemHistoryRow &imageHistoryRow);
		static bool add(DerivativeMetadata &derivativeMetadata);
		static bool newImage2History(const char *filepath, const char *comment);
	};

}; // namespace simplearchive 