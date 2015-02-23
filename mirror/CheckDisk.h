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
	//std::string m_archivePath;
        /// Make an XML file manifest in the target directory 
	bool makeXML(const char *targetdir);
public:
        /// Constructor
        /// @param archivePath  The root archive folder  
	//CheckDisk(const char *archivePath);
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
