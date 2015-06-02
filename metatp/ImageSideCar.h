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

#ifndef IMAGEGROUP_H_
#define IMAGEGROUP_H_

#include <string>
#include <vector>

namespace simplearchive {
/**
 * @Brief This class manages the Side car files.
 *
 * This contains the
 * 	Sequence Id
 * 	Unique Id
 *  CRC
 *  MD5
 *  Filename
 *  Filepath
 *
 *  These are used to identify the file in the set of side car files.
 *  The name of the set is the primary image file pass in the constructor.
 */
class SideCarItem {
	std::string m_sequenceId;
	std::string m_uniqueId;
	std::string m_crc;
	std::string m_md5;
	std::string m_filename;
	std::string m_filepath;
	std::string m_row;
public:

	SideCarItem() {

	};
	/// The constructor for the side car item.
	SideCarItem(const char *sequenceId, const char * uniqueId, const char *crc,
					const char *md5, const char * filename, const char * filepath);

	bool fromString(const char *line);

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

	const char *toString();

};

class MetadataObject;
/**
 * @brief This class provides support for side car files in the application.
 *
 * The Side Car support provides a table in the database if enabled, uses XML
 * files placed in the data folders and csv files laso placed in the data folder.
 * The csv file are required for the side car suport to work correctly.
 */
class SideCar : public std::vector<SideCarItem> {
	/// Primary name of the side car set
	std::string m_name;
public:
	/// Constructor.
	/// @pram Primary name of the side car set
	///
	SideCar(const char *name) {
		m_name = name;
	}

	/// Adds the information of the side car file from a MetadataObject.
	void add(MetadataObject &metadataObject);

	/// Provide the default methon of adding a Side Car file.
	void add(const char *sequenceId, const char * uniqueId, const char *crc,
						const char *md5, const char * filename, const char * filepath);

	/// reads a csv side car file
	bool read(const char *datafile);

	/// reads a csv side car file
	bool write(const char *path);
	bool write(const char *path, const char *filename);

	virtual ~SideCar();

	/// Gets the primary name of the side car set
	const std::string& getName() const {
		return m_name;
	}

	/// Sets the primary name of the side car set
	void setName(const std::string& name) {
		m_name = name;
	}


};

} /* namespace simplearchive */
#endif /* IMAGEGROUP_H_ */
