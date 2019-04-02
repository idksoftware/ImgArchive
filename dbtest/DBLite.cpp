/*
 * DBLite.cpp
 *
 *  Created on: May 5, 2015
 *      Author: wzw7yn
 */
#include <fstream>
#include <sstream>
#include "DBLite.h"

#include "siaglobal.h"
#ifdef _WIN32
#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif


namespace DBLite {
//using namespace simplearchive;


std::string DBLiteDatabase::m_path;
const char *DBLiteColumn::nullStr = "";


class DBLiteObject {
	typedef enum {
		Integer,
	    Text,
	    Date,
	    Float,
	    None
	} EType;
	EType m_type;
	DBLiteObject() {
		m_type = None;
	};
	typedef union {
		double m_double;
		std::string *m_string;
		unsigned long m_int;
		ExifDateTime *m_date;
	} TypeObject;
	TypeObject m_typeObject;
public:

	DBLiteObject(double d) {
		m_typeObject.m_double = d;
		m_type = Float;
	}
	DBLiteObject(const char *s) {
		m_typeObject.m_string = new std::string(s);
		m_type = Text;
	}
	DBLiteObject(unsigned long l) {
		m_typeObject.m_int = l;
		m_type = Integer;
	}
	DBLiteObject(const ExifDateTime &date) {
		m_typeObject.m_date = new ExifDateTime(date);
		m_type = Date;
	}
	virtual ~DBLiteObject() {
		switch(m_type) {
		case Integer: break;
		case Text:
			delete m_typeObject.m_string;
			break;
		case Date:
			delete m_typeObject.m_date;
			break;
		case Float:
			break;
		case None:
			break;
		}
	}
	void set(unsigned long l) {
		if (m_type != DBLiteObject::Integer) {
			throw DBLiteTypeException("Not integer type");
		}
		m_typeObject.m_int = l;
	}

	void operator=(unsigned long l) {
		set(l);
	}
	void set(const std::string &s) {
		if (m_type != DBLiteObject::Text) {
			throw MTTypeException("Not a text type");
		}
		*(m_typeObject.m_string) = s;
	}

	void operator=(std::string &s) {
		set(s);
	}

	void operator=(const char *s) {
		set(s);
	}

	void set(ExifDateTime &d) {
		if (m_type != DBLiteObject::Date) {
			throw DBLiteTypeException("Not a date type");
		}
		*(m_typeObject.m_date) = d;
	}

	void operator=(ExifDateTime &d) {
		set(d);
	}
	void set(double d) {
		if (m_type != DBLiteObject::Float) {
			throw DBLiteTypeException("Not a float type");
		}
		m_typeObject.m_double = d;
	}

	void operator=(double d) {
		set(d);
	}

	const ExifDateTime& getDate() {
		return *(m_typeObject.m_date);
	}

	double getDouble() const {
		return m_typeObject.m_double;
	}

	unsigned long getInt() const {
		return m_typeObject.m_int;
	}

	const std::string& getString() const {
		return *(m_typeObject.m_string);
	}


};

const char *DBLiteSchema::getTypeString() {
	switch(m_type)  {
	case Integer: return "integer";
	case Text: return "text";
	case Date: return "date";
	case Float: return "float";
	}
	return "unknown";
}

const char *DBLiteTypeException::what() const throw() {
	//std::stringstream s;
	//s << message << " line:" << __LINE__ << " file:" << __FILE__;
	//std::string tmp = s.str();
	//output = tmp.c_str();
	return message.c_str();
}


DBLiteColumn::DBLiteColumn(DBLiteSchema &info) : m_info(new DBLiteSchema(info)) {
	m_object = 0;
	m_boundValue = 0;
}

static int count = 0;
DBLiteColumn::~DBLiteColumn() {
	if (m_object != 0) {
		count++;
		if (count == 33) {
			int i = 0;
		}
		delete m_object;
	}
}
std::string &DBLiteColumn::toString() {

	std::string *returned = 0;
	if (isBound()) {
		returned = m_boundValue;
	}
	else {
		m_unboundValue = std::make_shared<std::string>();
		returned = m_unboundValue.get();
	}
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (m_object == 0) {
		*returned = nullStr;
		return *returned;
	}
	switch(type) {
	case DBLiteSchema::Integer:
		{
		std::stringstream tmp;
		unsigned long integerVal = m_object->getInt();
		tmp << integerVal;
		*returned = tmp.str();
		break;
		}
	case DBLiteSchema::Text:
		*returned = m_object->getString();
		break;
	case DBLiteSchema::Date:
		{
		//const CDate& getDate()
			const ExifDateTime& date = m_object->getDate();
			*returned = ((ExifDateTime&)date).current();
		break;
		}
	case  DBLiteSchema::Float:
		{
		std::stringstream tmp;
		double doubleVal =  m_object->getDouble();
		tmp << doubleVal;
		*returned = tmp.str();
		break;
		}
	default:
		throw DBLiteTypeException("Invalid type");
	}

	return *returned;
}

bool DBLiteColumn::fromString(const char *str) {
	const std::string tmp(str);
	return fromString(tmp);
}

bool DBLiteColumn::fromString(const std::string &str) {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	std::stringstream tmp(str);
	switch(type) {
	case DBLiteSchema::Integer:
		{
		unsigned long i = 0;
		tmp >> i;
		set(i);
		return true;
		}
	case DBLiteSchema::Text:
		set(str);
		//= (const std::string)str;
		return true;
	case DBLiteSchema::Date:
		{
		ExifDateTime date(str.c_str());
		set(date);
		return true;
		}
	case  DBLiteSchema::Float:
		{
		double doubleVal = 0.0;
		tmp >> doubleVal;
		set(doubleVal);
		return true;
		}
	}
	return false;
}

std::string DBLiteColumn::token(int *pos, std::string &str) {
	int delim = str.find_first_of(DELIM, *pos);
	std::string tmp = str.substr(*pos, (delim - *pos));
	//printf("%s\n",tmp.c_str());
	*pos = delim;
	return tmp;
}


void DBLiteColumn::set(int i) {
	set((unsigned long)i);
}
void DBLiteColumn::set(long i) {
	set((unsigned long)i);
}
void DBLiteColumn::set(unsigned int i) {
	set((unsigned long)i);
}
void DBLiteColumn::set(unsigned long i) {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Integer) {
		if (m_object == nullptr) {
			 m_object = new DBLiteObject(i);
		} else {
			*m_object = i;
		}
//		boundUpdate();
	}
	else {
		throw DBLiteTypeException("Invalid type");
	}
}
void DBLiteColumn::set(const char *str) {
	std::string tmp(str);
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Text) {
		if (m_object == nullptr) {
			 m_object = new DBLiteObject(tmp.c_str());
		} else {
			*m_object = tmp;
		}
//		boundUpdate();
	}
	else {
		throw DBLiteTypeException("Invalid type");
	}
}
void DBLiteColumn::set(const std::string &str) {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Text) {
		if (m_object == nullptr) {
			 m_object = new DBLiteObject(str.c_str());
		} else {
			*m_object = str.c_str();
		}
//		boundUpdate();
	}
	else {
		throw DBLiteTypeException("Invalid type");
	}
}
void DBLiteColumn::set(const ExifDateTime &date) {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Date) {
		if (m_object == nullptr) {
			 m_object = new DBLiteObject(date);
		} else {
			*m_object = date;
		}
//		boundUpdate();
	}
	else {
		throw DBLiteTypeException("Invalid type");
	}
}
void DBLiteColumn::set(double d) {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Float) {
		if (m_object == nullptr) {
			m_object = new DBLiteObject(d);
		} else {
			*m_object = d;
		}
//		boundUpdate();
	}
	else {
		throw DBLiteTypeException("Invalid type");
	}
}
void DBLiteColumn::set(float f) {
	set((double)f);
}

void DBLiteColumn::set(DBLiteColumn &c) {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (c.isNull()) {
		return;
	}
	switch(type) {
	case DBLiteSchema::Integer:
		set(c.getULong());
		break;
	case DBLiteSchema::Text:
		set(c.getString());
		break;
	case DBLiteSchema::Date:
		set((ExifDateTime&)c.getDate());
		break;
	case  DBLiteSchema::Float:
		set(c.getDouble());
		break;
	default:
		throw DBLiteTypeException("Invalid type");
	}

}

const int DBLiteColumn::getInt() const {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Integer) {
		return m_object->getInt();
	}
	else {
		throw DBLiteTypeException("Invalid type");
	}
}
const long DBLiteColumn::getLong() {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Integer) {
		return m_object->getInt();
	}
	else {
		throw DBLiteTypeException("Invalid type");
	}
}
const unsigned int DBLiteColumn::getUInt() {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Integer) {
		return m_object->getInt();
	}
	else {
		throw DBLiteTypeException("Invalid type");
	}
}

const unsigned long DBLiteColumn::getULong() {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Integer) {
		return m_object->getInt();
	}
	else {
		throw DBLiteTypeException("Invalid type");
	}
}

const std::string &DBLiteColumn::getString() {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Text) {
		return m_object->getString();
	}
	else {
		throw DBLiteTypeException("Invalid type");
	}
}

const ExifDateTime &DBLiteColumn::getDate() {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Date) {
		if (m_object == nullptr) {
			throw DBLiteTypeException("null Date type");
		}
		return m_object->getDate();
	}
	else {
		throw DBLiteTypeException("Date Invalid type");
	}
}

const double DBLiteColumn::getDouble() {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Float) {
		return m_object->getDouble();
	}
	else {
		throw DBLiteTypeException("Invalid type");
	}
}

const float DBLiteColumn::getFloat() {
	DBLiteSchema::EIteDBLiteype type = m_info->getType();
	if (type == DBLiteSchema::Float) {
		return (const float)m_object->getDouble();
	}
	else {
		throw DBLiteTypeException("Invalid type");
	}
}

/*
void DBLiteColumn::boundUpdate() {
	if (m_object != 0 && m_boundString != 0) {
		*m_boundString = toString();
	}
}
*/

DBLiteRow::DBLiteRow(DBLiteTableSchema &schemaTable) : m_schema(schemaTable), m_delim(',')
{
	for (std::vector<DBLiteSchema>::iterator i = m_schema.begin(); i != m_schema.end(); i++) {
		DBLiteSchema& columnInfo = *i;
		push_back(new DBLiteColumn(columnInfo));
	}
}

DBLiteRow::DBLiteRow(const DBLiteRow &row) : m_schema(row.m_schema), m_delim(row.m_delim) {
	m_schema = row.m_schema;
	for (std::vector<DBLiteSchema>::iterator i = m_schema.begin(); i != m_schema.end(); i++) {
		DBLiteSchema& columnInfo = *i;
		push_back(new DBLiteColumn(columnInfo));
	}
	join((DBLiteRow&)row);
}

DBLiteRow &DBLiteRow::operator=(const DBLiteRow &row) {
	m_schema = row.m_schema;
	m_delim = row.m_delim;

	for (std::vector<DBLiteColumn *>::iterator i = this->begin(); i != this->end(); i++) {
		DBLiteColumn *column = *i;
		delete column;
	}
	clear();
	for (std::vector<DBLiteSchema>::iterator i = m_schema.begin(); i != m_schema.end(); i++) {
		DBLiteSchema& columnInfo = *i;
		push_back(new DBLiteColumn(columnInfo));
	}
	join((DBLiteRow&)row);
	return *this;
}

DBLiteRow::~DBLiteRow() {
	for (std::vector<DBLiteColumn *>::iterator i = this->begin(); i != this->end(); i++) {
		DBLiteColumn *column = *i;
		delete column;
	}
	this->clear();
	//delete m_schema;
};
bool DBLiteRow::join(DBLiteRow &otherRow) {

	DBLiteTableSchema& thisSchema = (DBLiteTableSchema&)getSchema();
	DBLiteTableSchema& otherSchema = (DBLiteTableSchema&)otherRow.getSchema();
	int thisIndex = 0;
	int otherIndex = 0;
	try {
	for (std::vector<DBLiteSchema>::iterator i = thisSchema.begin(); i != thisSchema.end(); i++, thisIndex++) {
		DBLiteSchema& columnInfo = *i;
		//printf("%s\n", columnInfo.getName().c_str());
		otherIndex = otherSchema.getIndex(columnInfo.getName().c_str());
		if (otherIndex == -1) {
			continue;
		}
		DBLiteColumn& thisColumn = columnAt(thisIndex);
		DBLiteColumn& otherColumn = otherRow.columnAt(otherIndex);
		thisColumn.set(otherColumn);

	}
	} catch (const std::exception& ex) {
		//printf(" thisIndex:%d otherIndex:%d\n", thisIndex, otherIndex);
		printf("%s\n", ex.what());
	}
	return true;
}

bool DBLiteRow::join(const DBLiteRow &otherRow) {

	DBLiteTableSchema& thisSchema = (DBLiteTableSchema&)getSchema();
	DBLiteTableSchema& otherSchema = (DBLiteTableSchema&)otherRow.getSchema();
	int thisIndex = 0;
	int otherIndex = 0;
	try {
		for (std::vector<DBLiteSchema>::iterator i = thisSchema.begin(); i != thisSchema.end(); i++, thisIndex++) {
			DBLiteSchema& columnInfo = *i;
			printf("%s\n", columnInfo.getName().c_str());
			otherIndex = otherSchema.getIndex(columnInfo.getName().c_str());
			if (otherIndex == -1) {
				continue;
			}
			DBLiteColumn& thisColumn = columnAt(thisIndex);
			DBLiteColumn& otherColumn = otherRow.columnAt(otherIndex);
			if (thisColumn.getInfo().getType() != otherColumn.getInfo().getType()) {
				printf("Invalid Types");
				ErrorCode::setErrorCode(SIA_ERROR::TYPE_MISMATCH);
			}
			thisColumn.set(otherColumn);

		}
	}
	catch (const std::exception& ex) {
		printf(" thisIndex:%d otherIndex:%d\n", thisIndex, otherIndex);
		printf("%s\n", ex.what());
	}
	return true;
}


SharedDBLiteRow DBLiteTable::makeRow() {
	auto row = std::make_shared<DBLiteRow>(*(m_TableSchema.get()));
	return row;
}

bool DBLiteTable::addRow(const DBLiteRow &r) {
	auto row = std::make_shared<DBLiteRow>(r);
	push_back(std::move(row));
	return true;
}

bool DBLiteTable::fromString(const std::string &r) {
	auto rowPtr = std::make_shared<DBLiteRow>(*(m_TableSchema.get()));
	DBLiteRow *row = rowPtr.get();
	if (!(row->fromString(r))) {
		return false;
	}
	push_back(std::move(rowPtr));
	return true;
}

bool DBLiteTable::read(const char *path, const char *filename) {
	std::string fullpath(path);
	fullpath += '/';
	fullpath += filename;

	
	std::ifstream file(fullpath);
	if (file.is_open() == false) {
		ErrorCode::setErrorCode(SIA_ERROR::OPEN_ERROR);
		return false;
	}

	for (std::string line; std::getline(file, line);) {
		if (line.length() > 0) {
			if (fromString(line) == false) {
				ErrorCode::setErrorCode(SIA_ERROR::READ_ERROR);
				return false;
			}
		}
		else {
			break;
		}
	}

	// make this more robust??
	/*
	while (file.getline(text, 1024 * 2)) {
		if (strlen(text) > 0) {
			if (fromString(text) == false) {
				ErrorCode::setErrorCode(SIA_ERROR::READ_ERROR);
				return false;
			}
		}
		else {
			break;
		}
	}
	*/
	file.close();

	return true;
}

bool DBLiteTable::write(const char *path, const char *filename) {
	std::string fullpath(path);
	fullpath += '/';
	fullpath += filename;
	
	std::ofstream file(fullpath.c_str(), std::ofstream::trunc);
	if (file.is_open() == false) {
		ErrorCode::setErrorCode(SIA_ERROR::OPEN_ERROR);
		return false;
	}
	std::stringstream s;
	for (auto i = this->begin(); i != this->end(); i++) {
		s << (*i)->toString() << "\n";
		printf("%s\n", (*i)->toString().c_str());
	}

	file << s.str();
	file.close();
	return true;
}

void DBLiteDatabase::setPath(const char *path) {
	m_path = path;
}

bool DBLiteDatabase::addTable(DBLiteTableSchema *pSchemaTable) {
	auto table = std::make_unique<DBLiteTable>(pSchemaTable);
	insert(std::make_pair(pSchemaTable->getName(), std::move(table)));
	return true;
}

SharedDBLiteTable DBLiteDatabase::getTable(std::string name) {
	auto iter = find(name);
	if (iter == end()) {
		throw std::exception();
	}
	SharedDBLiteTable table = std::move(iter->second);
	return table;
}

} /* namespace simplearchive */
