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

#ifndef CATALOGMANAGER_H_
#define CATALOGMANAGER_H_
#include <memory>
#include <string>

namespace simplearchive {
/**
 * @brief This class is the main object responsible for managing the cataloguing activities in
 * is application.
 *
 * This will include the main archive and any views that require cataloguing.
 * 
 * Each catalog will consist of a catalog root folder that will contain the catalog data files.
 * In the root folder will be a main index file that will contain a main index file and a folder for
 * each year.
 * the main index will contain references to each of the year folder. The year folder will contain
 * the day files. Each day file will contain the catalog data for each image in the catalog.  
 * This will allow the calalog to be mantained in files and foldes of a manageable size.
 */
class GetImagesManager {
	static std::shared_ptr<GetImagesManager> m_this;
	static std::string m_workspacePath;
	static std::string m_primaryIndexPath;
	bool addDayFolder(const char *folderName);
public:
	GetImagesManager();
	virtual ~GetImagesManager();

	/// @brief Add image filename.
	/// @param folderPath contains the folder path string in the form of Year-month-day
	/// i.e. "2015-03-23"
	/// @param fileName contains the image file name.
	///
	bool addFile(const char *folderPath, const char *fileName);
	/// Reads the Master archive and creates a catalog of the Master archive
	/// This may be changed so views can be cataloged
	bool makeList();

	static GetImagesManager &get();
	static void setPaths(const char* workspacePath, const char *primaryIndexPath) {
		m_workspacePath = workspacePath;
		m_primaryIndexPath = primaryIndexPath;
	}
};

} /* namespace simplearchive */
#endif /* CATALOGMANAGER_H_ */
