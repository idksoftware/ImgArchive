/*
 * VersionControl.h
 *
 *  Created on: Feb 24, 2015
 *      Author: wzw7yn
 */

#ifndef VERSIONCONTROL_H_
#define VERSIONCONTROL_H_
#include <memory>
namespace simplearchive {

class VersionControl {
	static std::auto_ptr<VersionControl> m_this;

	/// @brief Path to the root folder of the archive
	std::string m_pathToArchive;

	/// @brief Path to the root folder of the source of the images.
	std::string m_pathToSourceRoot;

	static std::string versonString(int idx);
	static bool setHistory(const char *hstpath, const char *filepath, const char *comment, HistoryEvent evt, int idx);
	void getPathAndFilename(const char *filepath, std::string &filename, std::string &path);

public:
	VersionControl();

	/// @brief Processes the images to be checked out

	/// @param filepath     Relative path to the impage to be checked out.
	/// @param comment     Comment to be added into the history.
	bool checkout(const char *filepath, const char *comment);

	/// @brief Processes the images to be checked out

	/// @param filepath     Relative path to the impage to be checked in.
	/// @param comment     Comment to be added into the history.
	bool checkin(const char *filepath, const char *comment);

	/// @brief Processes the images to be checked out

	/// @param filepath     Relative path to the impage to be un-checked out.
	/// @param comment     Comment to be added into the history.
	bool uncheckout(const char *filepath, const char *comment);


	int getNumberOfVersions(const char *filepath);

	bool getImageVersion(const char *imagePath,const char *targetPath, int idx);

	bool uncheckout(const char *filepath,const char *targetPath, int idx, const char *comment);
	/// @return A path to archive.
	const std::string& getPathToArchive() const {
		return m_pathToArchive;
	}

	void setPathToArchive(std::string& pathToArchive) {
		m_pathToArchive = pathToArchive;
	}

	const std::string& getPathToSourceRoot() const {
		return m_pathToSourceRoot;
	}

	void setPathToSourceRoot(std::string& pathToSourceRoot) {
		m_pathToSourceRoot = pathToSourceRoot;
	}

	static VersionControl &get();
	virtual ~VersionControl();
};

} /* namespace simplearchive */
#endif /* VERSIONCONTROL_H_ */
