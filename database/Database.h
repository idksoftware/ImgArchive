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

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <stdexcept>

#ifndef DATABASE_H_
#define DATABASE_H_
//namespace simplearchive {
struct sqlite3;

/**
 * @brief This class is the base for a table column.
 */
class TableField {
public:
	typedef enum {
		Integer,
		Double,
		Text,
		Blob,
		Null
	} DataType;
private:
	std::string stringValue;
	DataType m_type;
	bool m_isUpdated;
public:
	bool isUpdated() const {
		return m_isUpdated;
	}

	void setUpdated(bool isUpdated) {
		m_isUpdated = isUpdated;
	}

	void setUpdated() {
		m_isUpdated = true;
	}

	void setType(DataType type) {
		m_type = type;
	}

	DataType getType() {
		return m_type;
	}

	const char *toCharString() const {
		return stringValue.c_str();
	}

	void setValue(unsigned int value) {
		std::stringstream tmp;
		tmp << value;
		stringValue = tmp.str();
		setUpdated();
		m_type = Integer;
	}
	void setValue(unsigned long value) {
		std::stringstream tmp;
		tmp << value;
		stringValue = tmp.str();
		setUpdated();
		m_type = Integer;
	}
	void setValue(double value) {
		std::stringstream tmp;
		tmp << value;
		stringValue = tmp.str();
		setUpdated();
		m_type = Double;
	}

	void setValue(const char *str) {
		stringValue = str;
		setUpdated();
		m_type = Text;
	}

	void setValue(const std::string& value) {
		stringValue = value;
		setUpdated();
		m_type = Text;
	}
	TableField &operator = (unsigned int value) {
		setValue(value);
		return *this;
	}

	TableField &operator = (unsigned long value) {
		setValue(value);
		return *this;
	}

	TableField &operator = (double value) {
		setValue(value);
		return *this;
	}

	TableField &operator = (const char *value) {
		setValue(value);
		return *this;
	}

	TableField &operator = (const TableField& value) {
		stringValue = value.stringValue;
		m_type = value.m_type;
		return *this;
	}

	TableField &operator = (const std::string& value) {
			setValue(value);
			return *this;
		}

	unsigned int getIntValue() {
		if (m_type != Integer) {
			throw std::bad_exception();
		}
		return strtol(stringValue.c_str(), NULL, 10);
	}
	unsigned long getLongValue() {
		if (m_type != Integer) {
			throw std::bad_exception();
		}
		return strtol(stringValue.c_str(), NULL, 10);
	}
	double getDoubleValue() {
		if (m_type != Double) {
			throw std::bad_exception();
		}

		std::stringstream tmp(stringValue);
		double value;
		if(!(tmp >> value)) {
			throw std::bad_exception();
		}
		return value;
	}

	const std::string& toString() const {
		return stringValue;
	}

	const char *getDataTypeString();

	TableField(TableField::DataType dataType) {
			m_type = dataType;
			m_isUpdated = false;
		};

	TableField(const TableField &t) {
		m_type = t.m_type;
		m_isUpdated = t.m_isUpdated;
		stringValue = t.stringValue;
	};

	TableField() {
		m_isUpdated = false;
	};

	virtual ~TableField() {};
};
class TableRow;
class TableColumn {
	friend class TableRow;
	std::string m_key;
	TableField *m_tableField;
	std::vector<std::string> m_attributes;
public:
	/*
	TableColumn(const char *key, TableField::DataType dataType) {
		m_key = key;
		m_tableField.setType(dataType);
	};
	*/
	TableColumn(const char *key, TableField &field) {
		m_key = key;
		m_tableField = &field;
	};


	/*
	TableColumn(const char *key, const char *defaultValue) {
		m_key = key;
		m_tableField = &field;
	};
	*/
	TableColumn(const TableColumn &t) {
		m_key = t.m_key;
		m_tableField = new TableField(*(t.m_tableField));
		//m_tableField = t.m_tableField;
	};

	void addAttributes(const char *attr) {
		std::string item(attr);
		m_attributes.push_back(item);
	}
	virtual ~TableColumn() {};

	const char *getName() {
		return m_key.c_str();
	}

	const char *getDataType() {
		return m_tableField->getDataTypeString();
	}

	bool isUpdated() {
		return m_tableField->isUpdated();
	}

	std::vector<std::string> &getAttributes() {
		return m_attributes;
	}

	const TableField& getTableField() const {
		return *m_tableField;
	}

	//void setTableField(const TableField& tableField) {
	//	m_tableField = &tableField;
	//}


};
/**
 * @brief This class is the base for a table row.
 */
class TableItem {
protected:
	TableItem() {};

	virtual ~TableItem() {};
};

/**
 * This class is a C++ version of the table row.
 *
 * It uses two containers vector for the column ordering,
 * and map as a lookup for values.
 */
class TableRow : public std::vector<TableColumn *> {
	std::map<std::string, TableField *> m_lookup;
protected:
	TableRow() {};

	void add(TableColumn *tableColumn) {
		this->push_back(tableColumn);
		m_lookup.insert(std::make_pair(tableColumn->m_key, tableColumn->m_tableField));
	}
	/*
	TableColumn &add(const char *key, TableField::DataType dataType) {
		TableColumn *tableColumn = new TableColumn(key, dataType);
		add(tableColumn);
		return *tableColumn;
	}
	*/
	TableColumn &add(const char *key, TableField &field) {
		TableColumn *tableColumn = new TableColumn(key, field);
		add(tableColumn);
		return *tableColumn;
	}

	TableField &getTableField(const char *key) {
		std::map<std::string, TableField *>::iterator iter = m_lookup.find(key);
		if (iter == m_lookup.end()) {
			throw std::exception();
		}
		return *(iter->second);
	}
public:

	virtual ~TableRow() {};

};

/**
 * @brief This is the sqlite database wrapper class.
 *
 * This class provide the interface to sqlite. All database actions are
 * provide by this class.
 */
class Database {
private:
	/// This is used so only one instance of this
	/// object is available
	static Database *m_pThis;
	/// Path to the database file.
	std::string m_DBPath;
	/// Username for the database
	std::string m_UserName;
	/// Password for the database
	std::string m_Password;
	/// This is the point to the underlying database
	/// object
	sqlite3 *dbfile;
	/// Flag is the database is currently open
	bool isOpenDB;
	/// Database return codes. 0 is successful
	int m_retCode;
	/// Extended error string
	char *m_retErrorStr;
	/// private constructor
	Database();
	/// private copy constructor
	Database(Database const &);
	/// private equality operator
	void operator = (Database const&) {};
public:
	/// Used to get an instance of the database
	static Database& getInstance() {
		static Database instance;
		return instance;
	}
	/// distructor of class
	virtual ~Database();
	/// opens the database. use zero length strings if username/password not required
	bool open(const char *dbpath, const char *username, const char *password);
	/// Close the database
	bool close();
	/// Execute a database command
	bool execute(const char *sqlstatement);
	/// Get error string from error code.
	static const char *getError(int err);
	/// get the current error string.
	const char *getError();
	/// get the detailed error string which is addition to the main
	/// error string
	const char *getDetailedError() const {
		return m_retErrorStr;
	}
	/// get a pointer to the under lying sqlite database.
	const sqlite3* getDbfile() const {
		return dbfile;
	}
	/// creates a table from row definition.
	bool create(const char *tablename, TableRow &row);

	/// creates a table from row definition.
	bool insert(const char *tablename, TableRow &row);

};
//} /* namespace simplearchive */
#endif /* DATABASE_H_ */
