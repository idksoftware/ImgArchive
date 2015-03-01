/*
 * CoreMetadata.cpp
 *
 *  Created on: Jun 26, 2014
 *      Author: wzw7yn
 */

#include "AssetProperties.h"
#include "DBDefines.h"
//namespace simplearchive {

AssetProperties::AssetProperties() {

	/*
	TableField m_sequenceId;
		TableField m_md5;
		TableField m_crc;
		TableField m_filename;
		TableField m_filepath;
		TableField m_orginalName;
		TableField m_dateCreate;
		TableField m_dateModified;
		TableField m_dateAdded;
		TableField m_captureDate;
		TableField m_uuid;
		TableField m_label;
		TableField m_rating;
		TableField m_mediaType;
		TableField m_fileSize;
		TableField m_viewRotation;
		TableField m_description;
	*/
	TableColumn &tableColumn = add(DB_SEQUENCEID, m_sequenceId);
	tableColumn.addAttributes("primary key");
	tableColumn.addAttributes("autoincrement");

	add(DB_FILENAME, m_filename);
	add(DB_FILEPATH, m_filepath);
	add(DB_ORGINALNAME, m_orginalName);
	add(DB_UUID, m_uuid);
	add(DB_LABEL, m_label);
	add(DB_RATING, m_rating);
	add(DB_MEDIATYPE, m_mediaType);
	add(DB_MD5, m_md5);
	add(DB_CRC, m_crc);
	add(DB_FILESIZE, m_fileSize);
	add(DB_DATECREATE, m_dateCreate);
	add(DB_DATEMODIFIED, m_dateModified);
	add(DB_DATEADDED, m_dateAdded);
	add(DB_DESCRIPTION, m_description);
	add(DB_VIEWROTATION, m_viewRotation);

}

AssetProperties::~AssetProperties() {

}

/*
AssetPropertiesRow::AssetPropertiesRow() {
	TableColumn &tableColumn = add(DB_SEQUENCEID, TableField::Integer);
	tableColumn.addAttributes("primary key");
	tableColumn.addAttributes("autoincrement");

	add(DB_FILENAME, TableField::Text);
	add(DB_FILEPATH, TableField::Text);
	add(DB_ORGINALNAME, TableField::Text);
	add(DB_UUID, TableField::Text);
	add(DB_LABEL, TableField::Text);
	add(DB_RATING, TableField::Text);
	add(DB_MEDIATYPE, TableField::Text);
	add(DB_MD5, TableField::Text);
	add(DB_CRC, TableField::Text);
	add(DB_FILESIZE, TableField::Integer);
	add(DB_DATECREATE, TableField::Text);
	add(DB_DATEMODIFIED, TableField::Text);
	add(DB_DATEADDED, TableField::Text);
	add(DB_DESCRIPTION, TableField::Text);
	add(DB_VIEWROTATION, TableField::Text);

}

AssetPropertiesRow::~AssetPropertiesRow() {
	// TODO Auto-generated destructor stub
}

bool AssetPropertiesRow::append(AssetProperties& assetProperties) {

	TableField &s = assetProperties.m_sequenceId;
	if (s.isUpdated()) {
		getTableField(DB_SEQUENCEID) = s;

	}
*/
/*
	s = assetProperties.m_filename;
	if (s.isUpdated()) {
		getTableField(DB_FILENAME) = field;
		getTableField(DB_FILENAME).setUpdated();
	}

	s = assetProperties.m_filepath;
	if (s.isUpdated()) {
		getTableField(DB_FILEPATH) = field;
	}

	s = assetProperties.m_orginalName;
	if (s.isUpdated()) {
		getTableField(DB_ORGINALNAME) = field;
	}

	s = assetProperties.m_uuid;
	if (s.isUpdated()) {
		getTableField(DB_UUID) = field;
	}

	s = assetProperties.m_label;
	if (s.isUpdated()) {
		getTableField(DB_LABEL) = field;
	}


	s = assetProperties.m_rating;
	if (s.isUpdated()) {
		getTableField(DB_RATING) = field;
	}

	s = assetProperties.m_mediaType;
	if (s.isUpdated()) {
		getTableField(DB_MEDIATYPE) = field;
	}

	s = assetProperties.m_md5;
	if (s.isUpdated()) {
		getTableField(DB_MD5) = field;
	}

	s = assetProperties.m_crc;
	if (s.isUpdated()) {
		getTableField(DB_CRC) = field;
	}

	s = assetProperties.m_fileSize;
	if (s.isUpdated()) {
		getTableField(DB_FILESIZE) = field;
	}

	s = assetProperties.m_dateCreate;
	if (s.isUpdated()) {
		getTableField(DB_DATECREATE) = field;
	}

	s = assetProperties.m_dateModified;
	if (s.isUpdated()) {
		getTableField(DB_DATEMODIFIED) = field;
	}

	s = assetProperties.m_dateAdded;
	if (s.isUpdated()) {
		getTableField(DB_DATEADDED) = field;
	}

	s = assetProperties.m_description;
	if (s.isUpdated()) {
		getTableField(DB_DESCRIPTION) = field;
	}

	s = assetProperties.m_viewRotation;
	if (s.isUpdated()) {
		getTableField(DB_VIEWROTATION) = field;
	}
*/
/*
	return true;
}

bool AssetPropertiesRow::update(AssetProperties& assetProperties) {
	return true;
}

bool AssetPropertiesRow::remove(std::string& getSequenceId) {
	return true;
}
*/
//} /* namespace simplearchive */
