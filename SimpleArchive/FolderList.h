/*
 * FolderList.h
 *
 *  Created on: Oct 3, 2014
 *      Author: wzw7yn
 */

#ifndef FOLDERLIST_H_
#define FOLDERLIST_H_

#include <string>

namespace simplearchive {

class FolderFile;
/**
 * @Brief This class manages the image folder list in the archive.
 * It records changes to the image folder and maintains an XML list
 * of folders.
 */

class FolderList {
	std::string m_archivePath;
	//FolderFile &m_folderFile;
	bool makeXML();
public:
	FolderList(const char *archivePath);
	virtual ~FolderList();

	bool addDayFolder(const char *folderName);
	bool incFolders(const char *folderName);
	bool incFiles(const char *folderName);
	bool makeList();
};

} /* namespace simplearchive */
#endif /* FOLDERLIST_H_ */
