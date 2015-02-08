/*
 * CSVRow.h
 *
 *  Created on: Jul 11, 2014
 *      Author: wzw7yn
 */

#ifndef CSVROW_H_
#define CSVROW_H_
#include <vector>
#include <sstream>
#include <stdio.h>
namespace simplearchive {

#define DELIM ','

class CSVRowItem {
	int m_id;
	CSVRowItem() { m_id = -1; };
	std::stringstream m_tmp;
	std::string *m_row;
protected:
	
public:
	CSVRowItem(int id);
	virtual ~CSVRowItem();

	virtual const char *toString() = 0;
	virtual bool fromString(int pos, std::string &str) = 0;


	int getId() const {
		return m_id;
	}
	/*
	void start() {
		m_tmp.clear();
		m_row.clear();
		m_tmp << getId();
	}

	void add(std::string &item) {
		m_tmp << DELIM << item;
	}
	
	*/
	void start() {
		m_tmp.clear();
		//m_row.clear();
		if (m_row != 0) {
			delete m_row;
		}
		m_row = new std::string;
		*m_row = std::to_string(getId());
	}

	void add(std::string &item) {
		*m_row += DELIM;
		*m_row += item.c_str();
	}
	
	void end() {
		//m_row = m_tmp.str();
	}
	
	std::string token(int *pos, std::string &str);
	std::string& row() {
		return *m_row;
	}
};

class CSVString {
	
public:
	CSVString() {};
	virtual ~CSVString() {};
	virtual const char *toString(CSVRowItem &item) = 0;

};


class MetadataObject;
class CSVRow : public std::vector<CSVRowItem *> {
public:
	CSVRow() {};
	virtual ~CSVRow() {
		for (std::vector<CSVRowItem *>::iterator i = this->begin(); i != this->end(); i++) {
			CSVRowItem *csvRow = *i;
			delete csvRow;
		}
		this->clear();
	};

	/// Adds the information of the GPS properties from a MetadataObject.
	virtual void add(MetadataObject &metadataObject) = 0;

	/// reads a csv GPSProperties file
	bool read(const char *datafile);

	/// writes a csv GPSProperties file
	bool write(const char *path, const char *filename, CSVString &strcov);

	/// makes a row item and inserts into rows list
	virtual bool add(const char *row) = 0;
	int readId(std::string &row, int *pos);
};

} /* namespace simplearchive */

#endif /* CSVROW_H_ */
