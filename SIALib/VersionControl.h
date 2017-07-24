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

#pragma once
#ifdef THIS_CODE_IS_NOT_USED
#include <memory>
namespace simplearchive {

	class ImageHistoryLog;
class VersionControl {
	static std::auto_ptr<VersionControl> m_this;

	/// @brief Path to the root folder of the archive
	static std::string m_pathToArchive;
	static std::string m_pathToMaster;

	/// @brief Path to the root folder of the source of the images.
	std::string m_pathToSourceRoot;

	static std::string versonString(int idx);
	static bool setHistory(const char *hstpath, const char *filepath, const char *comment, HistoryEvent evt, int idx);
	/**
	 * @brief Splits the filename and path from filepath and cheaks the paths exist
	 */
	bool getPathAndFilename(const char *filepath, std::string &filename, std::string &path);
	bool CopyNewVersion2Data(const std::string &newImagePath, const std::string &dataPath, const std::string &imageName);
public:
	VersionControl();

	/// @brief Processes the images to be checked out

	/// @param filepath     Relative path to the image to be checked out.
	/// @param comment     Comment to be added into the history.
	bool checkout(const char *filepath, const char *comment);

	/// @brief Processes the images to be checked out

	/// @param filepath     Relative path to the image to be checked in.
	/// @param comment     Comment to be added into the history.
	bool checkin(const char *filepath, const char *comment);

	/// @brief Processes the images to be checked out

	/// @param filepath     Relative path to the image to be un-checked out.
	/// @param comment     Comment to be added into the history.
	bool uncheckout(const char *filepath, const char *comment);


	int getNumberOfVersions(const char *filepath);

	bool getImageVersion(const char *imagePath,const char *targetPath, int idx);

	bool getAllImageVersions(const char *imagePath,const char *targetPath);

	bool uncheckout(const char *filepath,const char *targetPath, int idx, const char *comment);
	/// @return A path to archive.
	const std::string& getPathToArchive() const {
		return m_pathToArchive;
	}

	static void setPathToArchives(const char *pathToArchive, const char *pathToMaster) {
		m_pathToArchive = pathToArchive;
		m_pathToMaster = pathToMaster;
	}

	const std::string& getPathToSourceRoot() const {
		return m_pathToSourceRoot;
	}

	void setPathToSourceRoot(std::string& pathToSourceRoot) {
		m_pathToSourceRoot = pathToSourceRoot;
	}

	static VersionControl &get();
	virtual ~VersionControl();

	std::auto_ptr<ImageHistoryLog>  getHistory(const char *filepath);
};

} /* namespace simplearchive */
#endif
