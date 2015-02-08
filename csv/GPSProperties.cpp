/*
 * GPSProperties.cpp
 *
 *  Created on: Jul 9, 2014
 *      Author: wzw7yn
 */
#include <stdio.h>
#include <fstream>
#include "CSVRow.h"
#include <cstdlib>
#include "GPSProperties.h"
#include "MetadataObject.h"

namespace simplearchive {

class GPSPropertiesString : public CSVString {
public:
	GPSPropertiesString() {};
	virtual ~GPSPropertiesString() {};
	virtual const char *toString(CSVRowItem &item);

};

const char *GPSPropertiesString::toString(CSVRowItem &item) {
	GPSPropertiesItem &tmp = (GPSPropertiesItem &)item;
	return tmp.toString();
}

GPSProperties::GPSProperties() {

}

GPSProperties::~GPSProperties() {

}

// Adds the information of the side car file from a MetadataObject.
void GPSProperties::add(MetadataObject &metadataObject) {
	const char *idStr = metadataObject.getSequenceId().c_str();
	int seqId = atol(idStr);
	GPSPropertiesItem *item = new GPSPropertiesItem(seqId);
	item->setLatitude(metadataObject.getLatitude());
	item->setLongitude(metadataObject.getLongitude());
	item->setGpsTimeStamp(metadataObject.getGpsTimeStamp());
	push_back(item);
	return;
}

/// reads a csv side car file
bool GPSProperties::read(const char *datafile) {
	std::string path(datafile);
	path += "/GPSProperties.csv";
	if (CSVRow::read(path.c_str()) == false) {
		return false;
	}
	return true;
}

/// reads a csv side car file
bool GPSProperties::write(const char *path) {
	std::string filename("GPSProperties.csv");
	GPSPropertiesString cov;
	CSVRow::write(path, filename.c_str(), cov);
	//printf("%s", filename.c_str());
	return true;
}

bool GPSProperties::add(const char *row) {
	std::string tmp = row;
	int pos = 0;
	int seqId = readId(tmp, &pos);
	GPSPropertiesItem *item = new GPSPropertiesItem(seqId);
	item->fromString(pos+1, tmp);
	push_back(item);
	return true;
}





GPSPropertiesItem::GPSPropertiesItem(int id) : CSVRowItem(id) {

}

GPSPropertiesItem::~GPSPropertiesItem() {

}

inline const std::string& GPSPropertiesItem::getGpsTimeStamp() const {
	return m_gpsTimeStamp;
}

inline void GPSPropertiesItem::setGpsTimeStamp(const std::string& gpsTimeStamp) {
	m_gpsTimeStamp = gpsTimeStamp;
}

inline const std::string& GPSPropertiesItem::getLatitude() const {
	return m_latitude;
}

inline void GPSPropertiesItem::setLatitude(const std::string& latitude) {
	m_latitude = latitude;
}

inline const std::string& GPSPropertiesItem::getLongitude() const {
	return m_longitude;
}

inline void GPSPropertiesItem::setLongitude(const std::string& longitude) {
	m_longitude = longitude;
}

inline const int GPSPropertiesItem::getSequenceId() const {
	return getId();
}


const char *GPSPropertiesItem::toString() {
	start();
	add(m_latitude);
	add(m_longitude);
	add(m_gpsTimeStamp);
	end();
	return row().c_str();
}




bool GPSPropertiesItem::fromString(int pos, std::string &str) {
	m_latitude = token(&pos, str);
	pos++;
	m_longitude = token(&pos, str);
	pos++;
	m_gpsTimeStamp = token(&pos, str);
	return true;
}


} /* namespace simplearchive */
