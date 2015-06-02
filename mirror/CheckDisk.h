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

#ifndef CHECKDISK_H_
#define CHECKDISK_H_
#include <string>

namespace simplearchive {
/**
* @brief This class create a manifest file of the files in each folder.
* This can then be user to compare the file is the same folder to
* found if there is way changes. If there are changes that the system
* was not aware of then this me be an error that needs fixing by copying
* the changed file back from a mirror. 
*
*/
class CheckDisk {
        /// Helper function to print a tag into a XML file.  
	std::string writeTag(const char *tag, const std::string& value, int tab);
        /// Helper function to print a tag into a XML file.
	std::string writeTag(const char *tag, const unsigned int value, int tab);
        /// The root archive path
	static std::string m_archivePath;
        /// Make an XML file manifest in the target directory 
	bool makeXML(const char *targetdir);
public:
        /// Constructor
        /// @param archivePath  The root archive folder  
	//CheckDisk(const char *archivePath);
	static void setArchivePath(const char *archivePath);
	CheckDisk() {};
        /// Destructor
	virtual ~CheckDisk();
        /// Makes the check disk data from the target folder
	bool makeCheckData(const char *targetdir);
    	/// Makes the check disk data from the target folder, saves into the save folder
	bool makeCheckData(const char *targetdir, const char *saveDir);
        /// Checks the folder against the manifest file.
	bool check(const char *targetdir);
	bool check(const char *targetdir, const char *savedir);
	/// Write the status file
	//bool writeStatus(const char *path);
        /// Updates the manifest file with new file data.
	bool update(const char *targetdir, const char *targetfile);
};

} /* namespace simplearchive */
#endif /* CHECKDISK_H_ */
