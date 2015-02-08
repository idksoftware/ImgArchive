/*
 * AssetProperties.cpp
 *
 *  Created on: Jul 9, 2014
 *      Author: wzw7yn
 */

#include "AssetProperties.h"
#include "MetadataObject.h"
#include <stdio.h>
namespace simplearchive {

class AssetPropertiesString : public CSVString {
public:
	AssetPropertiesString() {};
	virtual ~AssetPropertiesString() {};
	virtual const char *toString(CSVRowItem &item);

};

const char *AssetPropertiesString::toString(CSVRowItem &item) {
	AssetPropertiesItem &tmp = (AssetPropertiesItem &)item;
	return tmp.toString();
}

AssetPropertiesItem::AssetPropertiesItem(int id) : CSVRowItem(id) {
}

AssetPropertiesItem::~AssetPropertiesItem() {
}

const char *AssetPropertiesItem::toString() {
	start();
	add(m_filename);
	add(m_filepath);
	add(m_orginalName);
	add(m_uniqueId);
	add(m_label);
	add(m_rating);
	add(m_mediaType);
	add(m_md5);
	add(m_crc);
	add(m_fileSize);
	add(m_dateCreate);
	add(m_dateModified);
	add(m_dateAdded);
	add(m_description);
	end();
	return row().c_str();
}

bool AssetPropertiesItem::fromString(int pos, std::string &str) {
	m_filename = token(&pos, str);
	pos++;
	m_filepath = token(&pos, str);
	pos++;
	m_orginalName = token(&pos, str);
	pos++;
	m_uniqueId = token(&pos, str);
	pos++;
	m_label = token(&pos, str);
	pos++;
	m_rating = token(&pos, str);
	pos++;
	m_mediaType = token(&pos, str);
	pos++;
	m_md5 = token(&pos, str);
	pos++;
	m_crc = token(&pos, str);
	pos++;
	m_fileSize = token(&pos, str);
	pos++;
	m_dateCreate = token(&pos, str);
	pos++;
	m_dateModified = token(&pos, str);
	pos++;
	m_dateAdded = token(&pos, str);
	pos++;
	m_description = token(&pos, str);

	return true;
}

AssetProperties::AssetProperties() {
}

AssetProperties::~AssetProperties() {
}

// Adds the information of the side car file from a MetadataObject.
void AssetProperties::add(MetadataObject &metadataObject) {
	const char *idStr = metadataObject.getSequenceId().c_str();
	int seqId = atol(idStr);
	AssetPropertiesItem *item = new AssetPropertiesItem(seqId);
    item->setFilename(metadataObject.getFilename());
	item->setFilepath(metadataObject.getFilepath());
	item->setOrginalName(metadataObject.getOrginalName());
	item->setUniqueId(metadataObject.getUniqueId());
	item->setLabel(metadataObject.getLabel());
	item->setRating(metadataObject.getRating());
	item->setMediaType(metadataObject.getMediaType());
	item->setMd5(metadataObject.getMd5());
	item->setCrc(metadataObject.getCrc());
	item->setFileSize(metadataObject.getFileSize());
	item->setDateCreate(metadataObject.getDateCreate());
	item->setDateModified(metadataObject.getDateModified());
	item->setDateAdded(metadataObject.getDateAdded());
	item->setDescription(metadataObject.getDescription());
	push_back(item);
    return;
}

/// reads a csv side car file
bool AssetProperties::read(const char *datafile) {
	std::string path(datafile);
	path += "/AssetProperties.csv";
	if (CSVRow::read(path.c_str()) == false) {
		return false;
	}
	return true;
}

/// reads a csv side car file
bool AssetProperties::write(const char *path) {
	std::string filename("AssetProperties.csv");
	AssetPropertiesString cov;
	CSVRow::write(path, filename.c_str(), cov);

	return true;
}

bool AssetProperties::add(const char *row) {
	std::string tmp = row;
	int pos = 0;
	int seqId = readId(tmp, &pos);
	AssetPropertiesItem *item = new AssetPropertiesItem(seqId);
	item->fromString(pos+1, tmp);
	push_back(item);
	return true;
}


const std::string& AssetPropertiesItem::getCrc() const {
	return m_crc;
}

void AssetPropertiesItem::setCrc(const std::string& crc) {
	m_crc = crc;
}

const std::string& AssetPropertiesItem::getDateAdded() const {
	return m_dateAdded;
}

void AssetPropertiesItem::setDateAdded(const std::string& dateAdded) {
	m_dateAdded = dateAdded;
}

const std::string& AssetPropertiesItem::getDateCreate() const {
	return m_dateCreate;
}

void AssetPropertiesItem::setDateCreate(const std::string& dateCreate) {
	m_dateCreate = dateCreate;
}

const std::string& AssetPropertiesItem::getDateModified() const {
	return m_dateModified;
}

void AssetPropertiesItem::setDateModified(const std::string& dateModified) {
	m_dateModified = dateModified;
}

const std::string& AssetPropertiesItem::getDescription() const {
	return m_description;
}

void AssetPropertiesItem::setDescription(const std::string& description) {
	m_description = description;
}

const std::string& AssetPropertiesItem::getFilename() const {
	return m_filename;
}

void AssetPropertiesItem::setFilename(const std::string& filename) {
	m_filename = filename;
}

const std::string& AssetPropertiesItem::getFilepath() const {
	return m_filepath;
}

void AssetPropertiesItem::setFilepath(const std::string& filepath) {
	m_filepath = filepath;
}

const std::string& AssetPropertiesItem::getFileSize() const {
	return m_fileSize;
}

void AssetPropertiesItem::setFileSize(const std::string& fileSize) {
	m_fileSize = fileSize;
}

const std::string& AssetPropertiesItem::getLabel() const {
	return m_label;
}

void AssetPropertiesItem::setLabel(const std::string& label) {
	m_label = label;
}

const std::string& AssetPropertiesItem::getMd5() const {
	return m_md5;
}

void AssetPropertiesItem::setMd5(const std::string& md5) {
	m_md5 = md5;
}

const std::string& AssetPropertiesItem::getMediaType() const {
	return m_mediaType;
}

void AssetPropertiesItem::setMediaType(const std::string& mediaType) {
	m_mediaType = mediaType;
}

const std::string& AssetPropertiesItem::getOrginalName() const {
	return m_orginalName;
}

void AssetPropertiesItem::setOrginalName(const std::string& orginalName) {
	m_orginalName = orginalName;
}

const std::string& AssetPropertiesItem::getRating() const {
	return m_rating;
}

void AssetPropertiesItem::setRating(const std::string& rating) {
	m_rating = rating;
}

const std::string& AssetPropertiesItem::getUniqueId() const {
	return m_uniqueId;
}

void AssetPropertiesItem::setUniqueId(const std::string& uniqueId) {
	m_uniqueId = uniqueId;
}

} /* namespace simplearchive */
