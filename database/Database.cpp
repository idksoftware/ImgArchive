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

#include "Database.h"
#include "sqlite3.h"
#include "SQLBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
//#define new DEBUG_NEW
#endif

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
	//if (SQLITE_OK != sqlite3_initialize()) {
	//	m_retCode = 25;
	//}

}

Database::Database(Database const &) {
	dbfile = 0;
	isOpenDB = false;
	m_retCode = SQLITE_OK;
	m_retErrorStr = 0;
	//if (SQLITE_OK != sqlite3_initialize()) {
	//	m_retCode = 25;
	//}
}

Database::~Database() {
	close();
}

bool Database::makeSQLfile(const char *file) {
	sqlite3* pDb = NULL;
	int rc = sqlite3_open_v2(file, &pDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (SQLITE_OK != rc) {
		m_retCode = 25;
		return false;
	}
	return true;
}

bool Database::open(const char *dbpath, const char *username, const char *password)
{
	m_DBPath = dbpath;
	m_UserName = username;
	m_Password = password;

	int res = sqlite3_open(dbpath, &dbfile);
	if (res != SQLITE_OK) {
		return false;
	}
	isOpenDB = true;
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
	for (auto ii = row.begin(); ii != row.end(); ++ii) {
		TableColumn &item = *(ii->get());
		createBulder.addfield(item.getName(), item.getDataType(), item.getAttributes());
	}
	//cout << createBulder.toString() << endl;
	if (!execute(createBulder.toString().c_str())) {
		return false;
	}
	return true;
}

bool Database::insert(const char *tablename, TableRow &row) {
	SQLInsertBulder insertBulder(tablename);
	for (auto ii = row.begin(); ii != row.end(); ++ii) {
		TableColumn &item = *(ii->get());
		if (item.isUpdated()) {
			insertBulder.addfield(item.getName(), item.getTableField().toString());
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
	//return Database::getError(m_retCode);
	return sqlite3_errmsg((sqlite3 *)dbfile);
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
