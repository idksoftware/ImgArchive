/*
 * MetaType.h
 *
 *  Created on: May 5, 2015
 *      Author: wzw7yn
 */

#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <stdexcept>
#include <map>
#include "ExifDateTime.h"
#include "CSVArgs.h"
#include "MetaType.h"
#include "ErrorCode.h"

#define DELIM ','

using namespace simplearchive;
	
namespace DBLite {

class DBLiteTypeException : public std::exception {
	std::string message;
	
public:
	explicit DBLiteTypeException(const char *msg) : message(msg){

	}
	explicit DBLiteTypeException(std::string msg) : message(msg) {}
	virtual ~DBLiteTypeException() throw() {}
	virtual const char *what() const throw();

};

class DBLiteSchema {
public:
        typedef enum {
                Integer,
                Text,
                Date,
                Float
        } EIteDBLiteype;
private:
       EIteDBLiteype m_type;
       std::string m_name;
public:

       DBLiteSchema(EIteDBLiteype type, const char *name) {
    	   m_type  = type;
    	   m_name  = name;
       }

       DBLiteSchema(EIteDBLiteype type, std::string &name) {
    	   m_type  = type;
    	   m_name  = name;
       }

	   DBLiteSchema(const DBLiteSchema &c) {
		   m_type = c.m_type;
		   m_name = c.m_name;
	   }
	

	const std::string& getName() const {
		return m_name;
	}

	EIteDBLiteype getType() const {
		return m_type;
	}

	const char *getTypeString();
};

class DBLiteTableSchema : private std::vector<DBLiteSchema> {
	std::map<std::string, int> m_index;
	int m_count;
protected:
	std::string m_tablename;
public:
	DBLiteTableSchema(const char *name) {
		m_tablename = name;
		m_count = 0;
	}
	DBLiteTableSchema(DBLiteTableSchema &ts) {
		m_tablename = ts.m_tablename;
		m_count = ts.m_count;
		int count = 0;
		for (auto i = ts.begin(); i != ts.end(); i++) {
			DBLiteSchema& item = *i;
			push_back(item);
			m_index.insert(std::make_pair(item.getName(), count++));
		}

	}
	virtual ~DBLiteTableSchema() {
		m_index.clear();
		clear();
	}

	void add(DBLiteSchema item) {
		push_back(DBLiteSchema(item));
		m_index.insert(std::make_pair(item.getName(), m_count));
		m_count++;
	}
	int getIndex(const char *name) const {
		
		if (m_index.find(name) == m_index.end()) {
			return -1;
		}
		else {
			return m_index.at(name);
		}
		
	}
	void print() {
		//m_name = name;
		printf("%s\n", getName().c_str());
		for (std::vector<DBLiteSchema>::iterator i = begin(); i != end(); i++) {
			DBLiteSchema& columnInfo = *i;
			printf("%s      %s\n", columnInfo.getName().c_str(), columnInfo.getTypeString());
		}
	}

	const std::string& getName() const {
		return m_tablename;
	}

	const std::string& getColumnName(int i) const {
		return at(i).getName();
	}

	const unsigned int size() const {
		return std::vector<DBLiteSchema>::size();
	}

	std::vector<DBLiteSchema>::iterator begin() {
		return std::vector<DBLiteSchema>::begin();
	}

	std::vector<DBLiteSchema>::iterator end() {
		return std::vector<DBLiteSchema>::end();
	}
};



class DBLiteObject;
class DBLiteColumn {
	static const char *nullStr;
	std::shared_ptr<DBLiteSchema> m_info;
	DBLiteObject *m_object;
	std::string *m_boundValue;
	std::shared_ptr<std::string> m_unboundValue;
	DBLiteColumn() : m_info(nullptr) {
		//m_info = nullptr;
		m_object = nullptr;
		m_boundValue = nullptr;
	};
//	void boundUpdate();
	bool isBound() {
		return (m_boundValue != 0);
	}
public:
	
	DBLiteColumn(DBLiteSchema &info);
	virtual ~DBLiteColumn();
	bool isNull() {
		if (m_object != 0) {
			return false;
		}
		return true;
	}
	std::string &toString();
	bool fromString(const char *str);
	bool fromString(const std::string &str);
	std::string token(int *pos, std::string &str);
	void set(int i);
	void set(long i);
	void set(unsigned int i);
	void set(unsigned long i);
	void set(const char *str);
	void set(const std::string &str);
	void set(const ExifDateTime &date);
	void set(double d);
	void set(float f);
	void set(DBLiteColumn &c);

	DBLiteSchema &getInfo() {
		return *m_info;
	}

	void operator=(const int i) {
		set(i);
    }
	void operator=(const long i) {
		set(i);
    }
	void operator=(const unsigned int i) {
		set(i);
    }
	void operator=(const unsigned long i) {
		set(i);
    }
	void operator=(const char *str) {
		set(str);
    }
	void operator=(std::string &str) {
		set(str);
    }
	void operator=(const ExifDateTime &date) {
		set(date);
    }
	void operator=(const double d) {
		set(d);
    }
	void operator=(const float f) {
		set(f);
    }

	std::shared_ptr<DBLiteSchema> getDBLiteSchemaItem() {
		return m_info;
	}

	const int getInt() const;
	const long getLong();
	const unsigned int getUInt();
	const unsigned long getULong();
	const std::string &getString();
	const ExifDateTime &getDate();
	const double getDouble();
	const float getFloat();

	inline DBLiteColumn& operator<<(int& val);
	inline DBLiteColumn& operator>>(int& val);
	inline friend void operator>>(int& val, DBLiteColumn& col);
	inline friend void operator<<(int& val, DBLiteColumn& col);

	inline DBLiteColumn& operator<<(long& val);
	inline DBLiteColumn& operator>>(long& val);
	inline friend void operator>>(long& val, DBLiteColumn& col);
	inline friend void operator<<(long& val, DBLiteColumn& col);

	inline DBLiteColumn& operator<<(unsigned int& val);
	inline DBLiteColumn& operator>>(unsigned int& val);
	inline friend void operator>>(unsigned int& val, DBLiteColumn& col);
	inline friend void operator<<(unsigned int& val, DBLiteColumn& col);

	inline DBLiteColumn& operator<<(unsigned long& val);
	inline DBLiteColumn& operator>>(unsigned long& val);
	inline friend void operator>>(unsigned long& val, DBLiteColumn& col);
	inline friend void operator<<(unsigned long& val, DBLiteColumn& col);

	inline DBLiteColumn& operator<<(std::string& val);
	inline DBLiteColumn& operator>>(std::string& val);
	inline friend void operator>>(std::string& val, DBLiteColumn& col);
	inline friend void operator<<(std::string& val, DBLiteColumn& col);

	inline DBLiteColumn& operator<<(ExifDateTime& val);
	inline DBLiteColumn& operator>>(ExifDateTime& val);
	inline friend void operator>>(ExifDateTime& val, DBLiteColumn& col);
	inline friend void operator<<(ExifDateTime& val, DBLiteColumn& col);

	inline DBLiteColumn& operator<<(double& val);
	inline DBLiteColumn& operator>>(double& val);
	inline friend void operator>>(double& val, DBLiteColumn& col);
	inline friend void operator<<(double& val, DBLiteColumn& col);

	inline DBLiteColumn& operator<<(float& val);
	inline DBLiteColumn& operator>>(float& val);
	inline friend void operator>>(float& val, DBLiteColumn& col);
	inline friend void operator<<(float& val, DBLiteColumn& col);


	void bind(std::string &str) {
		m_boundValue = &str;
	}
};



class DBLiteRow : public std::vector<DBLiteColumn *> {
	DBLiteTableSchema &m_schema;
	std::string m_text;
	char m_delim;
protected:
	//DBLiteTableSchema& getSchema() {
	//	return *m_schema;
	//}
	friend class DBLiteTable;
	
public:
	
	DBLiteRow(DBLiteTableSchema &ts);
	DBLiteRow(const DBLiteRow &row);
	DBLiteRow &operator=(const DBLiteRow &row);
	virtual ~DBLiteRow();

	const std::string& getName() const {
		return m_schema.getName();
	}

	DBLiteColumn& columnAt(int i) const {
		return *(at(i));
	}

	DBLiteColumn& columnAt(const char *name) const {

		try {
			int idx = m_schema.getIndex(name);
			return *(at(idx));
		} catch (std::exception /*&e */) {
			throw std::invalid_argument("Column name invalid");
		}
	}
	void print() {
		std::string &text = toString();
		std::cout << text << '\n';
	}

	/*
	for (std::vector<DBLiteSchema>::iterator i = begin(); i != end(); i++) {
	DBLiteSchema& columnInfo = *i;
	printf("%s      %s\n", columnInfo.getName().c_str(), columnInfo.getTypeString());
	}
	*/
	void debugPrint() {
		for (std::vector<DBLiteColumn *>::iterator i = this->begin(); i != this->end(); i++) {
			DBLiteColumn *column = *i;
			std::cout << column->toString() << "\n";
		}
	}

	std::string &toDebugString() {
		m_text.clear();
		bool first = true;
		for (std::vector<DBLiteColumn *>::iterator i = this->begin(); i != this->end(); i++) {
			DBLiteColumn *column = *i;
			std::shared_ptr<DBLiteSchema> DBLiteSchema = column->getDBLiteSchemaItem();
			m_text += DBLiteSchema->getName();
			if (DBLiteSchema == nullptr) {
				continue;
			}
			m_text += ',';
			m_text += column->toString();
			/*
			if (first) {
				first = false;
				m_text = column->toString();
			}
			else {
				m_text += ',';
				m_text += column->toString();
			}
			*/
			m_text += '\n';
		}
		return m_text;
	}

	std::string escapeString(std::string &s) {
		if (s.find_first_of(m_delim) == std::string::npos) {
			return s;
		}
		return ('\"' + s + '\"');
	}

	std::string &toString() {
		m_text.clear();
		bool first = true;
		for (std::vector<DBLiteColumn *>::iterator i = this->begin(); i != this->end(); i++) {
			DBLiteColumn *column = *i;
			std::shared_ptr<DBLiteSchema> DBLiteSchema = column->getDBLiteSchemaItem();
			if (DBLiteSchema == nullptr) {
				continue;
			}
			
			if (first) {
			first = false;
			m_text = escapeString(column->toString());
			}
			else {
			m_text += m_delim;
			m_text += escapeString(column->toString());
			}
		}
		return m_text;
	}

	bool fromString(const char *s) {
		std::string str = s;
		return fromString(str);
	}

	bool fromString(const std::string &str) {
		CSVArgs csvArgs(m_delim);
		csvArgs.process(str);
		printf("Row size = %d\n", csvArgs.size());
		if (m_schema.size() != csvArgs.size() && m_schema.size() < (csvArgs.size() - 1)) {
			// Note if the last item is null then csvArgs will be one less then m_schema size
			ErrorCode::setErrorCode(SIA_ERROR::ROW_SCHEMA_MISMATCH);
			return false;
		}
		std::vector<std::string>::iterator arg = csvArgs.begin();
		for (std::vector<DBLiteColumn *>::iterator i = this->begin(); i != this->end(); i++, arg++) {
			
			if (arg == csvArgs.end()) {
				break;
			}
			std::string argStr = *arg;
			DBLiteColumn *column = *i;
			column->fromString(argStr.c_str());
		}

		return true;
	}

	bool join(DBLiteRow &row);

	bool join(const DBLiteRow &row);

	const DBLiteTableSchema& getSchema() const {
		return m_schema;
	}

	
	
};

using SharedDBLiteRow = std::shared_ptr<DBLiteRow>;
class DBLiteTable : public std::vector<SharedDBLiteRow> {
	std::unique_ptr<DBLiteTableSchema> m_TableSchema;
public:
	DBLiteTable(DBLiteTableSchema *pSchemaTable) :
		m_TableSchema(std::make_unique<DBLiteTableSchema>(pSchemaTable))
	{};
	virtual ~DBLiteTable() {};
	DBLiteTableSchema &getSchema() const {
		return *m_TableSchema;
	}
	
	SharedDBLiteRow makeRow();
	bool addRow(const DBLiteRow &r);
	bool fromString(const std::string &r);
	/// reads a csv GPSProperties file
	bool read(const char *path, const char *filename);

	/// writes a csv GPSProperties file
	bool write(const char *path, const char *filename);

	bool truncate() {
		clear();
		return true;
	}
};

using SharedDBLiteTable = std::shared_ptr<DBLiteTable>;
class DBLiteDatabase : public std::map<std::string, std::unique_ptr<DBLiteTable>> {
	static std::string m_path;
public:
	DBLiteDatabase() {		
	};
	virtual ~DBLiteDatabase() {};
	bool addTable(DBLiteTableSchema *pSchemaTable);

	SharedDBLiteTable getTable(std::string name);

	static void setPath(const char *path);
};

// int
inline DBLiteColumn& DBLiteColumn::operator<<(int& val) {
	set(val);
	return *this;
}

inline DBLiteColumn& DBLiteColumn::operator>>(int& val) {
	val = getInt();
	return *this;
}

inline void operator>>(int& val, DBLiteColumn& col) {
	col.set(val);
}

inline void operator<<(int& val, DBLiteColumn& col) {
	val = col.getInt();
}

// long
inline DBLiteColumn& DBLiteColumn::operator<<(long& val) {
	set(val);
	return *this;
}

inline DBLiteColumn& DBLiteColumn::operator>>(long& val) {
	val = getLong();
	return *this;
}

inline void operator>>(long& val, DBLiteColumn& col) {
	col.set(val);
}

inline void operator<<(long& val, DBLiteColumn& col) {
	val = col.getLong();
}

// unsigned int
inline DBLiteColumn& DBLiteColumn::operator<<(unsigned int& val) {
	set(val);
	return *this;
}

inline DBLiteColumn& DBLiteColumn::operator>>(unsigned int& val) {
	val = getUInt();
	return *this;
}

inline void operator>>(unsigned int& val, DBLiteColumn& col) {
	col.set(val);
}

inline void operator<<(unsigned int& val, DBLiteColumn& col) {
	val = col.getUInt();
}

// unsigned long
inline DBLiteColumn& DBLiteColumn::operator<<(unsigned long& val) {
	set(val);
	return *this;
}

inline DBLiteColumn& DBLiteColumn::operator>>(unsigned long& val) {
	val = getULong();
	return *this;
}

inline void operator>>(unsigned long& val, DBLiteColumn& col) {
	col.set(val);
}

inline void operator<<(unsigned long& val, DBLiteColumn& col) {
	val = col.getULong();
}

// std::string
inline DBLiteColumn& DBLiteColumn::operator<<(std::string& val) {
	set(val);
	return *this;
}

inline DBLiteColumn& DBLiteColumn::operator>>(std::string& val) {
	val = getString();
	return *this;
}

inline void operator>>(std::string& val, DBLiteColumn& col) {
	col.set(val);
}

inline void operator<<(std::string& val, DBLiteColumn& col) {
	val = col.getString();
}

// CDate
inline DBLiteColumn& DBLiteColumn::operator<<(ExifDateTime& val) {
	set(val);
	return *this;
}

inline DBLiteColumn& DBLiteColumn::operator>>(ExifDateTime& val) {
	val = getDate();
	return *this;
}

inline void operator>>(ExifDateTime& val, DBLiteColumn& col) {
	col.set(val);
}

inline void operator<<(ExifDateTime& val, DBLiteColumn& col) {
	val = col.getDate();
}

// double
inline DBLiteColumn& DBLiteColumn::operator<<(double& val) {
	set(val);
	return *this;
}

inline DBLiteColumn& DBLiteColumn::operator>>(double& val) {
	val = getDouble();
	return *this;
}

inline void operator>>(double& val, DBLiteColumn& col) {
	col.set(val);
}

inline void operator<<(double& val, DBLiteColumn& col) {
	val = col.getDouble();
}

// float
inline DBLiteColumn& DBLiteColumn::operator<<(float& val) {
	set(val);
	return *this;
}

inline DBLiteColumn& DBLiteColumn::operator>>(float& val) {
	val = getFloat();
	return *this;
}

inline void operator>>(float& val, DBLiteColumn& col) {
	col.set(val);
}

inline void operator<<(float& val, DBLiteColumn& col) {
	val = col.getFloat();
}

} /* namespace simplearchive */

