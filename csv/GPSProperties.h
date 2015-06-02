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

#ifndef GPSPROPERTIES_H_
#define GPSPROPERTIES_H_

#include <string>
#include <vector>
#include "CSVRow.h"

namespace simplearchive {


class MetadataObject;
class GPSPropertiesItem : public CSVRowItem {

	std::string m_latitude;
	std::string m_longitude;
	std::string m_gpsTimeStamp;
public:

	GPSPropertiesItem(int id); //: CSVRowItem(id) {}
	virtual ~GPSPropertiesItem();
	const std::string& getGpsTimeStamp() const;
	void setGpsTimeStamp(const std::string& gpsTimeStamp);
	const std::string& getLatitude() const;
	void setLatitude(const std::string& latitude);
	const std::string& getLongitude() const;
	void setLongitude(const std::string& longitude);
	const int getSequenceId() const;

//protected:

	virtual const char *toString();
	virtual bool fromString(int pos, std::string &str);

};




class GPSProperties : public CSVRow {
public:
	GPSProperties();
	virtual ~GPSProperties();

	/// Adds the information of the GPS properties from a MetadataObject.
	void add(MetadataObject &metadataObject);

	/// Adds a GPSPropertiesItem object.
	void add(GPSPropertiesItem &item);

	/// reads a csv GPSProperties file
	bool read(const char *datafile);

	/// writes a csv GPSProperties file
	bool write(const char *path);

	unsigned int findImage(const char *text, int col);
	bool load(unsigned int row, MetadataObject &mo);
	bool save(unsigned int row, MetadataObject &mo);
	//bool write(const char *path, const char *filename, CSVString& u) {
	//	return CSVRow::write(path, filename, u);
	//}

	virtual bool add(const char *row);

};

} /* namespace simplearchive */
#endif /* GPSPROPERTIES_H_ */
