/*
 * MetaType.cpp
 *
 *  Created on: May 5, 2015
 *      Author: wzw7yn
 */
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
	case Integer: return "Integer";
	case Text: return "Text";
	case Date: return "Date";
	case Float: return "Float";
	}
	return "Unknown";
}

const char *MTTypeException::what() const throw() {
	//std::stringstream s;
	//s << message << " line:" << __LINE__ << " file:" << __FILE__;
	//std::string tmp = s.str();
	//output = tmp.c_str();
	return message.c_str();
}


MTColumn::MTColumn(MTSchema &info) {
	m_info = &info;
	m_object = 0;
	m_boundValue = 0;
}

static int count = 0;
MTColumn::~MTColumn() {
	if (m_object != 0) {
		count++;
		if (count == 33) {
			int i = 0;
		}
		delete m_object;
	}
}
std::string &MTColumn::toString() {

	std::string *returned = 0;
	if (isBound()) {
		returned = m_boundValue;
	}
	else {
		m_unboundValue.reset(new std::string);
		returned = m_unboundValue.get();
	}
	MTSchema::EItemType type = m_info->getType();
	if (m_object == 0) {
		*returned = "null";
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
		if (m_object == 0) {
			 m_object = new MetaTypeObject(i);
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
		if (m_object == 0) {
			 m_object = new MetaTypeObject(tmp.c_str());
		} else {
			*m_object = tmp;
		}
//		boundUpdate();
	}
	else {
		throw MTTypeException("Invalid type");
	}
}
void MTColumn::set(const std::string &str) {
	MTSchema::EItemType type = m_info->getType();
	if (type == MTSchema::Text) {
		if (m_object == 0) {
			 m_object = new MetaTypeObject(str.c_str());
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
		if (m_object == 0) {
			 m_object = new MetaTypeObject(date);
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
		if (m_object == 0) {
			m_object = new MetaTypeObject(d);
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

MTRow::MTRow(MTTableSchema *pSchemaTable) {
	m_schema = pSchemaTable;

	for (std::vector<MTSchema>::iterator i = pSchemaTable->begin(); i != pSchemaTable->end(); i++) {
		MTSchema& columnInfo = *i;
		push_back(new MTColumn(columnInfo));
	}
}

MTRow::MTRow(const MTRow &row) {
	m_schema = row.m_schema;
	for (std::vector<MTSchema>::iterator i = m_schema->begin(); i != m_schema->end(); i++) {
		MTSchema& columnInfo = *i;
		push_back(new MTColumn(columnInfo));
	}
	join((MTRow&)row);
}

MTRow &MTRow::operator=(const MTRow &row) {
	m_schema = row.m_schema;
	for (std::vector<MTColumn *>::iterator i = this->begin(); i != this->end(); i++) {
		MTColumn *column = *i;
		delete column;
	}
	clear();
	for (std::vector<MTSchema>::iterator i = m_schema->begin(); i != m_schema->end(); i++) {
		MTSchema& columnInfo = *i;
		push_back(new MTColumn(columnInfo));
	}
	join((MTRow&)row);
	return *this;
}

MTRow::~MTRow() {
	for (std::vector<MTColumn *>::iterator i = this->begin(); i != this->end(); i++) {
		MTColumn *column = *i;
		delete column;
	}
	this->clear();
	delete m_schema;
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
			thisColumn.set(otherColumn);

		}
	}
	catch (const std::exception& ex) {
		printf(" thisIndex:%d otherIndex:%d\n", thisIndex, otherIndex);
		printf("%s\n", ex.what());
	}
	return true;
}

} /* namespace simplearchive */
