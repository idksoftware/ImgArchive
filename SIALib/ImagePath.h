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

#ifndef IMAGEPATH_H_
#define IMAGEPATH_H_
#include <string>
#include "IntegrityManager.h"
#include "AppPaths.h"

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

	std::string m_shadowYearStrPath;
	std::string m_shadowYyyymmddStrPath;
	
	std::string m_dataPath;
	std::string m_dbPath;
	std::string m_metadataPath;
	std::string m_relMetadataPath;

	std::string m_localShadowDataPath;
	std::string m_localShadowMetadataPath;
	std::string m_localShadowDBPath;
	// Local shadow history path
	std::string m_localShadowHistoryPath;

	static std::string m_pathToArchive;
	static std::string m_mainMetadataPath;
//	static std::string m_userDefinedSequenceNumberPath;
	static std::string m_mainDupsPath;
	static std::string m_mainHistory;

	static std::string m_pathToShadow;
	static std::string m_shadowMetadataPath;
	static std::string m_shadowSequenceNumberPath;
	static std::string m_shadowHistory;
	static std::string m_shadowJournalPath;
	static std::string m_shadowCatalog;
	//IntegrityManager& m_integrityManager;
	void init(std::string &yyyymmddStr);
public:
	ImagePath(time_t time);
	ImagePath(const char *filepath);
	ImagePath(std::string &yyyymmddStr);
	bool copyFile(std::string  pathToSourceRoot, std::string file);

	virtual ~ImagePath();

	void setImageName(const char *imageName) {
		m_imageName = imageName;
	}

	const std::string& getImageName() const {
		return m_imageName;
	}

	const std::string& getDataPath() const {
		return m_localShadowDataPath;
	}

	const std::string& getImagePath() {
		m_imagePath = m_relpath.c_str();
		m_imagePath += '/';
		m_imagePath += m_imageName;
		return m_imagePath;
	}

	const std::string& getLocalShadowPath() {
		return m_shadowYyyymmddStrPath;
	}

	const std::string& getLocalShadowMetadataPath() const {
		return m_localShadowMetadataPath;
	}
	
	const std::string& getLocalShadowHistoryPath() const {
		return m_localShadowHistoryPath;
	}

	const std::string& getRelMetadataPath() const {
		return m_relMetadataPath;
	}

	const std::string& getDBPath() const {
		return m_localShadowDBPath;
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

	static bool settupMainArchiveFolders(const char *pathToArchive, const char *pathToShadow);

	// main
	/// @brief The dups path is set in the configuation file (Default is $SHADOW_FOLDER/root/dups)
	/// 
	static const std::string& getMainDupsPath() {
		return m_mainDupsPath;
	}

	static const std::string& getMainMetadataPath() {
		return m_mainMetadataPath;
	}

	//static const std::string& getMainSequenceNumberPath() {
	//	return m_mainSequenceNumberPath;
	//}

	// shadow
	

	static const std::string& getMainShadowMetadataPath() {
		return m_shadowMetadataPath;
	}

	static const std::string& getShadowSequenceNumberPath() {
		return m_shadowSequenceNumberPath;
	}

	// Get shadow history path
	static const std::string& getShadowHistoryPath() {
		return m_shadowHistory;
	}

	// Get shadow history path
	static const std::string& getShadowCatalogPath() {
		return m_shadowCatalog;
	}

	// Get shadow journal path
	static const std::string& getShadowJournalPath() {
		return m_shadowJournalPath;
	}

	static std::string getYearDay(std::string &dataImage) {
		return dataImage.substr(0, 10);
	}

	static std::string getYear(std::string &path) {
		return path.substr(0, 4);
	}
};

} /* namespace simplearchive */
#endif /* IMAGEPATH_H_ */
