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

#ifndef ASSETPROPERTIES_H_
#define ASSETPROPERTIES_H_
#include <string>
#include <vector>
#include <cstdlib>
#include "CSVRow.h"
#include "MetadataObject.h"


namespace simplearchive {

class AssetPropertiesItem : public CSVRowItem {
	/// Asset Properties
	std::string m_sequenceId;
	std::string m_filename;
	std::string m_filepath;
	std::string m_orginalName;
	std::string m_uniqueId;
	std::string m_label;
	std::string m_rating;
	std::string m_mediaType;
	std::string m_md5;
	std::string m_crc;
	std::string m_fileSize;
	std::string m_dateCreate;
	std::string m_dateModified;
	std::string m_dateAdded;
	std::string m_description;
public:
	AssetPropertiesItem(int id);
	virtual ~AssetPropertiesItem();
	const std::string& getCrc() const;
	void setCrc(const std::string& crc);
	const std::string& getDateAdded() const;
	void setDateAdded(const std::string& dateAdded);
	const std::string& getDateCreate() const;
	void setDateCreate(const std::string& dateCreate);
	const std::string& getDateModified() const;
	void setDateModified(const std::string& dateModified);
	const std::string& getDescription() const;
	void setDescription(const std::string& description);
	const std::string& getFilename() const;
	void setFilename(const std::string& filename);
	const std::string& getFilepath() const;
	void setFilepath(const std::string& filepath);
	const std::string& getFileSize() const;
	void setFileSize(const std::string& fileSize);
	const std::string& getLabel() const;
	void setLabel(const std::string& label);
	const std::string& getMd5() const;
	void setMd5(const std::string& md5);
	const std::string& getMediaType() const;
	void setMediaType(const std::string& mediaType);
	const std::string& getOrginalName() const;
	void setOrginalName(const std::string& orginalName);
	const std::string& getRating() const;
	void setRating(const std::string& rating);
	const std::string& getSequenceId() const;
	void setSequenceId(const std::string& sequenceId);
	const std::string& getUniqueId() const;
	void setUniqueId(const std::string& uniqueId);

	virtual const char *toString();
	virtual bool fromString(int pos, std::string &str);
};

class MetadataObject;
class AssetProperties : public CSVRow {
public:
	AssetProperties();
	virtual ~AssetProperties();

	/// Adds the information of the GPS properties from a MetadataObject.
	void add(MetadataObject &metadataObject);

	/// Adds a GPSPropertiesItem object.
	void add(AssetPropertiesItem &item);

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
#endif /* ASSETPROPERTIES_H_ */
