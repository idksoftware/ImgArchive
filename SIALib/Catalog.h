/*
 * FolderList.h
 *
 *  Created on: Oct 3, 2014
 *      Author: wzw7yn
 */

#ifndef CATALOG_H_
#define CATALOG_H_

#include <string>

namespace simplearchive {

#define CATALOG_FOLDER	"/.catalog"


/**
 * @brief This class manages the image folder list in the archive.
 * It records changes to the image folder and maintains an XML list
 * of folders.
 */

class Catalog {
	std::string m_archivePath;
	bool makeXML();
public:
	Catalog(const char *archivePath);
	virtual ~Catalog();

	bool addDayFolder(const char *folderName);
	bool incFolders(const char *folderName);
	bool incFiles(const char *folderName);
	bool makeList();
	bool makeFolder(std::string &fileFolder, std::string &filepath);
};

} /* namespace simplearchive */
#endif /* CATALOG_H_ */
