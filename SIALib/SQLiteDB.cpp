#include "SQLiteDB.h"
#include "AppPaths.h"
#include "Database.h"
#include "CSVDBFile.h"
#include "MetadataObject.h"
#include "CSVDerivativeDatabase.h"
#include "History.h"
#include "HistoryEvent.h"
#include "SAUtils.h"
#include "CLogger.h"


namespace simplearchive {


	
	IndexRow::IndexRow(MTTableSchema &tableSchema) : MTRow(tableSchema) {}
	IndexRow::IndexRow(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date, int dbidx, MTTableSchema &tableSchema) : MTRow(tableSchema) {
		if (insert(idx, imagePath, name, size, crc, md5, uuid, version, date, dbidx) == false) {
			throw SQLInsertException("Error in index table insert");
		}
	}

	bool IndexRow::insert(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date, int dbidx) {

		ExifDateTime edt(date.toString().c_str());
		columnAt(static_cast<int>(PrimaryIndexIndex::PI_SEQUENCEID_IDX)).set(idx);
		columnAt(static_cast<int>(PrimaryIndexIndex::PI_SHORTIMAGEPATH_IDX)).set(imagePath);
		columnAt(static_cast<int>(PrimaryIndexIndex::PI_FILENAME_IDX)).set(name);
		columnAt(static_cast<int>(PrimaryIndexIndex::PI_FILESIZE_IDX)).set(size);
		columnAt(static_cast<int>(PrimaryIndexIndex::PI_CRC_IDX)).set(crc);
		columnAt(static_cast<int>(PrimaryIndexIndex::PI_MD5_IDX)).set(md5);
		columnAt(static_cast<int>(PrimaryIndexIndex::PI_UUID_IDX)).set(uuid);
		columnAt(static_cast<int>(PrimaryIndexIndex::PI_VERSION_IDX)).set(version);
		columnAt(static_cast<int>(PrimaryIndexIndex::PI_DATEADDED_IDX)).set(edt);
		columnAt(static_cast<int>(PrimaryIndexIndex::PI_DATABASEID_IDX)).set(dbidx);
		return true;
	}

	const char *SQLInsertException::what() const throw() {
		//std::stringstream s;
		//s << message << " line:" << __LINE__ << " file:" << __FILE__;
		//std::string tmp = s.str();
		//output = tmp.c_str();
		return message.c_str();
	}

	
	bool IndexTable::insert(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date, int dbidx)
	{
		if (CSVDBFile::insert(idx, imagePath, name, size, crc, md5, uuid, version, date, dbidx) == false) {
			return false;
		}


		if (m_row.insert(idx, imagePath, name, size, crc, md5, uuid, version, date, dbidx) == false) {
			return false;
		}
		

		Database& db = Database::getInstance();
		if (db.isOpen()) {
			::TableRow row;
			SQLiteDB::insertRow(row, m_row);
			if (db.insert(m_row.getSchema().getName().c_str(), row) == false) {
				return false;
			}
		}
		return true;
		
	}

	PrimaryIndexSchema PrimaryIndexRow::m_tableSchema;
	MasterIndexSchema MasterIndexRow::m_tableSchema;
	DerivativeIndexSchema DerivativeIndexRow::m_tableSchema;

	PrimaryIndexRow PrimaryIndexTable::m_indexRow;
	MasterIndexRow MasterIndexTable::m_indexRow;
	DerivativeIndexRow DerivativeIndexTable::m_indexRow;

	PrimaryIndexRow::PrimaryIndexRow() : IndexRow(m_tableSchema) {}
	PrimaryIndexRow::PrimaryIndexRow(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date, int dbidx) : IndexRow(idx, imagePath, name, size, crc,
			md5, uuid, version, date, dbidx, m_tableSchema) {};
	PrimaryIndexRow::PrimaryIndexRow(const MTRow &row) : IndexRow(row, m_tableSchema) {};

	MasterIndexRow::MasterIndexRow() : IndexRow(m_tableSchema) {}
	MasterIndexRow::MasterIndexRow(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date, int dbidx) : IndexRow(idx, imagePath, name, size, crc,
			md5, uuid, version, date, dbidx, m_tableSchema) {};
	MasterIndexRow::MasterIndexRow(const MTRow &row) : IndexRow(row, m_tableSchema) {};

	DerivativeIndexRow::DerivativeIndexRow() : IndexRow(m_tableSchema) {}
	DerivativeIndexRow::DerivativeIndexRow(int idx, const char *imagePath, const char *name, unsigned long size, unsigned long crc,
		const char *md5, const char *uuid, int version, ExifDate &date, int dbidx) : IndexRow(idx, imagePath, name, size, crc,
			md5, uuid, version, date, dbidx, m_tableSchema) {};
	DerivativeIndexRow::DerivativeIndexRow(const MTRow &row) : IndexRow(row, m_tableSchema) {};



	bool SQLiteDB::m_enabled = true;

	SQLiteDB::SQLiteDB()
	{
	}


	SQLiteDB::~SQLiteDB()
	{
	}

	bool SQLiteDB::makeSQLfile(const char *file) {
		Database& db = Database::getInstance();
		return db.makeSQLfile(file);
	}

	bool SQLiteDB::init(const char *file, bool createIfNotFound) {

		if (!m_enabled) return true;

		CLogger &logger = CLogger::getLogger();
		logger.log(LOG_OK, CLogger::Level::FINE, "Checking SQL Database path: \"%s\"", file);
		if (SAUtils::DirExists(file) == false) {
			if (!createIfNotFound) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "SQL Database path not found: \"%s\"", file);
				return false;
			}
			logger.log(LOG_OK, CLogger::Level::FINE, "SQL Database path not found, making folder: \"%s\"", file);
			if (SAUtils::mkDir(file) == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Failed making SQL Database folder: \"%s\"", file);
				return false;
			}
		}
		std::string dbpath = file;
		dbpath += '/';
		dbpath += SQLITE3_DB;
		logger.log(LOG_OK, CLogger::Level::FINE, "Checking SQL Database file: \"%s\"", dbpath.c_str());
		if (SAUtils::IsFile(dbpath.c_str()) == false) {
			if (!createIfNotFound) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "SQL Database file not found: \"%s\"", dbpath.c_str());
				return false;
			}
			logger.log(LOG_OK, CLogger::Level::FINE, "SQL Database path not found, making file: \"%s\"", dbpath.c_str());
			if (makeSQLfile(dbpath.c_str()) == false) {
				return false;
			}
			logger.log(LOG_OK, CLogger::Level::FINE, "Opening SQL Database File: \"%s\"", dbpath.c_str());
			if (SQLiteDB::open(file, "", "") == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Failed opening SQL Database file: \"%s\"", dbpath.c_str());
				return false;
			}
			logger.log(LOG_OK, CLogger::Level::FATAL, "Creating database tables in database: \"%s\"", dbpath.c_str());
			if (CreateDB() == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Failed creating database tables: \"%s\"", dbpath.c_str());
				return false;
			}
			
		}
		else {
			logger.log(LOG_OK, CLogger::Level::FINE, "Opening SQL Database File: \"%s\"", dbpath.c_str());
			if (SQLiteDB::open(file, "", "") == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Failed opening SQL Database File: \"%s\"", dbpath.c_str());
				return false;
			}
			// testing only
			/*
			logger.log(LOG_OK, CLogger::Level::FATAL, "Creating database tables in database: \"%s\"", dbpath.c_str());
			if (CreateDB() == false) {
				logger.log(LOG_OK, CLogger::Level::FATAL, "Failed creating database tables: \"%s\" error: %s", dbpath.c_str(), error());
				return false;
			}
			*/
		}
		logger.log(LOG_OK, CLogger::Level::FINE, "SQL Database \"%s\" ready", dbpath.c_str());
		return true;
	}

	bool SQLiteDB::CreateDB()
	{
		Database& db = Database::getInstance();
		MetadataSchema metadataSchema;
		::TableRow metadataRow;
		SQLiteDB::createRow(metadataRow, metadataSchema);
		if (db.create(TABLE_METADATA_PROPERTIES, metadataRow) == false) {
			return false;
		}

		ImageHistorySchema imageHistorySchema;
		::TableRow imageHistoryRow;
		SQLiteDB::createRow(imageHistoryRow, imageHistorySchema);
		if (db.create(TABLE_IMAGE_HISTORY, imageHistoryRow) == false) {
			return false;
		}

		SystemHistorySchema systemHistorySchema;
		::TableRow systemHistoryRow;
		SQLiteDB::createRow(systemHistoryRow, systemHistorySchema);
		if (db.create(TABLE_SYSTEM_HISTORY, systemHistoryRow) == false) {
			return false;
		}

		PrimaryIndexSchema primaryIndexSchema;
		::TableRow primaryIndexRow;
		SQLiteDB::createRow(primaryIndexRow, primaryIndexSchema);
		if (db.create(TABLE_PRIMARY_INDEX, primaryIndexRow) == false) {
			return false;
		}

		MasterIndexSchema masterIndexSchema;
		::TableRow masterIndexRow;
		SQLiteDB::createRow(masterIndexRow, masterIndexSchema);
		if (db.create(TABLE_MASTER_INDEX, masterIndexRow) == false) {
			return false;
		}

		DerivativeIndexSchema derivativeIndexSchema;
		::TableRow derivativeIndexRow;
		SQLiteDB::createRow(derivativeIndexRow, derivativeIndexSchema);
		if (db.create(TABLE_DERIVATIVE_INDEX, derivativeIndexRow) == false) {
			return false;
		}

		DerivativeMetadataSchema derivativeMetadataSchema;
		::TableRow derivativeMetadata;
		SQLiteDB::createRow(derivativeMetadata, derivativeMetadataSchema);
		if (db.create(TABLE_DERIVATIVE_METADATA, systemHistoryRow) == false) {
			return false;
		}
		return true;
	}

	const char *SQLiteDB::error()
	{
		Database& db = Database::getInstance();
		return db.getError();
	}
	bool SQLiteDB::createRow(::TableRow &sqlRow, MTTableSchema &mtRow)
	{
		for (auto i = mtRow.begin(); i != mtRow.end(); i++) {
			MTSchema column = *i;
			MTSchema::EItemType type = column.getType();
			std::string name = column.getName();
			std::shared_ptr<TableColumn> tableColumn = nullptr;
			TableField field;
			switch (type) {
			case MTSchema::Integer:
				field.setType(TableField::DataType::Integer);
				break;
			case MTSchema::Text:
				field.setType(TableField::DataType::Text);	
				break;
			case MTSchema::Date:
				tableColumn = std::make_shared<TableColumn>(name.c_str(), TableField::DataType::Text);
				break;
			case MTSchema::Float:
				tableColumn = std::make_shared<TableColumn>(name.c_str(), TableField::DataType::Double);
				break;
			}
			sqlRow.add(name.c_str(), field);
			
		}
		return true;
	
	}

	bool SQLiteDB::insertRow(::TableRow &sqlRow, MTRow &mtRow)
	{
		for (auto i = mtRow.begin(); i != mtRow.end(); i++) {
			std::shared_ptr<MTColumn> column = *i;
			std::string name = column->getMTSchemaItem()->getName();
			
			TableField field;
			field = column->toString();
			
			sqlRow.add(name.c_str(), field);

		}
		return true;

	}

	bool SQLiteDB::open(const char *path, const char *username, const char *password) {
		
		if (!m_enabled) return true;

		Database &db = Database::getInstance();
		std::string dbpath = path;
		dbpath += '/';
		dbpath += SQLITE3_DB;
		if (db.open(dbpath.c_str(), username, password) == false) {
			printf("database open returned %s", db.getError());
			return false;
		}
		return true;
	}

	bool SQLiteDB::add(MetadataObject &metadataObject) {

		if (!m_enabled) return true;

		Database& db = Database::getInstance();
		
		::TableRow row;
		SQLiteDB::insertRow(row, metadataObject);
		if (db.insert(TABLE_METADATA_PROPERTIES, row) == false) {
			return false;
		}
		return true;

	}

	bool SQLiteDB::newImage2History(const char *filepath, const char *comment) {
		HistoryEvent he(HistoryEvent::Event::ADDED);
		ImageHistoryRow imageHistoryRow(filepath, "0000", comment, he);
		SystemHistoryRow systemHistoryRow(filepath, "0000", comment, he);
		
		if (add(imageHistoryRow) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}

		if (add(systemHistoryRow) == false) {
			ErrorCode::setErrorCode(IMGA_ERROR::INVALID_PATH);
			return false;
		}
		

		return true;
	}

	bool SQLiteDB::add(ImageHistoryRow &imageHistoryRow) {

		if (!m_enabled) return true;

		Database& db = Database::getInstance();

		::TableRow row;
		SQLiteDB::insertRow(row, imageHistoryRow);
		if (db.insert(TABLE_IMAGE_HISTORY, row) == false) {
			return false;
		}
		return true;

	}

	bool SQLiteDB::add(SystemHistoryRow &systemHistoryRow) {

		if (!m_enabled) return true;

		Database& db = Database::getInstance();

		::TableRow row;
		SQLiteDB::insertRow(row, systemHistoryRow);
		if (db.insert(TABLE_SYSTEM_HISTORY, row) == false) {
			return false;
		}
		return true;
	}

	bool SQLiteDB::add(PrimaryIndexRow &primaryIndexRow) {

		if (!m_enabled) return true;

		Database& db = Database::getInstance();

		::TableRow row;
		SQLiteDB::insertRow(row, primaryIndexRow);
		if (db.insert(TABLE_PRIMARY_INDEX, row) == false) {
			return false;
		}
		return true;
	}

	
	bool SQLiteDB::add(MasterIndexRow &masterIndexRow) {

		if (!m_enabled) return true;

		Database& db = Database::getInstance();

		::TableRow row;
		SQLiteDB::insertRow(row, masterIndexRow);
		if (db.insert(TABLE_MASTER_INDEX, row) == false) {
			return false;
		}
		return true;
	}

	bool SQLiteDB::add(DerivativeIndexRow &derivativeIndexRow) {

		if (!m_enabled) return true;

		Database& db = Database::getInstance();

		::TableRow row;
		SQLiteDB::insertRow(row, derivativeIndexRow);
		if (db.insert(TABLE_DERIVATIVE_INDEX, row) == false) {
			return false;
		}
		return true;
	}

	bool SQLiteDB::add(DerivativeMetadata &derivativeMetadata) {

		if (!m_enabled) return true;

		Database& db = Database::getInstance();

		::TableRow row;
		SQLiteDB::insertRow(row, derivativeMetadata);
		if (db.insert(TABLE_DERIVATIVE_METADATA, row) == false) {
			return false;
		}
		return true;
	}
}; // namespace simplearchive
