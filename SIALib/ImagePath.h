/*
 * ImagePath.h
 *
 *  Created on: Sep 30, 2014
 *      Author: wzw7yn
 */

#ifndef IMAGEPATH_H_
#define IMAGEPATH_H_
#include <string>
#include "IntegrityManager.h"
namespace simplearchive {

#define RELATIVE_PATH           "RelativePath"
#define IMAGE_PATH              "ImagePath"
#define YEAR                    "Year"
#define YYYYMMDD                "YearMonthDay"
#define PATH_TO_ARCHIVE         "ArchivePath"
#define DATA_PATH               "DataPath"
#define METADATA_PATH           "MetadataPath"
#define MAIN_METADATA_PATH      "GlobalMetadataPath"


class ImagePath {
	std::string m_relpath;
	std::string m_imagePath;
	std::string m_imageName;
	std::string m_yearStr;
	std::string m_yyyymmddStr;
	std::string m_yearStrPath;
	std::string m_yyyymmddStrPath;
	
	std::string m_dataPath;
	std::string m_dbPath;
	std::string m_metadataPath;
	std::string m_relMetadataPath;
	static std::string m_mainMetadataPath;
	static std::string m_mainSequenceNumberPath;
	static std::string m_mainDupsPath;
	static std::string m_pathToArchive;
	static std::string m_mainHistory;

	//IntegrityManager& m_integrityManager;
public:
	ImagePath(time_t time, const char *pathToArchive);
	ImagePath(const char *filepath, const char *pathToArchive);

	bool copyFile(std::string  pathToSourceRoot, std::string file);

	virtual ~ImagePath();

	void setImageName(const char *imageName) {
		m_imageName = imageName;
	}

	const std::string& getImageName() const {
		return m_imageName;
	}

	const std::string& getDataPath() const {
		return m_dataPath;
	}

	const std::string& getImagePath() {
		m_imagePath = m_relpath.c_str();
		m_imagePath += '/';
		m_imagePath += m_imageName;
		return m_imagePath;
	}

	const std::string& getMetadataPath() const {
		return m_metadataPath;
	}

	const std::string& getRelMetadataPath() const {
		return m_relMetadataPath;
	}

	const std::string& getDBPath() const {
		return m_dbPath;
	}

	const std::string& getYearStr() const {
		return m_yearStr;
	}

	const std::string& getYearStrPath() const {
		return m_yearStrPath;
	}

	const std::string& getYyyymmddStr() const {
		return m_yyyymmddStr;
	}

	const std::string& getYyyymmddStrPath() const {
		return m_yyyymmddStrPath;
	}

	const std::string& getPathToArchive() const {
		return m_pathToArchive;
	}

	const std::string& getRelativePath() const {

		return m_relpath;
	}

	static bool settupMainArchiveFolders(const char *pathToArchive);

	static const std::string& getMainDupsPath() {
		return m_mainDupsPath;
	}

	static const std::string& getMainMetadataPath() {
		return m_mainMetadataPath;
	}

	static const std::string& getMainSequenceNumberPath() {
		return m_mainSequenceNumberPath;
	}
};

} /* namespace simplearchive */
#endif /* IMAGEPATH_H_ */
