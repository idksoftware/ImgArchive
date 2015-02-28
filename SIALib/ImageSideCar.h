/*
 * ImageGroup.h
 *
 *  Created on: Jul 8, 2014
 *      Author: wzw7yn
 */

#ifndef IMAGEGROUP_H_
#define IMAGEGROUP_H_

#include <string>
#include <vector>

namespace simplearchive {

class SideCarItem {
	std::string m_sequenceId;
	std::string m_uniqueId;
	std::string m_crc;
	std::string m_md5;
	std::string m_filename;
	std::string m_filepath;
public:
	SideCarItem(const char *sequenceId, const char * uniqueId, const char *crc,
					const char *md5, const char * filename, const char * filepath);

	virtual ~SideCarItem();

	const std::string& getCrc() const {
		return m_crc;
	}

	void setCrc(const std::string& crc) {
		m_crc = crc;
	}

	const std::string& getFilename() const {
		return m_filename;
	}

	void setFilename(const std::string& filename) {
		m_filename = filename;
	}

	const std::string& getFilepath() const {
		return m_filepath;
	}

	void setFilepath(const std::string& filepath) {
		m_filepath = filepath;
	}

	const std::string& getMd5() const {
		return m_md5;
	}

	void setMd5(const std::string& md5) {
		m_md5 = md5;
	}

	const std::string& getSequenceId() const {
		return m_sequenceId;
	}

	void setSequenceId(const std::string& sequenceId) {
		m_sequenceId = sequenceId;
	}

	const std::string& getUniqueId() const {
		return m_uniqueId;
	}

	void setUniqueId(const std::string& uniqueId) {
		m_uniqueId = uniqueId;
	}
};

class MetadataObject;
class SideCar : public std::vector<SideCarItem> {
	std::string m_name;
public:
	SideCar(const char *name) {
		m_name = name;
	}

	void add(MetadataObject &metadataObject);

	void add(const char *sequenceId, const char * uniqueId, const char *crc,
						const char *md5, const char * filename, const char * filepath);
	virtual ~SideCar();

	const std::string& getName() const {
		return m_name;
	}

	void setName(const std::string& name) {
		m_name = name;
	}
};

} /* namespace simplearchive */
#endif /* IMAGEGROUP_H_ */
