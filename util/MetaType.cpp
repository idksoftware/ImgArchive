/*
 * MetaType.cpp
 *
 *  Created on: May 5, 2015
 *      Author: wzw7yn
 */
#include <fstream>
#include <sstream>
#include "MetaType.h"

#include "siaglobal.h"
#ifdef _WIN32
#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif
namespace simplearchive {

const char *MTColumn::nullStr = "";


class MetaTypeObject {
	typedef enum {
		Integer,
	    Text,
	    Date,
	    Float,
	    None
	} EType;
	EType m_type;
	MetaTypeObject() {
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

	MetaTypeObject(double d) {
		m_typeObject.m_double = d;
		m_type = Float;
	}
	MetaTypeObject(const char *s) {
		m_typeObject.m_string = new std::string(s);
		m_type = Text;
	}
	
	MetaTypeObject(unsigned long l) {
		m_typeObject.m_int = l;
		m_type = Integer;
	}
	MetaTypeObject(const ExifDateTime &date) {
		m_typeObject.m_date = new ExifDateTime(date);
		m_type = Date;
	}
	MetaTypeObject& operator=(const ExifDateTime &date) {
		delete m_typeObject.m_date;
		m_typeObject.m_date = new ExifDateTime(date);
		m_type = Date;
		return *this;
	}
	virtual ~MetaTypeObject() {
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
		if (m_type != MetaTypeObject::Integer) {
			throw MTTypeException("Not integer type");
		}
		m_typeObject.m_int = l;
	}

	void operator=(unsigned long l) {
		set(l);
	}
	void set(const std::string &s) {
		if (m_type != MetaTypeObject::Text) {
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
		if (m_type != MetaTypeObject::Date) {
			throw MTTypeException("Not a date type");
		}
		*(m_typeObject.m_date) = d;
	}

	void operator=(ExifDateTime &d) {
		set(d);
	}
	void set(double d) {
		if (m_type != MetaTypeObject::Float) {
			throw MTTypeException("Not a float type");
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

const char *MTSchema::getTypeString() {
	switch(m_type)  {
	case Integer: return "integer";
	case Text: return "text";
	case Date: return "date";
	case Float: return "float";
	}
	return "unknown";
}

const char *MTTypeException::what() const throw() {
	//std::stringstream s;
	//s << message << " line:" << __LINE__ << " file:" << __FILE__;
	//std::string tmp = s.str();
	//output = tmp.c_str();
	return message.c_str();
}


MTColumn::MTColumn(const MTSchema &info) : m_info(new MTSchema(info)) {
}

MTColumn::MTColumn(const MTColumn& r) {
	m_object = nullptr;
	m_boundValue = nullptr;
	MTSchema::EItemType type = m_info->getType();
	if (r.m_object == nullptr) {
		m_object = nullptr;
	}
	switch (type) {
	case MTSchema::Integer:
		set(r.m_object->getInt());
		break;
	case MTSchema::Text:
		set(r.m_object->getString());
		break;
	case MTSchema::Date:
		set(r.m_object->getDate());
		break;
	case  MTSchema::Float:
		set(r.m_object->getDouble());
		break;
	}
	return;
}

MTColumn& MTColumn::operator=(const MTColumn& r) {
	MTSchema::EItemType type = m_info->getType();
	if (r.m_object == 0) {
		m_object = 0;
	}
	switch (type) {
	case MTSchema::Integer:
		set(r.m_object->getInt());
		break;
	case MTSchema::Text:
		set(r.m_object->getString());
		break;
	case MTSchema::Date:
		set(r.m_object->getDate());
		break;
	case  MTSchema::Float:
		set(r.m_object->getDouble());
		break;
	}
	return *this;
}



MTColumn::~MTColumn() {
	if (m_object != 0) {
		//delete m_object;
	}
}
std::string &MTColumn::toString() {

	std::string *returned = 0;
	if (isBound()) {
		returned = m_boundValue.get();
	}
	else {
		m_unboundValue.reset(new std::string);
		returned = m_unboundValue.get();
	}
	MTSchema::EItemType type = m_info->getType();
	if (m_object == 0) {
		*returned = nullStr;
		return *returned;
	}
	switch(type) {
	case MTSchema::Integer:
		{
		std::stringstream tmp;
		unsigned long integerVal = m_object->getInt();
		tmp << integerVal;
		*returned = tmp.str();
		break;
		}
	case MTSchema::Text:
		*returned = m_object->getString();
		break;
	case MTSchema::Date:
		{
		//const CDate& getDate()
			const ExifDateTime& date = m_object->getDate();
			*returned = ((ExifDateTime&)date).current();
		break;
		}
	case  MTSchema::Float:
		{
		std::stringstream tmp;
		double doubleVal =  m_object->getDouble();
		tmp << doubleVal;
		*returned = tmp.str();
		break;
		}
	default:
		throw MTTypeException("Invalid type");
	}

	return *returned;
}

bool MTColumn::fromString(const char *str) {
	const std::string tmp(str);
	return fromString(tmp);
}

bool MTColumn::fromString(const std::string &str) {
	MTSchema::EItemType type = m_info->getType();
	std::stringstream tmp(str);
	switch(type) {
	case MTSchema::Integer:
		{
		unsigned long i = 0;
		tmp >> i;
		set(i);
		return true;
		}
	case MTSchema::Text:
		set(str);
		//= (const std::string)str;
		return true;
	case MTSchema::Date:
		{
		ExifDateTime date(str.c_str());
		set(date);
		return true;
		}
	case  MTSchema::Float:
		{
		double doubleVal = 0.0;
		tmp >> doubleVal;
		set(doubleVal);
		return true;
		}
	}
	return false;
}

std::string MTColumn::token(int *pos, std::string &str) {
	int delim = str.find_first_of(DELIM, *pos);
	std::string tmp = str.substr(*pos, (delim - *pos));
	//printf("%s\n",tmp.c_str());
	*pos = delim;
	return tmp;
}


void MTColumn::set(int i) {
	set((unsigned long)i);
}
void MTColumn::set(long i) {
	set((unsigned long)i);
}
void MTColumn::set(unsigned int i) {
	set((unsigned long)i);
}
void MTColumn::set(unsigned long i) {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Integer) {
		if (m_object == nullptr) {
			m_object.reset(new MetaTypeObject(i));
		} else {
			*m_object = i;
		}
//		boundUpdate();
	}
	else {
		throw MTTypeException("Invalid type");
	}
}
void MTColumn::set(const char *str) {
	std::string tmp(str);
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Text) {
		if (m_object == nullptr) {
			m_object.reset(new MetaTypeObject(tmp.c_str()));
		} else {
			*m_object = str;
		}
//		boundUpdate();
	}
	else if (type == MTSchema::Date) {
		if (m_object == nullptr) {
			ExifDateTime date(str);
			m_object.reset(new MetaTypeObject(date));
		}
	}
	else {
		throw MTTypeException("Invalid type");
	}
}
void MTColumn::set(const std::string &str) {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Text) {
		if (m_object == nullptr) {
			m_object.reset(new MetaTypeObject(str.c_str()));
		} else {
			*m_object = str.c_str();
		}
//		boundUpdate();
	}
	else {
		throw MTTypeException("Invalid type");
	}
}
void MTColumn::set(const ExifDateTime &date) {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Date) {
		if (m_object == nullptr) {
			m_object.reset(new MetaTypeObject(date));
		} else {
			*m_object = date;
		}
//		boundUpdate();
	}
	else {
		throw MTTypeException("Invalid type");
	}
}
void MTColumn::set(double d) {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Float) {
		if (m_object == nullptr) {
			m_object.reset(new MetaTypeObject(d));
		} else {
			*m_object = d;
		}
//		boundUpdate();
	}
	else {
		throw MTTypeException("Invalid type");
	}
}
void MTColumn::set(float f) {
	set((double)f);
}

void MTColumn::set(MTColumn &c) {
	MTSchema::EItemType type = m_info->getType();
	if (c.isNull()) {
		return;
	}
	switch(type) {
	case MTSchema::Integer:
		set(c.getULong());
		break;
	case MTSchema::Text:
		set(c.getString());
		break;
	case MTSchema::Date:
		set((ExifDateTime&)c.getDate());
		break;
	case  MTSchema::Float:
		set(c.getDouble());
		break;
	default:
		throw MTTypeException("Invalid type");
	}

}

const int MTColumn::getInt() const {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Integer) {
		return m_object->getInt();
	}
	else {
		throw MTTypeException("Invalid type");
	}
}
const long MTColumn::getLong() {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Integer) {
		return m_object->getInt();
	}
	else {
		throw MTTypeException("Invalid type");
	}
}
const unsigned int MTColumn::getUInt() {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Integer) {
		return m_object->getInt();
	}
	else {
		throw MTTypeException("Invalid type");
	}
}

const unsigned long MTColumn::getULong() {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Integer) {
		return m_object->getInt();
	}
	else {
		throw MTTypeException("Invalid type");
	}
}

const std::string &MTColumn::getString() {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Text) {
		return m_object->getString();
	}
	else {
		throw MTTypeException("Invalid type");
	}
}

const ExifDateTime &MTColumn::getDate() {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Date) {
		if (m_object == nullptr) {
			throw MTTypeException("null Date type");
		}
		return m_object->getDate();
	}
	else {
		throw MTTypeException("Date Invalid type");
	}
}

const double MTColumn::getDouble() {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Float) {
		return m_object->getDouble();
	}
	else {
		throw MTTypeException("Invalid type");
	}
}

const float MTColumn::getFloat() {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Float) {
		return (const float)m_object->getDouble();
	}
	else {
		throw MTTypeException("Invalid type");
	}
}

/*
void MTColumn::boundUpdate() {
	if (m_object != 0 && m_boundString != 0) {
		*m_boundString = toString();
	}
}
*/

MTRow::MTRow(MTTableSchema &schemaTable) : m_schema(schemaTable), m_delim(',')
{
	for (std::vector<MTSchema>::iterator i = m_schema.begin(); i != m_schema.end(); i++) {
		std::shared_ptr<MTColumn> col = std::make_shared<MTColumn>(*i);
		this->emplace_back(col);
	}
}

MTRow::MTRow(const MTRow &row) : m_schema(row.m_schema), m_delim(row.m_delim) {
	
	for (std::vector<MTSchema>::iterator i = m_schema.begin(); i != m_schema.end(); i++) {
		std::shared_ptr<MTColumn> col = std::make_shared<MTColumn>(*i);
		this->emplace_back(col);
	}
	join((MTRow&)row);
}

MTRow &MTRow::operator=(const MTRow &row) {
	m_schema = row.m_schema;
	m_delim = row.m_delim;


	//for (auto i = this->begin(); i != this->end(); i++) {
	//	MTColumn *column = *i;
	//	delete column;
	//}
	clear();
	for (std::vector<MTSchema>::iterator i = m_schema.begin(); i != m_schema.end(); i++) {
		std::shared_ptr<MTColumn> col = std::make_shared<MTColumn>(*i);
		this->emplace_back(col);
	}
	join((MTRow&)row);
	return *this;
}

MTRow::~MTRow() {
	//for (std::vector<MTColumn *>::iterator i = this->begin(); i != this->end(); i++) {
	//	MTColumn *column = *i;
	//	delete column;
	//}
	this->clear();
	//delete m_schema;
};
bool MTRow::join(MTRow &otherRow) {

	MTTableSchema& thisSchema = (MTTableSchema&)getSchema();
	MTTableSchema& otherSchema = (MTTableSchema&)otherRow.getSchema();
	int thisIndex = 0;
	int otherIndex = 0;
	try {
	for (std::vector<MTSchema>::iterator i = thisSchema.begin(); i != thisSchema.end(); i++, thisIndex++) {
		MTSchema& columnInfo = *i;
		//printf("%s\n", columnInfo.getName().c_str());
		otherIndex = otherSchema.getIndex(columnInfo.getName().c_str());
		if (otherIndex == -1) {
			continue;
		}
		MTColumn& thisColumn = columnAt(thisIndex);
		MTColumn& otherColumn = otherRow.columnAt(otherIndex);
		thisColumn.set(otherColumn);

	}
	} catch (const std::exception& ex) {
		//printf(" thisIndex:%d otherIndex:%d\n", thisIndex, otherIndex);
		printf("%s\n", ex.what());
	}
	return true;
}

bool MTRow::join(const MTRow &otherRow) {

	MTTableSchema& thisSchema = (MTTableSchema&)getSchema();
	MTTableSchema& otherSchema = (MTTableSchema&)otherRow.getSchema();
	int thisIndex = 0;
	int otherIndex = 0;
	try {
		for (std::vector<MTSchema>::iterator i = thisSchema.begin(); i != thisSchema.end(); i++, thisIndex++) {
			MTSchema& columnInfo = *i;
			//printf("%s\n", columnInfo.getName().c_str());
			otherIndex = otherSchema.getIndex(columnInfo.getName().c_str());
			if (otherIndex == -1) {
				continue;
			}
			MTColumn& thisColumn = columnAt(thisIndex);
			MTColumn& otherColumn = otherRow.columnAt(otherIndex);
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


SharedMTRow MTTable::makeRow() {
	auto row = std::make_shared<MTRow>(*(m_TableSchema.get()));
	return row;
}

bool MTTable::addRow(const MTRow &r) {
	auto row = std::make_shared<MTRow>(r);
	push_back(std::move(row));
	return true;
}

bool MTTable::fromString(const std::string &r) {
	MTTableSchema& ts = *m_TableSchema;
	auto rowPtr = std::make_shared<MTRow>(ts);
	//MTRow *row = rowPtr.get();
	if (!(rowPtr->fromString(r))) {
		return false;
	}
	push_back(std::move(rowPtr));
	return true;
}

bool MTTable::read(const char *path, const char *filename) {
	std::string fullpath(path);
	fullpath += '/';
	fullpath += filename;
	return read(fullpath.c_str());
}


bool MTTable::read(const char *fullpath) {
	
	
	clear();
	
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

	
	file.close();

	return true;
}

bool MTTable::write(const char *path, const char *filename) {
	std::string fullpath(path);
	fullpath += '/';
	fullpath += filename;
	return write(fullpath.c_str());
}

bool MTTable::write(const char *fullpath) {

	
	std::ofstream file(fullpath, std::ofstream::trunc);
	if (file.is_open() == false) {
		ErrorCode::setErrorCode(SIA_ERROR::OPEN_ERROR);
		return false;
	}
	std::stringstream s;
	for (auto i = this->begin(); i != this->end(); i++) {
		s << (*i)->toString() << "\n";
		
	}
	this->clear();
	file << s.str();
	file.close();
	return true;
}

bool MTTable::find(MTColumn& column) {
	
	if (rowCursor == NOT_FOUND) {
		rowCursor = 0;
	}
	if (empty()) {
		return false;
	}
	int pos = rowCursor;
	if (size() <= (size_type)pos) {
		return false;
	}
	MTSchema& thisSchema = column.getInfo();
	std::string name = thisSchema.getName();
	int idx = this->m_TableSchema->getIndex(name.c_str());
	bool first = true;
	for (auto i = begin()+pos; i != end(); i++, pos++) {
		auto row = *i;
		MTColumn& other = row->columnAt(idx);
		MTSchema::EItemType type = thisSchema.getType();
		
		switch (type) {
		case MTSchema::Integer:
			if (other.getULong() == column.getULong()) {
				rowCursor = pos;
				return true;
			}
			break;
		case MTSchema::Text:
			if (other.getString() == column.getString()) {
				rowCursor = pos;
				return true;
			}
			break;
		case MTSchema::Date:
			{
			ExifDateTime otherDate = other.getDate();
			ExifDateTime columnDate = column.getDate();
			if (otherDate == columnDate) {
				rowCursor = pos;
				return true;
			}
			break;
			}
		case  MTSchema::Float:
			if (other.getDouble() == column.getDouble()) {
				rowCursor = pos;
				return true;
			}
			break;
		default:
			throw MTTypeException("Invalid type");
		}
		
	}
	return false;
}

bool MTDatabase::addTable(MTTableSchema *pSchemaTable) {
	auto table = std::make_unique<MTTable>(pSchemaTable);
	insert(std::make_pair(pSchemaTable->getName(), std::move(table)));
	return true;
}

MTTable& MTDatabase::getTable(std::string name) {
	auto iter = find(name);
	if (iter == end()) {
		throw std::exception();
	}
	return *(iter->second);
}

} /* namespace simplearchive */
