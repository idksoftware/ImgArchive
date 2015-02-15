/*
 * StagingManager.h
 *
 *  Created on: Oct 15, 2014
 *      Author: wzw7yn
 */

#ifndef STAGINGMANAGER_H_
#define STAGINGMANAGER_H_
#include <string>
namespace simplearchive {

class StagingManager {
	std::string m_name;
	std::string m_stagingFolder;
	std::string m_remoteMirrorPath;
	std::string m_root;
	time_t m_time;
	std::string unqueNumger();
	bool txtFile(std::string &number, const char *path);
	bool verifyTxtFile(std::string &number, const char *path);
	/// reads and returns the path of the file. Empty on error
	std::string readTxtFile(const char *txtFilePath);

public:
	StagingManager(const char *root, const char *name, const char *stagingFolder);
	/// Puts the data into the staging area.
	bool put(const char *path);
	/// Forward file to mirror
	bool putMirror();
	virtual ~StagingManager();
	const char *getRemoteMirrorPath() const;
	void setRemoteMirrorPath(const char *remoteMirrorPath);
};

} /* namespace simplearchive */
#endif /* STAGINGMANAGER_H_ */
