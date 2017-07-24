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

namespace simplearchive {

class MTTypeException : public std::exception {
	std::string message;
	
public:
	explicit MTTypeException(const char *msg) : message(msg){

	}
	explicit MTTypeException(std::string msg) : message(msg) {}
	virtual ~MTTypeException() throw() {}
	virtual const char *what() const throw();

};

class MTSchema {
public:
        typedef enum {
                Integer,
                Text,
                Date,
                Float
        } EItemType;
private:
       EItemType m_type;
       std::string m_name;
public:

       MTSchema(EItemType type, const char *name) {
    	   m_type  = type;
    	   m_name  = name;
       }

       MTSchema(EItemType type, std::string &name) {
    	   m_type  = type;
    	   m_name  = name;
       }

	   MTSchema(const MTSchema &c) {
		   m_type = c.m_type;
		   m_name = c.m_name;
	   }
	

	const std::string& getName() const {
		return m_name;
	}

	EItemType getType() const {
		return m_type;
	}

	const char *getTypeString();
};

class MTTableSchema : private std::vector<MTSchema> {
	std::map<std::string, int> m_index;
	int m_count;
protected:
	std::string m_tablename;
public:
	MTTableSchema(const char *name) {
		m_tablename = name;
		m_count = 0;
	}
	MTTableSchema(MTTableSchema &ts) {
		m_tablename = ts.m_tablename;
		m_count = ts.m_count;
		int count = 0;
		for (auto i = ts.begin(); i != ts.end(); i++) {
			MTSchema& item = *i;
			push_back(item);
			m_index.insert(std::make_pair(item.getName(), count++));
		}

	}
	virtual ~MTTableSchema() {
		m_index.clear();
		clear();
	}

	void add(MTSchema item) {
		push_back(MTSchema(item));
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
		for (std::vector<MTSchema>::iterator i = begin(); i != end(); i++) {
			MTSchema& columnInfo = *i;
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
		return std::vector<MTSchema>::size();
	}

	std::vector<MTSchema>::iterator begin() {
		return std::vector<MTSchema>::begin();
	}

	std::vector<MTSchema>::iterator end() {
		return std::vector<MTSchema>::end();
	}
};



class MetaTypeObject;
class MTColumn {
	static const char *nullStr;
	std::shared_ptr<MTSchema> m_info;
	MetaTypeObject *m_object;
	std::string *m_boundValue;
	std::unique_ptr<std::string> m_unboundValue;
	MTColumn() : m_info(nullptr) {
		//m_info = nullptr;
		m_object = nullptr;
		m_boundValue = nullptr;
	};
//	void boundUpdate();
	bool isBound() {
		return (m_boundValue != 0);
	}
public:
	
	MTColumn(MTSchema &info);
	virtual ~MTColumn();
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
	void set(MTColumn &c);

	MTSchema &getInfo() {
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

	std::shared_ptr<MTSchema> getMTSchemaItem() {
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

	inline MTColumn& operator<<(int& val);
	inline MTColumn& operator>>(int& val);
	inline friend void operator>>(int& val, MTColumn& col);
	inline friend void operator<<(int& val, MTColumn& col);

	inline MTColumn& operator<<(long& val);
	inline MTColumn& operator>>(long& val);
	inline friend void operator>>(long& val, MTColumn& col);
	inline friend void operator<<(long& val, MTColumn& col);

	inline MTColumn& operator<<(unsigned int& val);
	inline MTColumn& operator>>(unsigned int& val);
	inline friend void operator>>(unsigned int& val, MTColumn& col);
	inline friend void operator<<(unsigned int& val, MTColumn& col);

	inline MTColumn& operator<<(unsigned long& val);
	inline MTColumn& operator>>(unsigned long& val);
	inline friend void operator>>(unsigned long& val, MTColumn& col);
	inline friend void operator<<(unsigned long& val, MTColumn& col);

	inline MTColumn& operator<<(std::string& val);
	inline MTColumn& operator>>(std::string& val);
	inline friend void operator>>(std::string& val, MTColumn& col);
	inline friend void operator<<(std::string& val, MTColumn& col);

	inline MTColumn& operator<<(ExifDateTime& val);
	inline MTColumn& operator>>(ExifDateTime& val);
	inline friend void operator>>(ExifDateTime& val, MTColumn& col);
	inline friend void operator<<(ExifDateTime& val, MTColumn& col);

	inline MTColumn& operator<<(double& val);
	inline MTColumn& operator>>(double& val);
	inline friend void operator>>(double& val, MTColumn& col);
	inline friend void operator<<(double& val, MTColumn& col);

	inline MTColumn& operator<<(float& val);
	inline MTColumn& operator>>(float& val);
	inline friend void operator>>(float& val, MTColumn& col);
	inline friend void operator<<(float& val, MTColumn& col);


	void bind(std::string &str) {
		m_boundValue = &str;
	}
};



class MTRow : public std::vector<MTColumn *> {
	MTTableSchema &m_schema;
	std::string m_text;
	char m_delim;
protected:
	//MTTableSchema& getSchema() {
	//	return *m_schema;
	//}
	friend class MTTable;
	
public:
	
	MTRow(MTTableSchema &ts);
	MTRow(const MTRow &row);
	MTRow &operator=(const MTRow &row);
	virtual ~MTRow();

	const std::string& getName() const {
		return m_schema.getName();
	}

	MTColumn& columnAt(int i) const {
		return *(at(i));
	}

	MTColumn& columnAt(const char *name) const {

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
	for (std::vector<MTSchema>::iterator i = begin(); i != end(); i++) {
	MTSchema& columnInfo = *i;
	printf("%s      %s\n", columnInfo.getName().c_str(), columnInfo.getTypeString());
	}
	*/
	void debugPrint() {
		for (std::vector<MTColumn *>::iterator i = this->begin(); i != this->end(); i++) {
			MTColumn *column = *i;
			std::cout << column->toString() << "\n";
		}
	}

	std::string &toDebugString() {
		m_text.clear();
		bool first = true;
		for (std::vector<MTColumn *>::iterator i = this->begin(); i != this->end(); i++) {
			MTColumn *column = *i;
			std::shared_ptr<MTSchema> mtSchema = column->getMTSchemaItem();
			m_text += mtSchema->getName();
			if (mtSchema == nullptr) {
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
		for (std::vector<MTColumn *>::iterator i = this->begin(); i != this->end(); i++) {
			MTColumn *column = *i;
			std::shared_ptr<MTSchema> mtSchema = column->getMTSchemaItem();
			if (mtSchema == nullptr) {
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
		for (std::vector<MTColumn *>::iterator i = this->begin(); i != this->end(); i++, arg++) {
			
			if (arg == csvArgs.end()) {
				break;
			}
			std::string argStr = *arg;
			MTColumn *column = *i;
			column->fromString(argStr.c_str());
		}

		return true;
	}

	bool join(MTRow &row);

	bool join(const MTRow &row);

	const MTTableSchema& getSchema() const {
		return m_schema;
	}

	
	
};

using SharedMTRow = std::shared_ptr<MTRow>;
class MTTable : public std::vector<SharedMTRow> {
	std::unique_ptr<MTTableSchema> m_TableSchema;
public:
	MTTable(MTTableSchema *pSchemaTable) {
		m_TableSchema.reset(pSchemaTable);
	};
	virtual ~MTTable() {};
	MTTableSchema &getSchema() const {
		return *m_TableSchema;
	}
	
	SharedMTRow makeRow();
	bool addRow(const MTRow &r);
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


class MTDatabase : public std::map<std::string, std::unique_ptr<MTTable>> {
	
public:
	MTDatabase() {		
	};
	virtual ~MTDatabase() {};
	bool addTable(MTTableSchema *pSchemaTable);

	MTTable& getTable(std::string name);
};

// int
inline MTColumn& MTColumn::operator<<(int& val) {
	set(val);
	return *this;
}

inline MTColumn& MTColumn::operator>>(int& val) {
	val = getInt();
	return *this;
}

inline void operator>>(int& val, MTColumn& col) {
	col.set(val);
}

inline void operator<<(int& val, MTColumn& col) {
	val = col.getInt();
}

// long
inline MTColumn& MTColumn::operator<<(long& val) {
	set(val);
	return *this;
}

inline MTColumn& MTColumn::operator>>(long& val) {
	val = getLong();
	return *this;
}

inline void operator>>(long& val, MTColumn& col) {
	col.set(val);
}

inline void operator<<(long& val, MTColumn& col) {
	val = col.getLong();
}

// unsigned int
inline MTColumn& MTColumn::operator<<(unsigned int& val) {
	set(val);
	return *this;
}

inline MTColumn& MTColumn::operator>>(unsigned int& val) {
	val = getUInt();
	return *this;
}

inline void operator>>(unsigned int& val, MTColumn& col) {
	col.set(val);
}

inline void operator<<(unsigned int& val, MTColumn& col) {
	val = col.getUInt();
}

// unsigned long
inline MTColumn& MTColumn::operator<<(unsigned long& val) {
	set(val);
	return *this;
}

inline MTColumn& MTColumn::operator>>(unsigned long& val) {
	val = getULong();
	return *this;
}

inline void operator>>(unsigned long& val, MTColumn& col) {
	col.set(val);
}

inline void operator<<(unsigned long& val, MTColumn& col) {
	val = col.getULong();
}

// std::string
inline MTColumn& MTColumn::operator<<(std::string& val) {
	set(val);
	return *this;
}

inline MTColumn& MTColumn::operator>>(std::string& val) {
	val = getString();
	return *this;
}

inline void operator>>(std::string& val, MTColumn& col) {
	col.set(val);
}

inline void operator<<(std::string& val, MTColumn& col) {
	val = col.getString();
}

// CDate
inline MTColumn& MTColumn::operator<<(ExifDateTime& val) {
	set(val);
	return *this;
}

inline MTColumn& MTColumn::operator>>(ExifDateTime& val) {
	val = getDate();
	return *this;
}

inline void operator>>(ExifDateTime& val, MTColumn& col) {
	col.set(val);
}

inline void operator<<(ExifDateTime& val, MTColumn& col) {
	val = col.getDate();
}

// double
inline MTColumn& MTColumn::operator<<(double& val) {
	set(val);
	return *this;
}

inline MTColumn& MTColumn::operator>>(double& val) {
	val = getDouble();
	return *this;
}

inline void operator>>(double& val, MTColumn& col) {
	col.set(val);
}

inline void operator<<(double& val, MTColumn& col) {
	val = col.getDouble();
}

// float
inline MTColumn& MTColumn::operator<<(float& val) {
	set(val);
	return *this;
}

inline MTColumn& MTColumn::operator>>(float& val) {
	val = getFloat();
	return *this;
}

inline void operator>>(float& val, MTColumn& col) {
	col.set(val);
}

inline void operator<<(float& val, MTColumn& col) {
	val = col.getFloat();
}

} /* namespace simplearchive */

