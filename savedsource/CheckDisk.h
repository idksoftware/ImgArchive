/*
 * CheckDisk.h
 *
 *  Created on: May 19, 2014
 *      Author: wzw7yn
 */

#ifndef CHECKDISK_H_
#define CHECKDISK_H_
#include <string>

namespace simplearchive {

class CheckDisk {
	std::string writeTag(const char *tag, const std::string& value, int tab);
	std::string writeTag(const char *tag, const int value, int tab);
	std::string m_archivePath;
	bool makeXML(const char *targetdir);
public:
	CheckDisk() {};
	CheckDisk(const char *archivePath);
	virtual ~CheckDisk();
	bool makeCheckData(const char *targetdir);
	bool check(const char *targetdir);
	bool update(const char *targetdir, const char *targetfile);
};

} /* namespace simplearchive */
#endif /* CHECKDISK_H_ */
