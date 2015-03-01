/*
 * Database.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: wzw7yn
 */


#include "Database.h"
#include "sqlite3.h"
#include "SQLBuilder.h"

using namespace std;

//namespace simplearchive {

static const char *errCodes[] = {
        "Successful result",
/* beginning-of-error-codes */
        "SQL error or missing database",
        "Internal logic error in SQLite",
        "Access permission denied",
        "Callback routine requested an abort",
        "The database file is locked",
        "A table in the database is locked",
        "A malloc() failed",
        "Attempt to write a readonly database",
        "Operation terminated by sqlite3_interrupt()",
        "Some kind of disk I/O error occurred",
        "The database disk image is malformed",
        "NOT USED. Table or record not found",
        "Insertion failed because database is full",
        "Unable to open the database file",
        "NOT USED. Database lock protocol error",
        "Database is empty",
        "The database schema changed",
        "String or BLOB exceeds size limit",
        "Abort due to constraint violation",
        "Data type mismatch",
        "Library used incorrectly",
        "Uses OS features not supported on host",
        "Authorization denied",
        "Auxiliary database format error",
        "2nd parameter to sqlite3_bind out of range",
        "File opened that is not a database file",
        "sqlite3_step() has another row ready",
        "sqlite3_step() has finished executing"
};

static int callback(void *notUsed, int argc, char **argv, char **colName) {
	return 0;
}

const char *TableField::getDataTypeString() {
	switch(m_type) {
	case Integer:
		return "integer";
	case Double:
		return "float";
	case Text:
		return "text";
	case Blob:
		return "blob";
	case Null:
		return "null";
	}
	return "text";
}



Database::Database()
{
	dbfile = 0;
	isOpenDB = false;
	m_retCode = SQLITE_OK;
	m_retErrorStr = 0;
}

Database::Database(Database const &) {
	dbfile = 0;
	isOpenDB = false;
	m_retCode = SQLITE_OK;
	m_retErrorStr = 0;
}

Database::~Database() {
	close();
}

bool Database::open(const char *dbpath, const char *username, const char *password)
{
	m_DBPath = dbpath;
	m_UserName = username;
	m_Password = password;

	int res = sqlite3_open(dbpath, &dbfile);
	if (res == SQLITE_OK) {
		isOpenDB = true;
		return true;
	}

	return true;
}

bool Database::close() {
	if (isOpenDB == true) {
		m_retCode = sqlite3_close(((sqlite3 *)dbfile));
		if (m_retCode == SQLITE_OK) {
			return true;
		}
		return false;
	}
	return true;
}

bool Database::execute(const char *sqlstatement)
{
	m_retCode = sqlite3_exec((sqlite3 *)dbfile, sqlstatement, callback, 0, &m_retErrorStr);
	if (m_retCode != SQLITE_OK) {
		return false;
	}
	return true;
}

bool Database::create(const char *tablename, TableRow &row) {
	SQLCreatBulder createBulder(tablename);
	for (std::vector<TableColumn *>::iterator ii = row.begin(); ii != row.end(); ++ii) {
		TableColumn *item = *ii;
		createBulder.addfield(item->getName(), item->getDataType(), item->getAttributes());
	}

	if (!execute(createBulder.toString().c_str())) {
		return false;
	}
	return true;
}

bool Database::insert(const char *tablename, TableRow &row) {
	SQLInsertBulder insertBulder(tablename);
	for (std::vector<TableColumn *>::iterator ii = row.begin(); ii != row.end(); ++ii) {
		TableColumn *item = *ii;
		if (item->isUpdated()) {
			insertBulder.addfield(item->getName(), item->getTableField().toString());
		}

	}
	if (!insertBulder.isUpdated()) {
		return true;
	}
	//printf("%s", insertBulder.toString().c_str());

	if (!execute(insertBulder.toString().c_str())) {
		return false;
	}

	return true;
}

const char *Database::getError()
{
	return Database::getError(m_retCode);
}

const char *Database::getError(int err)
{
	int n = sizeof(errCodes);
	if (err < 0 || err > n) {
		return "Internal logic error in SQLite";
	}
	return errCodes[err];
}
//} /* namespace simplearchive */
