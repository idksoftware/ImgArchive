/*
 * CoreMetadata.h
 *
 *  Created on: Jun 26, 2014
 *      Author: wzw7yn
 */


//AssetProperties
#ifndef ASSETPROPERTIES_H_
#define ASSETPROPERTIES_H_
#include <string>
#include "Database.h"
#include "DBDefines.h"


//namespace simplearchive {
class AssetProperties : public TableRow {
	friend class AssetPropertiesRow;
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

public:
	/*
	const std::string& getCheckedOutName() const {
		return m_checkedOutName;
	}

	void setCheckedOutName(const std::string& checkedOutName) {
		m_checkedOutName = checkedOutName;
	}
	*/
	const std::string& getCrc() const {
		return m_crc.toString();
	}

	void setCrc(const std::string& crc) {
		m_crc = crc;
	}

	const std::string& getFilename() const {
		return m_filename.toString();
	}

	void setFilename(const std::string& filename) {
		m_filename = filename;
	}

	const std::string& getMd5() const {
		return m_md5.toString();
	}

	void setMd5(const std::string& md5) {
		m_md5 = md5;
	}

	const std::string& getOrginalName() const {
		return m_orginalName.toString();
	}

	void setOrginalName(const std::string& orginalName) {
		m_orginalName = orginalName;
	}

	const std::string& getUuid() const {
		return m_uuid.toString();
	}

	void setUuid(const std::string& uuid) {
		m_uuid = uuid;
	}


	const std::string& getCaptureDate() const {
		return m_captureDate.toString();
	}

	void setCaptureDate(const std::string& captureDate) {
		m_captureDate = captureDate;
	}


	const std::string& getDateAdded() const {
		return m_dateAdded.toString();
	}

	void setDateAdded(const std::string& dateAdded) {
		m_dateAdded = dateAdded;
	}

	const std::string& getDateCreate() const {
		return m_dateCreate.toString();
	}

	void setDateCreate(const std::string& dateCreate) {
		m_dateCreate = dateCreate;
	}

	const std::string& getDateModified() const {
		return m_dateModified.toString();
	}

	void setDateModified(const std::string& dateModified) {
		m_dateModified = dateModified;
	}



	const std::string& getFilepath() const {
		return m_filepath.toString();
	}

	void setFilepath(const std::string& filepath) {
			m_filepath = filepath;
	}

	const std::string& getFileSize() const {
		return m_fileSize.toString();
	}

	void setFileSize(const std::string& fileSize) {
		m_fileSize = fileSize;
	}



	const std::string& getLabel() const {
		return m_label.toString();
	}

	void setLabel(const std::string& label) {
		m_label = label;
	}


	const std::string& getRating() const {
		return m_rating.toString();
	}

	void setRating(const std::string& rating = "5") {
		m_rating = rating;
	}



	const std::string& getViewRotation() const {
		return m_viewRotation.toString();
	}

	void setViewRotation(const std::string& viewRotation) {
		m_viewRotation = viewRotation;
	}

	void setSequenceId(const std::string& id) {
		m_sequenceId = id;
	}

	const std::string& getSequenceId() const {
		return m_sequenceId.toString();
	}
public:
	AssetProperties();
	virtual ~AssetProperties();
};

/*
class AssetPropertiesRow : public TableRow {

public:
	AssetPropertiesRow();
	virtual ~AssetPropertiesRow();
	bool append(AssetProperties& assetProperties);
	bool update(AssetProperties& assetProperties);
	bool remove(std::string& getSequenceId);
};
*/
// } /* namespace simplearchive */
#endif /* ASSETPROPERTIES_H_ */
