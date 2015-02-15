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
	time_t m_time;
	std::string unqueNumger();
	bool txtFile(std::string &number, const char *path);
	bool readTxtFile(std::string &number, const char *path);
	std::string m_root;
public:
	StagingManager(const char *root, const char *name, const char *stagingFolder);
	bool put(const char *path);
	virtual ~StagingManager();

};

} /* namespace simplearchive */
#endif /* STAGINGMANAGER_H_ */
